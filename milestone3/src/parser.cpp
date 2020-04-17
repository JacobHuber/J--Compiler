// Jacob Huber - 30052459
// CPSC 411 - TUT 03

#include "parser.h"
#include <iostream> // cout, cerr
#include <stdlib.h> // exit

// Parser constructor
//
// Passes in filename to its scanner.
Parser::Parser(std::string fileName) {
	s.openFile(fileName);
}

// Deconstructor for parser
Parser::~Parser() {

}

void Parser::status() {
	if (debugMode) {
		std::cout << "lookAhead (lineNumber " << lookAhead.getLineNumber() << ") = " << lookAhead.getTypeStr() << "\n";
	}
}

void Parser::match(TOKENTYPE t) {
	fetch();
	if (lookAhead.getType() != t) {
		// Very minimal error message
		Token tempToken = Token(0, t);
		std::cerr << "ERROR: Unexpected token (" << lookAhead.getTypeStr() << ") on line " << lookAhead.getLineNumber() << ".\nExpected (" << tempToken.getTypeStr() << ")\n";
		exit(-1);
	}
}

void Parser::fetch() {
	if (doPause) {
		doPause = false;
	} else if (doReplace) {
		lookAhead = replace;
		doReplace = false;
	} else {
		lookAhead = s.nextToken();
	}

}

void Parser::pause() {
	doPause = true;
}

void Parser::start() {
	fetch();

	//std::cout << "Start\n";
	if (lookAhead.getType() != ENDFILE) {
		globaldeclarations();
	}

	
}

void Parser::globaldeclarations() {
	if (debugMode) {
		std::cout << "Global Declarations\n";
	}
	while (lookAhead.getType() != ENDFILE) {
		globaldeclaration();
		fetch();
	}
}

void Parser::globaldeclaration() {
	if (debugMode) {
		std::cout << "Global Declaration\n";
	}
	Token type;
	Token id;
	switch (lookAhead.getType()) {
		case BOOLEAN:
		case INT:
		case VOID:
			type = lookAhead;
			match(IDENTIFIER);
			id = lookAhead;

			fetch();
			status();
			if (lookAhead.getType() == SEMICOLON){
				root.addChild(variabledeclarationhelper(type, id));
			} else {
				pause();
				root.addChild(functiondeclaration(type, id));
			}
			break;
		case IDENTIFIER:
			root.addChild(mainfunctiondeclaration());
			break;
		default:
			std::cerr << "ERROR: Expected token of type 'BOOLEAN, INT, VOID, or IDENTIFIER on line " << lookAhead.getLineNumber() << "\n";
			exit(-1);
			break;
	}
}

Node Parser::variabledeclaration() {
	Token type = lookAhead;
	match(IDENTIFIER);
	Token id = lookAhead;

	match(SEMICOLON);
	return variabledeclarationhelper(type, id);
}

Node Parser::variabledeclarationhelper(Token t, Token i) {
	Node vd = Node(n_varDecl);

	vd.addChild(typehelper(t));
	vd.addChild(identhelper(i));

	return vd;
}

Node Parser::functiondeclaration(Token t, Token i) {
	Node fd = Node(n_funcDecl);

	fd.addChild(typehelper(t));
	fd.addChild(identhelper(i));

	match(LB);
	fetch();
	// Function Formal Parameters
	fd.addChild(formalparameterlist());

	// Function Block
	fd.addChild(block());

	return fd;
}

Node Parser::formalparameterlist() {
	Node fpl = Node(n_formalParamList);

	while (lookAhead.getType() != RB) {
		Node fp = Node(n_formalParam);
		
		if (lookAhead.getType() != INT && lookAhead.getType() != BOOLEAN) {
			match(INT);
		}
		fp.addChild(type());
		
		match(IDENTIFIER);
		fp.addChild(ident());
		
		fpl.addChild(fp);

		fetch();
		if (lookAhead.getType() == COMMA) {
			fetch();
		}
	}

	return fpl;
}

Node Parser::mainfunctiondeclaration() {
	if (debugMode) {
		std::cout << "Main\n";
	}
	Node mfd = Node(n_mainDecl);

	// Identifier
	mfd.addChild(ident());

	match(LB);
	match(RB);

	// Block
	mfd.addChild(block());

	return mfd;
}

