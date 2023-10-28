#include "custom_class_58.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass58::add(int x, int y) {
    return x + y;
}

std::string CustomClass58::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass58::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}