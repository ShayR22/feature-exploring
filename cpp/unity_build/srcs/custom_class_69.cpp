#include "custom_class_69.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass69::add(int x, int y) {
    return x + y;
}

std::string CustomClass69::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass69::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}