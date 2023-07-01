#include "custom_class_92.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass92::add(int x, int y) {
    return x + y;
}

std::string CustomClass92::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass92::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}