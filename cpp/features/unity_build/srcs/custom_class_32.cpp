#include "custom_class_32.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass32::add(int x, int y) {
    return x + y;
}

std::string CustomClass32::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass32::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}