#include "custom_class_47.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass47::add(int x, int y) {
    return x + y;
}

std::string CustomClass47::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass47::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}