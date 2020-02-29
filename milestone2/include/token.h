// Jacob Huber - 30052459
// CPSC 411 - TUT 03

#include <string>

#pragma once

// Enums to distinguish type of token
enum TOKENTYPE { 
	ENDFILE,
	IDENTIFIER, STRING, INTEGER, // Lexeme Tokens
	PLUS, MINUS, MULT, DIV, MOD, LT, GT, LTE, GTE, ASSIGN, EQ, NEQ, NOT, AND, OR, // Operator Tokens
	LB, RB, LCB, RCB, SEMICOLON, COMMA, // Punctuation Tokens
	TRUE, FALSE, BOOLEAN, INT, VOID, IF, ELSE, WHILE, BREAK, RETURN // Reserved Word Tokens
};

class Token {
	private:
		int lineNumber; // The line that the Token occurs on
		TOKENTYPE type; // The type of token (see enum above)
		std::string lexeme; // The lexeme of the read token (see lexeme tokens in enum above)
		std::string val; // The string value of a STRING token

		bool doesExist = false;
	public:
		// Constructors
		Token();
		Token(int l, TOKENTYPE t);
		Token(int l, TOKENTYPE t, std::string lex);
		Token(int l, std::string lex, std::string val);

		bool hasLexeme();
		bool hasVal();

		// Getters
		int getLineNumber();
		TOKENTYPE getType();
		std::string getLexeme();
		std::string getVal();
		std::string getTypeStr();

		bool exists();
};