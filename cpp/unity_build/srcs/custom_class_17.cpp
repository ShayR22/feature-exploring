#include "custom_class_17.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass17::add(int x, int y) {
    return x + y;
}

std::string CustomClass17::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass17::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}