#include "custom_class_5.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass5::add(int x, int y) {
    return x + y;
}

std::string CustomClass5::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass5::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}