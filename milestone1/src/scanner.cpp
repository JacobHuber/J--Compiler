#include "scanner.h"
#include <iostream> // cout, cerr
#include <stdlib.h> // exit

#include <ctype.h> // isalpha, isalnum, isspace

Scanner::Scanner(std::string fn) {
	fileName = fn;

	
	//std::cout << fileName.c_str() << "\n";

	fs.open(fileName.c_str(), std::fstream::in);

	if (!fs.is_open()) {
		std::cerr << "ERROR: Unable to open file '" << fileName.c_str() << "'\n";
		exit(-1);
	}
}

Scanner::~Scanner() {
	// Close fs if it's opened
	if (fs.is_open()) {
		fs.close();
	}
	
	//std::cout << "Goodbye\n";
}


// Reads the next char from this
// scanners fileStream. Returns
// NULL if end of file.
char Scanner::readChar() {
	char c;
	fs.get(c);
	if (fs.eof())
		return NULL;
	
	lexeme += c;
	return c;
}

Token Scanner::nextToken() {
	char ch;
	
s_start:
	lexeme = "";

	switch (ch = readChar()) {
		case '"':
			goto s_str;
		case '+':
			goto s_plus;
		case '-':
			goto s_minus;
		case '*':
			goto s_mult;
		case '/':
			goto s_div;
		case '%':
			goto s_mod;
		case '<':
			goto s_lt;
		case '>':
			goto s_gt;
		case '=':
			goto s_eq;
		case '!':
			goto s_not;
		case '&':
			goto s_and;
		case '|':
			goto s_or;
		case '(':
			goto s_lb;
		case ')':
			goto s_rb;
		case '{':
			goto s_lcb;
		case '}':
			goto s_rcb;
		case ';':
			goto s_semi;
		case ',':
			goto s_comma;
		case NULL:
			goto s_eof;
		
		default:
			if (isspace(ch)) {
				goto s_start;
			} else if (isalpha(ch)) {
				goto s_ident;
			} else if (isdigit(ch)) {
				goto s_int;
			} else {
				std::cerr << "ERROR: Unknown character.\n";
				exit(-1);
			}
	}

s_str:
	// read until ending " include escape characters k thx
s_plus:

s_minus:

s_mult:

s_div:

s_mod:

s_lt:

s_gt:

s_eq:

s_not:

s_and:

s_or:

s_lb:

s_rb:

s_lcb:

s_rcb:

s_semi:

s_comma:

s_eof:
	return Token(0, Token::ENDFILE);

s_ident:

s_int:


	
	return NULL;
}