#ifndef CPP_OPER_AST
#define CPP_OPER_AST 1

// Main
#include <map>

using namespace std;

//* Операторы

// ExprOpAST - Оператор выражения
class ExprOpAST : public OperAST {
public:
	ExprAST *Expr;
	ExprOpAST(ExprAST* Expr) : Expr(Expr) {}
	virtual BaseIR *Codegen();
};

class RetOvrOperAST : public OperAST {
public:
	ExprAST *Id;
	OperAST *Var;
	ExprAST *Ovr;

	RetOvrOperAST(ExprAST *id, OperAST *var, ExprAST *ovr)
		: Id(id), Var(var), Ovr(ovr) {}

	virtual BaseIR *Codegen();
};

// BreakOperAST - break;
class BreakOperAST : public OperAST {
public:
	virtual BaseIR *Codegen();
};

// BreakOperAST - break;
class ContinueOperAST : public OperAST {
public:
	virtual BaseIR *Codegen();
};

// ExitOperAST - exit
class ExitOperAST : public OperAST {
public:
	virtual BaseIR *Codegen();
};

// RetOperAST - return
class RetOperAST : public OperAST {
public:
	ExprAST *Val;
	RetOperAST(ExprAST* Val) : Val(Val) {}
	virtual BaseIR *Codegen();
};

// BlockAST - Блок кода
class BlockOperAST : public OperAST {
public:
	vector<OperAST*> ops;
	vector<BaseIR*>  Res;
	
	void Append(OperAST* op) {
		//op->SetCurLine();		
		//BlockOperAST* b = dynamic_cast<BlockOperAST*>(op);

		/*if (b) {
			ops.splice(ops.end(), b->ops, b->ops.begin(), b->ops.end());
			delete b;
		} else ops.push_back(op);*/
		if (op != 0) ops.push_back(op);
	}
	
	BlockOperAST() {}
	BlockOperAST(OperAST* op) { Append(op); }
	BlockOperAST(OperAST* op1, OperAST* op2) { Append(op1); Append(op2); }
	
	virtual BaseIR *Codegen();
};

// StoreOperAST
class StoreOperAST : public OperAST {
public:
	OperAST *V;
	ExprAST *E;
	StoreOperAST(OperAST *v, ExprAST *e) : V(v), E(e) {}
	
	virtual BaseIR *Codegen();
};

//
class LifetimeVarsExprAST : public OperAST {
public:
	map<string, Var> *VD;
	OperAST *Ops;

	LifetimeVarsExprAST(map<string, Var> *vd, OperAST *ops) : VD(vd), Ops(ops) {}
	virtual BaseIR *Codegen();
};

#endif
