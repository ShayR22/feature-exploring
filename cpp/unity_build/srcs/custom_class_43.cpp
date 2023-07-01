#include "custom_class_43.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass43::add(int x, int y) {
    return x + y;
}

std::string CustomClass43::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass43::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}