#include "custom_class_46.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass46::add(int x, int y) {
    return x + y;
}

std::string CustomClass46::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass46::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}