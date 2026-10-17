#include "gtest_wrapper/gtest_wrapper_runner.h"
#include "gtest_wrapper/gtest_wrapper_runtime_configuration.h"
#include <chrono>

using std::chrono::high_resolution_clock;

bool GTestWrapperRunner::_is_max_session() {
    return _session_idx >= MAX_SESSION_IDX;
}

bool GTestWrapperRunner::_time_passed(high_resolution_clock::time_point& start, uint32_t tests_duration_sec) {
    high_resolution_clock::time_point now = high_resolution_clock::now();
    uint32_t seconds_passsed = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::seconds>(now - start).count());

    return seconds_passsed >= tests_duration_sec;
}

int GTestWrapperRunner::_run_tests_in_loop(uint32_t tests_duration_sec) {
    int err = 0;
    uint32_t _session_idx = START_SESSION_IDX;
    high_resolution_clock::time_point start = high_resolution_clock::now();

    while (!_time_passed(start, tests_duration_sec) && !_is_max_session()) {
        std::cout << "Starting to Run Loop " << _session_idx << std::endl;
        GTestWrapperRuntimeConfiguration::set_looping_session_idx(_session_idx++);
        err |= RUN_ALL_TESTS();
    }

    // update err in case max session occured (0 success 1 error)
    err |= _is_max_session();

    return err;
}

GTestWrapperRunner& GTestWrapperRunner::get_instance() {
    static GTestWrapperRunner runner;
    return runner;
}

int GTestWrapperRunner::run(int argc, char **argv, ::testing::Environment& environment) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(&environment);

    std::optional<uint32_t> tests_duration_sec = GTestWrapperRuntimeConfiguration::get_looping_duration_sec();

    if (!tests_duration_sec.has_value()) {
        return RUN_ALL_TESTS();
    }

    return _run_tests_in_loop(tests_duration_sec.value());
}