#include "custom_class_29.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass29::add(int x, int y) {
    return x + y;
}

std::string CustomClass29::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass29::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}