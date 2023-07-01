#include "custom_class_36.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass36::add(int x, int y) {
    return x + y;
}

std::string CustomClass36::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass36::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}