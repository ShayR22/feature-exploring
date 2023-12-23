#include "func_measuring/tester.hpp"

#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

class Matrix {
public:
    // Constructor to initialize the matrix with the given number of rows and columns
    Matrix(size_t rows, size_t cols) : _rows(rows), _cols(cols) {
        // Allocate memory for the matrix using unique_ptr
        _data = std::make_unique<uint32_t[]>(rows * cols);
    }

    void fill() {
        for (uint32_t i = 0; i < _rows * _cols; i++) {
            _data[i] = i + 1;
        }
    }


    // Function to access an element in the matrix using operator[]
    uint32_t& operator[](size_t index) {
        return _data[index];
    }

    // Function to access an element in the matrix using row and column indices
    uint32_t& operator()(size_t i, size_t j) {
        return _data[i * _cols + j];
    }

    // Function to get the number of rows in the matrix
    [[nodiscard]] size_t rows() const {
        return _rows;
    }

    // Function to get the number of columns in the matrix
    [[nodiscard]] size_t cols() const {
        return _cols;
    }

    // Function to transpose the matrix in place
    void transpose() {
        uint32_t count = _cols * _rows;

        for (uint32_t x= 0; x < _cols; ++x) {
            uint32_t count_adjustment= _cols - x - 1;

            for (uint32_t y= 0, step= 1; y<_rows; ++y, step+= count_adjustment){
                uint32_t last= count - (y+x*_rows);
                uint32_t first= last - step;

                std::rotate(_data.get() + first, _data.get() + first + 1, _data.get() + last);
            }
        }

        // Swap rows and columns after transposition
        std::swap(_rows, _cols);
    }

    Matrix non_inplace_transpose() {
        Matrix transposed(_cols, _rows);

        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _cols; ++j) {
                transposed(j, i) = (*this)(i, j);
            }
        }

        return transposed;
    }

private:
    size_t _rows;
    size_t _cols;
    std::unique_ptr<uint32_t[]> _data;
};


static void run_tests(uint32_t rows, uint32_t cols) {
    Matrix m{rows, cols};
    auto inplace_transpose_lambda = [&]() {
        m.transpose();
        m.fill();
    };

    auto transpose_lambda = [&]() {
        m = m.non_inplace_transpose();
        m.fill();
    };

    printf("rows=[%u], cols=[%u]\n", rows, cols);
    RUN_TEST(inplace_transpose_lambda);
    RUN_TEST(transpose_lambda);
    printf("=============================\n");

}

int main() {
    static constexpr uint16_t START = 100;
    static constexpr uint16_t STEP = 100;
    static constexpr uint16_t ROWS = 2000;
    static constexpr uint16_t COLS = 1000;
    for (uint32_t i = START; i <= ROWS; i+=STEP) {
        for (uint32_t j = START; j <= COLS; j+=STEP) {
            run_tests(i, j);
        }
    }
}

