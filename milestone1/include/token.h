class Token {
	public:
		enum tokenType { IDENTIFIER, STRING, INTEGER, WORD, OPERATOR, OTHER, ENDFILE };

	private:
		int lineNumber;
		tokenType type;

	public:
		Token(int l);
		Token(int l, tokenType t);
		tokenType getType();
		int getLineNumber();
};

class Word : Token {
	public:
		enum word { TRUE, FALSE, BOOLEAN, INT, VOID, IF, ELSE, WHILE, BREAK, RETURN };
	
	private:
		word wordType;
	
	public:
		Word(int l, word t);
		word getWordType();
};

/*
class Operator : Token {
	enum type { +, -, *, /, %, <, >, <=, >=, =, ==, !=, !, &&, || };
};*/