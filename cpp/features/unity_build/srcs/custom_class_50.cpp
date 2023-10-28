#include "custom_class_50.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass50::add(int x, int y) {
    return x + y;
}

std::string CustomClass50::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass50::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}