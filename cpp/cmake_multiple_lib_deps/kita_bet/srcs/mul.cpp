#include "kita_bet/mul.hpp"
#include "kita_alef/add.hpp"
#include <cstdio>

int mul(int number1, int number2) {
    printf("number1=[%d] number2=[%d]\n", number1, number2);
    int res = 0;
    for (int i = 0; i < number2; i++) {
        res = add(number1, res);
    }
    return res;
}