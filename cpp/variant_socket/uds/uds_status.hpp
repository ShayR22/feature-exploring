#pragma once

#include <string>

enum class UDS_STATUS {
    SUCCESS,
    INVALID_ARGUMENT,
    CLOSED_SOCKET,
    INTERNAL_ERROR
};

static std::string uds_status_to_string(UDS_STATUS status) {
    switch (status) {
        case UDS_STATUS::SUCCESS: {
            return "Success";
        }
        case UDS_STATUS::INVALID_ARGUMENT: {
            return "Invalid Argument";
        }
        case UDS_STATUS::CLOSED_SOCKET: {
            return "Closed Socket";
        }
        case UDS_STATUS::INTERNAL_ERROR: {
            return "Internal Error";
        }
    }

    return "N/A";
}