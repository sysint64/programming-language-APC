#ifndef CPP_LOOP_AST
#define CPP_LOOP_AST 1

// Main
#include <string>
#include <map>

using namespace std;

class WhileOperAST : public OperAST {
public:
	ExprAST *Cond;
	BlockOperAST Ops;
	
	WhileOperAST(ExprAST* cond, OperAST* ops) : Cond(cond), Ops(ops) {}
		
	virtual BaseIR *Codegen();
};

//
class ForOperAST : public OperAST {
public:
	ExprAST *Start;
	ExprAST *To;
	ExprAST *Step;
	ExprAST *Load;
	OperAST *Declare;
	BlockOperAST Do;
	bool DownTo;
	
	ForOperAST(ExprAST *Start, ExprAST *To, ExprAST *Step, ExprAST *Load, OperAST* Do, OperAST* Declare) :
		Start(Start), To(To), Load(Load), Do(Do), Step(Step), Declare(Declare) {}
		
	virtual BaseIR *Codegen();
};

#endif
