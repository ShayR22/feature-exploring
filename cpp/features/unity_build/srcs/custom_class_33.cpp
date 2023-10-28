#include "custom_class_33.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass33::add(int x, int y) {
    return x + y;
}

std::string CustomClass33::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass33::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}