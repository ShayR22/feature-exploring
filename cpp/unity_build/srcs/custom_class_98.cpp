#include "custom_class_98.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass98::add(int x, int y) {
    return x + y;
}

std::string CustomClass98::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass98::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}