#include "custom_class_90.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass90::add(int x, int y) {
    return x + y;
}

std::string CustomClass90::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass90::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}