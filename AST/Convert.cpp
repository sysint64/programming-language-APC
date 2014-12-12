#ifndef CPP_CONVERT_AST
#define CPP_CONVERT_AST 1

// Main
#include <string>
#include <map>

using namespace std;

// Преобразование типов

// Int to FLOAT
class I2DExprAST : public ExprAST {
public:
	ExprAST *Expr;
	I2DExprAST(ExprAST *Expr, struct TypeAST ty) : Expr(Expr) { SetTy(ty); }
	virtual BaseIR *Codegen();
};

// FLOAT to Int
class D2IExprAST : public ExprAST {
public:
	ExprAST *Expr;
	D2IExprAST(ExprAST *Expr, struct TypeAST ty) : Expr(Expr) { SetTy(ty); }
	virtual BaseIR *Codegen();
};

class IntConvExprAST : public ExprAST {
public:
	ExprAST *Expr;
	IntConvExprAST(ExprAST *Expr, struct TypeAST ty) : Expr(Expr) { SetTy(ty); }
	
	virtual BaseIR *Codegen();
};

class FloatConvExprAST : public ExprAST {
public:
	ExprAST *Expr;
	FloatConvExprAST(ExprAST *Expr, struct TypeAST ty) : Expr(Expr) { SetTy(ty); }
	
	virtual BaseIR *Codegen();
};

class Int2PtrExprAST : public ExprAST {
public:
	ExprAST *Expr, *Ty;
	Int2PtrExprAST(ExprAST *Expr, ExprAST *Ty) : Expr(Expr), Ty(Ty) { SetTy(Ty->GetTy()); }
	
	virtual BaseIR *Codegen();
};

class Ptr2IntExprAST : public ExprAST {
public:
	ExprAST *Expr, *Ty;
	Ptr2IntExprAST(ExprAST *Expr, ExprAST *Ty) : Expr(Expr), Ty(Ty) { SetTy(Ty->GetTy()); }
	
	virtual BaseIR *Codegen();
};

// Vector types
// Int to Float
class VecI2DExprAST : public ExprAST {
public:
	ExprAST *Expr;
	int	 Size;

	VecI2DExprAST(ExprAST *Expr, struct TypeAST ty, int size) : Expr(Expr), Size(size) { SetTy(ty); }
	virtual BaseIR *Codegen();
};

// Float To int
class VecD2IExprAST : public ExprAST {
public:
	ExprAST *Expr;
	int	 Size;

	VecD2IExprAST(ExprAST *Expr, struct TypeAST ty, int size) : Expr(Expr), Size(size) { SetTy(ty); }
	virtual BaseIR *Codegen();
};

// Bitcast
class BitcastExprAST : public ExprAST {
public:
	ExprAST *From, *To;
	BaseIR  *TypeIR;
	bool NoLoad;
	bool NoDeref;

	BitcastExprAST(ExprAST *from, ExprAST *to, struct TypeAST ty, bool noderef = false, bool noload = false)
		: From(from), To(to), NoDeref(noderef), NoLoad(noload) { SetTy(ty); TypeIR = 0; }

	BitcastExprAST(ExprAST *from, BaseIR *to, struct TypeAST ty, bool noderef = false, bool noload = false)
		: From(from), TypeIR(to), NoDeref(noderef), NoLoad(noload) { SetTy(ty); }
		
	virtual BaseIR *Codegen();
};

// Stmt IS
class IsCastExprAST : public ExprAST {
public:
	ExprAST *E, *Ty;
	
	IsCastExprAST(ExprAST *e, ExprAST *ty) : E(e), Ty(ty) {
		ExprAST *TyE = new IntTyExprAST(1);
		SetTy(TyE->GetTy());
	};
	
	virtual BaseIR *Codegen();
};

class NotNullExprAST : public ExprAST {
public:
	ExprAST *Val;

	NotNullExprAST(ExprAST *val) : Val(val) {
		ExprAST *TyE = new IntTyExprAST(1);
		SetTy(TyE->GetTy());
	}

	virtual BaseIR *Codegen();
};

#endif
