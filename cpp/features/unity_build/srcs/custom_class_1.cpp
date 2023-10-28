#include "custom_class_1.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass1::add(int x, int y) {
    return x + y;
}

std::string CustomClass1::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass1::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}