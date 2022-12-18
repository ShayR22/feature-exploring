import sys

def add(a, b):
	return a + b

def sigma(start_i=0, sigma_size=2*10**8):
	res = 0
	for i in range(start_i, sigma_size):
		res = add(res, i)
	return res


def sigma_inner(start_i=0, sigma_size=2*10**8):
	return sum([i for i in range(start_i, sigma_size)])

if __name__ == '__main__':

	if len(sys.argv) > 1:
		res = sigma()
	else:
		res = sigma_inner()

	res_str = "{:e}".format(res)
	print(res_str)
