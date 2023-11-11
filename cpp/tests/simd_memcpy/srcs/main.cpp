#include "func_measuring/tester.hpp"

#include <vector>
#include <memory>
#include <cstring>
#include <random>


static uint32_t rand_in_range(uint32_t start, uint32_t stop) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(static_cast<int>(start), static_cast<int>(stop)); // define the range
    return distr(gen);
}

template<typename T>
std::vector<T> randomize_container(uint32_t num_elements) {
    static constexpr auto MAX_RANDOM_ELEMENT_VALUE = 0XFFFF;
    std::vector<T> container(num_elements);
    for (size_t i = 0; i < num_elements; i++) {

        container[i] = rand_in_range(0, MAX_RANDOM_ELEMENT_VALUE);
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


int main() {
    static constexpr uint8_t TWO_POWER_MIN_ALLOC = 4;
    static constexpr uint8_t TWO_POWER_MAX_ALLOC = 20;
    for (uint32_t i = TWO_POWER_MIN_ALLOC; i <= TWO_POWER_MAX_ALLOC; i++) {
        uint32_t allocation_size = 1 << i;
        print_allocation_size(allocation_size);
        run_memcpy_tests(allocation_size);
    }
}

