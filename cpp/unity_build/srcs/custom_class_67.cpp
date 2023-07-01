#include "custom_class_67.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass67::add(int x, int y) {
    return x + y;
}

std::string CustomClass67::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass67::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}