#pragma once

#include <stdio.h>
#include <string_view>

#define ERROR(format, ...)  printf("ERR: %s:%d " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define WARN(format, ...)   printf("WARN: %s:%d " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define INFO(format, ...)   printf("INFO: %s:%d " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define DBG(format, ...)    printf("DBG: %s:%d " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)

consteval auto as_constant(auto value) {
    return value;
}

// wrapper template class for padded string, this would be used by logger, so it won't require
// string allocation and memcpy of the data on each log called.
template<size_t N>
class CompileTimePathPadString {
public:
    constexpr CompileTimePathPadString(const char* full_path) {
        std::string_view full_path_view{full_path};
        std::string_view path = _get_file_base_name_no_extension(full_path_view);

        // copy string
        size_t i = 0;
        for (auto it = path.cbegin(); i + 1 < sizeof(_str) && it != path.cend(); i++, it++) {
            _str[i] = *it;
        }

        // pad spaces
        while(i < N) {
            _str[i] = ' ';
            i++;
        }

        // ensure string terminator
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
    return CompileTimePathPadString<N+1>{path};
}

static constexpr auto __LOG_FILE_LENGTH__ = 30;
static constexpr auto __BASE_FILE_NAME__ = generate_pad_string<__LOG_FILE_LENGTH__>(__BASE_FILE__);

#define FO_ERROR(format, ...)  printf("ERR: %s:%d " format " FO_\n", __BASE_FILE_NAME__.str(), __LINE__, ##__VA_ARGS__)
#define FO_WARN(format, ...)   printf("WARN: %s:%d " format " FO_\n", __BASE_FILE_NAME__.str(), __LINE__, ##__VA_ARGS__)
#define FO_INFO(format, ...)   printf("INFO: %s:%d " format " FO_\n", __BASE_FILE_NAME__.str(), __LINE__, ##__VA_ARGS__)
#define FO_DBG(format, ...)    printf("DBG: %s:%d " format " FO_\n", __BASE_FILE_NAME__.str(), __LINE__, ##__VA_ARGS__)