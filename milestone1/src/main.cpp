// Jacob Huber - 30052459
// CPSC 411 - TUT 03
// 
// J-- Compiler Program
//
// This program in its current state
// will take a J-- file, converting
// any legal token and erroring on
// illegal characters/states.

#include <iostream> // cout, cerr
#include <stdlib.h> // exit
#include <string> // string

#include "scanner.h"


int main(int argc, char** argv) {
	// Error checking to make sure a filename was provided
	// as an argument.
	if (argc == 1) {
		std::cerr << "ERROR: No filename provided.\n";
		exit(-1);
	} else if (argc > 2) {
		std::cerr << "ERROR: Too many arguments, only need filename.\n";
		exit(-1);
	}

	std::string fileName = argv[1];

	// Create Scanner, and pass fileName
	Scanner scanner(fileName);
	
	// Keep fetching tokens from Scanner
	// and printing them out
	while (1) {
		Token t = scanner.nextToken();
		TOKENTYPE tokenType = t.getType();
		
		if (tokenType == ENDFILE)
			break;

		switch (tokenType) {
			// Print Token Type, Lexeme, and Line Number
			case (IDENTIFIER):
			case (INTEGER):
			case (STRING):
				std::cout << "(" << t.getTypeStr().c_str() << ") '" << t.getLexeme() << "' found on line " << t.getLineNumber() << std::endl;
				break;
			// Print Token Type, and Line Number
			default:
				std::cout << "(" << t.getTypeStr().c_str() << ") found on line " << t.getLineNumber() << std::endl;
				break;
		};
	}

	return 0;
}