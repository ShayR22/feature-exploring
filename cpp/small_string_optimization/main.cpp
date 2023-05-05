

#include <cstdint>
#include <string>
#include <random>

std::string generate_random_string(uint32_t string_size) {

    std::string rv{};
    rv.reserve(string_size);


    const uint32_t num_chars = 'z' - 'a';
    for (uint32_t i = 0; i < string_size; i++) {
        char rand_char = (rand() % num_chars) + 'a';
        rv[i] = rand_char;
    }

    rv[string_size - 1] = '\0';

    return rv;
}


int main(int argc, const char* argv[]) {
    srand(time(nullptr));

    if (argc < 3) {
        printf("%s <number of random string to generate> <size of string>\n", argv[0]);
        return -1;
    }

    uint32_t num_strings = std::atoi(argv[1]);
    uint32_t string_size = std::atoi(argv[2]);

    for (uint32_t i = 0; i < num_strings; i++) {
        printf("%s\n", generate_random_string(string_size).c_str());
    }

    return 0;
}