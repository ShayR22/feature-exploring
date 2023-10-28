#pragma once

#include <algorithm>
#include <map>
#include <vector>
#include <cstdint>
#include <cstdio>
#include <numeric>

class TimeStampCollector {
public:
    static TimeStampCollector get_instance() {
        TimeStampCollector tsc;
        return tsc;
    }

    void add_timestamp(const char* func_name, int64_t time_passed_micro) {
        printf("func_name=[%s] time_passed_micro=[%ld]\n", func_name, time_passed_micro);
        _func_name_to_benchmark_micro[func_name].push_back(time_passed_micro);
    }

    void dump() {
        printf("==================================================\n");
        for (const auto& [func_name, executions]: _func_name_to_benchmark_micro) {
            float worst_time = *std::min_element(executions.begin(), executions.end());
            float avg_time = std::accumulate(executions.begin(), executions.end(), 0.0) / executions.size();
            printf("%s num_execs=[%zu] worst_time=[%f] avg_time=[%f]\n",
                   func_name, executions.size(), worst_time, avg_time);
        }
        printf("==================================================\n");
    }

private:
    TimeStampCollector() = default;
    std::map<const char*, std::vector<int64_t>> _func_name_to_benchmark_micro;
};