#include "custom_class_44.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass44::add(int x, int y) {
    return x + y;
}

std::string CustomClass44::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass44::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}