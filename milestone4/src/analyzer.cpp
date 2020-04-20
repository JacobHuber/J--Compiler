// Jacob Huber - 30052459
// CPSC 411 - TUT 03

#include "analyzer.h"

#include <iostream> // cout, cerr


Analyzer::Analyzer(Node* ast_root) {
	root = ast_root;

	// Add predefined scope to scope stack
	openScope();

	stab_e getchar;
	getchar.name = "getchar";
	getchar.type = "f()";
	getchar.ret = "INT";
	scopeDeclare("getchar", getchar);

	stab_e halt;
	halt.name = "halt";
	halt.type = "f()";
	halt.ret = "VOID";
	scopeDeclare("halt", halt);

	stab_e printb;
	printb.name = "printb";
	printb.type = "f(BOOLEAN)";
	printb.ret = "VOID";
	scopeDeclare("printb", printb);

	stab_e printc;
	printc.name = "printc";
	printc.type = "f(INT)";
	printc.ret = "VOID";
	scopeDeclare("printc", printc);

	stab_e printi;
	printi.name = "printi";
	printi.type = "f(INT)";
	printi.ret = "VOID";
	scopeDeclare("printi", printi);

	stab_e prints;
	prints.name = "prints";
	prints.type = "f(STRING)";
	prints.ret = "VOID";
	scopeDeclare("prints", prints);

	// Global Scope
	openScope();
}


void Analyzer::closeScope() {
	scope_stack.pop();
}

void Analyzer::openScope() {
	stab new_scope;
	scope_stack.push(new_scope);
}

void Analyzer::scopeDeclare(std::string s, stab_e e) {
	//std::cout << "Adding: " << s << "\n";
	scope_stack.top().insert( {s, e} );
}

stab_e Analyzer::searchAll(std::string s) {
	stab_e found;
	while (scope_stack.size() > 0) {
		stab map = scope_stack.top();
		stab::iterator i = map.find(s);

		if (i != map.end()) {
			found = i->second;
			break;
		}

		reserve_stack.push(map);
		scope_stack.pop();
	}

	while (reserve_stack.size() > 0) {
		stab map = reserve_stack.top();
		scope_stack.push(map);
		reserve_stack.pop();
	}

	return found;
}

stab_e Analyzer::search(std::string s) {
	stab_e found;
	
	stab map = scope_stack.top();
	stab::iterator i = map.find(s);

	if (i != map.end()) {
		found = i->second;
	}

	return found;
}

// 1) Add all global declarations to scope stack
// 2) Pass up all type information 
stab_e Analyzer::passOne(Node* n, int depth) {
	std::vector<Node*> c = n->getChildren();
	NODETYPE t = n->getType();
	//std::cout << "Calling on node: " << n.getTypeStr() << "\n";
	
	stab_e info;
	
	if (depth == 1 && t == n_varDecl) {
		stab_e var;
		var.name = c[1]->getToken().getLexeme();
		var.type = c[0]->getToken().getTypeStr();

		stab_e result = search(var.name);
		if (result.name.empty()) {
			scopeDeclare(var.name, var);
		} else {
			error = true;
			int lineNo = c[1]->getToken().getLineNumber();
			std::cerr << "ERROR: Variable '" << var.name << "' redefined on line " << lineNo << ".\n";
		}
	} else if (depth == 2 && t == n_type) {
		return info;
	} else {
		
		if (c.size() > 0) {
			switch (t) {
				case n_start:
				case n_funcDecl:
				case n_formalParamList:
				case n_formalParam:

					for (int i = 0; i < c.size(); i++) {
						stab_e result = passOne(c[i], depth + 1);

						if (!result.type.empty() && i > 0 && !info.type.empty()) {
							info.type += ", ";
						}

						info.type += result.type;
					}
				break;	
			}
			
		}


		if (t == n_type) {
			std::string type = n->getToken().getTypeStr();

			//std::cout << type << ", " << depth << "\n";

			stab_e record;
			record.type = type;

			n->setRecord(record);
			
			return record;

		} else if (t == n_mainDecl) {
			if (md_amount == 0) {
				md_amount = 1;
			} else {
				error = true;
				int lineNo = c[0]->getToken().getLineNumber();
				std::cerr << "ERROR: Multiple main declarations found on line " << lineNo << ".\n";
			}

			stab_e record;
			record.name = c[0]->getToken().getLexeme();
			record.type = "mf()";
			record.ret = "VOID";

			n->setRecord(record);

			stab_e result = search(record.name);
			if (result.name.empty()) {
				scopeDeclare(record.name, record);
			} else {
				error = true;
				int lineNo = c[0]->getToken().getLineNumber();
				std::cerr << "ERROR: Function '" << record.name << "' redefined on line " << lineNo << ".\n";
			}
		} else if (t == n_funcDecl) {
			stab_e record = n->getRecord();
			record.name = c[1]->getToken().getLexeme();
			record.type = "f(" + info.type + ")";
			record.ret = c[0]->getToken().getTypeStr();;

			n->setRecord(record);

			stab_e result = search(record.name);
			if (result.name.empty()) {
				scopeDeclare(record.name, record);
			} else {
				error = true;
				int lineNo = c[1]->getToken().getLineNumber();
				std::cerr << "ERROR: Function '" << record.name << "' redefined on line " << lineNo << ".\n";
			}
		}
	}
	
	return info;
}

