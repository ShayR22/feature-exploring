#include "custom_class_23.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass23::add(int x, int y) {
    return x + y;
}

std::string CustomClass23::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass23::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}