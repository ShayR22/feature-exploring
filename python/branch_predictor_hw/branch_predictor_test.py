#!/bin/python3

import sys
import time
import random
from typing import List


def test_func(func, *args):
    start = time.time()
    func(*args)
    time_took = time.time() - start
    time_took_ms = time_took * 1000

    print(f"{time_took_ms=}")
    return time_took_ms


def sum_numbers_above(l: List[int], above: int):
    return sum(num for num in l if num > above)


def main():
    num_elemets = 10000
    if len(sys.argv) == 2:
        num_elemets = int(sys.argv[1])

    my_list = [i for i in range(num_elemets)]
    avg_number = num_elemets // 2

    time_ms = test_func(sum_numbers_above, my_list, avg_number)

    random.shuffle(my_list)
    randomize_time_ms = test_func(sum_numbers_above, my_list, avg_number)

    print(f"Ratio is: {randomize_time_ms / time_ms}")


main()