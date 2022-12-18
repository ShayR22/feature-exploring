#include <stdio.h>
#include <string>

#include "callable.hpp"

#define PRINT_LINE(s) printf("%s: %s\n", __func__, s.c_str());

int f(const std::string& s) {
    PRINT_LINE(s)
    return std::atoi(s.c_str());
}


std::string f2(const std::string& s) {
    PRINT_LINE(s)
    return s;
}


template <typename... Args, std::invocable<Args...> F>
void t_wrapper(F&& f, Args&&... args) {
    int rv = f(args...);
    printf("rv = %d\n", rv);
}

// void wrapper(std::invocable<int, const std::string&> auto&& fn, std::string& s) {
//     PRINT_LINE(s)
//     int rv = fn(s);
//     printf("rv = %d\n", rv);
// }


int main() {
    std::string s = "42";
    // wrapper(f, s);

    // t_wrapper(f, s);
    t_wrapper(f2, s);
    return 0;
}