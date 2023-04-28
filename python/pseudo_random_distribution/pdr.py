#!/bin/python3

import random


def ped_calculator(chance, num_iterations=int(1e5)):
    actual_chance = None
    
    num_procs = 0.0
    
    current_chance = chance
    for _ in range(num_iterations):
        rand = random.random()
        if rand < current_chance:
            num_procs += 1
            current_chance = chance
        else:
            current_chance += chance
    
    return num_procs / num_iterations



mapping = {}

for i in range(1000):
    proc = i / 1000.0
    mapping[proc] = ped_calculator(proc, int(1e6))
    print("proc=", proc, "value=", mapping[proc])

print(mapping)
