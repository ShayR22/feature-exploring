#include "custom_class_39.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass39::add(int x, int y) {
    return x + y;
}

std::string CustomClass39::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass39::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}