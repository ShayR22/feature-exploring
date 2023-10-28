#include "custom_class_74.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass74::add(int x, int y) {
    return x + y;
}

std::string CustomClass74::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass74::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}