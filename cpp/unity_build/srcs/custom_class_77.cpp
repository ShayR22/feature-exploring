#include "custom_class_77.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass77::add(int x, int y) {
    return x + y;
}

std::string CustomClass77::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass77::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}