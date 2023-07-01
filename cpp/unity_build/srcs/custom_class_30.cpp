#include "custom_class_30.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass30::add(int x, int y) {
    return x + y;
}

std::string CustomClass30::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass30::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}