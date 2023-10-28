#include "custom_class_64.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass64::add(int x, int y) {
    return x + y;
}

std::string CustomClass64::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass64::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}