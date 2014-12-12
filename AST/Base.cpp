#ifndef CPP_BASE_AST
#define CPP_BASE_AST 1

// Main
#include <string>
#include <map>
#include "math.h"
#include "../line.h"

//

bool IsRelop(char op) {
	if (op == '>' | op == '<' | op == -88 |
	    op == -30 | op == -31 | op == -32)  // TODO
		return true;
	
	return false;
}

// ExprAST - Базовый класс для всех узлов выражений
class ExprAST {
private:
	long double Val;
	bool   Const;
	bool   vConst;
	bool   vVar;
	int    ln, chn;
	string Emit;
	int Align;
	
public:
	TypeAST Ty;
	bool	IsAssign;
	bool	MemCpy;
	ExprAST() {
		Const = false;
		IsAssign = false;
		MemCpy = false;
		ln = lineno;
		chn = posno;
		curline = ln;
		curposno = chn;
		//Ty = new Type();
		Emit = "";
		SetAlign(0);
		vConst = false;
		vVar = false;
	}
	
	void SetCurLine() {
		curline  = ln;
		curposno = chn;
	}
	
	virtual ~ExprAST() {}
	virtual BaseIR *Codegen() { SetCurLine(); }
	virtual TypeAST GetTy() { return Ty; }
	virtual void SetTy(TypeAST ty) { Ty = ty; }
	virtual void SetEmit(const string emit) { Emit = emit; }
	virtual string GetEmit() { return Emit; }
	
	double  GetVal() { return Val; }
	void    SetVal(double val) { Val = val; }
	int	GetLine()  { return ln; }
	int	GetCharN() { return chn; }
	void	SetAlign(int align) { Align = align; }
	int		GetAlign() { return Align; }
	
	void    SetConst(bool c)  { Const  = c; };
	void    SetvConst(bool c) { vConst = c; };
	void    SetvVar(bool c)   { vVar   = c; };
	//
	bool    IsConst()  { return Const;  };
	bool    IsvConst() { return vConst; };
	bool    IsvVar()   { return vVar;   };
};

// OperAST - Базовый класс для всех узлов операторов
class OperAST {
private:
	int    ln, chn;
	
public:
	TypeAST Ty;
	OperAST() {
		curline = lineno;
		curposno = posno;
	}
	
	void SetCurLine() {
		curline  = ln;
		curposno = chn;
	}
	
	int	 	GetLine()  { return ln; }
	int	 	GetCharN() { return chn; }
	
	virtual ~OperAST() {}
	virtual BaseIR *Codegen() {SetCurLine();};
};

// ArgOperAST - Аргумент функции
class ArgExprAST : public ExprAST {
public:
	int Ty;
	ExprAST *Type;
	string Name;
	
	ArgExprAST(ExprAST *type, const int ty, const string name)
		: Type(type), Ty(ty), Name(name) {}
	
	virtual BaseIR *Codegen();
};

class ArgsOperAST : public OperAST {
public:
	vector<struct Var> Args;
	vector<OperAST*> vArgs;
	vector<BaseIR*> vArgsIR;
	ExprAST *Val;
	ExprAST *SRet;
	//
	ArgsOperAST(vector<struct Var> args, vector<OperAST*> vargs, ExprAST *val)
		: Args(args), vArgs(vargs), Val(val) { SRet = 0; }
	ArgsOperAST() { SRet = 0; }
	
	virtual BaseIR *Codegen();
	BaseIR *CodegenDef();
};

#endif
