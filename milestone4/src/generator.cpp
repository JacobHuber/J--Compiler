// Jacob Huber - 30052459
// CPSC 411 - TUT 03

#include "generator.h"
#include <iostream> // cout, cerr


Generator::Generator(std::string fn, Node* r) {
	fileName = fn;

	/*fs.open(fileName.c_str(), std::fstream::out);

	if (!fs.is_open()) {
		std::cerr << "ERROR: Unable to open file '" << fileName.c_str() << "'\n";
		exit(-1);
	}*/

	root = r;
}

Generator::~Generator() {
	if (fs.is_open()) {
		fs.close();
	}
}

void Generator::closeScope() {
	scope_stack.pop();

	lvc = 0;
	pc = 0;
	//ws = 0;
}

void Generator::openScope() {
	sym new_scope;
	scope_stack.push(new_scope);
}

void Generator::scopeDeclare(std::string s, std::string v) {
	//std::cout << "Adding: " << s << "\n";
	scope_stack.top().insert( {s, v} );
}

std::string Generator::searchAll(std::string s) {
	std::string found;
	while (scope_stack.size() > 0) {
		sym map = scope_stack.top();
		sym::iterator i = map.find(s);

		if (i != map.end()) {
			found = i->second;
			break;
		}

		reserve_stack.push(map);
		scope_stack.pop();
	}

	while (reserve_stack.size() > 0) {
		sym map = reserve_stack.top();
		scope_stack.push(map);
		reserve_stack.pop();
	}

	return found;
}

std::string Generator::search(std::string s) {
	std::string found;
	
	sym map = scope_stack.top();
	sym::iterator i = map.find(s);

	if (i != map.end()) {
		found = i->second;
	}

	return found;
}

std::string Generator::addGlobal(Node* varDecl) {
	std::string ident = varDecl->getChildren()[1]->getToken().getLexeme();
	std::string out = "$G" + std::to_string(gvc);

	scopeDeclare(ident, out);

	gvc += 1;
	return out;
}

std::string Generator::addLocal(Node* varDecl) {
	std::string ident = varDecl->getChildren()[1]->getToken().getLexeme();
	std::string out = "$L" + std::to_string(lvc);

	scopeDeclare(ident, out);

	lvc += 1;
	return out;
}

std::string Generator::addParam(Node* varDecl) {
	std::string ident = varDecl->getChildren()[1]->getToken().getLexeme();
	std::string out = "$P" + std::to_string(pc);

	scopeDeclare(ident, out);

	pc += 1;
	return out;
}

std::string Generator::addFunction(Node* funcDecl) {
	std::string ident = funcDecl->getChildren()[1]->getToken().getLexeme();
	std::string out = "$F" + std::to_string(fc);

	scopeDeclare(ident, out);

	fc += 1;
	return out;
}

std::string Generator::addMain(Node* funcDecl) {
	std::string ident = funcDecl->getChildren()[0]->getToken().getLexeme();
	std::string out = "$main";

	scopeDeclare(ident, out);

	return out;
}

std::string Generator::createLocals() {
	std::string out;

	for (int i = 0; i < lvc; i++) {
		out += "(local $L" + std::to_string(i) + " i32)\n";
	}

	return out;
}

std::string Generator::createParams() {
	std::string out;

	for (int i = 0; i < pc; i++) {
		out += "(param $P" + std::to_string(i) + " i32)\n";
	}

	return out;
}

std::string Generator::getImports() {
	return "(import \"host\" \"exit\" (func $halt))\n(import \"host\" \"getchar\" (func $getchar (result i32)))\n(import \"host\" \"putchar\" (func $printc (param i32)))\n";
}

