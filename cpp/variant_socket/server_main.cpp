#include "uds/uds_webserver.hpp"
#include "uds/uds_data.hpp"
#include "uds/uds_client.hpp"
#include "uds/uds_status.hpp"
#include "connect_data.h"

#include <thread>
#include <unistd.h>

int g_data_sock_fd = -1;
UDSData g_data_socket;
int g_req = -1;

// create web server and accept on socket, return data socket fd
void create_web_server_and_listen() {
    while (true) {
        UDSWebServer sock{SOCK_PATH};
        sock.initialize();

        int g_data_sock_fd = -1;
        sock.accept(g_data_sock_fd);
        if (g_data_sock_fd != -1) {
            printf("New data connection at fd = %d\n", g_data_sock_fd);
            g_data_socket.initialize(g_data_sock_fd);
            return;
        } else {
            printf("Failed at accepting socket, going retry establish links\n");
        }
    }
}



void handle_request() {
    printf("req=%d\n", g_req);
    if (g_req == RE_OPEN_CONNECTION) {
        create_web_server_and_listen();
    }
}

void recv_next_req() {
    printf("Going to wait on next int request\n");
    UDS_STATUS status = g_data_socket.read(g_req);
    if (status != UDS_STATUS::SUCCESS) {
        printf("error on socket = %s", uds_status_to_string(status).c_str());
        if (status == UDS_STATUS::CLOSED_SOCKET) {
            printf("Going to listen to new connection\n");
            create_web_server_and_listen();
        }
    }
}

void server_socket() {
    create_web_server_and_listen();
    while (true) {
        recv_next_req();
        handle_request();
    }
}


int main() {
    printf("ServerSocket main\n");

    server_socket();
    return 0;
}