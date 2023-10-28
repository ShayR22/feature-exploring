#include "custom_class_24.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass24::add(int x, int y) {
    return x + y;
}

std::string CustomClass24::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass24::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}