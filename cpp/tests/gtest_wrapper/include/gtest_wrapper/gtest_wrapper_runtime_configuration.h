#pragma once

#include <string>
#include <optional>

constexpr uint32_t START_SESSION_IDX = 1;
/**
 * @brief max session idx is the maximum number of loop iterations tlab
 * insfrastructure allow, this is mainly to prevent an empty test to run
 * in loops until disk is filled and require manual removal using ssh.
 */
constexpr uint32_t MAX_SESSION_IDX = 100000;

class GTestWrapperRuntimeConfiguration {

public:
    GTestWrapperRuntimeConfiguration() = delete;

    static void set_looping_session_idx(uint32_t session_idx);
    static std::optional<uint32_t> get_looping_session_idx();
    static std::optional<uint32_t> get_looping_duration_sec();
    static std::optional<uint32_t> get_random_seed();

private:
    static std::optional<uint32_t> looping_session_idx;

    static uint32_t _parse_looping_duration_to_sec(const std::string& looping_duration_str);
};