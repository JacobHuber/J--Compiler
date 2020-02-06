// Jacob Huber - 30052459
// CPSC 411 - TUT 03

#include "token.h"
#include <string> // std::string
#include <fstream> // std::fstream

class Scanner {
	int lineNumber = 1; // Start line count 1 (Used to set token line number)
	std::string fileName; // The name of the file to scan
	std::fstream fs; // Initialize file stream variable

	std::string lexeme = ""; // Used to hold current lexeme as it gets built
	char currChar; // The currChar that has been read

	public:
		// Constructors
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