#include "uds/uds_webserver.hpp"
#include "uds/uds_data.hpp"
#include "uds/uds_client.hpp"
#include "connect_data.h"

#include <thread>
#include <unistd.h>
#include <iostream>
#include <string>

static constexpr int NUM_RETRIES = 400;

int g_client_data_fd = -1;
UDSData g_data_socket;
int g_req = -1;

void create_client_and_connect() {

    for (int i = 0; i < NUM_RETRIES; i++) {
        try {
            UDSClient client(SOCK_PATH);
            g_client_data_fd = client.get_fd();
            g_data_socket.initialize(g_client_data_fd);
            printf("Client connection created with fd=%d\n", g_client_data_fd);
            return;
        } catch (const std::exception& e) {
            printf("Error: %s\n", e.what());
            if (i != NUM_RETRIES - 1) {
                uint32_t usec_sleep = (i + 1) * 1000;
                printf("Going to retry in %u useconds\n", usec_sleep);
                usleep(usec_sleep);
            }
        }
    }

    throw std::runtime_error("Client failed to connect");
}

void parse_cli_req() {
    printf("Please enter an integer or 42 to recreate connection\n");
    std::string line;
    std::getline(std::cin, line);
    g_req = std::atoi(line.c_str());
}

void send_next_req() {
    printf("client sending x = %d\n", g_req);
    UDS_STATUS status = g_data_socket.write(g_req);
    if (status != UDS_STATUS::SUCCESS) {
        printf("error on socket = %s", uds_status_to_string(status).c_str());
        if (status == UDS_STATUS::CLOSED_SOCKET) {
            printf("Going to re-create client and connect\n");
            create_client_and_connect();
        }
    }
}

void post_process_req() {
    if (g_req == RE_OPEN_CONNECTION) {
        int random_sleep = rand() % 5;
        printf("Got %d: going to listen to new connection in %d seconds",
                RE_OPEN_CONNECTION, random_sleep);
        fflush(stdout);
        sleep(random_sleep);
        create_client_and_connect();
    }
}

void client_socket() {
    create_client_and_connect();

    while (true) {
        parse_cli_req();

        send_next_req();

        post_process_req();
    }
}


int main() {
    printf("Client main\n");
    client_socket();
    return 0;
}