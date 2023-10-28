
/**
 * @file
 * @brief Provides functions for measuring the execution time of a given function.
 * This file contains several functions that can be used to measure the total and average execution time
 * of a given function in milliseconds/microseconds/nanoseconds.
 * The functions use C++11's chrono library to accurately measure the time taken by a function.
 * The results are then printed in a human-readable format, indicating the total and average execution time,
 * as well as the number of times the function was called.
 */

#pragma once

#include <cstdint>
#include <concepts>
#include <chrono>
#include <sstream>
#include <array>

static constexpr int64_t THOUSAND = 1000;
static constexpr int64_t MILLION = THOUSAND * THOUSAND;
static constexpr int64_t BILLION = MILLION * THOUSAND;
// ORDER MATTERS
static constexpr std::array<int64_t, 4> MEASUREMENTS_UNITS{BILLION, MILLION, THOUSAND, 1};
static constexpr std::array<std::string_view, 4> MEASUREMENTS_UNITS_STR{
    "seconds", "milliseconds", "microseonds", "nanoseconds"
};

std::string _to_string_with_precision(const std::floating_point auto a_value, const int n = 3) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

static uint32_t _get_measurement_index(int64_t time_measurement) {
    for (size_t i = 0; i < MEASUREMENTS_UNITS.size(); i++) {
        if (time_measurement > MEASUREMENTS_UNITS[i]) {
            return i;
        }
    }

    // fallback on smallest measurement unit
    return MEASUREMENTS_UNITS.size() - 1;
}

static std::string _time_took_to_string(int64_t time_nano) {
    uint32_t measurement_index = _get_measurement_index(time_nano);

    int64_t measurement = MEASUREMENTS_UNITS[measurement_index];
    double time_took = time_nano / (double)measurement;
    const auto measure_unit_str = MEASUREMENTS_UNITS_STR[measurement_index].data();
    const std::string time_took_round = _to_string_with_precision(time_took);
    return "took " + time_took_round + " [" + measure_unit_str + "]";
}

static void _print_results(int64_t total_time_nano, uint32_t num_calls, const std::string& func_name) {
    const std::string total_time_str =  _time_took_to_string(total_time_nano);
    const std::string avg_time_str =  _time_took_to_string(total_time_nano / num_calls);

    printf("function [%s]: num_calls=[%u], total time: %s, avg time: %s\n",
              func_name.c_str(), num_calls, total_time_str.c_str(), avg_time_str.c_str());
}

/**
 * @brief Measures the total execution time of a given function in nanoseconds.
 *
 * @param func A callable object (e.g. a function, functor, or lambda) to be measured.
 * @param num_calls Number of times to call the function. Default is 10000.
 * @param func_name Name of the function being measured. Default is "unnamed".
 * @return The total execution time of the function in nanoseconds.
*/
static uint32_t _measure_total_execution_time_nano(std::invocable auto& func,
                                                   uint32_t num_calls=1000,
                                                   const std::string& func_name="unnamed") {
    auto start_time = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < num_calls; i++) {
        func();
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    auto ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    _print_results(ms_int.count(), num_calls, func_name);
    return ms_int.count();
}

// NANO MEASURMENNTS
static uint32_t measure_total_execution_time_nano(std::invocable auto& func, uint32_t num_calls=1000) {
    return _measure_total_execution_time_nano(func, num_calls);
}

static uint32_t measure_total_execution_time_nano(const std::string& func_name,
                                                  std::invocable auto& func,
                                                  uint32_t num_calls=1000) {
    return _measure_total_execution_time_nano(func, num_calls, func_name);
}

static uint32_t measure_avg_execution_time_nano(std::invocable auto& func, uint32_t num_calls=1000) {
    return _measure_total_execution_time_nano(func, num_calls) / num_calls;
}

static uint32_t measure_avg_execution_time_nano(const std::string& func_name,
                                                std::invocable auto& func,
                                                uint32_t num_calls=1000) {
    return _measure_total_execution_time_nano(func, num_calls, func_name) / num_calls;
}

// MICRO MEASURMENNTS
static uint32_t measure_total_execution_time_micro(std::invocable auto& func, uint32_t num_calls=1000) {
    return measure_total_execution_time_nano(func, num_calls) / THOUSAND;
}

static uint32_t measure_total_execution_time_micro(const std::string& func_name,
                                                   std::invocable auto& func,
                                                   uint32_t num_calls=1000) {
    return measure_total_execution_time_nano(func_name, func, num_calls) / THOUSAND;
}

static uint32_t measure_avg_execution_time_micro(std::invocable auto& func, uint32_t num_calls=1000) {
    return measure_avg_execution_time_nano(func, num_calls) / THOUSAND;
}

static uint32_t measure_avg_execution_time_micro(const std::string& func_name,
                                                 std::invocable auto& func,
                                                 uint32_t num_calls=1000) {
    return measure_avg_execution_time_nano(func_name, func, num_calls) / THOUSAND;
}

// MILLI MEASURMENNTS
static uint32_t measure_total_execution_time_milli(std::invocable auto& func, uint32_t num_calls=1000) {
    return measure_total_execution_time_micro(func, num_calls) / THOUSAND;
}

static uint32_t measure_total_execution_time_milli(const std::string& func_name,
                                                   std::invocable auto& func,
                                                   uint32_t num_calls=1000) {
    return measure_total_execution_time_micro(func_name, func, num_calls) / THOUSAND;
}

static uint32_t measure_avg_execution_time_milli(std::invocable auto& func, uint32_t num_calls=1000) {
    return measure_avg_execution_time_micro(func, num_calls) / THOUSAND;
}

static uint32_t measure_avg_execution_time_milli(const std::string& func_name,
                                                 std::invocable auto& func,
                                                 uint32_t num_calls=1000) {
    return measure_avg_execution_time_micro(func, num_calls, func_name) / THOUSAND;
}

// NOLINTBEGIN(*)
#define RUN_TEST(func)                                          \
    do {                                                        \
        measure_avg_execution_time_nano(#func, func);           \
    } while(0)
// NOLINTEND(*)