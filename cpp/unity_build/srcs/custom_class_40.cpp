#include "custom_class_40.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass40::add(int x, int y) {
    return x + y;
}

std::string CustomClass40::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass40::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}