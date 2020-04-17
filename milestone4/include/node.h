// Jacob Huber - 30052459
// CPSC 411 - Tutorial 03

// Nodes used for syntactic analysis
// to create AST


#include "token.h"
#include <vector>

#pragma once

struct stab_e {
	std::string name;
	std::string type;
	std::string ret;
};

enum NODETYPE {
	n_start,
	n_literal, n_type, n_identifier, // Terminals
	n_semicolon, n_break, n_return,
	
	n_varDecl, n_funcDecl, n_mainDecl, n_block,
	n_formalParamList, n_formalParam, n_functionInvocation, n_assign, n_arglist,
	n_ifstatement, n_elsestatement, n_whilestatement,

	n_unaryexpression, n_multexpression, n_addexpression,

	n_minus, n_not, n_mult, n_div, n_mod, n_plus, n_lt, n_gt, n_lte, n_gte, n_and, n_or, n_eq, n_neq
};

class Node {
	private:
		NODETYPE type; // The type of the node
		std::vector<Node*> children;

		Node* parent;
		Token token; // Associated token with node

		stab_e record;

		void setParent(Node* p);

	public:
		Node();
		//Node(const Node &c);
		Node(NODETYPE n);
		~Node();

		std::vector<Node*> getChildren();
		void addChild(Node c);
		void addChildFront(Node c);
		void addChild(Node* c);
		void addChildFront(Node* c);
		void removeChild();

		
		Node* getParent();

		void setToken(Token t);
		Token getToken();

		std::string getTypeStr();
		NODETYPE getType();

		stab_e getRecord();
		void setRecord(stab_e s);

		void destroy();
};