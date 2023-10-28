#include "custom_class_62.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass62::add(int x, int y) {
    return x + y;
}

std::string CustomClass62::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass62::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}