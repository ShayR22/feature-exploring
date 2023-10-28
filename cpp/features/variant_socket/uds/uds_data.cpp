#include "uds_data.hpp"

#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <cerrno>

UDSData::UDSData() {
    // DO NOTHING
}

UDSData::~UDSData() {
    if (_fd == -1) {
        return;
    }
    int err = close(_fd);
    if (err) {
        printf("Failed to close DataSocket (fd=[%d]), error=[%d]: [%s]\n", _fd, errno, strerror(errno));
    }
}

UDS_STATUS UDSData::initialize(int fd) {
    if (fd <= 0) {
        printf("Invalid socket id [%d], must be a positive integer\n", fd);
        return UDS_STATUS::INVALID_ARGUMENT;
    }

    _fd = fd;

    return UDS_STATUS::SUCCESS;
}

UDS_STATUS UDSData::_validate_wr_args(const uint8_t* buffer) {
    if (!buffer) {
        errno = EINVAL;
        printf("Can't operate with null buffer\n");
        return UDS_STATUS::INVALID_ARGUMENT;
    }

    if (_fd == -1) {
        errno = EBADE;
        printf("Class isn't initialized\n");
        return UDS_STATUS::CLOSED_SOCKET;
    }

    return UDS_STATUS::SUCCESS;
}


UDS_STATUS UDSData::read_buffer(uint8_t* buffer, size_t buffer_size) {
    UDS_STATUS status = _validate_wr_args(buffer);
    if (status != UDS_STATUS::SUCCESS) {
        printf("Read failed to buffer=[%p] buffer_size=[%zu]\n", buffer, buffer_size);
        return UDS_STATUS::INVALID_ARGUMENT;
    }

    size_t bytes_read = 0;
    int tmp_read_result = 0;

    while (bytes_read < buffer_size) {
        tmp_read_result = ::read(_fd, buffer + bytes_read, buffer_size - bytes_read);

        if (tmp_read_result < 0 && errno != EINTR) {
            printf("Read failed [%s]\n", strerror(errno));
            // a negative result means an error and errno is positive
            return UDS_STATUS::INTERNAL_ERROR;
        }

        // read == 0 -> connection reset_by_peer
        if (tmp_read_result == 0) {
            if (bytes_read == buffer_size) {
                // this shouldn't be reached but let's put something informative here in case I'm wrong.
                // why shouldn't it be reached? because a situation where I expect X bytes and receive bytes should lead us
                // out of the loop anyways.
                printf("FYI read exact amount then closed connection. SHOULDN'T BE REACHED.\n");
                // return normally so that the packet will be processed.
                return UDS_STATUS::INTERNAL_ERROR;
            }
            if (bytes_read < buffer_size) {
                // This shouldn't happen in regular operation, but a rare failure might still cause it (or a pkill)
                printf("Connection reset midpacket.\n");
                // this is an error so return 0, there is no point in processing half a packet.
                return UDS_STATUS::CLOSED_SOCKET;
            }
            printf("Connection reset by peer in full_read.\n");
            return UDS_STATUS::CLOSED_SOCKET;
        }

        bytes_read += tmp_read_result;
        // is there a chance for an infinite loop? not really since read guarantees us never to get more bytes than we
        // asked for, so we can either error out or reach buffer_size exactly, as read won't return 0 (definitely more than
        // once, if socket should close externally).
    }

    return UDS_STATUS::SUCCESS;
}

UDS_STATUS UDSData::write_buffer(const uint8_t* buffer, size_t buffer_size) {
    UDS_STATUS status = _validate_wr_args(buffer);
    if (status != UDS_STATUS::SUCCESS) {
        printf("Write failed to buffer=[%p] buffer_size=[%zu]\n", buffer, buffer_size);
        return UDS_STATUS::INVALID_ARGUMENT;
    }

    size_t bytes_written = 0;
    int tmp_write_result = 0;

    while (bytes_written < buffer_size) {
        tmp_write_result = ::write(_fd, buffer + bytes_written, buffer_size - bytes_written);
        if (tmp_write_result < 0 && errno != EINTR) {
            printf("Write failed [%s]\n", strerror(errno));
            return UDS_STATUS::INTERNAL_ERROR;
        }

        if (tmp_write_result == 0) {
            printf("Write failed - connection reset by peer\n");
            return UDS_STATUS::CLOSED_SOCKET;
        }
        bytes_written += tmp_write_result;
    }

    return UDS_STATUS::SUCCESS;
}