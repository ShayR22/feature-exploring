#include "custom_class_78.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass78::add(int x, int y) {
    return x + y;
}

std::string CustomClass78::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass78::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}