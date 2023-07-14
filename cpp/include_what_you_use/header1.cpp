#include "header1.hpp"
#include <string>
#include <cstdio>
#include <algorithm>

void header1_print() {
    std::string s = "hello world";
    printf("%s: %s\n", __func__, s.c_str());
}