#include "custom_class_72.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass72::add(int x, int y) {
    return x + y;
}

std::string CustomClass72::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass72::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}