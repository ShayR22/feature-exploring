#include <cstdio>
#include <string>
#include <cmath>

#include "../func_measuring/tester.hpp"

float q_rsqrt(float number) {
    int i;
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = number * 0.5f;
    y = number;
    i = *(int*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));
    return y;
}


int main(int argc, const char* argv[]) {
    uint32_t num_iters = 20 * 1000;
    if (argc > 1) {
        num_iters = std::atoi(argv[1]);
    }

    printf("num_iters=[%u]\n", num_iters);

    auto lots_of_q_rsqrt = [&]() {
        for (int i = 2; i < num_iters; i++) {
            float root_of = i;
            float val = q_rsqrt(root_of);
            if (val < 0) {
                printf("q_rsqrt: %f\n", val);;
            }
        }
    };

    auto lots_of_rsqrt = [&]() {
        for (int i = 2; i < num_iters; i++) {
            float root_of = i;
            float val = 1 / sqrt(root_of);
            if (val < 0) {
                printf("rsqrt: %f\n", val);;
            }
        }
    };


    RUN_TEST(lots_of_rsqrt);
    RUN_TEST(lots_of_q_rsqrt);


    return 0;
}