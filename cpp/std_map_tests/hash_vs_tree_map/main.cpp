#include <thread>
#include <stdio.h>
#include <map>
#include <unordered_map>
#include "tester.hpp"

static constexpr uint32_t KB_SIZE_BYTES = 1024;
static constexpr uint32_t MB_SIZE_BYTES = KB_SIZE_BYTES * KB_SIZE_BYTES;


static uint32_t size_mb = 1;
static uint32_t num_calls = 0;

template<typename T>
static void test_generic_map(T& my_map, const std::string& map_type_str) {

    static constexpr uint8_t MAP_ENTRY_SIZE = 12;
    uint32_t num_elements = size_mb * MB_SIZE_BYTES / MAP_ENTRY_SIZE;
    printf("map type %s: num_elemnets = %u, size_mb = %u\n", map_type_str.c_str(), num_elements, size_mb);

    for (uint64_t i = 0; i < num_elements; i++) {
        my_map[i] = (i * i) % 1 << 16;
    }

    auto random_map_access = [&]() -> uint32_t {
        return my_map[rand() & num_elements];
    };

    auto sum_random_elements = [&]() -> uint32_t {
        uint32_t sum = 0;
        for (uint32_t i = 0; i < num_elements; i++) {
            sum += random_map_access();
        }
        return sum;
    };

    if (num_calls == 0) {
        measure_avg_execution_time(sum_random_elements);
    } else {
        measure_avg_execution_time(sum_random_elements, num_calls);
    }
}

static void test_std_map() {
    std::map<uint32_t, uint32_t> my_map{};
    test_generic_map(my_map, "std::map");
}

static void test_std_unordered_map() {
    std::unordered_map<uint32_t, uint32_t> my_map{};
    test_generic_map(my_map, "std::unordered_map");
}

int main(int argc, const char* argv[]) {
    auto hard_function = []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    };

    uint32_t hard_function_avg_exec_time_ms = -1;


    if (argc > 1) {
        num_calls = atoi(argv[1]);
    }

    if (argc > 2) {
        size_mb = atoi(argv[2]);
    }

    test_std_unordered_map();
    test_std_map();

    return 0;
}

