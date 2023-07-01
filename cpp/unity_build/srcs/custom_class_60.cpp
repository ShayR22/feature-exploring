#include "custom_class_60.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass60::add(int x, int y) {
    return x + y;
}

std::string CustomClass60::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass60::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}