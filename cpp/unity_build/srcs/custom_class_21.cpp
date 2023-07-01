#include "custom_class_21.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass21::add(int x, int y) {
    return x + y;
}

std::string CustomClass21::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass21::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}