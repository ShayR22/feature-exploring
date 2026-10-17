#include "gtest_wrapper/gtest_wrapper_reporter.h"
#include "gtest_wrapper/gtest_wrapper_runtime_configuration.h"
#include "gtest_wrapper/gtest_include_wrapper.h"
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <syslog.h>


GTestWrapperReporter& GTestWrapperReporter::get_instance() {
    static GTestWrapperReporter hermes_setup{};
    return hermes_setup;
}

void GTestWrapperReporter::setup_start() {
    std::stringstream test_setup_msg{};

    const std::string gtest_test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    const std::string test_name = this->get_report_test_name(gtest_test_name);

    test_setup_msg << "----------------------------- Captured logs start for: ";
    test_setup_msg << test_name;
    test_setup_msg << " -----------------------------";
    syslog(LOG_INFO, "%s", test_setup_msg.str().c_str());

    testing::internal::CaptureStdout();
    std::cout << " ----------------------------- Captured stdout setup -----------------------------"
              << std::endl;
}

void GTestWrapperReporter::setup_end() {
    std::cout << " ------------------------------ Captured stdout call -----------------------------"
              << std::endl
              << std::endl;
}

void GTestWrapperReporter::teardown_start() {
    std::cout << " ---------------------------- Captured stdout teardown ---------------------------"
              << std::endl;
}

void GTestWrapperReporter::teardown_end() {
    std::stringstream test_setup_msg{};

    const std::string gtest_test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    const std::string test_name = this->get_report_test_name(gtest_test_name);

    test_setup_msg << "----------------------------- Captured logs end for: ";
    test_setup_msg << test_name;
    test_setup_msg << " -----------------------------";
    syslog(LOG_INFO, "%s", test_setup_msg.str().c_str());

    this->make_test_report();
}

void GTestWrapperReporter::make_test_report() {
    const std::string test_stdout = ::testing::internal::GetCapturedStdout();
    std::cout << test_stdout;
    const char* keep_reports = std::getenv("GTEST_WRAPPER_KEEP_REPORTS");
    if (keep_reports == nullptr || std::string_view(keep_reports) != "1") {
        return;
    }
    const std::string gtest_test_suite_name = ::testing::UnitTest::GetInstance()->current_test_suite()->name();
    const std::string gtest_test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();

    const std::string report_directory_name = this->get_report_dir_name();

    const std::string test_name = this->get_report_test_name(gtest_test_name);
    const std::string test_suite_name = this->get_report_test_suite(gtest_test_suite_name);

    const std::string test_result_directory_path = report_directory_name + "/" + test_suite_name + "/";
    const std::string test_result_path = test_result_directory_path + test_name;

    std::filesystem::create_directories(test_result_directory_path);

    std::ofstream test_result_file(test_result_path);
    test_result_file << test_stdout;
}

std::string GTestWrapperReporter::get_report_test_suite(const std::string_view gtest_test_suite) {
    std::string report_test_suite_name{gtest_test_suite};

    // On parameterized test, the test suite name is "X/Y". We want only the X part
    // Extracts it here
    auto suite_slash_mark = report_test_suite_name.find('/');

    if (suite_slash_mark != std::string::npos) {
        report_test_suite_name.erase(suite_slash_mark);
    }

    return report_test_suite_name;
}

std::string GTestWrapperReporter::get_report_test_name(const std::string_view gtest_test_name) {
    constexpr auto FILTERS_MARK = "_FILTERS_";
    constexpr auto PARAMS_MARK = '@';

    std::string report_test_name{gtest_test_name};

    // On parameterized tests the name of the tests includes '/'.
    // In order to create file test report, we have to remove the '/'
    std::replace(report_test_name.begin(), report_test_name.end(), '/', PARAMS_MARK);

    // Removes the filters from the test name
    auto test_filter_mark = report_test_name.find(FILTERS_MARK);
    auto test_params_mark = report_test_name.find(PARAMS_MARK);

    if (test_filter_mark == std::string::npos) {
        throw std::runtime_error("Internal errors - Test doesn't contain filters");
    }

    if (test_params_mark == std::string::npos) {
        report_test_name.erase(test_filter_mark);
    }
    else {
        report_test_name.erase(test_filter_mark, test_params_mark-test_filter_mark);
    }

    return report_test_name;
}

std::string GTestWrapperReporter::get_report_dir_name() {
    static std::stringstream report_directory_name{};

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    if (report_directory_name.str().empty()) {
        report_directory_name << std::put_time(std::localtime(&in_time_t), "report_%d-%m-%Y_%H-%M-%S");
    }

    std::stringstream report_dir(report_directory_name.str(), std::ios_base::out | std::ios_base::ate);
    std::optional<uint32_t> session_idx  = GTestWrapperRuntimeConfiguration::get_looping_session_idx();

    if (session_idx.has_value()) {
        auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
        auto fraction = now - seconds;
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
        report_dir << "/loop";
        report_dir << std::to_string(session_idx.value());
        report_dir << std::put_time(std::localtime(&in_time_t), "_%H-%M-%S-");
        report_dir << std::to_string(milliseconds.count());
    }

    return report_dir.str();
}