#include "kita_alef/add.hpp"
#include "kita_alef/sub.hpp"
#include "kita_bet/mul.hpp"

#include <cstdio>

int main(int argc, const char* argv[]) {
    int add_res = add(10, 5);
    int sub_res = sub(10, 5);
    int mul_res = mul(10, 5);
    printf("add_res=[%d] sub_res=[%d] mul_res=[%d]\n", add_res, sub_res, mul_res);
    return 0;
}