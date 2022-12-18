

def mul(a, b):
	return a * b;

def add(a, b):
	return a + b


def sigma(l):
	rv = 0
	for item in l:
		rv = add(rv, item)

	return rv


def product(l):
	rv = 0
	for item in l:
		rv += mul(rv, item)

	return rv




if __name__ == '__main__':
	huge_list = [i for i in range(10**6)]

	total_add = sigma(huge_list)
	# total_mul = product(huge_list)
	print("total_add= {}".format(total_add))
	# print(f"{total_mul=}")

