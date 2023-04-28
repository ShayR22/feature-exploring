#include <cstdint>
#include <memory>
#include <cstdint>
#include <vector>
#include <concepts>


template<typename T>
concept Filter = requires(T t, const std::vector<uint32_t>& numbers) {
    {t.filter(numbers)} -> std::convertible_to<std::vector<uint32_t>>;
};


class EvenFilter {
public:
    std::vector<uint32_t> filter(const std::vector<uint32_t>& numbers) {
        std::vector<uint32_t> rv{};
        for (const auto& num: numbers) {
            if (num % 2 == 0) {
                rv.push_back(num);
            }
        }
        return rv;
    }
};


class OddFilter {
public:
    std::vector<uint32_t> filter(const std::vector<uint32_t>& numbers) {
        std::vector<uint32_t> rv{};
        for (const auto& num: numbers) {
            if (num % 2 == 1) {
                rv.push_back(num);
            }
        }
        return rv;
    }
};


void print_filter(Filter auto&& filter, const std::vector<uint32_t>& numbers) {
    std::vector<uint32_t> filtered_numbers = filter.filter(numbers);
    printf("filtered numbers: [");
    for (const auto& number: filtered_numbers) {
        printf("%u, ", number);
    }
    printf("]\n");
}

int main() {
    OddFilter odd_filter{};
    // EvenFilter even_filter{};
    std::vector<uint32_t> numbers = {1,2,3,4,5,6,7,8,9,10};
    print_filter(odd_filter, numbers);
    print_filter(EvenFilter(), numbers);
}
