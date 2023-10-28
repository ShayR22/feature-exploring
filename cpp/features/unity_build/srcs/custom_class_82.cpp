#include "custom_class_82.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass82::add(int x, int y) {
    return x + y;
}

std::string CustomClass82::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass82::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}