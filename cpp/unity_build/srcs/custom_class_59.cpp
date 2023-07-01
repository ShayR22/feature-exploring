#include "custom_class_59.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass59::add(int x, int y) {
    return x + y;
}

std::string CustomClass59::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass59::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}