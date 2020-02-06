#include <string>

enum TOKENTYPE { IDENTIFIER, STRING, INTEGER, ENDFILE, // 
				PLUS, MINUS, MULT, DIV, MOD, LT, GT, LTE, GTE, ASSIGN, EQ, NEQ, NOT, AND, OR, // Operators
				LB, RB, LCB, RCB, SEMICOLON, COMMA, // Punctuation
				TRUE, FALSE, BOOLEAN, INT, VOID, IF, ELSE, WHILE, BREAK, RETURN }; // Reserved Words

class Token {
	private:
		int lineNumber;
		TOKENTYPE type;
		std::string lexeme;
		std::string val;

	public:
		Token(int l, TOKENTYPE t);
		Token(int l, TOKENTYPE t, std::string lex);
		Token(int l, TOKENTYPE t, std::string lex, std::string val);

		int getLineNumber();
		TOKENTYPE getType();
		
		bool hasLexeme();
		std::string getLexeme();

		bool hasVal();
		std::string getVal();

		std::string getTypeStr();
};