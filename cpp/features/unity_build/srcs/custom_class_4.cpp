#include "custom_class_4.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass4::add(int x, int y) {
    return x + y;
}

std::string CustomClass4::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass4::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}