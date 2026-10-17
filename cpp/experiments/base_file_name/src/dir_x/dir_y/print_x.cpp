#include "print_x.h"
#include "../../logger.h"
#include <cmath>
#include <vector>


static void _print_x(uint32_t x){
	INFO("x=%u", x);
	FO_INFO("x=%u", x);
}


// // used to check if the logger does allocations or not by knowning amount of allocas
// static void _valgrind_print_x(uint32_t x) {
// 	// allocate vector with size of 1 element
// 	std::vector<uint32_t> xs(1);
// 	xs.push_back(x);
// 	INFO("x=%u", xs[0]);
// 	FO_INFO("x=%u", xs[0]);
// }

uint32_t power_of_10(uint32_t power_of) {
	static constexpr auto BASE_10 = 10;
	return static_cast<uint32_t>(pow(BASE_10, power_of));
}

void print_x(uint32_t x) {
	static constexpr auto NUM_ITERATIONS = 6;
	for (uint32_t i = 0; i <= NUM_ITERATIONS; i++) {
		uint32_t x_val = x * power_of_10(i);
		// switch comment and uncommeented codedone for seeing valgrind
		// _valgrind_print_x(x_val);

		_print_x(x_val);
	}
}
