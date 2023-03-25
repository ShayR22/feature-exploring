#include "../../func_measuring/tester.hpp"

#include <vector>
#include <memory>
#include <cstring>

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

    if (sum > 0xFFFFFFFFFFFFFFFFULL) {
        printf("sum=[%lu]\n", sum);
    }
}

static void run_allocation_tests(uint32_t allocation_size) {
    // all allocation are of number uint32, thus /4
    allocation_size = allocation_size / 4;

    auto allocate_vec_resize = [&]() {
        std::vector<uint32_t> vec;
        vec.resize(allocation_size);
        sum_container(vec, 10);
    };

    auto allocate_vec_ctor = [&]() {
        std::vector<uint32_t> vec(allocation_size);
    };

    auto allocate_buffer = [&]() {
        auto buffer = std::make_unique<uint32_t[]>(allocation_size);
    };

    bool err = false;
    auto allocate_stack = [&]() {
        if (allocation_size > 4096) {
            if (!err) {
                printf("ERROR to big to allcoate on stack\n");
            }
            err = true;
            return;
        }
        uint8_t buffer[allocation_size];
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
    for(uint32_t i = 4; i <= 23; i++) {
        uint32_t allocation_size = 1 << i;
        print_going_to(allocation_size);
        run_allocation_tests(allocation_size);
    }
}

