#include "custom_class_0.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass0::add(int x, int y) {
    return x + y;
}

std::string CustomClass0::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass0::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}