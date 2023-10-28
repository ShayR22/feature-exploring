#include "uds_webserver.hpp"

#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <cerrno>

#include <unistd.h>
#include <signal.h>
#include <thread>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <stdint.h>

static constexpr size_t UNIX_PATH_MAX = 108;

UDSWebServer::UDSWebServer(const std::string& socket_unix_path) :
    _socket_unix_path{socket_unix_path} {
    // DO NOTHING
}

UDSWebServer::~UDSWebServer() {
    UDS_STATUS status = _close_listen_socket();
    if (status != UDS_STATUS::SUCCESS) {
        printf("Failed to clear listen socket\n");
    }
}

UDS_STATUS UDSWebServer::initialize() {


    const char* c_path = _socket_unix_path.c_str();

    if (!_is_valid_uds_unix_path_size(_socket_unix_path)) {
        printf("<- Invalid socket unix domain path [%s]\n", c_path);
        return UDS_STATUS::INVALID_ARGUMENT;
    }

    UDS_STATUS status = _close_listen_socket();
    if (status != UDS_STATUS::SUCCESS) {
        printf("<- Failed to old listen socket\n");
        return status;
    }

    _listen_socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if (_listen_socket_fd < 0) {
        printf("<- Failed socket error=[%d]: [%s]\n", errno, strerror(errno));
        return UDS_STATUS::INTERNAL_ERROR;
    }

    struct sockaddr_un address{0};

    address.sun_family = AF_UNIX;

    strcpy(address.sun_path, c_path);

    int err = bind(_listen_socket_fd, (struct sockaddr*)&address, sizeof(struct sockaddr_un));
    if (err) {
        printf("<- Failed to bind newly created socket (path: [%s]) into address, error=[%d]: [%s]",
              c_path,
              errno,
              strerror(errno));
        return UDS_STATUS::INTERNAL_ERROR;
    }

    // we only allow one connection reactiongate shouldn't have multiple user, this is not negotiable (hence not a define)
    err = listen(_listen_socket_fd, 1);
    if (err) {
        printf("<- Failed to set newly created socket (path: [%s]) to listen to 1 connection\n", c_path);
        return UDS_STATUS::INTERNAL_ERROR;
    }

    return UDS_STATUS::SUCCESS;
}

UDS_STATUS UDSWebServer::accept(int& data_socket_fd) {

    if (_listen_socket_fd == -1) {
        printf("<- Trying to accept socket while not initialized\n");
        return UDS_STATUS::INTERNAL_ERROR;
    }

    struct sockaddr_un address{0};
    socklen_t address_length = sizeof(address);

    int data_fd = ::accept(_listen_socket_fd, (struct sockaddr*)&address, &address_length);
    if (data_fd == -1) {
        printf("<- Accept failed, error=[%d]: [%s]\n", errno, strerror(errno));
        return UDS_STATUS::INTERNAL_ERROR;
    }

    // small optimization - send immediately, disable any sort of buffering - makes no sense.
    int flags = 1;
    int err = setsockopt(data_fd, SOL_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));
    /* when setting the socket setsockopt does select and able to wake up on file descriptors which he has no privilege
     * and/or doesn't support operation on, thus, although succeeding in the command user set out to do, it can fail
     * on another socket
     */
    if (err && errno != EOPNOTSUPP) {
        printf("<- Failed to disable buffering from the socket=[%d] error=[%d] errno=[%d]: [%s]",
              data_fd,
              err,
              errno,
              strerror(errno));
        return UDS_STATUS::INTERNAL_ERROR;
    }

    data_socket_fd = data_fd;


    return UDS_STATUS::SUCCESS;
}

UDS_STATUS UDSWebServer::_close_listen_socket() {
    const char* c_path = _socket_unix_path.c_str();
    int err = unlink(c_path);
    // check if there is an error and the error is not file not found
    if (err != 0 && errno != ENOENT) {
        printf("Failed to unlink [%s], error [%d]: [%s]\n", c_path, err, strerror(errno));
        return UDS_STATUS::INTERNAL_ERROR;
    }

    if (_listen_socket_fd == -1) {
        return UDS_STATUS::SUCCESS;
    }

    err = close(_listen_socket_fd);
    if (err) {
        printf("Failed to close _listen_socket_fd=[%d] bind on path=[%s]\n", _listen_socket_fd, c_path);
        return UDS_STATUS::INTERNAL_ERROR;
    }

    return UDS_STATUS::SUCCESS;
}

bool UDSWebServer::_is_valid_uds_unix_path_size(const std::string& uds_unix_path) {
    // + 1 is for null termintaing string
    if (uds_unix_path.size() + 1 <= UNIX_PATH_MAX) {
        return true;
    }

    return false;
}