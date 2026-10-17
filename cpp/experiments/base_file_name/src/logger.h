#pragma once

#include <stdio.h>
#include <string_view>

#define ERROR(format, ...)  printf("ERR: %s:%d " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define WARN(format, ...)   printf("WARN: %s:%d " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define INFO(format, ...)   printf("INFO: %s:%d " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define DBG(format, ...)    printf("DBG: %s:%d " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define XSTR(a) STR(a)
#define STR(a) #a

#define __LINE_AS_STR__ XSTR(__LINE__)


consteval auto as_constant(auto value) {
    return value;
}

// wrapper template class for padded string, this would be used by logger, so it won't require
// string allocation and memcpy of the data on each log called.
template<size_t N>
class CompileTimePadString {
public:
    constexpr CompileTimePadString(const char* full_path) {
        std::string_view path{full_path};

        size_t num_chars_copy = std::min(N - 1, path.length());
        size_t num_space_char_padding = N - 1 - num_chars_copy;

        for (size_t i = 0; i < num_space_char_padding; i++) {
            _str[i] = ' ';
        }

        for (size_t i = 0; i < num_chars_copy; i++) {
            _str[num_space_char_padding + i] = path[i];
        }

        _str[N-1] = '\0';
    }

    constexpr const char* str() const {
        return _str;
    }

private:
    char _str[N] {'\0', };
};

// wrapper template class for padded string, this would be used by logger, so it won't require
// string allocation and memcpy of the data on each log called.
// this template is for <file>:<line number>
template<size_t N>
class CompileTimeFilePadString {
public:
    constexpr CompileTimeFilePadString(const char* full_path, const char* line_number_str) {
        std::string_view full_path_view{full_path};
        std::string_view path_view = _get_file_base_name_no_extension(full_path_view);
        std::string_view line_num_view{line_number_str};

        // + 1 for the ':'
        size_t line_num_length = line_num_view.length() + 1;

        // static_assert(N-1 > line_num_length, "string length can't hold at least :<line_number>");
        size_t num_file_char_copy = std::min(N - 1 - line_num_length, path_view.length());

        size_t total_string_length = num_file_char_copy + line_num_length;
        size_t num_space_char_padding = N - 1 - total_string_length;

        for (size_t i = 0; i < num_space_char_padding; i++) {
            _str[i] = ' ';
        }

        for (size_t i = 0; i < num_file_char_copy; i++) {
            _str[num_space_char_padding + i] = path_view[i];
        }

        _str[num_space_char_padding + num_file_char_copy] = ':';
        // start from 1 to skip ':'
        for (size_t i = 1; i < line_num_length; i++) {
            _str[num_space_char_padding + num_file_char_copy + i] = line_num_view[i - 1];
        }

        _str[N-1] = '\0';
    }

    constexpr const char* str() const {
        return _str;
    }

private:
    char _str[N] {'\0', };

    constexpr size_t _get_file_base_name_no_extension_length(const std::string_view& path) const {
        // if path is /x/y/my_cool_file.cpp we would like to return length of my_cool_file
        // which equals to last '.' - '/' - 1
        return path.find_last_of('.') - path.find_last_of('/') - 1;
    }

    constexpr std::string_view _get_file_base_name_no_extension(const std::string_view& path) const {
        size_t length = _get_file_base_name_no_extension_length(path);
        // + 1 in order for name not to start with '/'
        return path.substr(path.find_last_of('/') + 1, length);
    }
};



// consteval is a C++20 flag for the compiler to assert in compile time that the function
// is only evalutated in compile time, meaning it won't compile if used with "unknown at compile time"
// arguments.
// in this scope it is used to assert that all logs file path call reformatting are done in compile time
// as to not affect runtime, as this function runetime complexity is O(n) where n is the number of character
// of the full file path or template value N
template<size_t N>
static consteval auto generate_pad_string(const char* path) {
    // + 1 for string null termination
    return CompileTimePadString<N+1>{path};
}

template<size_t N>
static consteval auto generate_file_pad_string(const char* path, const char* line_number_str) {
    // + 1 for string null termination
    return CompileTimeFilePadString<N+1>{path, line_number_str};
}

static constexpr auto __MODULE__ = "TEST";
static constexpr auto __LOG_MODULE_LENGTH__ = 10;
static constexpr auto __MODULE_NAME__ = generate_pad_string<__LOG_MODULE_LENGTH__>(__MODULE__);

static constexpr auto __LOG_FILE_LENGTH__ = 30;
static constexpr auto __BASE_FILE_NAME__ = generate_file_pad_string<__LOG_FILE_LENGTH__>(__BASE_FILE__, __LINE_AS_STR__);

#define FO_ERROR(format, ...)  printf("ERR: %s%s " format "\n", __MODULE_NAME__.str(),__BASE_FILE_NAME__.str(), ##__VA_ARGS__)
#define FO_WARN(format, ...)   printf("WARN: %s%s  " format "\n", __MODULE_NAME__.str(),__BASE_FILE_NAME__.str(), ##__VA_ARGS__)
#define FO_INFO(format, ...)   printf("INFO: %s%s  " format "\n", __MODULE_NAME__.str(),__BASE_FILE_NAME__.str(), ##__VA_ARGS__)
#define FO_DBG(format, ...)    printf("DBG: %s%s " format "\n", __MODULE_NAME__.str(),__BASE_FILE_NAME__.str(), ##__VA_ARGS__)