#include <vector>
#include <algorithm>
#include <concepts>
#include <compare>
#include <random>

#include "tester.hpp"

template<typename T>
requires std::three_way_comparable<T>
bool my_compare(const T& a, const T& b) {
    return a < b;
}

// bool my_compare(const int& a, const int& b) {
//     return a < b;
// }


void print(const char* name, const auto& container) {
    printf("%s:\n[", name);
    for (const auto& item: container) {
        printf("%s, ", std::to_string(item).c_str());
    }
    printf("]\n");
}

template<typename T>
void shuffle_vector(std::vector<T>& vec) {
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::ranges::shuffle(vec, gen);
}

std::vector<int> gen_random_vec(uint32_t size) {
    std::vector<int> vec;
    vec.resize(size);
    for (uint32_t i = 0; i < size; i++) {
        vec[i] = i;
    }

    shuffle_vector(vec);
    return vec;
}

int main() {
    std::vector<int> vec = gen_random_vec(144);

    auto shuffle_lambda = [&]() {
        shuffle_vector(vec);
    };

    auto shuffle_sort_lambda = [&]() {
        shuffle_vector(vec);
        std::ranges::sort(vec);
    };

    uint32_t nano_took = measure_avg_execution_time_nano("shuffle", shuffle_lambda);
    printf("nano_took=%u\n", nano_took);
    nano_took =measure_avg_execution_time_nano("shuffle and sort", shuffle_sort_lambda);
    printf("nano_took=%u\n", nano_took);
    nano_took = measure_total_execution_time_nano("shuffle and sort", shuffle_sort_lambda);
    printf("nano_took=%u\n", nano_took);
    return 0;
}