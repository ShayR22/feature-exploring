#include "custom_class_19.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass19::add(int x, int y) {
    return x + y;
}

std::string CustomClass19::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass19::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}