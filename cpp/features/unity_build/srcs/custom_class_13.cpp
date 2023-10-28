#include "custom_class_13.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass13::add(int x, int y) {
    return x + y;
}

std::string CustomClass13::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass13::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}