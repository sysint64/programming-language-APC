#ifndef CPP_IFCASE_AST
#define CPP_IFCASE_AST 1

// Main
#include <string>
#include <map>

using namespace std;

//
class IfOperAST : public OperAST {
public:
	ExprAST *Cond;
	BlockOperAST ThenOps, ElseOps;
	bool ElseB;
	
	IfOperAST(ExprAST* cond, OperAST* thenops, OperAST* elseops, const bool e) :
		Cond(cond), ThenOps(thenops), ElseB(e)
		/*Cond(cond), ThenOps(thenops), ElseOps(elseops), ElseB(e)*/ {
			if (e) ElseOps = elseops;
		}
	
	virtual BaseIR *Codegen();
};

// Case
class TdOperAST : public OperAST {
public:
	ExprAST *Val;
	OperAST *Ops;
	BaseIR  *Label;
	
	TdOperAST(ExprAST *val, OperAST *ops) : Val(val), Ops(ops) {}
	
	virtual BaseIR *Codegen();
};

//
class CaseOperAST : public OperAST {
public:
	ExprAST *Id;
	OperAST *ElseOp;
	vector<TdOperAST*> Tds;
	
	CaseOperAST(ExprAST *id, vector<TdOperAST*> tds, OperAST *eop = 0) : Id(id), Tds(tds) {
		ElseOp = eop;
	}
	
	virtual BaseIR *Codegen();
};

#endif
