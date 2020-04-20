// Jacob Huber - 30052459
// CPSC 411 - TUT 03

#include "analyzer.h"
#include <fstream> // std::fstream

typedef std::unordered_map<std::string, std::string> sym;



class Generator {
	private:	
		std::string fileName;
		std::fstream fs;

		Node* root;


		int gvc = 0;
		int lvc = 0;
		int pc = 0;
		int fc = 0;
		int ws = 0;

		int stringSize = 0;
		std::string strings;

		std::stack< sym > scope_stack;
		std::stack< sym > reserve_stack;


		void closeScope();
		void openScope();
		void scopeDeclare(std::string s, std::string v);
		std::string searchAll(std::string s);
		std::string search(std::string s);

		std::string addGlobal(Node* varDecl);
		std::string addLocal(Node* varDecl);
		std::string addParam(Node* varDecl);
		std::string addFunction(Node* funcDecl);
		std::string addMain(Node* funcDecl);


		std::string createLocals();
		std::string createParams();

		std::string getImports();
		std::string createMain(Node* main);
		std::string createFunction(Node*);
		void addParams(Node*);
		std::string createBlock(Node* block);
		std::string expression(Node*);
		std::string operation(Node*);



		std::string createAssign(Node*);
		std::string createLiteral(Node*);
		void addString(std::string);
		std::string createFunctionInvocation(Node*);
		std::string getIdentifier(Node*);
		std::string setIdentifier(Node*);

		std::string createReturn(Node*);
		std::string createWhile(Node*);
		std::string createBreak(Node*);
		std::string createIfStmt(Node*);
		std::string getRTS();


	public:
		Generator(std::string fn, Node* r);
		~Generator();

		void generateCode();
};