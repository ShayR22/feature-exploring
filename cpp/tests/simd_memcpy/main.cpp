#include "../../func_measuring/tester.hpp"

#include <vector>
#include <memory>
#include <cstring>
#include <random>


static uint32_t rand_in_range(uint32_t start, uint32_t stop) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(start, stop); // define the range
    return distr(gen);
}

template<typename T>
std::vector<T> randomize_container(uint32_t num_elements) {
    std::vector<T> container(num_elements);
    for (size_t i = 0; i < num_elements; i++) {
        container[i] = rand_in_range(0, 0xFFFF);
    }
    return container;
}

template<typename T>
void memcpy_container(std::vector<T>& dst_container, const std::vector<T>& src_container) {
    const size_t num_bytes_copied = std::min(dst_container.size(), src_container.size()) * sizeof(T);
    std::memcpy(dst_container.data(), src_container.data(), num_bytes_copied);
}

template<typename T>
void for_memcpy_container(std::vector<T>& dst_container, const std::vector<T>& src_container) {
    const size_t num_elements = std::min(dst_container.size(), src_container.size());
    for (size_t i = 0; i < num_elements; i++) {
        std::memcpy(&dst_container[i], &src_container[i], sizeof(T));
    }
}


static void run_memcpy_tests(uint32_t allocation_size) {
    // all allocation are of number uint32, thus /4
    uint32_t num_elements = allocation_size / sizeof(uint32_t);
    std::vector<uint32_t> src_container = randomize_container<uint32_t>(num_elements);
    std::vector<uint32_t> dst_container(num_elements);

    auto regular_memcpy_container = [&]() {
        memcpy_container(dst_container, src_container);
    };

    auto forloop_memcpy_container = [&]() {
        for_memcpy_container(dst_container, src_container);
    };

    RUN_TEST(regular_memcpy_container);
    RUN_TEST(forloop_memcpy_container);
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
    printf("\nGoing to test memcpy with %s[%s]\n", allocation_size_str.c_str(), allocation_size_suffix.c_str());
}

int main() {
    for(uint32_t i = 4; i <= 20; i++) {
        uint32_t allocation_size = 1 << i;
        print_going_to(allocation_size);
        run_memcpy_tests(allocation_size);
    }
}

