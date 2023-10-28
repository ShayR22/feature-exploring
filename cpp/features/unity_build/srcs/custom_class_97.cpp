#include "custom_class_97.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass97::add(int x, int y) {
    return x + y;
}

std::string CustomClass97::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass97::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}