#include "custom_class_9.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass9::add(int x, int y) {
    return x + y;
}

std::string CustomClass9::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass9::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}