Node Parser::block() {
	if (debugMode) {
		std::cout << "Block\n";
	}
	Node blk = Node(n_block);

	match(LCB);
	fetch();
	while (lookAhead.getType() != RCB) {
		status();
		if (lookAhead.getType() == BOOLEAN || lookAhead.getType() == INT) {
			blk.addChild(variabledeclaration());
		} else {
			blk.addChild(statement());
		}
		fetch();
	}
	
	if (debugMode) {
		std::cout << "~ END OF BLOCK ~\n";
	}

	return blk;
}

Node Parser::statement() {
	if (debugMode) {
		std::cout << "Statement\n";
		status();
	}

	switch (lookAhead.getType()) {
		case LCB:
			pause();
			return block();
			break;
		case SEMICOLON:
			return Node(n_semicolon);
			break;
		case IDENTIFIER:
			return statementexpression();
			break;
		case BREAK:
			return breakstatement();
			break;
		case RETURN:
			return returnstatement();
			break;
		case IF:
			return ifstatement();
			break;
		case WHILE:
			return whilestatement();
			break;
		default:
			status();
			std::cerr << "Error on line " << lookAhead.getLineNumber() << "\n";
			exit(-1);
			break;
	}
}

Node Parser::assignment() {
	Token id = lookAhead;
	match(ASSIGN);
	return assignmenthelper(id);
}

Node Parser::assignmenthelper(Token i) {
	Node assgn = Node(n_assign);
	assgn.addChild(identhelper(i));
	fetch();
	assgn.addChild(assignmentexpression());

	return assgn;
}

Node Parser::functioninvocation() {
	Token id = lookAhead;
	return functioninvocationhelper(id);
}

Node Parser::functioninvocationhelper(Token i) {
	match(LB);
	fetch();
	//pause();

	Node fi = Node(n_functionInvocation);
	fi.addChild(identhelper(i));
	
	if (lookAhead.getType() != RB) {
		fi.addChild(argumentlist());
	} else {
		pause();
	}
	
	match(RB);

	return fi;
}

Node Parser::argumentlist() {
	Node arglist = Node(n_arglist);
	while (lookAhead.getType() != RB) {
		arglist.addChild(expression());
		fetch();
		if (lookAhead.getType() == COMMA) {
			fetch();
		}
	}
	pause();

	return arglist;
}

Node Parser::statementexpression() {
	if (debugMode) {
		std::cout << "Statement Expression\n";
	}
	status();
	Token id = lookAhead;

	fetch();

	Node se;

	// Either an assignment or functioninvocation
	if (lookAhead.getType() == ASSIGN) {
		se = assignmenthelper(id);
	} else {
		pause();
		se = functioninvocationhelper(id);
	}

	match(SEMICOLON);

	return se;
}

Node Parser::breakstatement() {
	Node brk = Node(n_break);
	brk.setToken(lookAhead);

	match(SEMICOLON);
	return brk;
}

Node Parser::returnstatement() {
	Node ret = Node(n_return);
	ret.setToken(lookAhead);

	fetch();

	if (lookAhead.getType() != SEMICOLON) {
		ret.addChild(expression());
	}

	return ret;
}

Node Parser::ifstatement() {
	if (debugMode) {
		std::cout << "If Statement\n";
	}
	Node ifs = Node(n_ifstatement);

	match(LB);
	fetch();
	ifs.addChild(expression());
	if (debugMode) {
		std::cout << "After Expression (in if)\n";
	}
	match(RB);
	fetch();
	ifs.addChild(statement());
	
	fetch();
	status();
	if (lookAhead.getType() == ELSE) {
		status();
		fetch();
		Node els = Node(n_elsestatement);
		els.addChild(statement());
		ifs.addChild(els);
	} else {
		pause();
	}

	return ifs;
}

Node Parser::whilestatement() {
	if (debugMode) {
		std::cout << "while statement\n";
	}
	status();
	Node whs = Node(n_whilestatement);

	match(LB);
	fetch();
	whs.addChild(expression());
	
	match(RB);
	fetch();
	whs.addChild(statement());

	return whs;
}

Node Parser::expression() {
	return assignmentexpression();
}

Node Parser::assignmentexpression() {
	if (debugMode) {
		std::cout << "Assignment Expression\n";
		status();
	}
	Token id = lookAhead;
	switch (lookAhead.getType()) {
		case MINUS:
		case NOT:
		case INTEGER:
		case STRING:
		case TRUE:
		case FALSE:
		case LB:
			return conditionalorexpression();
			break;
		case IDENTIFIER:
			fetch();
			if (lookAhead.getType() == ASSIGN) {
				return assignmenthelper(id);
			} else {
				doReplace = true;
				replace = lookAhead;
				lookAhead = id;
				return conditionalorexpression();
			}
			break;
		default:
			// ERROR
			status();
			std::cerr << "Error on line " << lookAhead.getLineNumber() << "\n";
			exit(-1);
			break;
	}
}

