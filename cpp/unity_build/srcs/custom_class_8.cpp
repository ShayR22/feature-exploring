#include "custom_class_8.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass8::add(int x, int y) {
    return x + y;
}

std::string CustomClass8::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass8::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}