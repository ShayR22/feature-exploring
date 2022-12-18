#include "uds/uds_webserver.hpp"
#include "uds/uds_data.hpp"
#include "uds/uds_client.hpp"

#include <thread>
#include <unistd.h>
#include <variant>

const std::string SOCK_PATH = "/tmp/ggwp";

using sn = std::variant<int, double>;

const char* get_variant_type_str(const sn& v) {
    int index = v.index();
    switch(index) {
        case 0:
            return "int";
        case 1:
            return "double";
    }
    return "invalid";
}

void client_socket() {
    const int CLIENT_SLEEP_SEC = 1;
    sleep(5);
    printf("after sleep %d\n", CLIENT_SLEEP_SEC);

    UDSClient client(SOCK_PATH);
    int fd = client.get_fd();

    UDSData data_socket;
    data_socket.initialize(fd);

    sn x = 5.5;
    printf("client sending x = %lf\n", std::get<double>(x));
    data_socket.write(x);
}

void server_socket() {
    UDSWebServer sock{SOCK_PATH};
    sock.initialize();

    int data_sock = -1;
    sock.accept(data_sock);

    UDSData data_socket;
    data_socket.initialize(data_sock);

    sn x = -1;
    data_socket.read(x);

    printf("server got type=%s\n", get_variant_type_str(x));

}


int main() {
    printf("VariantSocket main\n");

    std::thread server{server_socket};
    std::thread client{client_socket};

    client.join();
    server.join();
    return 0;
}