#include "scanner.h"
#include <iostream> // cout, cerr
#include <stdlib.h> // exit

#include <ctype.h> // isalpha, isalnum, isspace, isdigit

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

bool Scanner::newLine() {
	return (currChar == '\n');
}

// Reads the next char from this
// scanners fileStream. Returns
// NULL if end of file.
char Scanner::readChar() {
	char c;
	fs.get(c);
	
	currChar = c;

	if (isspace(c)) {
		if (newLine())
			lineNumber += 1;
	}
	
	lexeme += currChar;


	return c;
}

void Scanner::ungetChar() {
	fs.unget();
	
	if (isspace(currChar)) {
		if (newLine())
			lineNumber -= 1;

	}

	lexeme = lexeme.substr(0, lexeme.size()-1);
}

bool Scanner::isalpha2() {
	return isalpha(currChar) || currChar == '_';
}

TOKENTYPE checkReservedWord(std::string lex) {
	if (lex.compare("true") == 0) {
		return TRUE;
	} else if (lex.compare("false") == 0) {
		return FALSE;
	} else if (lex.compare("boolean") == 0) {
		return BOOLEAN;
	} else if (lex.compare("int") == 0) {
		return INT;
	} else if (lex.compare("void") == 0) {
		return VOID;
	} else if (lex.compare("if") == 0) {
		return IF;
	} else if (lex.compare("else") == 0) {
		return ELSE;
	} else if (lex.compare("while") == 0) {
		return WHILE;
	} else if (lex.compare("break") == 0) {
		return BREAK;
	} else if (lex.compare("return") == 0) {
		return RETURN;
	} else {
		return IDENTIFIER;
	}
}

char Scanner::getEscape() {
	if (currChar == 'b') {
		return '\b';
	} else if (currChar == 'r') {
		return '\r';
	} else if (currChar == 't') {
		return '\t';
	} else if (currChar == 'n') {
		return '\n';
	} else if (currChar == 'f') {
		return '\f';
	} else if (currChar == '\"') {
		return '"';
	} else if (currChar == '\'') {
		return '\'';
	} else if (currChar == '\\') {
		return '\\';
	}
	
	return ' '; // Use space to signify non escape character
}

Token Scanner::nextToken() {
	char ch;
	bool avoidEscape;
	std::string str;
	
s_start:
	lexeme = "";

	if (fs.eof())
		goto s_eof;

	readChar();

	if (fs.eof())
		goto s_eof;

	switch (currChar) {
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
			goto s_asn;
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
		
		default:
			if (isspace(currChar)) {
				goto s_start;
			} else if (isalpha2()) {
				goto s_ident;
			} else if (isdigit(currChar)) {
				goto s_int;
			} else {
				std::cerr << "ERROR: Unknown character '" << currChar << "' (INT: " << (int) currChar << ") on line " << lineNumber << ".\n";
				exit(-1);
			}
	}

s_str:
	str = "";

	do {
		avoidEscape = false;
		readChar();

		str += currChar;

		if (fs.eof())
			break;
		
		if (currChar == '\n') {
			lineNumber -= 1;
			break;			
		}

		if (currChar == '\\') {
			readChar();

			if (fs.eof())
				break;

			if (getEscape() == ' ') {
				ungetChar();
			} else {
				str = str.substr(0, str.size() - 1);
				str += getEscape();
				if (getEscape() == '\"')
					avoidEscape = true;
			}
		}

		
	} while (currChar != '\"' || avoidEscape);

	if (currChar != '\"') {
		std::cerr << "ERROR: String is missing closing quote on line " << lineNumber << ".\n";
		exit(-1);
	}

	return Token(lineNumber, STRING, lexeme.substr(1, lexeme.size()-2), str.substr(0, str.size()-1));
s_plus:
	return Token(lineNumber, PLUS);
s_minus:
	return Token(lineNumber, MINUS);
s_mult:
	return Token(lineNumber, MULT);
s_div:
	readChar();
	if (currChar == '/') {
		goto s_comment;
	} else {
		ungetChar();
		return Token(lineNumber, DIV);
	}
	
s_mod:
	return Token(lineNumber, MOD);
s_lt:
	readChar();
	if (currChar == '=') {
		goto s_lte;
	} else {
		ungetChar();
		return Token(lineNumber, LT);
	}
s_lte:
	return Token(lineNumber, LTE);
s_gt:
	readChar();
	if (currChar == '=') {
		goto s_gte;
	} else {
		fs.unget();
		return Token(lineNumber, GT);
	}
s_gte:
	return Token(lineNumber, GTE);
s_asn:
	readChar();
	if (currChar == '=') {
		goto s_eq;
	} else {
		ungetChar();
		return Token(lineNumber, ASSIGN);
	}
s_eq:
	return Token(lineNumber, EQ);
s_not:
	readChar();
	if (currChar == '=') {
		goto s_neq;
	} else {
		ungetChar();
		return Token(lineNumber, NOT);
	}
s_neq:
	return Token(lineNumber, NEQ);
s_and:
	readChar();
	if (currChar == '&') {
		return Token(lineNumber, AND);
	} else {
		std::cerr << "ERROR: Unexpected character '" << currChar << "' on line " << lineNumber << ". Expected '&'.\n";
		goto s_exit;
	}
s_or:
	ch = readChar();
	if (ch == '|') {
		return Token(lineNumber, OR);
	} else {
		std::cerr << "ERROR: Unexpected character '" << currChar << "' on line " << lineNumber << ". Expected '|'.\n";
		goto s_exit;
	}
s_lb:
	return Token(lineNumber, LB);
s_rb:
	return Token(lineNumber, RB);
s_lcb:
	return Token(lineNumber, LCB);
s_rcb:
	return Token(lineNumber, RCB);
s_semi:
	return Token(lineNumber, SEMICOLON);
s_comma:
	return Token(lineNumber, COMMA);

s_eof:
	return Token(lineNumber, ENDFILE);

s_ident:
	do {
		readChar();
		if (fs.eof())
			break;
	} while (isalpha2() || isdigit(currChar));

	ungetChar();

	if (checkReservedWord(lexeme) == IDENTIFIER)
		return Token(lineNumber, IDENTIFIER, lexeme);

	return Token(lineNumber, checkReservedWord(lexeme));

s_int:
	do {
		readChar();
		if (fs.eof())
			break;
	} while (isdigit(currChar));

	ungetChar();
	return Token(lineNumber, INTEGER, lexeme);
s_comment:
	do {
		readChar();	
		if (fs.eof())
			goto s_eof;
	} while (!newLine());

	goto s_start;
s_exit:
	exit(-1);
}