void Generator::generateCode() {
	std::string output;

	openScope(); // Global Scope
	scopeDeclare("getchar", "$getchar");
	scopeDeclare("halt", "$halt");
	scopeDeclare("printb", "$printb");
	scopeDeclare("printc", "$printc");
	scopeDeclare("printi", "$printi");
	scopeDeclare("prints", "$prints");

	output += "(module\n";

	output += getImports();

	output += "(start $main)\n";

	std::vector<Node*> globals = root->getChildren();

	std::string main;
	std::string functions;
	std::string globalDecls;

	addString("true");
	addString("false");

	for (int i = 0; i < globals.size(); i++) {
		NODETYPE gt = globals[i]->getType();

		if (gt == n_varDecl) {
			std::string name = addGlobal(globals[i]);
			globalDecls += "(global " + name + " (mut i32) (i32.const 0))\n";
		}
	}

	for (int i = 0; i < globals.size(); i++) {
		NODETYPE gt = globals[i]->getType();

		if (gt == n_funcDecl) {
			addFunction(globals[i]);
		}
	}

	for (int i = 0; i < globals.size(); i++) {
		NODETYPE gt = globals[i]->getType();

		if (gt == n_funcDecl) {
			functions += createFunction(globals[i]);
		}
	}

	for (int i = 0; i < globals.size(); i++) {
		NODETYPE gt = globals[i]->getType();

		if (gt == n_mainDecl) {
			main = createMain(globals[i]);
			break;
		}
	}

	output += globalDecls;
	output += functions;
	output += main;

	output += getRTS();

	output += strings;
	if (stringSize > 0) {
		output += "(memory 1)\n";
	}

	output += ")";

	/*
	for (auto it = scope_stack.top().begin(); it != scope_stack.top().end(); ++it) {
		std::string name = it->first;
		std::string var = it->second;

		std::cout << name << ": " << var;
		std::cout << "\n";
	}*/

	closeScope();

	std::cout << output;
}

std::string Generator::createMain(Node* main) {
	std::string output;
	addMain(main);
	output += "(func $main\n";

	openScope();
	std::string block = createBlock(main->getChildren()[1]);
	output += createLocals();
	output += block;
	closeScope();

	output += ")\n";
	return output;
}

std::string Generator::createFunction(Node* func) {
	std::string output;
	std::string name = searchAll(func->getChildren()[1]->getToken().getLexeme());
	output += "(func " + name + "\n";

	openScope();
	addParams(func->getChildren()[2]);
	std::string block = createBlock(func->getChildren()[3]);

	output += createParams();

	if (func->getChildren()[0]->getToken().getTypeStr().compare("VOID") != 0) {
		output += "(result i32)\n";
	}

	output += createLocals();

	output += block;
	closeScope();

	output += ")\n";
	return output;
}

void Generator::addParams(Node* paramList) {
	std::vector<Node*> params = paramList->getChildren();

	for (int i = 0; i < params.size(); i++) {
		Node* param = params[i];
		addParam(param);
	}
}

std::string Generator::createBlock(Node* block) {
	std::vector<Node*> c = block->getChildren();

	std::string output;

	for (int i = 0; i < c.size(); i++) {
		Node* child = c[i];

		switch (child->getType()) {
			case n_varDecl:
				addLocal(child);
				break;
			case n_assign:
				output += createAssign(child);
				break;
			case n_functionInvocation:
				output += createFunctionInvocation(child);
				break;
			case n_return:
				output += createReturn(child);
				break;
			case n_whilestatement:
				output += createWhile(child);
				break;
			case n_break:
				output += createBreak(child);
				break;
			case n_ifstatement:
				output += createIfStmt(child);
				break;
			default:
				break;
		}
	}

	return output;
}

std::string Generator::expression(Node* n) {
	std::string output;

	NODETYPE type = n->getType();

	switch (type) {
		case n_literal:
		case n_functionInvocation:
		case n_identifier:
			if (type == n_literal) {
				if (n->getToken().getTypeStr().compare("STRING") != 0) {
					output += "i32.const ";
				}
				output += createLiteral(n);
			} else if (type == n_functionInvocation) {
				output += createFunctionInvocation(n);
			} else {
				output += getIdentifier(n);
			}
			break;
		case n_unaryexpression:
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
			output += operation(n);
			break;
		default:
			break;
	}

	return output;
}

std::string Generator::operation(Node* n) {
	std::string output;

	NODETYPE type = n->getType();
	std::vector<Node*> children = n->getChildren();

	if (type == n_unaryexpression) {
		output += expression(children[1]);

		if (children[0]->getType() == n_minus) {
			output += "i32.const -1\n";
			output += "i32.mul\n";
		} else {
			output += "i32.eqz\n";
		}
	} else if (type == n_assign) {
		output += createAssign(n);
		output += getIdentifier(children[0]);
	} else {
		if (children.size() > 1) {
			output += expression(children[1]);
			output += expression(children[0]);

			switch (type) {
				case n_eq:
					output += "i32.eq";
					break;
				case n_neq:
					output += "i32.ne";
					break;
				case n_or:
					output += "i32.or";
					break;
				case n_and:
					output += "i32.and";
					break;
				case n_lt:
					output += "i32.lt_s";
					break;
				case n_gt:
					output += "i32.gt_s";
					break;
				case n_lte:
					output += "i32.le_s";
					break;
				case n_gte:
					output += "i32.ge_s";
					break;
				case n_plus:
					output += "i32.add";
					break;
				case n_minus:
					output += "i32.sub";
					break;
				case n_mult:
					output += "i32.mul";
					break;
				case n_div:
					output += "i32.div_s";
					break;
				case n_mod:
					output += "i32.rem_s";
					break;
				default:
					break;
			}
			output += "\n";
		}
	}

	return output;
}

