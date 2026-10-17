#pragma once

#include <cstdint>
#include <string>

class GTestWrapperReporter {
public:
    static GTestWrapperReporter& get_instance();

    void setup_start();
    void setup_end();

    void teardown_start();
    void teardown_end();

private:

    void make_test_report();
    std::string get_report_test_suite(const std::string_view gtest_test_suite);
    std::string get_report_test_name(const std::string_view gtest_test_name);
    std::string get_report_dir_name();

    GTestWrapperReporter() = default;
    GTestWrapperReporter(const GTestWrapperReporter&) = delete;
    GTestWrapperReporter(GTestWrapperReporter&&) = delete;
    void operator=(const GTestWrapperReporter&) = delete;
    void operator=(GTestWrapperReporter&&) = delete;
};