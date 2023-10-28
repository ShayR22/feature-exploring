#include "custom_class_34.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass34::add(int x, int y) {
    return x + y;
}

std::string CustomClass34::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass34::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}