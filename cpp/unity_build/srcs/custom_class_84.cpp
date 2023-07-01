#include "custom_class_84.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass84::add(int x, int y) {
    return x + y;
}

std::string CustomClass84::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass84::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}