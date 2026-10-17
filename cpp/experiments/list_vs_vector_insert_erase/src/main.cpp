#include "func_measuring/tester.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include <vector>
#include <vector>


static void run_executation_tests(uint32_t allocation_size) {
    std::vector<uint32_t> vec;
    vec.resize(allocation_size);
    std::list<uint32_t> list(allocation_size);
    for (uint32_t i = 0; i < allocation_size; i++) {
        list.push_back(0);
    }

    constexpr uint32_t NUM_INDICIES = 1000;
    std::vector<uint32_t> indicies(NUM_INDICIES);
    for (uint32_t i = 0; i < NUM_INDICIES; i++) {
        indicies[i] = rand() % allocation_size;
    }

    auto vec_insertion_erase_lambda = [&]() {
        bool is_insert = true;
        for (const auto& index: indicies) {
            if (is_insert) {
                vec.insert(std::next(vec.begin(), index), 0UL);
            } else {
                vec.erase(std::next(vec.begin(), index));
            }
            is_insert = !is_insert;
        }
    };

    auto list_insertion_erase_lambda = [&]() {
        bool is_insert = true;
        for (const auto& index: indicies) {
            if (is_insert) {
                list.insert(std::next(list.begin(), index), 0UL);
            } else {
                list.erase(std::next(list.begin(), index));
            }
            is_insert = !is_insert;
        }
    };

    RUN_TEST(vec_insertion_erase_lambda);
    RUN_TEST(list_insertion_erase_lambda);
}

int main() {
    srand(time(nullptr));
    static constexpr uint8_t TWO_POWER_MIN_ALLOC = 4;
    static constexpr uint8_t TWO_POWER_MAX_ALLOC = 21;
    for (uint32_t i = TWO_POWER_MIN_ALLOC; i <= TWO_POWER_MAX_ALLOC; i++) {
        uint32_t allocation_size = 1 << i;
        print_allocation_size(allocation_size);
        run_executation_tests(allocation_size / sizeof(uint32_t));
    }
}

