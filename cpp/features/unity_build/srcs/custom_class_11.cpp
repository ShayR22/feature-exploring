#include "custom_class_11.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass11::add(int x, int y) {
    return x + y;
}

std::string CustomClass11::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass11::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}