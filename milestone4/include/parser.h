// Jacob Huber - 30052459
// CPSC 411 - TUT 03

#include "scanner.h"
#include "node.h"
#include "token.h"
#include <string>

#include <vector>

class Parser {
	private:
		Scanner s;
		Token lookAhead;
		bool doPause = false;
		bool doReplace = false;
		Token replace;

		bool debugMode = false;

		Node root = Node(n_start);

		void status();
		void match(TOKENTYPE t); 
		void fetch();
		void pause();


		void start();
		void globaldeclarations();
		void globaldeclaration();
		
		Node variabledeclaration();
		Node variabledeclarationhelper(Token t, Token i);
		Node functiondeclaration(Token t, Token i);
		Node formalparameterlist();
		Node mainfunctiondeclaration();
		
		Node block();
		Node statement();
		
		Node assignment();
		Node assignmenthelper(Token i);
		
		Node functioninvocation();
		Node functioninvocationhelper(Token i);
		Node argumentlist();
		
		Node statementexpression();
		Node breakstatement();
		Node returnstatement();
		Node ifstatement();
		Node whilestatement();

		Node expression();
		Node assignmentexpression();
		Node conditionalorexpression();
		Node conditionalorexpression2(Node n);
		Node conditionalandexpression();
		Node conditionalandexpression2(Node n);
		Node equalityexpression();
		Node equalityexpression2(Node n);
		Node relationalexpression();
		Node relationalexpression2(Node n);
		Node additiveexpression();
		Node additiveexpression2(Node n);
		Node multiplicativeexpression();
		Node multiplicativeexpression2(Node n);
		Node unaryexpression();
		Node postfixexpression();

		Node literal();
		Node ident();
		Node identhelper(Token t);
		Node type();
		Node typehelper(Token t);

		void printNode(Node n, int indent);

	public:
		Parser(std::string fileName);
		~Parser();

		Node* fetchAST();
};