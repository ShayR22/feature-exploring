#include "../../func_measuring/tester.hpp"

#include "logger..hpp"
#include <vector>
#include <memory>
#include <cstring>

static void run_allocation_tests(uint32_t allocation_size) {
    std::vector<std::uint8_t> data(allocation_size);
    for (uint32_t i = 0; i < allocation_size; i++) {
        uint8_t byte_value = (i % 27) + 97;
        data[i] = byte_value;
    }

    auto buffer_to_str_lambda = [&]() {
        std::string v = buf_to_str("data", data);
        if (v.size() > 25000) {
            printf("%s\n", v.c_str());
        }
    };

    RUN_TEST(buffer_to_str_lambda);
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
    for(uint32_t i = 2; i <= 11; i++) {
        uint32_t allocation_size = 1 << i;
        print_going_to(allocation_size);
        run_allocation_tests(allocation_size);
    }
}

