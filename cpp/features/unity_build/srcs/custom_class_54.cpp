#include "custom_class_54.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass54::add(int x, int y) {
    return x + y;
}

std::string CustomClass54::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass54::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}