std::string Generator::createAssign(Node* assign) {
	std::string output;

	Node* rhs = assign->getChildren()[1];
	output += expression(rhs);
	
	
	Node* lhs = assign->getChildren()[0];
	output += setIdentifier(lhs);

	return output;
}


std::string Generator::createLiteral(Node* literal) {
	std::string output;

	Token tok = literal->getToken();
	std::string type = tok.getTypeStr();


	if (type.compare("INTEGER") == 0) {
		output += tok.getLexeme();
	} else if (type.compare("FALSE") == 0) {
		output += "0";
	} else if (type.compare("TRUE") == 0) {
		output += "1";
	} else {
		// String
		std::string val = tok.getLexeme();

		strings += "(data 0 (i32.const " + std::to_string(stringSize) + ") \"" + val + "\")\n";

		output += "i32.const " + std::to_string(stringSize) + "\n";
		
		stringSize += (int) val.size();

		output += "i32.const " + std::to_string(val.size());
	}
	return output + "\n";
}

void Generator::addString(std::string s) {
	strings += "(data 0 (i32.const " + std::to_string(stringSize) + ") \"" + s + "\")\n";
	stringSize += (int) s.size();
}

std::string Generator::createFunctionInvocation(Node* invocation) {
	std::string output;

	std::vector<Node*> children = invocation->getChildren();

	std::string name = children[0]->getToken().getLexeme();

	std::string func = searchAll(name);


	// Add arguments here
	if (children.size() > 1) {
		std::vector<Node*> args = children[1]->getChildren();

		for (int i = 0; i < args.size(); i++) {
			Node* arg = args[i];

			output += expression(arg);
		}
	}
	



	output += "call " + func + "\n";

	if (name.compare("halt") == 0) {
		output += "unreachable\n";	
	}

	return output;
}

std::string Generator::getIdentifier(Node* identifier) {
	std::string name = identifier->getToken().getLexeme();

	std::string var = searchAll(name);

	if (var[1] == 'G') {
		return "get_global " + var + "\n";
	} else {
		return "get_local " + var + "\n";
	}
	
}

std::string Generator::setIdentifier(Node* identifier) {
	std::string name = identifier->getToken().getLexeme();

	std::string var = searchAll(name);

	if (var[1] == 'G') {
		return "set_global " + var + "\n";
	} else {
		return "set_local " + var + "\n";
	}
}

std::string Generator::createReturn(Node* ret) {
	std::string output;

	if (ret->getChildren().size() > 0) {
		Node* result = ret->getChildren()[0];

		if (result->getType() == n_assign) {
			output += createAssign(result);
			output += getIdentifier(result->getChildren()[0]);
		} else {
			output += expression(result);
		}
	}

	output += "return\n";
	return output;
}

std::string Generator::createWhile(Node* statement) {
	ws += 1;

	std::string output;

	std::vector<Node*> children = statement->getChildren();

	std::string condition = expression(children[0]);

	std::string block;

	if (children[1]->getType() == n_block) {
		block = createBlock(children[1]);	
	} else {
		block = createBlock(statement);
	}

	output += "(block $B" + std::to_string(ws) + "\n";
	output += "(loop $W" + std::to_string(ws) + "\n";

	output += block;
	output += condition;

	output += "br_if $W" + std::to_string(ws) + "\n";

	output += ")\n";
	output += ")\n";


	ws -= 1;
	return output;
}

std::string Generator::createBreak(Node* statement) {
	std::string output;

	output += "br $B" + std::to_string(ws) + "\n";

	return output;
}

