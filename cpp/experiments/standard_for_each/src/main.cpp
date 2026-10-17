#include "func_measuring/tester.hpp"

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

    auto complicated_transform = [](auto& element) {
        // random values
        static constexpr auto I_POW = 5;
        static constexpr auto VAL_MODULE = 100;
        uint8_t value = element % VAL_MODULE;
        for (uint64_t i = 1; i <= value; i++) {

            element += std::pow(i, I_POW) - std::sin(i) + std::exp(i);
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

    static constexpr uint64_t DEFAULT_ELEMENET_VAL = 1000;

    for (uint32_t i = 0; i < allocation_size; i++) {
        data[i] = DEFAULT_ELEMENET_VAL;
    }
    RUN_TEST(scalar_seq_multiplication_lambda);

    for (uint32_t i = 0; i < allocation_size; i++) {
        data[i] = DEFAULT_ELEMENET_VAL;
    }
    RUN_TEST(scalar_par_multiplication_lambda);

    for (uint32_t i = 0; i < allocation_size; i++) {
        data[i] = DEFAULT_ELEMENET_VAL;
    }
    RUN_TEST(scalar_par_unseq_multiplication_lambda);
}

int main() {
    static constexpr uint8_t TWO_POWER_MIN_ALLOC = 18;
    static constexpr uint8_t TWO_POWER_MAX_ALLOC = 21;
    for (uint32_t i = TWO_POWER_MIN_ALLOC; i <= TWO_POWER_MAX_ALLOC; i++) {
        uint32_t allocation_size = 1 << i;
        print_allocation_size(allocation_size);
        run_executation_tests(allocation_size);
    }
}

