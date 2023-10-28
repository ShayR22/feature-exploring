#include <thread>
#include <stdio.h>
#include <vector>
#include <cmath>
#include "../func_measuring/tester.hpp"

static constexpr uint32_t KB_SIZE_BYTES = 1024;
static constexpr uint32_t MB_SIZE_BYTES = KB_SIZE_BYTES * KB_SIZE_BYTES;


static uint32_t size_mb = 1;
static uint32_t num_calls = 0;

static void test_allocator() {
    uint32_t num_elements = size_mb * MB_SIZE_BYTES / sizeof(double);
    printf("num_elemnets = %u, size_mb = %u\n", num_elements, size_mb);


    // initialize non fragment double array
    auto doubles_non_fragment = std::make_unique<double[]>(num_elements);
    for (int i = 0; i < num_elements; i++) {
        doubles_non_fragment[i] = i;
    }

    auto heavy_math_operation = [&](uint32_t index, double value, double& sum) {
        sum += value;
        if (index % 1000 == 0) {
            sum = sqrt(sum);
        }
        if (index % 900 == 0) {
            sum = pow(sum, 1.5);
        }
        if (index % 3 == 0) {
            sum += 17;
        }
        if (index % 5 == 0) {
            sum -= 20;
        }
    };

    // create non fragment sum lambda to test
    auto non_fragment_sum = [&]() -> double {
        double rv = 0;
        for (uint32_t i = 0; i < num_elements; i++) {
            heavy_math_operation(i, doubles_non_fragment[i], rv);
        }
        return rv;
    };

    // initialize fragment double array
    std::vector<std::unique_ptr<double>> doubles_fragment;
    for (int i = 0; i < num_elements; i++) {
        doubles_fragment.emplace_back(std::make_unique<double>(i+1.0));
    }
    // create fragment sum lambda to test
    auto fragment_sum = [&]() -> double {
        double rv = 0;
        for (uint32_t i = 0; i < num_elements; i++) {
            heavy_math_operation(i, *doubles_fragment[i], rv);
        }
        return rv;
    };

    if (num_calls == 0) {
        measure_avg_execution_time_nano(non_fragment_sum);
        measure_avg_execution_time_nano(fragment_sum);
    } else {
        measure_avg_execution_time_nano(non_fragment_sum, num_calls);
        measure_avg_execution_time_nano(fragment_sum, num_calls);
    }
}

int main(int argc, const char* argv[]) {
    if (argc > 1) {
        num_calls = atoi(argv[1]);
    }

    if (argc > 2) {
        size_mb = atoi(argv[2]);
    }

    test_allocator();

    return 0;
}

