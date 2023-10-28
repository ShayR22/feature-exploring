#include "kita_alef/add.hpp"
#include "kita_alef/sub.hpp"
#include "kita_bet/mul.hpp"

#include <cstdio>

int main(int /*argc*/, const char* /*argv*/[]) {
    const int TEST_NUM1 = 10;
    const int TEST_NUM2 = 5;

    int add_res = add(TEST_NUM1, TEST_NUM2);
    int sub_res = sub(TEST_NUM1, TEST_NUM2);
    int mul_res = mul(TEST_NUM1, TEST_NUM2);
    printf("add_res=[%d] sub_res=[%d] mul_res=[%d]\n", add_res, sub_res, mul_res);
    return 0;
}