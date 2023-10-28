#include "custom_class_20.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass20::add(int x, int y) {
    return x + y;
}

std::string CustomClass20::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass20::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}