Node Parser::conditionalorexpression() {
	//std::cout << "Conditional Or Expression\n";
	status();
	Node cand = conditionalandexpression();

	Node op = conditionalorexpression2(cand);
	if (op.getChildren().size() > 0) {
		//op.addChildFront(cand);
		return op;
	}

	return cand;
}

Node Parser::conditionalorexpression2(Node n) {
	Node op;
	fetch();

	if (debugMode) {
		std::cout << "C or Exp 2\n";
		status();
	}

	switch (lookAhead.getType()) {
		case OR:
			op = Node(n_or);
			fetch();
			op.addChild(conditionalandexpression());
			break;
		default:
			pause();
			break;
	}

	if (op.getChildren().size() > 0) {
		Node op2 = conditionalorexpression2(n);

		if (op2.getChildren().size() > 0)
			op.addChild(op2);
		else
			op.addChild(n);
	}

	return op;
}

Node Parser::conditionalandexpression() {
	Node eq = equalityexpression();

	Node op = conditionalandexpression2(eq);
	if (op.getChildren().size() > 0) {
		//op.addChildFront(eq);
		return op;
	}

	return eq;
}

Node Parser::conditionalandexpression2(Node n) {
	Node op;

	fetch();
	switch (lookAhead.getType()) {
		case AND:
			op = Node(n_and);
			fetch();
			op.addChild(equalityexpression());
			break;
		default:
			pause();
			break;
	}

	if (op.getChildren().size() > 0) {
		Node op2 = conditionalandexpression2(n);

		if (op2.getChildren().size() > 0)
			op.addChild(op2);
		else
			op.addChild(n);
	}

	return op;
}

Node Parser::equalityexpression() {
	Node rel = relationalexpression();

	Node op = equalityexpression2(rel);
	if (op.getChildren().size() > 0) {
		//op.addChildFront(rel);
		return op;
	}

	return rel;
}

Node Parser::equalityexpression2(Node n) {
	Node op;

	fetch();
	switch (lookAhead.getType()) {
		case EQ:
			op = Node(n_eq);
			fetch();
			op.addChild(relationalexpression());
			break;
		case NEQ:
			op = Node(n_neq);
			fetch();
			op.addChild(relationalexpression());
			break;
		default:
			pause();
			break;
	}

	if (op.getChildren().size() > 0) {
		Node op2 = equalityexpression2(n);

		if (op2.getChildren().size() > 0)
			op.addChild(op2);
		else
			op.addChild(n);
	}

	return op;
}

Node Parser::relationalexpression() {
	Node add = additiveexpression();

	Node op = relationalexpression2(add);
	if (op.getChildren().size() > 0) {
		//op.addChildFront(add);
		return op;
	}

	return add;
}

Node Parser::relationalexpression2(Node n) {
	Node op;

	fetch();
	switch (lookAhead.getType()) {
		case LT:
			op = Node(n_lt);
			op.setToken(lookAhead);
			fetch();
			op.addChild(additiveexpression());
			break;
		case GT:
			op = Node(n_gt);
			op.setToken(lookAhead);
			fetch();
			op.addChild(additiveexpression());
			break;
		case LTE:
			op = Node(n_lte);
			op.setToken(lookAhead);
			fetch();
			op.addChild(additiveexpression());
			break;
		case GTE:
			op = Node(n_gte);
			op.setToken(lookAhead);
			fetch();
			op.addChild(additiveexpression());
			break;
		default:
			pause();
			break;
	}

	if (op.getChildren().size() > 0) {
		Node op2 = relationalexpression2(n);

		if (op2.getChildren().size() > 0)
			op.addChild(op2);
		else
			op.addChild(n);
	}

	return op;
}

Node Parser::additiveexpression() {
	Node mult = multiplicativeexpression();

	Node op = additiveexpression2(mult);
	if (op.getChildren().size() > 0) {
		//op.addChildFront(mult);
		return op;
	}

	return mult;
}

