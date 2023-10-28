#include "custom_class_7.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass7::add(int x, int y) {
    return x + y;
}

std::string CustomClass7::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass7::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}