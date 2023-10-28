#include "custom_class_28.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass28::add(int x, int y) {
    return x + y;
}

std::string CustomClass28::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass28::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}