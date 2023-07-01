#include "custom_class_14.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass14::add(int x, int y) {
    return x + y;
}

std::string CustomClass14::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass14::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}