Node Parser::additiveexpression2(Node n) {
	Node op;

	fetch();
	switch (lookAhead.getType()) {
		case PLUS:
			op = Node(n_plus);
			op.setToken(lookAhead);
			fetch();
			op.addChild(multiplicativeexpression());
			break;
		case MINUS:
			op = Node(n_minus);
			op.setToken(lookAhead);
			fetch();
			op.addChild(multiplicativeexpression());
			break;
		default:
			pause();
			break;
	}

	if (op.getChildren().size() > 0) {
		Node op2 = additiveexpression2(n);

		if (op2.getChildren().size() > 0)
			op.addChild(op2);
		else
			op.addChild(n);
	}

	return op;
}

Node Parser::multiplicativeexpression() {
	Node unary = unaryexpression();

	Node op = multiplicativeexpression2(unary);
	if (op.getChildren().size() > 0) {
		//op.addChildFront(unary);
		return op;
	}

	return unary;
}

Node Parser::multiplicativeexpression2(Node n) {
	Node op;

	fetch();
	switch (lookAhead.getType()) {
		case MULT:
			op = Node(n_mult);
			fetch();
			op.addChild(unaryexpression());
			break;
		case DIV:
			op = Node(n_div);
			fetch();
			op.addChild(unaryexpression());
			break;
		case MOD:
			op = Node(n_mod);
			fetch();
			op.addChild(unaryexpression());
			break;
		default:
			pause();
			break;
	}

	if (op.getChildren().size() > 0) {
		Node op2 = multiplicativeexpression2(n);

		if (op2.getChildren().size() > 0)
			op.addChild(op2);
		else
			op.addChild(n);
	}

	return op;
}

Node Parser::unaryexpression() {
	if (debugMode) {
		std::cout << "Unary Expression\n";
		status();
	}

	Node op;
	Node unary = Node(n_unaryexpression);
	switch (lookAhead.getType()) {
		case MINUS:
			op = Node(n_minus);
			op.setToken(lookAhead);
			unary.addChild(op);
			fetch();
			unary.addChild(unaryexpression());
			return unary;
			break;
		case NOT:
			op = Node(n_not);
			op.setToken(lookAhead);
			unary.addChild(op);
			fetch();
			unary.addChild(unaryexpression());
			return unary;
			break;
		default:
			return postfixexpression();
			break;
	}
}

Node Parser::postfixexpression() {
	if (debugMode) {
		std::cout << "Postfix Expression\n";
		status();
	}

	Token first = lookAhead;

	Node temp;

	switch (lookAhead.getType()) {
		case INTEGER:
		case STRING:
		case TRUE:
		case FALSE:
			return literal();
			break;
		case LB:
			fetch();
			temp = expression();
			match(RB);
			return temp;
			break;
		case IDENTIFIER:
			fetch();
			pause();
			if (lookAhead.getType() == LB) {
				return functioninvocationhelper(first);
			} else {
				return identhelper(first);
			}
			break;
		default:
			std::cerr << "ERROR: Expected INTEGER, STRING, TRUE, FALSE, LB, or IDENTIFIER at line " << lookAhead.getLineNumber() << "\n";
			exit(-1);// ERROR
			break;
	}
}

Node Parser::literal() {
	Node lit = Node(n_literal);
	lit.setToken(lookAhead);

	return lit;
}

Node Parser::ident() {
	return identhelper(lookAhead);
}

Node Parser::identhelper(Token t) {
	Node ident = Node(n_identifier);
	ident.setToken(t);
	return ident;
}

Node Parser::type() {
	return typehelper(lookAhead);
}

Node Parser::typehelper(Token t) {
	Node typ = Node(n_type);
	typ.setToken(t);
	return typ;
}

Node* Parser::fetchAST() {
	start();
	return new Node(root);
	//printNode(root, 0);
}

void Parser::printNode(Node n, int indent) {
	for (int x = 0; x < indent; x++) {
		std::cout << "    ";
	}


	std::cout << n.getTypeStr();

	Token t = n.getToken();
	if (t.exists()) {
		std::cout << " {";
		std::cout << "lineno: '" << t.getLineNumber() << "', ";
		std::cout << "type: '" << t.getTypeStr() << "'";

		if (t.hasLexeme()) {
			std::cout << ", attr: '" << t.getLexeme() << "'";
		}
		std::cout << "}";
	}
	std::cout << "\n";
	/*
	if (n.getToken().getType() != NULL) {
		std::cout << ": " << n.getToken().getTypeStr() << "\n";
	}*/
	/*
	std::vector<Node> c = n.getChildren();
	for (int i = 0; i < c.size(); i++)
		printNode(c[i], indent + 1);*/
}