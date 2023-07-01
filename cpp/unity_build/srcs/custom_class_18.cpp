#include "custom_class_18.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass18::add(int x, int y) {
    return x + y;
}

std::string CustomClass18::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass18::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}