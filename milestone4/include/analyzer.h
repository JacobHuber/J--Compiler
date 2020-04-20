// Jacob Huber - 30052459
// CPSC 411 - TUT 03

#include "node.h"
#include <stack>
#include <unordered_map>
#include <string>

/*
	Pass One
		An identifier is redefined within the same scope. [x]
		No main declaration found. [x]
		Multiple main declarations found. [x]

	Pass Two
		An identifier is redefined within the same scope. [x]
		An undeclared identifier is used. [x]


	Pass Three
		Type mismatch for an operator (||, &&, ==, !=, =, <, >, <=, >=, +, - (unary and binary), *, /, %, !). [x]
		An if- or while-condition must be of Boolean type. [x]

		A value returned from a function has the wrong type. [x]
		A void function can't return a value. [x]
		
		No return statement in a non-void function.
		A non-void function must return a value.

		The main function can't be called. [x]
		The number/type of arguments in a function call doesn't match the function's declaration. [x]


	Pass Four
		Break statements must be inside a while statement. [x]
		The main declaration can't have parameters. (kinda handled by the grammar) [x]?
		A local declaration was not in an outermost block. [x]
*/

typedef std::unordered_map<std::string, stab_e> stab;

class Analyzer {
	private:
		Node* root;

		bool error = false;

		
		// Pass 3
		stab_e func_record;

		// Pass 4
		int md_amount = 0;
		int ws_depth = 0;
		int bl_depth = 0;

		std::stack< stab > scope_stack;
		std::stack< stab > reserve_stack;

		void closeScope();
		void openScope();
		void scopeDeclare(std::string s, stab_e e);
		stab_e searchAll(std::string s);
		stab_e search(std::string s);


		stab_e passOne(Node* n, int depth);
		
		void passTwo(Node* n);
		void passTwoFunction(Node* n);
		
		void passThree(Node* n);
		std::string typeCheckBi(Node* n);
		std::string typeCheckUnary(Node* n);
		void typeChecking(Node* n);
		void ifWhileCond(Node* n);
		void valueReturn(Node* n);
		void funcCall(Node* n);

		void passFour(Node* n);

		void passFive(Node* n);
		bool funcCanReturn(Node* n);
		bool blockHasReturn(Node* n);

		Node* test();

			
		
		/*
		std::unordered_map<NODETYPE, TOKENTYPE[2]> unary_matches
		(

		);*/

	public:
		Analyzer(Node* ast_root);

		Node* analyzeAST();
};