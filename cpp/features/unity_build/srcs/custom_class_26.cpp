#include "custom_class_26.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass26::add(int x, int y) {
    return x + y;
}

std::string CustomClass26::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass26::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}