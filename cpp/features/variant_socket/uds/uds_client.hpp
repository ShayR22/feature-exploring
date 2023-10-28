#pragma once

#include <string>

class UDSClient {
public:
    UDSClient(const std::string& path);

    int get_fd();
private:
    int _fd = -1;
};