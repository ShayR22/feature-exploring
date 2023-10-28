#pragma once

#include <source_location>
#include <chrono>
#include "timestamp_collector.hpp"

class TimeStamp {
public:
    TimeStamp(const std::source_location loc = std::source_location::current()) {
        _func_name = loc.function_name();
        _start = std::chrono::high_resolution_clock::now();
        printf("function name=[%s], epoch=[%s]\n", _func_name, std::ctime(_start.time_since_epoch()));
    }

    ~TimeStamp() {
        auto end = std::chrono::high_resolution_clock::now();
        auto time_passed_us = std::chrono::duration_cast<std::chrono::microseconds>(end - _start).count();
        printf("time_passed_us=[%ld]\n", time_passed_us);
        TimeStampCollector::get_instance().add_timestamp(_func_name, time_passed_us);
    }

private:
    const char* _func_name{nullptr};
    std::chrono::high_resolution_clock::time_point _start;
};

