#include <fstream>
#include <random>
#include <time.h>
#include <string>


std::string create_random_string(uint32_t size) {
	srand(time(NULL));
	std::string rv{""};
	rv.reserve(size);
	for (uint32_t i = 1; i <= size; i++) {
		char random_char = (rand() % 27) + 'a';
		rv += random_char;
		if (i % 100 == 0) {
			rv += '\n';
		}
	}

	return rv;
}


void write_file(const std::string& file_name, const std::string& content) {
	std::ofstream ofs(file_name);
	ofs.write(content.c_str(), content.size());
}

int main(int argc, const char* argv[]) {
	uint32_t size = 1000;
	std::string file_name = "random_string_file.txt";
	if (argc > 1) {
		size = std::atoi(argv[1]);
	}

	if (argc > 2) {
		file_name = argv[2];
	}

	std::string content = create_random_string(size);
	write_file(file_name, content);

	return 0;
}
