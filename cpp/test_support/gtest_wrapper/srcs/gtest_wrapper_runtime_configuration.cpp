#include "gtest_wrapper/gtest_wrapper_runtime_configuration.h"

#include <ctime>
#include <iostream>
#include <limits.h>
#include <optional>
#include <stdarg.h>
#include <stdlib.h>
#include <string>
#include <syslog.h>
#include <unordered_map>

std::optional<uint32_t> GTestWrapperRuntimeConfiguration::get_random_seed() {
    const char* random_seed_cstr = secure_getenv("RANDOM_SEED");

    if (random_seed_cstr == nullptr) {
        return std::nullopt;
    }

    const std::optional<uint32_t> rv = std::atoi(random_seed_cstr);
    return rv;
}

void GTestWrapperRuntimeConfiguration::set_looping_session_idx(uint32_t session_idx) {
    GTestWrapperRuntimeConfiguration::looping_session_idx = session_idx;
}

std::optional<uint32_t> GTestWrapperRuntimeConfiguration::get_looping_session_idx() {
    return GTestWrapperRuntimeConfiguration::looping_session_idx;
}

std::optional<uint32_t> GTestWrapperRuntimeConfiguration::get_looping_duration_sec() {
    const char* looping_duration_cstr = secure_getenv("LOOPING_DURATION");

    if (looping_duration_cstr == nullptr) {
        return std::nullopt;
    }

    const std::string looping_duration_str{looping_duration_cstr};

    const std::optional<uint32_t> rv = GTestWrapperRuntimeConfiguration::_parse_looping_duration_to_sec(looping_duration_str);

    return rv;
}


uint32_t GTestWrapperRuntimeConfiguration::_parse_looping_duration_to_sec(const std::string& looping_duration_str) {
    auto throw_invalid_arguments = [](){
        throw std::invalid_argument("Please set GTestWrapperRuntimeConfiguration variable LOOPING_DURATION=[<number>[h|m|s] | '']");
    };

    const std::unordered_map<char, uint32_t> type_to_sec = {
        {'s', 1},
        {'m', 60},
        {'h', 3600}
    };

    float duration;
    char type;

    if (sscanf(looping_duration_str.c_str(), "%f%c", &duration, &type) != 2) {
        throw_invalid_arguments();
    }

    const auto pos = type_to_sec.find(type);

    if (pos == type_to_sec.end()) {
        throw_invalid_arguments();
    }

    const uint32_t looping_duration_sec = static_cast<uint32_t>(static_cast<float>(pos->second) * duration);
    std::cout << looping_duration_sec << std::endl;
    return looping_duration_sec;
}

std::optional<uint32_t> GTestWrapperRuntimeConfiguration::looping_session_idx = std::nullopt;