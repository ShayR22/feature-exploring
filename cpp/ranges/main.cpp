#include <concepts>
#include <ranges>
#include <cstdio>
#include <vector>

template<typename T>
concept Number = std::integral<T> || std::floating_point<T>;


template<Number ...Args>
Number auto sum(Args... args) {
    return (args + ...);
}



float get_max(const std::vector<float>& elements) {
    return std::ranges::max_element(elements);
}



int main() {
    printf("sum=[%f]\n", sum(1,2,3.5,4.5, -1));

    std::vector<float> floats{1,2,3.5,4.5, -1};
    float floats_max = get_max(floats);
    printf("floats_max=[%f]", floats_max);
    return 0;
}