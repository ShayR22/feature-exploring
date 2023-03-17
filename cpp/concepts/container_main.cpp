#include <concepts>
#include <type_traits>
#include <stdint.h>
#include <stdio.h>

#include <set>

#include <array>
#include <vector>
#include <variant>

template<typename T, typename ... U>
concept IsAnyOf = (std::same_as<T, U> || ...);

template<typename T>
concept IsVecOrSet = IsAnyOf<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>, std::vector, std::set>;

template<IsVecOrSet T>
void iterate(const char* name, const T& container) {
    printf("%s: ", name);
    for (const auto& item: container) {
        printf("%u, ", item);
    }
    printf("\n");
}



int main() {
    std::set s{1,2,3};
    std::vector<uint32_t> v{1,2,3};

    iterate("vec", v);
    // iterate("set", s);
    return 0;
}












