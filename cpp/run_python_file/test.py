#!/bin/python3

import sys
import time

argv = sys.argv
argc = len(argv)


loop_size = 5
if argc > 1:
	loop_size = int(argv[1])
	

for i in range(loop_size):
	time.sleep(i)
	print(i)

print("hello world")
sys.exit(13)
