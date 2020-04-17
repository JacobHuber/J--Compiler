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

#include "parser.h"

#include "generator.h"


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
	//Scanner scanner(fileName);
	
	
	Parser parser(fileName);
	Node* ast_root = parser.fetchAST();


	Analyzer analyzer(ast_root);
	analyzer.analyzeAST();


	Generator generator("test.wat", ast_root);
	generator.generateCode();

	ast_root->destroy();
	return 0;
}