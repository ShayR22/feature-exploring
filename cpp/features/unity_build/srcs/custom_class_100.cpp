#include "custom_class_100.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass100::add(int x, int y) {
    return x + y;
}

std::string CustomClass100::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass100::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}