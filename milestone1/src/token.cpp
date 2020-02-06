#include "token.h"

const std::string names[39] = { "IDENTIFIER", "STRING", "INTEGER", "ENDFILE",
						"PLUS", "MINUS", "MULT", "DIV", "MOD", "LT", "GT", "LTE", "GTE", "ASSIGN", "EQ", "NEQ", "NOT", "AND", "OR",
						"LB", "RB", "LCB", "RCB", "SEMICOLON", "COMMA",
						"TRUE", "FALSE", "BOOLEAN", "INT", "VOID", "IF", "ELSE", "WHILE", "BREAK", "RETURN" };


Token::Token(int n, TOKENTYPE t) {
	lineNumber = n;
	type = t;
}

Token::Token(int n, TOKENTYPE t, std::string lex) {
	lineNumber = n;
	type = t;
	lexeme = lex;
}


Token::Token(int n, TOKENTYPE t, std::string lex, std::string v) {
	lineNumber = n;
	type = t;
	lexeme = lex;
	val = v;
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

std::string Token::getTypeStr() {
	return names[type];
}