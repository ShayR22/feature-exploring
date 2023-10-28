#pragma once


#include "uds_status.hpp"
#include <string>
#include <stdint.h>
#include <stddef.h>

class UDSData {
public:
    UDSData();
    ~UDSData();
    UDSData(const UDSData& rhs) = delete;
    void operator=(const UDSData& rhs) = delete;

    UDS_STATUS initialize(int fd);

    template <typename T>
    UDS_STATUS write(const T& element) {
        const uint8_t* element_ptr = reinterpret_cast<const uint8_t*>(&element);
        return write_buffer(element_ptr, sizeof(T));
    }

    /* in order to template specialization regardless of the compiler,
     * we must have a namespace (due to known bug++ doesn't allow non namespace template
     * specialization, this is part of the standard since C++17).
     */
    UDS_STATUS write(const std::string& element) {
        const uint8_t* element_ptr = reinterpret_cast<const uint8_t*>(element.data());
        size_t element_size = element.size();
        return write_buffer(element_ptr, element_size);
    }

    template <typename T>
    UDS_STATUS read(T& element) {
        uint8_t* element_ptr = reinterpret_cast<uint8_t*>(&element);
        return read_buffer(element_ptr, sizeof(T));
    }

    /**
     * @brief read operations on c socket
     *
     * @param buffer pointer to buffer of information to read
     * @param buffer_size buffer size
     * @return status enum
     */
    UDS_STATUS read_buffer(uint8_t* buffer, size_t buffer_size);

    /**
     * @brief write operations on c socket
     *
     * @param buffer pointer to buffer of information to write
     * @param buffer_size buffer size
     * @return status enum
     */
    UDS_STATUS write_buffer(const uint8_t* buffer, size_t buffer_size);

private:
    int _fd{-1};

    UDS_STATUS _validate_wr_args(const uint8_t* buffer);
};