std::string Generator::createIfStmt(Node* statement) {
	std::string output;

	std::vector<Node*> children = statement->getChildren();

	std::string condition = expression(children[0]);
	std::string block = createBlock(children[1]);


	output += condition;
	output += "if\n";
	output += block;
	if (children.size() > 2) {
		output += "else\n";

		std::vector<Node*> children2 = children[2]->getChildren();

		if (children2.size() > 0) {
			Node* elseBlock = children2[0];

			if (elseBlock->getType() == n_ifstatement) {
				output += createIfStmt(elseBlock);
			} else {
				output += createBlock(elseBlock);
			}
		}
	}

	output += "end\n";

	return output;
}

std::string Generator::getRTS() {
	std::string output;


	output += "(func $printi (param $i i32) (local $n i32)\nget_local $i\ni32.const 2000000000\ni32.ge_s\nif\ni32.const 50\ncall $printc\nget_local $i\ni32.const 2000000000\ni32.sub\nset_local $i\nend\nget_local $i\ni32.const -2000000000\ni32.le_s\nif\ni32.const 45\ncall $printc\ni32.const 50\ncall $printc\nget_local $i\ni32.const 2000000000\ni32.add\ni32.const -1\ni32.mul\nset_local $i\nend\nget_local $i\ni32.const 0\ni32.lt_s\nif\ni32.const 45\ncall $printc\nget_local $i\ni32.const -1\ni32.mul\nset_local $i\nend\n(i32.eqz\n(get_local $i))\nif ;; if (i == 0)\ni32.const 48\ncall $printc ;; print '0'\nelse ;; (i != 0)\n;; n = 10\ni32.const 10\nset_local $n\n;; while (i / n > 0)\nloop\n(i32.lt_u\n(i32.div_s\n(get_local $i)\n(get_local $n))\n(i32.const 10))\nif ;; if (i / n == 0) Found cap n (n digits)\nloop ;; while (n != 1)\n;; putchar (print left most digit)\n(i32.add\n(i32.const 48) ;; offset for ascii digits\n(i32.div_s ;; $i / $n (integer division)\n(get_local $i)\n(get_local $n)))\ncall $printc\n;; i = i - (i / n) * n (this gets rid of the highest order digit in i)\nget_local $i\nget_local $n\ni32.rem_s\nset_local $i\n;; n /= 10\n(i32.div_s\n(get_local $n)\n(i32.const 10))\nset_local $n\n;; loop statement\n(i32.ne\n(get_local $n)\n(i32.const 0))\nbr_if 0\nreturn\nend\nend\n;; Cap isn't found yet\n;; n *= 10\n(i32.mul\n(get_local $n)\n(i32.const 10))\nset_local $n\nbr 0\nend\nend)";

	output += ";; memory start location\n;; length of string (how many bytes)\n(func $prints (param $start i32) (param $bytes i32)\n;; current byte\n(local $i i32) \n;; every 4 bytes increase by 1 (load only loads 4 bytes at a time, gotta keep track of this)\n(local $offset i32)\n;; Initialize locals\ni32.const 0\nset_local $i\ni32.const 0\nset_local $offset\n(loop $charLoop\n;; loaded = load(start + offset) - loaded\n(i32.load\n(i32.add\n(get_local $start)\n(i32.mul\n(get_local $offset)\n(i32.const 4))))\n;; loaded = loaded % (2^( 8 * (i+1) )  )\n;; removes higher order bits\n(i32.sub\n(i32.const 24)\n(i32.mul\n(i32.const 8)\n(get_local $i)))\ni32.shl\ni32.const 24\ni32.shr_u\ncall $printc\n;; i += 1\n(i32.add\n(get_local $i)\n(i32.const 1))\nset_local $i\nget_local $i\ni32.const 4\ni32.ge_s\n;; if (i >= 4) {\n;;	i = 0\n;;	offset += 1\n;; }\nif\ni32.const 0\nset_local $i\n(i32.add\n(get_local $offset)\n(i32.const 1))\nset_local $offset\nend\n;; bytes <= (offset * 4) + i\n(i32.add\n(i32.mul\n(get_local $offset)\n(i32.const 4))\n(get_local $i))\nget_local $bytes\ni32.lt_s\nbr_if $charLoop))";

	output += "(func $printb (param $bool i32)\nget_local $bool\ni32.eqz\nif\ni32.const 4\ni32.const 5\ncall $prints\nelse\ni32.const 0\ni32.const 4\ncall $prints\nend)\n\n";

	return output;
}