void Analyzer::passTwo(Node* n) {
	std::vector<Node*> c = n->getChildren();

	for (int i = 0; i < c.size(); i++) {
		if (c[i]->getType() == n_funcDecl || c[i]->getType() == n_mainDecl) {
			openScope();
			passTwoFunction(c[i]);

			/*for (auto it = scope_stack.top().begin(); it != scope_stack.top().end(); ++it) {
				std::string name = it->first;
				stab_e entry = it->second;

				std::cout << name << " | Type: " << entry.type;
				if (!entry.ret.empty())
					std::cout << " | Ret: " << entry.ret;
				std::cout << "\n";
			}*/

			closeScope();
		}
	}
}

void Analyzer::passTwoFunction(Node* n) {
	std::vector<Node*> c = n->getChildren();
	NODETYPE t = n->getType();
	
	if (t == n_formalParam || t == n_varDecl) {
		std::string varType = c[0]->getToken().getTypeStr();
		std::string varName = c[1]->getToken().getLexeme();

		stab_e record;

		record.name = varName;
		record.type = varType;

		stab_e result = search(record.name);
		if (result.name.empty()) {
			scopeDeclare(record.name, record);
		} else {
			error = true;
			int lineNo = c[1]->getToken().getLineNumber();
			std::cerr << "ERROR: Variable '" << record.name << "' redefined on line " << lineNo << ".\n";
		}
	} else if (t == n_identifier) {

		std::string varName = n->getToken().getLexeme();

		stab_e result = searchAll(varName);

		if (result.name.empty()) {
			error = true;
			int lineNo = n->getToken().getLineNumber();
			std::cerr << "ERROR: Undeclared identifier '" << varName << "' on line " << lineNo << ".\n";
		} else {
			n->setRecord(result);
		}
	} else {
		for (int i = 0; i < c.size(); i++) {
			passTwoFunction(c[i]);
		}
	}
}

void Analyzer::passThree(Node* n) {
	std::vector<Node*> c = n->getChildren();
	NODETYPE t = n->getType();

	// Keep track of function that nodes belong to
	if (t == n_funcDecl) {
		func_record = n->getRecord();
	}

	// Recursive Calls
	for (int i = 0; i < c.size(); i++) {
		passThree(c[i]);
	}

	
	typeChecking(n);
	ifWhileCond(n);

	valueReturn(n);

	funcCall(n);
}

