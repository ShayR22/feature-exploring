#!/bin/python3

import sys
import pstats

def usage():
	print("print_profile_output <output_file> <num rows> <sort_by>")


def print_pstats(file_path: str, num_rows: int=15, sort_by: str=None):
	p = pstats.Stats(file_path)
	
	if sort_by is not None:
		p.sort_stats(sort_by)

	p.print_stats(num_rows)

def main():
	argc = len(sys.argv)
	if argc < 2:
		usage()

	num_rows=15
	sort_by=None
	
	output_file_path = sys.argv[1]
	if argc > 2:
		num_rows = int(sys.argv[2])
	if argc > 3:
		sort_by = sys.argv[3]

	print_pstats(output_file_path, num_rows, sort_by)

main()
