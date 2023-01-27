#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <map>


static void print_map(const std::map<uint32_t, uint32_t>& numbers_map) {
    printf("#pragma once\n");
    printf("#include <map>\n");
    printf("#include <stdint.h>\n");
    printf("static const std::map<uint32_t, uint32_t> numbers_map = {\n");
    for (const auto& [key, val]: numbers_map) {
        printf("\t{%u, %u},\n", key, val);
    }
    printf("};\n");
}

static void print_map_array(const std::map<uint32_t, uint32_t>& numbers_map) {
    printf("#pragma once\n");
    printf("#include <map>\n");
    printf("#include <stdint.h>\n\n");
    printf("struct MapRow {\n");
    printf("\tuint32_t key;\n");
    printf("\tuint32_t val;\n");
    printf("};\n\n");

    printf("static const MapRow number_map_entries[] = {\n");
    for (const auto& [key, val]: numbers_map) {
        printf("\t{%u, %u},\n", key, val);
    }
    printf("};\n");
    
    printf("static void init_map(std::map<uint32_t, uint32_t>& numbers_map) {\n");
    printf("\tfor (const auto& [key, val]: number_map_entries) {\n");
    printf("\t\t numbers_map[key] = val;\n");
    printf("\t};\n");
    printf("};\n");
}

static std::map<uint32_t, uint32_t> gen_map(long num_elements) {
    std::map<uint32_t, uint32_t> number_map{};
    
    while (number_map.size() < (size_t)num_elements) {
        uint32_t random_key = rand();
        if (number_map.find(random_key) != number_map.end()) {
            continue;
        }

        uint32_t random_val = rand();
        number_map[random_key] = random_val;
    }

    return number_map;
}

int main(int argc, const char* argv[]) {
    if (argc <= 2) {
        printf("usage: ./a.out <1,2 <map/array_entries>> <num_elements> \n");
        return -1;
    }

    srand(time(NULL));  
    long choice = atoi(argv[1]);
    long num_elements = atoi(argv[2]);

    if (choice != 1 && choice != 2) {
        printf("invalid choice %ld\n", choice);
        return -1;
    }

    std::map<uint32_t, uint32_t> number_map = gen_map(num_elements);
    if (choice == 1) {
        print_map(number_map);
    } else if (choice == 2) {
        print_map_array(number_map);
    }
    return 0;
}