// Type mismatch for an operator (||, &&, ==, !=, =, <, >, <=, >=, +, - (unary and binary), *, /, %, !)
// n_minus, n_not, n_mult, n_div, n_mod, n_plus, n_lt, n_gt, n_lte, n_gte, n_and, n_or, n_eq, n_neq
std::string Analyzer::typeCheckBi(Node* n) {
	std::vector<Node*> c = n->getChildren();
	NODETYPE t = n->getType();

	if (c.size() > 0) {
		std::string lh_type;
		std::string rh_type;

		NODETYPE lhn_type = c[0]->getType();
		if (lhn_type == n_literal) {
			std::string lit_type = c[0]->getToken().getTypeStr();

			if (lit_type.compare("INTEGER") == 0) {
				lh_type = "INT";
			} else if (lit_type.compare("TRUE") == 0 || lit_type.compare("FALSE") == 0) {
				lh_type = "BOOLEAN";
			} else {
				error = true;
				int lineNo = c[0]->getToken().getLineNumber();
				std::cerr << "ERROR: Unexpected type '" << lit_type << "' on line " << lineNo << ".\n";
				lh_type = lit_type;
			}
		} else if (lhn_type == n_functionInvocation) {
			Node* ident = c[0]->getChildren()[0];
			lh_type = ident->getRecord().ret;
		} else {
			lh_type = c[0]->getRecord().type;
		}

		NODETYPE rhn_type = c[1]->getType();
		if (rhn_type == n_literal) {
			std::string lit_type = c[1]->getToken().getTypeStr();

			if (lit_type.compare("INTEGER") == 0) {
				rh_type = "INT";
			} else if (lit_type.compare("TRUE") == 0 || lit_type.compare("FALSE") == 0) {
				rh_type = "BOOLEAN";
			} else {
				error = true;
				int lineNo = c[1]->getToken().getLineNumber();
				std::cerr << "ERROR: Unexpected type '" << lit_type << "' on line " << lineNo << ".\n";
				rh_type = lit_type;
			}
		} else if (rhn_type == n_functionInvocation) {
			Node* ident = c[1]->getChildren()[0];
			rh_type = ident->getRecord().ret;
		} else {
			rh_type = c[1]->getRecord().type;
		}

		if (lh_type.compare(rh_type) != 0) {
			error = true;
			int lineNo = c[0]->getToken().getLineNumber();
			std::cerr << "ERROR: Types, '" << lh_type << "', and '" << rh_type << "', do not match on line " << lineNo << ".\n";
		} else {
			switch (t) {
				case n_eq:
				case n_neq:
					if (lh_type.compare("INT") != 0 && lh_type.compare("BOOLEAN") != 0) {
						error = true;
						int lineNo = c[0]->getToken().getLineNumber();
						std::cerr << "ERROR: Unexpected type '" << lh_type << "' on line " << lineNo << ".\n";
					}
					return "BOOLEAN";
					break;
				case n_assign:
					if (lh_type.compare("INT") != 0 && lh_type.compare("BOOLEAN") != 0) {
						error = true;
						int lineNo = c[0]->getToken().getLineNumber();
						std::cerr << "ERROR: Unexpected type '" << lh_type << "' on line " << lineNo << ".\n";
					}

					return lh_type;

					break;
				case n_or:
				case n_and:
					if (lh_type.compare("BOOLEAN") != 0) {
						error = true;
						int lineNo = c[0]->getToken().getLineNumber();
						std::cerr << "ERROR: Unexpected type '" << lh_type << "' on line " << lineNo << ". Expected 'BOOLEAN'.\n";
					}
					return "BOOLEAN";
					break;
				case n_lt:
				case n_gt:
				case n_lte:
				case n_gte:
					if (lh_type.compare("INT") != 0) {
						error = true;
						int lineNo = c[0]->getToken().getLineNumber();
						std::cerr << "ERROR: Unexpected type '" << lh_type << "' on line " << lineNo << ". Expected 'INT'.\n";
					}
					return "BOOLEAN";
					break;
				case n_plus:
				case n_minus:
				case n_mult:
				case n_div:
				case n_mod:
					if (lh_type.compare("INT") != 0) {
						error = true;
						int lineNo = c[0]->getToken().getLineNumber();
						std::cerr << "ERROR: Unexpected type '" << lh_type << "' on line " << lineNo << ". Expected 'INT'.\n";
					}
					return "INT";
					break;
				default:
					break;
			}
		}	
	}

	return "";
}

std::string Analyzer::typeCheckUnary(Node* n) {
	std::vector<Node*> c = n->getChildren();

	if (c.size() > 0) {
		NODETYPE t = c[0]->getType();

		std::string lh_type;

		NODETYPE lhn_type = c[1]->getType();
		if (lhn_type == n_literal) {
			std::string lit_type = c[1]->getToken().getTypeStr();

			if (lit_type.compare("INTEGER") == 0) {
				lh_type = "INT";
			} else if (lit_type.compare("TRUE") == 0 || lit_type.compare("FALSE") == 0) {
				lh_type = "BOOLEAN";
			} else {
				error = true;
				int lineNo = c[0]->getToken().getLineNumber();
				std::cerr << "ERROR: Unexpected type '" << lit_type << "' on line " << lineNo << ".\n";
				lh_type = lit_type;
			}
		} else if (lhn_type == n_functionInvocation) {
			Node* ident = c[1]->getChildren()[0];
			lh_type = ident->getRecord().ret;
		} else {
			lh_type = c[1]->getRecord().type;
		}

		switch (t) {
			case n_not:
				if (lh_type.compare("BOOLEAN") != 0) {
					error = true;
					int lineNo = c[1]->getToken().getLineNumber();
					std::cerr << "ERROR: Unexpected type '" << lh_type << "' on line " << lineNo << ". Expected 'BOOLEAN'.\n";
				}
				return "BOOLEAN";
				break;
			case n_minus:
				if (lh_type.compare("INT") != 0) {
					error = true;
					int lineNo = c[1]->getToken().getLineNumber();
					std::cerr << "ERROR: Unexpected type '" << lh_type << "' on line " << lineNo << ". Expected 'INT'.\n";
				}
				return "INT";
				break;
			default:
				break;
		}
	}

	return "";
}

