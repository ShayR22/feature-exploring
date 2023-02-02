#include "../../func_measuring/tester.hpp"

#include <vector>
#include <memory>

static inline void sum_container(const auto& container, const uint32_t size) {
    uint64_t sum = 0;
    for (uint32_t i = 0; i < size; i++) {
        sum += container[i];
    }

    if (sum > 0xFFFFFFFFULL) {
        printf("sum=[%lu]\n", sum);
    }
}

static void run_allocation_tests(uint32_t allocation_size) {
    auto allocate_vec_resize = [&]() {
        std::vector<uint32_t> vec;
        vec.resize(allocation_size);
        sum_container(vec, allocation_size);
    };

    auto allocate_vec_ctor = [&]() {
        std::vector<uint32_t> vec(allocation_size);
        sum_container(vec, allocation_size);
    };

    auto allocate_buffer = [&]() {
        auto buffer = std::make_unique<uint32_t[]>(allocation_size);
        sum_container(buffer, allocation_size);
    };

    auto allocate_stack = [&]() {
        if (allocation_size > 4096) {
            return;
        }
        uint8_t buffer[allocation_size];
        sum_container(reinterpret_cast<uint8_t*>(buffer), allocation_size);
    };

    std::vector<uint32_t> sum_vec(allocation_size);
    for (uint32_t i = 0; i < allocation_size; i++) {
        sum_vec[i] = i;
    }
    auto container_sum = [&]() {
        sum_container(sum_vec, allocation_size);
    };

    RUN_TEST(container_sum);
    RUN_TEST(allocate_vec_resize);
    RUN_TEST(allocate_vec_ctor);
    RUN_TEST(allocate_buffer);
    RUN_TEST(allocate_stack);
}

int main() {
    for(uint32_t i = 4; i < 16; i++) {
        uint32_t allocation_size = 1 << i;
        printf("\nGoing to test allocation with %u\n", allocation_size);
        run_allocation_tests(allocation_size);
    }
}

