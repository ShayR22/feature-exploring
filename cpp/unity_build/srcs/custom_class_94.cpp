#include "custom_class_94.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass94::add(int x, int y) {
    return x + y;
}

std::string CustomClass94::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass94::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}