#pragma once

#include <concepts>
#include <chrono>

template<std::invocable F>
uint32_t measure_avg_execution_time(F& f, uint32_t num_calls=10000) {

    auto start_time = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < num_calls; i++) {
        f();
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    auto ms_int = std::chrono::duration_cast< std::chrono::milliseconds>(end_time - start_time);
    uint32_t avg_call_ms = ms_int.count() / num_calls;

    printf("function [num_calls=%u] avg call time: %u [milliseconds], total_time = %lu [milliseconds]\n",
           num_calls, avg_call_ms, ms_int.count());
    return avg_call_ms;
}

template<std::invocable F>
uint32_t measure_total_execution_time(F& f, uint32_t num_calls=10000) {

    auto start_time = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < num_calls; i++) {
        f();
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    auto ms_int = std::chrono::duration_cast< std::chrono::milliseconds>(end_time - start_time);
    printf("function [num_calls=%u] total call time: %u [milliseconds]\n", num_calls, ms_int.count());
    return ms_int.count();
}