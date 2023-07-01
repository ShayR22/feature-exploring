#include "custom_class_16.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass16::add(int x, int y) {
    return x + y;
}

std::string CustomClass16::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass16::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}