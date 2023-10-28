#include "custom_class_80.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass80::add(int x, int y) {
    return x + y;
}

std::string CustomClass80::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass80::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}