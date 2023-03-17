
#include <vector>
#include <stdio.h>


void swap(std::vector<int>& numbers, int i, int j) {
	int tmp = numbers[i];
	numbers[i] = numbers[j];
	numbers[j] = tmp;
}


void bubble_sort(std::vector<int>& numbers) {
	for (int i = 0; i < numbers.size() - 1; i++) {
		for (int j = i; j < numbers.size(); j++) {
			if (numbers[i] > numbers[j]) {
				swap(numbers, i, j);
			}
		}
	}
}

void print_first_x_numbers(const std::vector<int>& numbers, int x) {
	printf("numbers: [");
	for (int i = 0; i < x; i++) {
		printf("%d, ", numbers[i]);
	}
	printf("]\n");
}

int main(){
	std::vector<int> numbers{};
	constexpr size_t vec_size = 1 << 16;
	numbers.resize(vec_size);
	for (int i = 0; i < vec_size; i++) {
		numbers[i] = vec_size - i;
	}

	print_first_x_numbers(numbers, 10);
	bubble_sort(numbers);
	print_first_x_numbers(numbers, 10);
	
	return 0;
}
