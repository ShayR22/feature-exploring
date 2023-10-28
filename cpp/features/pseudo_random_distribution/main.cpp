#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <unordered_map>
#include <map>
#include <cmath>

inline float random_0_to_1() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}


static float pseudo_random_distribution(float proc_chance, uint32_t num_iterations=1e6) {
	uint32_t num_procs = 0;
	float current_chance = proc_chance;
	for (uint32_t i = 0; i < num_iterations; i++) {
		float rand = random_0_to_1();
		if (rand < current_chance) {
			num_procs++;
			current_chance = proc_chance;
		} else {
			current_chance += proc_chance;
		}
	}


	return static_cast<float>(num_procs) / num_iterations;
}

static bool is_far(float actual_chance, uint8_t decimal_points=2) {
	static constexpr float EPSILON = 1e-2;
	uint32_t round_value = pow(10, decimal_points);
	float scaled = actual_chance * round_value;
	uint32_t floored = std::floor(scaled);
	return std::abs(scaled - floored) < EPSILON;
}

int main() {
	uint32_t constexpr PERCISION_SIZE = 1e6;
	std::map<float, float> pdr{};

	for (uint32_t i = 0; i <= PERCISION_SIZE/2; i++) {
		float pdr_chance = i / static_cast<float>(PERCISION_SIZE);
		float chance = pseudo_random_distribution(pdr_chance);
		if (is_far(chance)) {
			pdr[pdr_chance] = chance;
			printf("%08.6f: %08.6f\n", pdr_chance, chance);
		}
	}

	for (const auto& [pdr_proc, actual_proc]: pdr) {
		printf("%08.6f: %08.6f\n", pdr_proc, actual_proc);
	}
	printf("number of nice numbers is %zu\n", pdr.size());

	return 0;
}
