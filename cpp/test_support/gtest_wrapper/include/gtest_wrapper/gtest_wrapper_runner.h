
#pragma once

#include "gtest_wrapper/gtest_include_wrapper.h"
#include <chrono>
#include <string>

class GTestWrapperRunner {
public:
    static GTestWrapperRunner& get_instance();
    int run(int argc, char** argv, ::testing::Environment& enviornment);

private:
    uint32_t _session_idx;

    bool _is_max_session();
    bool _time_passed(std::chrono::high_resolution_clock::time_point& start, uint32_t tests_duration_sec);
    int _run_tests_in_loop(uint32_t tests_duration_sec);

    GTestWrapperRunner() = default;
};