#include "custom_class_79.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass79::add(int x, int y) {
    return x + y;
}

std::string CustomClass79::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass79::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}