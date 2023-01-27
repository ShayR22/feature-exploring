#include <map>
#include <stdint.h>
#include <stdio.h>
#include "static_map_tests.hpp"
// #include "map100k.hpp"


static void print_map(const std::unordered_map<uint32_t, uint32_t>& number_maps) {
    printf("static const std::map<uint32_t, uint32_t> number_maps = {\n");
    for (const auto& [key, val]: number_maps) {
        printf("\t{%u, %u},\n", key, val);
    }
    printf("}\n");
}

int main() {
    std::unordered_map<uint32_t, uint32_t> numbers_map{};
    //init_map(numbers_map);
    print_map(numbers_map);
    return 0;
}

