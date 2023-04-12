#include <cstdint>
#include <vector>
#include <cstdio>
#include <random>
#include <unistd.h>
#include <chrono>

static int i = 0;
template<typename T>
void iterate(const char* name, const T& container) {
    usleep(1 * 100);
    if (name != nullptr && ((i++ % 1024) == 0) ) {
        printf("%s:\n", name);
    }
    // for (const auto& item: container) {
    //     printf("%02x, ", item);
    // }
    // printf("\n");
}


static uint32_t rand_in_range(uint32_t start, uint32_t stop) {
        static std::random_device rd; // obtain a random number from hardware
        static std::mt19937 gen(rd()); // seed the generator
        static std::uniform_int_distribution<> distr(start, stop); // define the range
        return distr(gen);
}

static void printer(const char* name, uint32_t size) {
    if (size == 0) {
        iterate(name, std::vector<uint8_t>());
        return;
    }

    std::vector<uint32_t> numbers(size);
    for (uint32_t i = 0; i < numbers.size(); i++) {
        numbers[i] = i; //rand_in_range(0, 255);
    }
    iterate(nullptr, numbers);

    printer(name, size/2);
}

class C {
public:
    C(uint32_t size) {
        // printf("C::size=%u", size);
        printer(__func__, size);
    }
};

class B {
public:
    B(uint32_t size) {
        // printf("B::size=%u", size);
        printer(__func__, size);
        C c(size/2);
    }
};


class A {
public:
    A(uint32_t size) {
        // printf("A::size=%u", size);
        printer(__func__, size);
        B b(size/2);
    }
};


bool has_time_passed(const auto& start, const auto now, const uint32_t seconds) {
    auto micro_duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
    auto micro_passed = micro_duration.count();
    auto micro_time = seconds * 1000000;
    return micro_passed >= micro_time;
}

int main(int argc, const char** argv) {
    uint32_t allocation_size = 32;
    uint32_t seconds_to_run = 1;

    if (argc > 1) {
        allocation_size = std::atoi(argv[1]);
    }

    if (argc > 2) {
        seconds_to_run = std::atoi(argv[2]);
    }

    printf("seconds_to_run=[%u], allocation_size=[%u]\n", seconds_to_run, allocation_size);
    auto start_time = std::chrono::high_resolution_clock::now();
    while (!has_time_passed(start_time, std::chrono::high_resolution_clock::now(), seconds_to_run)) {
        A a(allocation_size);
    }

    return 0;
}