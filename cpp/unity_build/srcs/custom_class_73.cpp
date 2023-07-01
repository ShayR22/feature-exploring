#include "custom_class_73.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass73::add(int x, int y) {
    return x + y;
}

std::string CustomClass73::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass73::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}