void Analyzer::typeChecking(Node* n) {
	std::string type;
	switch (n->getType()) {
		case n_unaryexpression:
			type = typeCheckUnary(n);
			break;
		case n_eq:
		case n_neq:
		case n_assign:
		case n_or:
		case n_and:
		case n_lt:
		case n_gt:
		case n_lte:
		case n_gte:
		case n_plus:
		case n_minus:
		case n_mult:
		case n_div:
		case n_mod:
			type = typeCheckBi(n);
			break;
		default:
			break;
	}

	if (!type.empty()) {
		stab_e record = n->getRecord();
		record.type = type;
		n->setRecord(record);
	}
}

void Analyzer::ifWhileCond(Node* n) {
	std::vector<Node*> c = n->getChildren();
	NODETYPE t = n->getType();

	if (t == n_ifstatement || t == n_whilestatement) {
		NODETYPE ct = c[0]->getType();
		bool pass = true;
		int lineNo;

		if (ct == n_literal) {
			std::string res = c[0]->getToken().getTypeStr();
			lineNo = c[0]->getToken().getLineNumber();
			pass = (res.compare("TRUE") == 0 || res.compare("FALSE") == 0);
		} else if (ct == n_functionInvocation) {
			std::string type = c[0]->getChildren()[0]->getRecord().ret;
			lineNo = c[0]->getChildren()[0]->getToken().getLineNumber();
			pass = (type.compare("BOOLEAN") == 0);
		} else {
			std::string type = c[0]->getRecord().type;
			lineNo = c[0]->getToken().getLineNumber();
			pass = (type.compare("BOOLEAN") == 0);
		}

		if (!pass) {
			error = true;
			
			std::cerr << "ERROR: If/While condition must be of 'BOOLEAN' type on line " << lineNo << ".\n";
		}
	}
}

void Analyzer::valueReturn(Node* n) {
	NODETYPE t = n->getType();

	if (t == n_return) {
		std::vector<Node*> c = n->getChildren();
		std::string ret_type = "VOID";

		if (c.size() > 0) {
			NODETYPE ct = c[0]->getType();

			if (ct == n_literal) {
				std::string lit_type = c[0]->getToken().getTypeStr();

				if (lit_type.compare("INTEGER") == 0) {
					ret_type = "INT";
				} else if (lit_type.compare("TRUE") == 0 || lit_type.compare("FALSE") == 0) {
					ret_type = "BOOLEAN";
				} else {
					error = true;
					int lineNo = c[0]->getToken().getLineNumber();
					std::cerr << "ERROR: Unexpected type '" << lit_type << "' on line " << lineNo << ".\n";
					ret_type = lit_type;
				}
			} else if (ct == n_functionInvocation) {
				Node* ident = c[0]->getChildren()[0];
				ret_type = ident->getRecord().ret;
			} else {
				ret_type = c[0]->getRecord().type;
			}
		}

		if (ret_type.compare(func_record.ret) != 0) {
			error = true;
			int lineNo = n->getToken().getLineNumber();
			std::cerr << "ERROR: Value returned from function '" << func_record.name << "' has wrong type on line " << lineNo << ". Should be '" << func_record.ret << "'.\n";
		}
	}
}

void Analyzer::passFive(Node* n) {
	std::vector<Node*> c = n->getChildren();

	for (int i = 0; i < c.size(); i++) {
		NODETYPE ct = c[i]->getType();

		if (ct == n_funcDecl) {
			func_record = c[i]->getRecord();
			if (func_record.ret.compare("VOID") != 0 && !funcCanReturn(c[i])) {
				error = true;
				int lineNo = c[i]->getChildren()[0]->getToken().getLineNumber();
				std::cerr << "ERROR: Not all control paths of function '" << func_record.name << "' return. Line " << lineNo << ".\n";
			}
		}
	}
}

