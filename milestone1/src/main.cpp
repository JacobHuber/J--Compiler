#include <iostream> // cout, cerr
#include <stdlib.h> // exit
#include <string> // string


//#include "token.h"
#include "scanner.h"

int main(int argc, char** argv) {
	if (argc == 1) {
		std::cerr << "ERROR: No filename provided.\n";
		exit(-1);
	} else if (argc > 2) {
		std::cerr << "ERROR: Too many arguments, only need filename.\n";
		exit(-1);
	}

	std::string fileName = argv[1];

	/*
	Token a = Token(1);
	std::cout << a.getLineNumber() << std::endl;

	Word b = Word(2, Word::BREAK);
	std::cout << b.getType() << std::endl;
	*/

	// Create Scanner, and pass fileName
	Scanner scanner(fileName);
	
	// Keep fetching tokens from Scanner
	
	
	Token t = scanner.nextToken();
	while (1) {
		if (t.getType() == Token::ENDFILE)
			break;

		t = scanner.nextToken();
	}

	return 0;
}