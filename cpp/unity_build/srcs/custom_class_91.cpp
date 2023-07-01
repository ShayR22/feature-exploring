#include "custom_class_91.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass91::add(int x, int y) {
    return x + y;
}

std::string CustomClass91::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass91::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}