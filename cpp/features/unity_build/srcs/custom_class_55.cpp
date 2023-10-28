#include "custom_class_55.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <sstream>

int CustomClass55::add(int x, int y) {
    return x + y;
}

std::string CustomClass55::to_string() {
    std::stringstream ss{};
    ss << _x;
    return ss.str();
}

void CustomClass55::sort(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}