#include <concepts>
#include <stdio.h>

template<typename T>
void call_func(T init_func, const char* name) {
    int err = init_func();
    if (err) {
        printf("Failed to init %s\n", name);
    } else {
        printf("success to init %s\n", name);
    }
}


class X {
public:
    static int init_success() {
        return 0;
    }


    static int init_x_failure() {
        return -1;
    }

};


int main() {
    X x;
    call_func(x.init_success, "x");
    call_func(x.init_x_failure, "x");
    return 0;
}