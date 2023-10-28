#include "custom_class_35.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass35::add(int x, int y) {
    return x + y;
}

std::string CustomClass35::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass35::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}