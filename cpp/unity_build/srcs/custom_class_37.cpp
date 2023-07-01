#include "custom_class_37.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass37::add(int x, int y) {
    return x + y;
}

std::string CustomClass37::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass37::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}