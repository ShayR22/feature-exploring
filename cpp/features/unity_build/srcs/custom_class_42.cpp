#include "custom_class_42.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass42::add(int x, int y) {
    return x + y;
}

std::string CustomClass42::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass42::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}