#include "token.h"
#include <string> // std::string
#include <fstream> // std::ifstream

class Scanner {
	int lineNumber = 1;
	std::string fileName;
	std::fstream fs;

	std::string lexeme = ""; // Used to hold current lexeme as it gets built
	char currChar;

	public:
		Scanner(const Scanner&) = delete;
		Scanner(std::string fn);
		~Scanner();

		Token nextToken();

	private:
		char readChar();
		void ungetChar();
		bool newLine();

		bool isalpha2();

		char getEscape();
};