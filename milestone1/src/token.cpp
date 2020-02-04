#include "token.h"

Token::Token(int l) {
	lineNumber = l;
}

Token::Token(int l, Token::tokenType t) {
	lineNumber = l;
	type = t;
}

Token::tokenType Token::getType() {
	return type;
}

int Token::getLineNumber() {
	return lineNumber;
}


Word::Word(int l, Word::word t) : Token::Token(l, Token::WORD) {
	wordType = t;
}

Word::word Word::getWordType() {
	return wordType;
}

