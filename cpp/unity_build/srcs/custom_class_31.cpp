#include "custom_class_31.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass31::add(int x, int y) {
    return x + y;
}

std::string CustomClass31::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass31::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}