#ifndef JOBELL_CONST_AST
#define JOBELL_CONST_AST 1

// Main
#include <string>
#include <map>

using namespace std;

// Arr
class ArrConstAST : public ExprAST {
public:
	vector<ExprAST*> Vals;
	ArrConstAST(vector<ExprAST*> vals) : Vals(vals) {
		//
		ExprAST *Ty = new ArrTyExprAST(vals.size(), vals[0]->GetTy().TypeExpr);
		SetTy(Ty->GetTy());
		SetConst(true);
	}
	
	virtual BaseIR *Codegen();
};

// Vec
class VecConstAST : public ExprAST {
public:
	vector<ExprAST*> Vals;
	VecConstAST(vector<ExprAST*> vals) : Vals(vals) {
		//
		ExprAST *Ty = new VecTyExprAST(vals.size(), vals[0]->GetTy().TypeExpr);
		SetTy(Ty->GetTy());
		SetConst(true);
	}
	
	virtual BaseIR *Codegen();
};

// Константа
class ConstOperAST : public OperAST {
public:
	string IdName, Ty;
	ExprAST *Val;
	ConstOperAST(const string name, ExprAST *val, const string ty) : IdName(name), Val(val), Ty(ty) {}
	virtual BaseIR *Codegen();
};

#endif
