#!/bin/python3

def print_elements_size(prbs_num):
    num_elements = 2**prbs_num
    element_size_bytes = 4
    elements_size = num_elements * element_size_bytes

    sizes_prints = [
        [0, "Bytes"],
        [2**10, "KB"],
        [2**20, "MB"],
        [2**30, "GB"]
    ]

    taken_size, taken_size_str = 0, ""
    for size, size_str in sizes_prints:
        if elements_size >= size:
            taken_size = size
            taken_size_str = size_str

    print(f"prbs{prbs_num}: {elements_size/taken_size}[{taken_size_str}]")


for i in range(20, 32 + 1):
    print_elements_size(i)
