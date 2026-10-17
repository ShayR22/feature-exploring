#include "func_measuring/tester.hpp"

#include <vector>
#include <memory>
#include <cstring>

static constexpr uint32_t MAX_STACK_SIZE = 4 * KB;

template<typename T>
void memcpy_container(std::vector<T>& dst_container, const std::vector<T>& src_container) {
    const size_t num_bytes_copied = std::min(dst_container.size(), src_container.size()) * sizeof(T);
    std::memcpy(dst_container.data(), src_container.data(), num_bytes_copied);
}

static inline void sum_container(const auto& container, const uint32_t size) {
    uint64_t sum = 0;
    for (uint32_t i = 0; i < size; i++) {
        sum += container[i];
    }

    static constexpr auto MAX_SUM_FOR_PRINT = 0xFFFFFFFFFFFFFFFFULL;
    if (sum > MAX_SUM_FOR_PRINT) {
        printf("sum=[%lu]\n", sum);
    }
}

static void run_allocation_tests(uint32_t allocation_size) {
    // all allocation are of number uint32, thus /4
    allocation_size = allocation_size / 4;

    auto allocate_vec_resize = [&]() {
        std::vector<uint32_t> vec;
        vec.resize(allocation_size);
        static constexpr uint8_t CACHE_READ_SIZE = 10;
        sum_container(vec, CACHE_READ_SIZE);
    };

    auto allocate_vec_ctor = [&]() {
        std::vector<uint32_t> vec(allocation_size);
    };

    auto allocate_buffer = [&]() {
        // NOLINTNEXTLINE
        auto buffer = std::make_unique<uint32_t[]>(allocation_size);
    };

    bool err = false;
    auto allocate_stack = [&]() {
        if (allocation_size > MAX_STACK_SIZE) {
            if (!err) {
                printf("ERROR to big to allcoate on stack\n");
            }
            err = true;
            return;
        }
        // NOLINTNEXTLINE
        [[maybe_unused]] uint8_t buffer[MAX_STACK_SIZE];
    };

    std::vector<uint32_t> sum_vec(allocation_size);
    for (uint32_t i = 0; i < allocation_size; i++) {
        sum_vec[i] = i;
    }

    auto container_sum = [&]() {
        sum_container(sum_vec, allocation_size);
    };

    std::vector<uint32_t> dest_vec(allocation_size);
    auto memcpy_containers = [&]() {
        memcpy_container(dest_vec, sum_vec);
    };

    RUN_TEST(container_sum);
    RUN_TEST(allocate_vec_resize);
    RUN_TEST(allocate_vec_ctor);
    RUN_TEST(allocate_buffer);
    RUN_TEST(allocate_stack);
    RUN_TEST(memcpy_containers);
}

int main() {
    static constexpr uint8_t TWO_POWER_MIN_ALLOC = 4;
    static constexpr uint8_t TWO_POWER_MAX_ALLOC = 23;
    for(uint32_t i = TWO_POWER_MIN_ALLOC; i <= TWO_POWER_MAX_ALLOC; i++) {
        uint32_t allocation_size = 1 << i;
        print_allocation_size(allocation_size);
        run_allocation_tests(allocation_size);
    }
}

