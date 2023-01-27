#include "../../func_measuring/tester.hpp"

#include <vector>
#include <memory>



static void run_allocation_tests(uint32_t allocation_size) {
    auto allocate_vec_resize = [&]() {
        std::vector<uint32_t> vec;
        vec.resize(allocation_size);
    };

    auto allocate_vec_ctor = [&]() {
        std::vector<uint32_t> vec(allocation_size);
    };

    auto allocate_buffer = [&]() {
        auto buffer = std::make_unique<uint32_t[]>(allocation_size);
    };

    auto allocate_stack = [&]() {
        if (allocation_size > 4096) {
            return;
        }
        uint8_t buffer[allocation_size];
    };

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

