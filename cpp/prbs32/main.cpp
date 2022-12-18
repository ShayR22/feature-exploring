
#include <unordered_set>
#include <math.h>
#include <stdint.h>
#include <stdio.h>



static uint32_t PRBS_SIZE = 0XFFFF;


static uint32_t prbs_calc_prev(uint32_t reg, uint32_t tap1, uint32_t tap2, uint32_t tap3, uint32_t tap4) {
  uint32_t fb = reg;

  reg = reg >> 1;
  fb = ( fb ^ (reg >> tap1) ^ (reg >> tap2) ^ (reg >> tap3) ^ (reg >> tap4) ) & 1;

  return reg | (fb << 31);
}


// static uint32_t prbs_calc_next(uint32_t reg, uint32_t tap1, uint32_t tap2, uint32_t tap3, uint32_t tap4) {
//   uint32_t fb = ((reg >> tap1) ^ (reg >> tap2) ^ (reg >> tap3) ^ (reg >> tap4) ) & 1;
//   return (reg << 1) | fb;
// }

static uint32_t prbs_calc_next(uint32_t reg, uint32_t tap1, uint32_t tap2, uint32_t tap3, uint32_t tap4) {
	return reg + 1;
//   uint32_t fb = ((reg >> tap1) ^ (reg >> tap2) ^ (reg >> tap3) ^ (reg >> tap4) ) & 1;
//   return (reg << 1) | fb;
}


static void print_percent(double iter, double num_poss) {
	static uint32_t last_percent = 0;

	uint32_t percent = floor(iter / num_poss * 100);
	if (percent > last_percent) {
		printf("%u complete\n", percent);
		last_percent = percent;
	}
}

static bool validate_prbs(uint32_t prbs_size) {
	const size_t num_poss = pow(2, prbs_size);
	std::unordered_set<uint32_t> seen_values;
	uint32_t init_val = 0x0;

	uint32_t val = init_val;

	for (uint64_t i = 0; i < num_poss ; i++) {
		// print_percent(i, num_poss);

		seen_values.insert(val);
		val = prbs_calc_next(val, 31, 28, 26, 22);
	}


	printf("seen_values.size() = %zu, num_poss=%zu\n", seen_values.size(), num_poss);

	return seen_values.size() == num_poss;
}

int main() {
	for (auto i = 20; i < 30; i++) {
		bool good = validate_prbs(i);
		printf("prbs_%u = %s\n", i, good ? "Success" : "Failure");
	}
	return 0;
}

