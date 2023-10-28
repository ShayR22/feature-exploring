#include "custom_class_53.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass53::add(int x, int y) {
    return x + y;
}

std::string CustomClass53::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass53::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}