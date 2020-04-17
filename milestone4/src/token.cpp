// Jacob Huber - 30052459
// CPSC 411 - TUT 03

#include "token.h"

// Used for getting string representation of enums of TOKENTYPE
const std::string names[39] = { 
	"ENDFILE", 
	"IDENTIFIER", "STRING", "INTEGER",
	"PLUS", "MINUS", "MULT", "DIV", "MOD", "LT", "GT", "LTE", "GTE", "ASSIGN", "EQ", "NEQ", "NOT", "AND", "OR",
	"LB", "RB", "LCB", "RCB", "SEMICOLON", "COMMA",
	"TRUE", "FALSE", "BOOLEAN", "INT", "VOID", "IF", "ELSE", "WHILE", "BREAK", "RETURN"
};

Token::Token() {

}

// Constructor for non-lexeme Tokens
Token::Token(int n, TOKENTYPE t) {
	lineNumber = n;
	type = t;
	doesExist = true;
}

// Constructor for IDENTIFIERs, and INTEGERs
Token::Token(int n, TOKENTYPE t, std::string lex) {
	lineNumber = n;
	type = t;
	lexeme = lex;
	doesExist = true;
}

// Constructor for STRINGs
Token::Token(int n, std::string lex, std::string v) {
	lineNumber = n;
	type = STRING;
	lexeme = lex;
	val = v;
	doesExist = true;
}


int Token::getLineNumber() {
	return lineNumber;
}

TOKENTYPE Token::getType() {
	return type;
}

bool Token::hasLexeme() {
	return !lexeme.empty();
}

std::string Token::getLexeme() {
	return lexeme;
}

bool Token::hasVal() {
	return !val.empty();
}

std::string Token::getVal() {
	return val;
}

// Return string representation of TOKENTYPE
std::string Token::getTypeStr() {
	return names[type];
}

bool Token::exists() {
	return doesExist;
}