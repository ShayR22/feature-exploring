#include "custom_class_87.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass87::add(int x, int y) {
    return x + y;
}

std::string CustomClass87::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass87::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}