bool Analyzer::funcCanReturn(Node* n) {
	NODETYPE t = n->getType();

	if (t == n_block && blockHasReturn(n)) {
		return true;
	} else {
		std::vector<Node*> c = n->getChildren();

		bool all = false;

		for (int i = 0; i < c.size(); i++) {
			all = all || funcCanReturn(c[i]);
		}

		return all;
	}
}

bool Analyzer::blockHasReturn(Node* n) {
	bool ret = false;

	std::vector<Node*> c = n->getChildren();

	for (int i = 0; i < c.size(); i++) {
		NODETYPE ct = c[i]->getType();

		if (ct == n_return) {
			ret = true;
			break;
		}
	}

	return ret;
}

void Analyzer::funcCall(Node* n) {
	NODETYPE t = n->getType();

	if (t == n_functionInvocation) {
		std::vector<Node*> c = n->getChildren();

		std::string funcName = c[0]->getToken().getLexeme();

		stab_e record = searchAll(funcName);
		std::string funcType = record.type;

		if (!funcType.empty()) {
			if (funcType.compare("mf()") == 0) {
				error = true;
				int lineNo = c[0]->getToken().getLineNumber();
				std::cerr << "ERROR: The main function cannot be called. Line " << lineNo << ".\n";
			} else if (c.size() > 1) {
				std::vector<Node*> args = c[1]->getChildren();


				std::string callType = "f(";
				for (int i = 0; i < args.size(); i++) {
					Node* arg = args[i];
					NODETYPE arg_type = arg->getType();

					if (i > 0) {
						callType += ", ";
					}


					if (arg_type == n_literal) {
						std::string lit_type = arg->getToken().getTypeStr();

						if (lit_type.compare("INTEGER") == 0) {
							callType += "INT";
						} else if (lit_type.compare("TRUE") == 0 || lit_type.compare("FALSE") == 0) {
							callType += "BOOLEAN";
						} else {
							callType += "STRING";
						}
					} else if (arg_type == n_functionInvocation) {
						Node* ident = arg->getChildren()[0];
						callType += ident->getRecord().ret;
					} else {
						callType += arg->getRecord().type;
					}
				}
				callType += ")";

				if (callType.compare(funcType) != 0) {
					error = true;
					int lineNo = c[0]->getToken().getLineNumber();
					std::cerr << "ERROR: Number/type of arguments in a function call doesn't match the function's declaration. Line " << lineNo << ".\n";
					std::cerr << "CT: '" << callType << "' | FT: '" << funcType << "'.\n";
				}
			}
		}


	}
}

void Analyzer::passFour(Node* n) {
	bool while_statement = false;

	switch (n->getType()) {
		case n_whilestatement:
			ws_depth += 1;
			break;
		case n_block:
			bl_depth += 1;
			break;
		case n_break:
			if (ws_depth <= 0) {
				error = true;
				int lineNo = n->getToken().getLineNumber();
				std::cerr << "ERROR: Break statement outside of while statement. Line " << lineNo << ".\n";
			}
			break;
		case n_varDecl:
			if (bl_depth > 1) {
				error = true;
				int lineNo = n->getChildren()[0]->getToken().getLineNumber();
				std::cerr << "ERROR: Local declarations must be in outermost block. Line " << lineNo << ".\n";
			}
			break;
		default:
			break;
	}

	std::vector<Node*> c = n->getChildren();
	if (c.size() > 0) {
		for (int i = 0; i < c.size(); i++) {
			passFour(c[i]);
		}
	}



	switch (n->getType()) {
		case n_whilestatement:
			ws_depth -= 1;
			break;
		case n_block:
			bl_depth -= 1;
			break;
		default:
			break;
	}
}

Node* Analyzer::test() {
	Node* a = new Node(n_start);
	Node* b = new Node(n_varDecl);
	Node* c = new Node(n_type);

	b->addChild(c);
	a->addChild(b);

	stab_e record;
	record.type = "BOOLEAN";
	c->setRecord(record);

	return a;
}

Node* Analyzer::analyzeAST() {
	
	passOne(root, 0);
	passTwo(root);
	passThree(root);
	passFour(root);
	
	//passFive(root);
	
	/*
	for (auto it = scope_stack.top().begin(); it != scope_stack.top().end(); ++it) {
		std::string name = it->first;
		stab_e entry = it->second;

		std::cout << name << " | Type: " << entry.type;
		if (!entry.ret.empty())
			std::cout << " | Ret: " << entry.ret;
		std::cout << "\n";
	}*/

	if (md_amount == 0) {
		error = true;
		std::cerr << "ERROR: No main declaration found.\n";
	}

	if (error) {
		exit(-1);
	}

	return root;
}