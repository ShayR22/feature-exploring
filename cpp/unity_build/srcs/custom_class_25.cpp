#include "custom_class_25.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass25::add(int x, int y) {
    return x + y;
}

std::string CustomClass25::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass25::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}