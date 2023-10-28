/*
 * Copyright (c) 2021 Chain-reaction LTD
 * All Rights Reserved
 *
 * This product is protected by copyright and distributed under
 * licenses restricting copying, distribution and decompilation.
 *
 */

#pragma once

#include <string>
#include "uds_status.hpp"

class UDSWebServer {
public:
    UDSWebServer(const std::string& socket_unix_path);
    ~UDSWebServer();
    UDSWebServer(const UDSWebServer& rhs) = delete;
    void operator=(const UDSWebServer& rhs) = delete;

    /**
     * @brief initailize the socket and bind it to the address give in the constructor
     *
     * @return UDS_STATUS enum status
     */
    UDS_STATUS initialize();

    /**
     * @brief
     *
     * @param data_socket_fd: output parameter for the data socket file descriptor
     * @return UDS_STATUS enum status
     */
    UDS_STATUS accept(int& data_socket_fd);
private:
    std::string _socket_unix_path{""};
    int _listen_socket_fd{-1};

    UDS_STATUS _close_listen_socket();

    bool _is_valid_uds_unix_path_size(const std::string& uds_unix_path);

};