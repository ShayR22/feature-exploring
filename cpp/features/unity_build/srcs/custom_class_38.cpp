#include "custom_class_38.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass38::add(int x, int y) {
    return x + y;
}

std::string CustomClass38::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass38::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}