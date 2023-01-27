#include "../../func_measuring/tester.hpp"

#include <vector>
#include <memory>
#include <cstring>

static void run_allocation_tests(uint32_t num_partitions) {
    uint32_t allocation_size = num_partitions * sizeof(uint32_t);
    auto input_buffer_unique = std::make_unique<uint8_t[]>(allocation_size);
    auto input_buffer = input_buffer_unique.get();

    auto res_buffer_unique = std::make_unique<uint8_t[]>(allocation_size);
    auto res_buffer = res_buffer_unique.get();

    auto for_memcpy = [&]() {
        for (uint32_t i = 0; i < num_partitions; i++) {
            memcpy(&res_buffer[i * sizeof(uint32_t)], &input_buffer[i * sizeof(uint32_t)], sizeof(uint32_t));
        }
    };

    auto single_memcpy = [&]() {
        memcpy(res_buffer, input_buffer, allocation_size);
    };

    RUN_TEST(single_memcpy);
    RUN_TEST(for_memcpy);
}

int main() {
    std::vector<uint32_t> partitions_sizes{1,18,36,144};
    for(const auto& num_partitions: partitions_sizes) {
        printf("\nGoing to test num_paritions with %u\n", num_partitions);
        run_allocation_tests(num_partitions);
    }
}

