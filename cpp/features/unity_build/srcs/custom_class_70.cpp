#include "custom_class_70.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass70::add(int x, int y) {
    return x + y;
}

std::string CustomClass70::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass70::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}