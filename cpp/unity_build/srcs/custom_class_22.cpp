#include "custom_class_22.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass22::add(int x, int y) {
    return x + y;
}

std::string CustomClass22::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass22::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}