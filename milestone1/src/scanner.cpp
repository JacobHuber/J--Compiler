// Jacob Huber - 30052459
// CPSC 411 - TUT 03

#include "scanner.h"
#include <iostream> // cout, cerr
#include <stdlib.h> // exit

#include <ctype.h> // isalpha, isalnum, isspace, isdigit

// Scanner Constructor 
//
// String argument: File Name
Scanner::Scanner(std::string fn) {
	fileName = fn;

	fs.open(fileName.c_str(), std::fstream::in);

	if (!fs.is_open()) {
		std::cerr << "ERROR: Unable to open file '" << fileName.c_str() << "'\n";
		exit(-1);
	}
}

// Scanner Deconstructor
//
// Closes open file
Scanner::~Scanner() {
	if (fs.is_open()) {
		fs.close();
	}
}

// Helper Function
//
// Returns true if currChar represents
// a new line (false otherwise) so we
// know when to increment or decrement
// the line count.
bool Scanner::newLine() {
	return (currChar == '\n');
}

// Reads the next char from this
// scanner's fileStream.
// - Sets currChar to the read char
// - If new line, increments line count
// - Appends the read char to internal lexeme
void Scanner::readChar() {
	fs.get(currChar);

	if (newLine())
		lineNumber += 1;
	
	lexeme += currChar;
}

// Ungets the last char from this
// scanner's fileStream.
// - If last read char was a new line, decrements line count
// - Removes the last char from the internal lexeme
void Scanner::ungetChar() {
	fs.unget();
	
	if (newLine())
		lineNumber -= 1;

	lexeme = lexeme.substr(0, lexeme.size()-1);
}

// Helper Function
//
// Checks if the curr char isalpha
// or equal to '_' due to the way
// identifiers have been defined.
bool Scanner::isalpha2() {
	return isalpha(currChar) || currChar == '_';
}

// Helper Function
//
// This checks if a string matches
// any of the keywords and if it does
// then it will return that enum of
// its keyword, otherwise the enum
// for an identifier.
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

// Help Function
//
// Checks a character against
// known escape characters so
// that we know whether we need
// to add an escape character or
// not (to a string).
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

// The main function of the scanner.
//
// Returns the next found token in
// or reporting an error and exiting.
//
// Reads each character, switching states
// until it eventually finds a token.
// - Labels are used to define states
Token Scanner::nextToken() {
	// Used in s_str to know if the read " should be ignored as a closing quote
	bool avoidEscape; 

	// Used in s_str to hold string representation before passing to token (includes ACTUAL escape chars)
	std::string str;
	
s_start:
	// Clear last found lexeme
	lexeme = "";

	if (fs.eof())
		goto s_eof;

	readChar();

	if (fs.eof())
		goto s_eof;

	// Compare read char to known state entrances
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
			// Ignore whitespace
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
	// Clear last read string
	str = "";

	// Read character by character until reaching a closing quote
	// making sure that the closing quote wasn't an escaped quote
	// - Also checks for end of file within string
	// - Appends characters to str
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

	// If the string wasn't closed (ie no closing quote found) then error
	if (currChar != '\"') {
		std::cerr << "ERROR: String is missing closing quote on line " << lineNumber << ".\n";
		exit(-1);
	}

	// Return STRING Token with its lexeme and string value trimmed to exclude outer quotation marks
	return Token(lineNumber, lexeme.substr(1, lexeme.size()-2), str.substr(0, str.size()-1));

s_plus:
	return Token(lineNumber, PLUS);

s_minus:
	return Token(lineNumber, MINUS);

s_mult:
	return Token(lineNumber, MULT);

s_div:
	readChar();
	// If the char following the first / is another / then go to comment
	// else unget char and return DIV Token
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
	// If the char following the first < is = then go to lesser than equal
	// else unget char and return LT Token
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
	// If the char following the first > is = then go to greater than equal
	// else unget char and return GT Token
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
	// If the char following the first = is another = then go to equal
	// else unget char and return ASSIGN Token
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
	// If the char following the first ! is = then go to not equal
	// else unget char and return NOT Token
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
	// If the char following the first & is another & then return AND Token
	// else error
	if (currChar == '&') {
		return Token(lineNumber, AND);
	} else {
		std::cerr << "ERROR: Unexpected character '" << currChar << "' on line " << lineNumber << ". Expected '&'.\n";
		goto s_exit;
	}

s_or:
	readChar();
	// If the char following the first | is another | then return AND Token
	// else error
	if (currChar == '|') {
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
	// Read chars that are digits, alpha, or '_'
	// Stop when char is not any of those (or EOF)
	// 
	// Make sure read identifier is not a reserved word
	// if it is return the reserved word
	// else return the identifier
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
	// Read digits until char is no longer a digit or (EOF)
	// Return INTEGER Token
	do {
		readChar();
		if (fs.eof())
			break;
	} while (isdigit(currChar));

	ungetChar();
	return Token(lineNumber, INTEGER, lexeme);

s_comment:
	// Ignore ENTIRE line by read chars until reading a new line char
	// or by getting to EOF token
	//
	// DO NOT RETURN, go back to start looking for an actual token
	do {
		readChar();	
		if (fs.eof())
			goto s_eof;
	} while (!newLine());

	goto s_start;

s_exit:
	exit(-1);
}