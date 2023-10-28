#include "custom_class_57.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass57::add(int x, int y) {
    return x + y;
}

std::string CustomClass57::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass57::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}