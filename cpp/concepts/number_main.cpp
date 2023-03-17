#include <concepts>
#include <cstdint>
#include <cstdio>
#include <string>

template<typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template<typename ...Args>
concept AtLeastOneElement = requires(Args ... args) {
    requires sizeof...(args) >= 1;
};

template<typename ...Args>
concept LessThan10Elements = requires(Args ... args) {
    requires sizeof...(args) < 10;
};




template<Number ...Args>
requires AtLeastOneElement<Args...> && LessThan10Elements<Args...>
Number auto sum(Args... args) {
    return (args + ...);
}

int main() {
    int x = sum(1,2,3,4);
    int y = sum(2,3,4,5);
    printf("x=%d, y=%d\n", x, y);
}
