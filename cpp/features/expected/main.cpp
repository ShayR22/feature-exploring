#include "expected.hpp"
#include <iostream>
#include <string>
#include <cerrno>
#include <cstdlib>

Expected<double, int> parse_double(const std::string& double_str) {
    char* end = nullptr;
    double rv = std::strtod(double_str.c_str(), &end);

    if (end - double_str.c_str() != double_str.size()) {
        return -EINVAL;
    }

    return rv;
}

void handle_parsed_str(const std::string& double_str) {
    const auto rv = parse_double(double_str);

    if (rv.is_error()) {
        printf("failed to parse %s\n", double_str.c_str());
    } else {
        double val = rv.value();
        printf("parsed %s into %lf\n", double_str.c_str(), val);
    }
}

int main() {
    std::string valid_double{"3.141"};
    std::string invalid_double{"3.1a41"};

    handle_parsed_str(valid_double);
    handle_parsed_str(invalid_double);

    return 0;
}