#include "custom_class_45.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass45::add(int x, int y) {
    return x + y;
}

std::string CustomClass45::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass45::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}