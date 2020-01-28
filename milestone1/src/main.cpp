#include <iostream> // cout, cerr
#include <stdlib.h> // exit
#include <string> // string

int main(int argc, char** argv) {
	if (argc == 1) {
		std::cerr << "ERROR: No filename provided.\n";
		exit(-1);
	} else if (argc > 2) {
		std::cerr << "ERROR: Too many arguments, only need filename.\n";
		exit(-1);
	}

	std::string fileName = argv[1];
	std::cout << fileName << std::endl;

	return 0;
}