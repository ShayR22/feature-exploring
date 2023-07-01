#include "custom_class_75.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass75::add(int x, int y) {
    return x + y;
}

std::string CustomClass75::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass75::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}