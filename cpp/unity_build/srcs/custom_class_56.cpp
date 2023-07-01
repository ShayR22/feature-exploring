#include "custom_class_56.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass56::add(int x, int y) {
    return x + y;
}

std::string CustomClass56::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass56::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}