// Jacob Huber - 30052459
// CPSC 411 - TUT 03

#include "node.h"

// Used for getting string representation of enums of NODETYPE
const std::string node_names[36] = { "n_start",
	"n_literal", "n_type", "n_identifier", // Terminals
	"n_semicolon", "n_break", "n_return",
	
	"n_varDecl", "n_funcDecl", "n_mainDecl", "n_block",
	"n_formalParamList", "n_formalParam", "n_functionInvocation", "n_assign", "n_arglist",
	"n_ifstatement", "n_elsestatement", "n_whilestatement",

	"n_unaryexpression", "n_multexpression", "n_addexpression",

	"n_minus", "n_not", "n_mult", "n_div", "n_mod", "n_plus", "n_lt", "n_gt", "n_lte", "n_gte", "n_and", "n_or", "n_eq", "n_neq" };


Node::Node() {

}

Node::Node(NODETYPE n) {
	type = n;
}

// Node deconstructor
Node::~Node() {
}

// Return the children of this node
std::vector<Node> Node::getChildren() {
	return children;
}

// Add a child to this node
void Node::addChild(Node c) {
	children.push_back(c);
	c.setParent(this);
}

// Add a child to this node at the front of the vector
void Node::addChildFront(Node c) {
	children.insert(children.begin(), c);
	c.setParent(this);
}

// Remove a child from this node
void Node::removeChild() {
	if (children.size() > 0)
		children.pop_back();
}

void Node::setParent(Node *p) {
	parent = p;
}

Node* Node::getParent() {
	return parent;
}

void Node::setToken(Token t) {
	token = t;
}

Token Node::getToken() {
	return token;
}

std::string Node::getTypeStr() {
	return node_names[type];
}