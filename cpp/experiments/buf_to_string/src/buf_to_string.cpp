#include "func_measuring/tester.hpp"

#include "logger.hpp"
#include <vector>
#include <memory>
#include <cstring>
#include <map>
#include <set>

static constexpr uint8_t NUM_EN_LETTERS = 27;
static constexpr uint8_t A_SMALL_LETTER = 'a';

static void run_allocation_tests(uint32_t allocation_size) {
    std::vector<std::uint8_t> data(allocation_size);
    for (uint32_t i = 0; i < allocation_size; i++) {
        uint8_t byte_value = (i % NUM_EN_LETTERS) + A_SMALL_LETTER;
        data[i] = byte_value;
    }

    static constexpr size_t STR_PRINT_LIMITER = 25000;
    auto buffer_to_str_lambda = [&]() {
        std::string v = buf_to_str("data", data);
        if (v.size() > STR_PRINT_LIMITER) {
            printf("%s\n", v.c_str());
        }
    };

    RUN_TEST(buffer_to_str_lambda);
}

int main() {
    static constexpr uint8_t TWO_POWER_MIN_ALLOC = 2;
    static constexpr uint8_t TWO_POWER_MAX_ALLOC = 11;
    for (uint32_t i = TWO_POWER_MIN_ALLOC; i <= TWO_POWER_MAX_ALLOC; i++) {
        uint32_t allocation_size = 1 << i;
        print_allocation_size(allocation_size);
        run_allocation_tests(allocation_size);
    }
}

