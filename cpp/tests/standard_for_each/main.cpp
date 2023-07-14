#include "../../func_measuring/tester.hpp"

#include <vector>
#include <memory>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>
#include <execution>


static void run_executation_tests(uint32_t allocation_size) {
    std::vector<std::uint64_t> data(allocation_size);

    auto scalar_multiplier = [](auto& element) {
        uint8_t scalar = rand() % 3;
        element *= element;
        if (element > (1ULL << 31)) {
            element = element << 31;
        }
    };

    auto complicated_transform = [](auto& element) {
        uint8_t value = element % 100;
        for (uint64_t i = 1; i <= value; i++) {
            element += std::pow(i, 5) - std::sin(i) + std::exp(i);
        }
    };

    auto scalar_seq_multiplication_lambda = [&]() {
        std::for_each(std::execution::seq, data.begin(), data.end(), complicated_transform);
    };

    auto scalar_par_multiplication_lambda = [&]() {
        std::for_each(std::execution::par, data.begin(), data.end(), complicated_transform);
    };

    auto scalar_par_unseq_multiplication_lambda = [&]() {
        std::for_each(std::execution::par_unseq, data.begin(), data.end(), complicated_transform);
    };

    uint64_t element_val = 1000;

    for (uint32_t i = 0; i < allocation_size; i++) {
        data[i] = element_val;
    }
    RUN_TEST(scalar_seq_multiplication_lambda);

    for (uint32_t i = 0; i < allocation_size; i++) {
        data[i] = element_val;
    }
    RUN_TEST(scalar_par_multiplication_lambda);

    for (uint32_t i = 0; i < allocation_size; i++) {
        data[i] = element_val;
    }
    RUN_TEST(scalar_par_unseq_multiplication_lambda);
}

static void print_going_to(uint32_t allocation_size) {
    float allocation_size_f = allocation_size;
    std::string allocation_size_suffix = "Bytes";

    if (allocation_size_f >= 1<<30) {
        allocation_size_f /= 1<<30;
        allocation_size_suffix = "GB";
    }

    if (allocation_size_f >= 1<<20) {
        allocation_size_f /= 1<<20;
        allocation_size_suffix = "MB";
    }

    if (allocation_size_f >= 1<<10) {
        allocation_size_f /= 1<<10;
        allocation_size_suffix = "KB";
    }

    std::string allocation_size_str = _to_string_with_precision(allocation_size_f);
    printf("\nGoing to test allocation with %s[%s]\n", allocation_size_str.c_str(), allocation_size_suffix.c_str());
}

int main() {
    for(uint32_t i = 18; i <= 21; i++) {
        uint32_t allocation_size = 1 << i;
        print_going_to(allocation_size);
        run_executation_tests(allocation_size);
    }
}

