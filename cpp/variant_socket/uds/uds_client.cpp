
#include "uds_client.hpp"
#include <stdexcept>

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

UDSClient::UDSClient(const std::string& path) {
    int socket_fd;
	struct sockaddr_un address;

	socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		throw std::runtime_error("failed to create socket\n");
	}

	/* start with a clean address structure */
	memset(&address, 0, sizeof(struct sockaddr_un));

	address.sun_family = AF_UNIX;
    sprintf(address.sun_path, path.c_str());

	if (connect(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr_un))) {
		throw std::runtime_error("failed to connect socket\n");
	}

	// small optimization - send immediately, disable any sort of buffering - makes no sense.
	int flags =1;
	setsockopt(socket_fd, SOL_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));

    _fd = socket_fd;
}

int UDSClient::get_fd() {
    return _fd;
}
