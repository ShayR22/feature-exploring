/*
 * Copyright (c) 2021 Chain-reaction LTD
 * All Rights Reserved
 *
 * This product is protected by copyright and distributed under
 * licenses restricting copying, distribution and decompilation.
 *
 */

#pragma once

#include <cmath>
#include <cstring>
#include <execinfo.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>


// in order for stringstream to print int8_t/uint8_t which are signed and unsigned char
// we would override operator<< adding the namespace, and using namespace numerical_chars before printing
namespace numerical_chars {
inline std::ostream &operator<<(std::ostream &os, signed char c) {
    return os << static_cast<int32_t>(c);
}

inline std::ostream &operator<<(std::ostream &os, unsigned char c) {
    return os << static_cast<uint32_t>(c);
}
} // namespace numerical_chars


// sizeof("4294967295") - 1
static const uint8_t calc_uint32_length_as_str(uint32_t num) {
    return static_cast<uint8_t>(std::log10(num)) + 1;
}

static const uint8_t LEN_MAX_UINT32_AS_STR{calc_uint32_length_as_str(UINT32_MAX)};
static const std::string_view SIZE_FMT{":\nsize=["};

template <typename T>
size_t calc_buf_to_str_size(const char* buffer_name, size_t len, bool is_hex=true) {
    const uint8_t nibble_size = sizeof(T) * 2;

    size_t buffer_size = len;
    if (is_hex) {
        // Adjust for std::setw and the space after
        buffer_size *= (nibble_size + 1);
    } else {
        // Adjust for non hex printing with opening and closing brackets and a space after
        buffer_size *= (LEN_MAX_UINT32_AS_STR + 3);
    }

    // Adjust for buffer name
    buffer_size += std::strlen(buffer_name);

    // Adjust for printing size and buffer
    buffer_size += SIZE_FMT.size() + LEN_MAX_UINT32_AS_STR;

    // Adjust for the last newline
    buffer_size += 1;

    return buffer_size;
}

template <typename T>
std::string buf_to_str(const char* buffer_name, const T* elements, size_t len, bool is_hex=true) {
    // Calculate buffer size to prevent reallocation later on
    const size_t buffer_size = calc_buf_to_str_size<T>(buffer_name, len, is_hex);

    // Only used for preallocation, DO NOT use after creation of string_buffer
    std::string _s{};
    _s.reserve(buffer_size);
    std::stringstream string_buffer{std::move(_s)};

    string_buffer << buffer_name
                  << SIZE_FMT
                  << len
                  << "] [";

    const uint8_t nibble_size = sizeof(T) * 2;

    if (is_hex) {
        string_buffer << std::uppercase
                      << std::setfill('0')
                      << std::setw(nibble_size)
                      << std::hex;

        for (uint32_t i = 0; i < len; i++) {
            string_buffer << static_cast<uint32_t>(elements[i])
                          << " ";
        }
    } else {
        using namespace numerical_chars;
        for (uint32_t i = 0; i < len; i++) {
            string_buffer << elements[i] << ' ';
        }
    }

    string_buffer << ']';

    return string_buffer.str();
}

template <typename T>
std::string buf_to_str(const char* buffer_name, const std::vector<T>& buffer, bool is_hex=true) {
    return buf_to_str(buffer_name, buffer.data(), buffer.size(), is_hex);
}