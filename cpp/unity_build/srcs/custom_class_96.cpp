#include "custom_class_96.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass96::add(int x, int y) {
    return x + y;
}

std::string CustomClass96::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass96::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}