#include "custom_class_83.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass83::add(int x, int y) {
    return x + y;
}

std::string CustomClass83::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass83::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}