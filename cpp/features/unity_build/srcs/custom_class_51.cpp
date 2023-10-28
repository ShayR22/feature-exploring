#include "custom_class_51.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass51::add(int x, int y) {
    return x + y;
}

std::string CustomClass51::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass51::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}