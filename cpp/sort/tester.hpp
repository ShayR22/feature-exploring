#pragma once

#include <concepts>
#include <chrono>
#include <sstream>

static constexpr int64_t THOUSAND = 1000;
static constexpr int64_t MILLION = THOUSAND * THOUSAND;
static constexpr int64_t BILLION = MILLION * THOUSAND;
// ORDER MATTERS
static constexpr std::array<int64_t, 4> MEASUREMENTS_UNITS{BILLION, MILLION, THOUSAND, 1};
static constexpr std::array<std::string_view, 4> MEASUREMENTS_UNITS_STR{"seconds", "milliseconds", "microseonds", "nanoseconds"};

std::string to_string_with_precision(const std::floating_point auto a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

static uint32_t get_measurement_index(int64_t time_measurement) {
    for (auto i = 0; i < MEASUREMENTS_UNITS.size(); i++) {
        if (time_measurement > MEASUREMENTS_UNITS[i]) {
            return i;
        }
    }
    // fallback on smallest measurement unit
    return MEASUREMENTS_UNITS.size() - 1;
}

static std::string time_took_to_string(int64_t time_nano) {
    uint32_t measurement_index = get_measurement_index(time_nano);

    int64_t measurement = MEASUREMENTS_UNITS[measurement_index];
    double time_took = time_nano / (double)measurement;
    const auto measure_unit_str = MEASUREMENTS_UNITS_STR[measurement_index].data();
    const std::string time_took_round = to_string_with_precision(time_took);
    return "took " + time_took_round + " [" + measure_unit_str + "]";
}

static void print_results(int64_t total_time_nano, uint32_t num_calls, const std::string& func_name) {
    const std::string total_time_str =  time_took_to_string(total_time_nano);
    const std::string avg_time_str =  time_took_to_string(total_time_nano / num_calls);

    printf("function [%s]: num_calls=[%u], total time: %s, avg time: %s\n",
           func_name.c_str(), num_calls, total_time_str.c_str(), avg_time_str.c_str());
}

uint32_t _measure_total_execution_time(std::invocable auto& f, uint32_t num_calls=10000, const std::string& func_name="unnamed") {
    auto start_time = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < num_calls; i++) {
        f();
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    auto ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    print_results(ms_int.count(), num_calls, func_name);
    return ms_int.count();
}

uint32_t measure_total_execution_time(std::invocable auto& f, uint32_t num_calls=10000) {
    return _measure_total_execution_time(f, num_calls);
}

uint32_t measure_total_execution_time(const std::string& func_name, std::invocable auto& f, uint32_t num_calls=10000) {
    return _measure_total_execution_time(f, num_calls, func_name);
}

uint32_t measure_avg_execution_time(std::invocable auto& f, uint32_t num_calls=10000) {
    return _measure_total_execution_time(f, num_calls) / num_calls;
}

uint32_t measure_avg_execution_time(const std::string& func_name, std::invocable auto& f, uint32_t num_calls=10000) {
    return _measure_total_execution_time(f, num_calls, func_name) / num_calls;
}