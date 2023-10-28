#include "custom_class_15.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass15::add(int x, int y) {
    return x + y;
}

std::string CustomClass15::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass15::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}