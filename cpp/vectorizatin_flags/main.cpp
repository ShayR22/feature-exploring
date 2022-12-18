#include <array>
#include <vector>
#include <stdio.h>

static constexpr size_t VEC_SIZE = 100000000;

void print_vec(const std::vector<int>& vec) {
    printf("vec: [");
    for (const auto& num : vec) {
        printf("%d ", num);
    }
    printf("]\n");
}

void fill_vec(std::vector<int>& vec, int start_from=0) {
    vec.resize(VEC_SIZE);
    for (size_t i = 0; i < VEC_SIZE; i++) {
        vec[i] = i + start_from;
    }
}

void add_vec(std::vector<int>& res, const std::vector<int>& a, const std::vector<int>& b) {
    res.resize(VEC_SIZE);
    for (size_t i = 0; i < VEC_SIZE; i++) {
        res[i] = a[i] + b[i];
    }
}

int main() {
    std::vector<int> a;
    std::vector<int> b;
    std::vector<int> c;

    fill_vec(a);
    fill_vec(b);

    add_vec(c, a, b);
    for (int i = 0; i < 2; i++) {
        add_vec(c, a, b);
    }

    // print_vec(a);
    // print_vec(b);
    // print_vec(c);

    return 0;
}