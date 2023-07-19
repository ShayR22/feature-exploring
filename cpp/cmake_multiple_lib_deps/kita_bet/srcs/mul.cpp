#include "kita_bet/mul.hpp"
#include "kita_alef/add.hpp"
#include <cstdio>

int mul(int x, int y) {
    printf("x=[%d] y=[%d]\n", x, y);
    int res = 0;
    for (int i = 0; i < y; i++) {
        res = add(res, x);
    }
    return res;
}