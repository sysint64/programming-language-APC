#ifndef CPP_EXPR_AST
#define CPP_EXPR_AST 1

using namespace std;

int valinc = 1;
BaseIR	*ThisClassIR = 0;

// IntExprAST - Класс узла выражения для числовых литералов
class IntExprAST : public ExprAST {
public:
	int64_t Val;
	char* sVal;
	int Size;
	
	IntExprAST(int64_t val, const int size = 32, const bool Unsigned = false,
		const bool Char = false) : Val(val), Size(size)
	{
		ExprAST *Ty = new IntTyExprAST(size, Unsigned);
		SetTy(Ty->GetTy());
		SetConst(true);
		SetVal(val);
	}
	
	virtual BaseIR *Codegen();
};

// FloatExprAST - Класс узла выражения для числовых литералов с плавающей точкой
class FloatExprAST : public ExprAST {
public:
	double Val;
	int Size;
	
	FloatExprAST(double val, int size = 32,
		const bool PPC = false) : Val(val), Size(size)
	{
		string name;
		//
		if (size == 0 ) size = 32;
		if (size == 16) name = "half";
		if (size == 32) name = "single";
		if (size == 64) name = "double";
		//
		ExprAST *Ty = new FloatTyExprAST(size);
		SetTy(Ty->GetTy());
		SetConst(true);
		SetVal(val);
	}
	
	virtual BaseIR *Codegen();
};

// UnaryExprAST - Класс узла выражения для унарных операторов
class UnaryExprAST : public ExprAST {
public:
	char Op;
	ExprAST *Val;
	
	UnaryExprAST(char op, ExprAST *val, TypeAST) : Op(op), Val(val) {
		//
		struct TypeAST ty = Val->GetTy();
		
		if (op == '@') { 
			ExprAST *TyE = new PtrTyExprAST(Val->GetTy().TypeExpr);
			ty = TyE->GetTy();
		}
		
		SetTy(ty);
	}
		
	virtual BaseIR *Codegen();
};

// Null Pointer Value
class NullExprAST : public ExprAST {
public:
	ExprAST *Ty;
	NullExprAST(ExprAST *ty) : Ty(ty) { SetTy(Ty->GetTy()); }
	virtual BaseIR *Codegen();
};

// NumBinaryExprAST - Класс узла выражения для числовых бираных операторов
class NumBinaryExprAST : public ExprAST {
public:
	char Op;
	ExprAST *LHS, *RHS;
	
	NumBinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs)
		: Op(op), LHS(lhs), RHS(rhs)
	{
		struct TypeAST lty = lhs->GetTy();
		struct TypeAST rty = rhs->GetTy();
		
		if (IsRelop(op)) {
			if (IsVecTy(lty)) {
				SetTy(CreateBoolTy(CountOfTy(lty)));
			} else SetTy(CreateBoolTy());
		} else {
			//SetTy(NullTy);
			if (IsIntTy(lty) && IsFloatTy(rty)) SetTy(rty);
			else SetTy(lty);
		}
		
		//SetTy(rhs->GetTy());
	}
		
	virtual BaseIR *Codegen();
};

// Vector Vals ex: <0, 2, a, b, ...>
class VecValAST : public ExprAST {
public:
	vector<ExprAST*> Vals;
	int Size;
	bool Store, Left;
	ExprAST *Id;
	
	VecValAST(vector<ExprAST*> vals, const int size, bool store, const bool left, ExprAST *id)
	  : Vals(vals), Size(size), Store(store), Left(left), Id(id) {
		//struct TypeAST T = vals[0]->GetTy();
		//string ty = "<"+IntToStr(Size)+" x "+T.Name+">";
		//SetTy(CreateASTType(TVEC, ty, Size*SizeOfTy(T), vals[0]));
		//SetConst(true);
		ExprAST *Ty = new VecTyExprAST(Size, vals[0]->GetTy().TypeExpr);
		SetTy(Ty->GetTy());
	}
	
	virtual BaseIR *Codegen();
};

// Array Vals ex: [..., [a, b], [b, c]]
class ArrValAST : public ExprAST {
public:
	vector<ExprAST*> Vals;
	int Size;
	vector<int> *Unni;
	bool Store, Left;
	ExprAST *Id;
	//
	ArrValAST(vector<ExprAST*> vals, const int size, vector<int> *ui, bool store, const bool left, ExprAST *id)
		: Vals(vals), Size(size), Unni(ui), Store(store), Left(left), Id(id)
	{
		ExprAST *Ty = new ArrTyExprAST(Size, vals[0]->GetTy().TypeExpr);
		SetTy(Ty->GetTy());
		//SetConst(true);
	}
	//
	virtual BaseIR *Codegen();
};

// PtrBinaryExprAST - Класс узла выражения для числовых бираных операторов
class PtrBinaryExprAST : public ExprAST {
public:
	char Op;
	ExprAST *LHS, *RHS;
	
	PtrBinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs)
		: Op(op), LHS(lhs), RHS(rhs)
	{
		if (IsRelop(Op)) {
			SetTy(CreateBoolTy());
		} else {
			struct TypeAST lty = lhs->GetTy();
			struct TypeAST rty = rhs->GetTy();
			
			if (IsPtrTy(lty)) SetTy(lty);
			else SetTy(lty);
		}
	}
		
	virtual BaseIR *Codegen();
};

// NewExprAST - класс для выделения памяти
class NewExprAST : public ExprAST {
public:
	vector<ExprAST*> Sizes;
	int TypeSize;
	ExprAST *AssignE;
	//
	NewExprAST(ExprAST *type, vector<ExprAST*> sizes, ExprAST *assignE) : Sizes(sizes), AssignE(assignE) {
		TypeSize = SizeOfTy(type->GetTy());
		SetTy(assignE->GetTy());
	}

	virtual BaseIR *Codegen();
};

// DeleteExprAST - free dynamic memory
class DeleteExprAST : public ExprAST {
public:
	//
	vector<ExprAST*> Ids;
	vector<ExprAST*> Calls;
	//
	DeleteExprAST(vector<ExprAST*> ids, vector<ExprAST*> calls) : Ids(ids), Calls(calls) {}
	virtual BaseIR *Codegen();
};

// VarExprAST - Класс узла выражения для переменных
class VarExprAST : public ExprAST {
public:
	string  Name;
	map<string, Var> *VD;
	//
	VarExprAST(const string name, struct TypeAST Ty, map<string, Var> *vd) : Name(name) {
		SetTy(Ty);
		VD = vd;
	}

	virtual BaseIR *Codegen();
};

// IdxExprAST
class IdxExprAST : public ExprAST {
public:
	ExprAST *E;
	vector<ExprAST*> Idx;
	
	IdxExprAST(ExprAST *e, vector<ExprAST*> idx) : E(e), Idx(idx) {
		struct TypeAST Ty = e->GetTy();
		
		for (int i = 0; i < idx.size(); i++)
			Ty = DerefTy(Ty);

		SetTy(Ty);
	}
	
	virtual BaseIR *Codegen();
};

// PtrExprAST
class PtrExprAST : public ExprAST {
public:
	ExprAST *E;
	int N;
	bool ccp;
	
	PtrExprAST(ExprAST *e, int n, bool ccp = false) : E(e), N(n), ccp(ccp) {
		struct TypeAST Ty = e->GetTy();
		
		for (int i = 0; i < n; i++)
			Ty = DerefTy(Ty);
			
		SetTy(Ty);
	}
	
	virtual BaseIR *Codegen();
};

// Structure param
class ParamExprAST : public ExprAST {
public:
	ExprAST *E, *Ty;
	int	 Offset;
	bool	 IsClass;
	BaseIR	*ClassTyIR;
	//
	ParamExprAST(ExprAST *e, int offset, ExprAST *ty, bool isClass, BaseIR *classTyIR)
		: E(e), Offset(offset), Ty(ty), IsClass(isClass), ClassTyIR(classTyIR) {
		SetTy(ty->GetTy());
	}
	
	virtual BaseIR *Codegen();
};

// AssignExprAST
class AssignExprAST : public ExprAST {
public:
	ExprAST *Id;
	ExprAST *Expr;
	
	AssignExprAST(ExprAST *id, ExprAST *expr) : Expr(expr), Id(id) {
		SetTy(id->GetTy());
	}
	
	virtual BaseIR *Codegen();
};

// AssignObjExprAST
class AssignObjExprAST : public ExprAST {
public:
	ExprAST *Id;
	ExprAST *Expr;
	
	AssignObjExprAST(ExprAST *id, ExprAST *expr) : Expr(expr), Id(id) {
		SetTy(id->GetTy());
	}
	
	virtual BaseIR *Codegen();
};

// Load
class LoadExprAST : public ExprAST {
public:
	ExprAST *E;
	BaseIR *IR;
	LoadExprAST(ExprAST *e) : E(e)   {IR = 0;}
	LoadExprAST(BaseIR *ir) : IR(ir) {E = 0;}
	virtual BaseIR *Codegen();
};

//
class GetRetExprAST : public ExprAST {
public:
	ExprAST *Expr;
	GetRetExprAST(ExprAST *E) : Expr(E) {
		SetTy(DerefTy(E->GetTy()));
	}

	virtual BaseIR *Codegen();
};

// CallExprAST - Класс узла выражения для вызова функций
class CallExprAST : public ExprAST {
public:
	ExprAST *Expr, *ProtExpr;
	BaseIR  *IR, *Prot;
	vector<ExprAST*> Args;
	string Name;
	bool Destructor;
	bool Constructor;
	BaseIR *TyIR;
	ExprAST *TyExpr;
	ExprAST *AssignE;
	bool Virtual;
	int Offset;
	ExprAST *ClassTy;
	//
	CallExprAST(string name, ExprAST *E, BaseIR *prot, vector<ExprAST*> args, struct TypeAST Ty,
		    bool c = false, bool d = false, BaseIR *ti = 0, ExprAST *te = 0, ExprAST *ae = 0,
		    bool aVirtual = false, int offset = 0 , ExprAST *classTy = 0, ExprAST *protExpr = 0, bool memCpy = false)
			: Name(name), Expr(E), Prot(prot), Args(args), AssignE(ae), Offset(offset),
			  Constructor(c), Destructor(d), TyIR(ti), TyExpr(te), Virtual(aVirtual),
			  ClassTy(classTy), ProtExpr(protExpr)
			  { SetTy(Ty); IR = 0; MemCpy = memCpy; }
	//
	CallExprAST(string name, BaseIR *B, BaseIR *prot, vector<ExprAST*> args, struct TypeAST Ty, 
		    bool c = false, bool d = false, BaseIR *ti = 0, ExprAST *te = 0, ExprAST *ae = 0,
		    bool aVirtual = false, int offset = 0, ExprAST *classTy = 0, ExprAST *protExpr = 0, bool memCpy = false)
			: Name(name), IR(B), Prot(prot), Args(args), AssignE(ae), Offset(offset),
			  Constructor(c), Destructor(d), TyIR(ti), TyExpr(te), Virtual(aVirtual),
			  ClassTy(classTy), ProtExpr(protExpr)
			  { SetTy(Ty); Expr = 0; MemCpy = memCpy; }
	//
	virtual BaseIR *Codegen();
};

// CallOvrExprAST - Класс узла выражения для вызова функций для перегруженных операторов
class CallOvrExprAST : public ExprAST {
public:
	int OverloadId;
	ExprAST *LHS, *RHS, *RES;
	
	CallOvrExprAST(int ovrid, ExprAST *lhs, ExprAST *rhs, ExprAST *res, struct TypeAST Ty)
		: OverloadId(ovrid), LHS(lhs), RHS(rhs), RES(res) { SetTy(Ty); }
		
	virtual BaseIR *Codegen();
};

// FuncExprAST
class FuncExprAST : public ExprAST {
public:
	string	IdName;
	int 	Idx;
	//
	FuncExprAST(string idName, struct TypeAST Ty, int ai) : IdName(idName), Idx(ai)
	{
		SetTy(Ty);
	}
	
	virtual BaseIR *Codegen();
};

//
class FuncObjExprAST : public ExprAST {
public:
	map<string, struct Proc> *pMethods;
	ExprAST	*Obj;
	string	 IdName;
	int 	 Idx;
	//
	FuncObjExprAST(string idName, map<string, struct Proc> *methods, struct TypeAST Ty, ExprAST *obj, int ai)
		: IdName(idName), Obj(obj), pMethods(methods), Idx(ai)
	{
			SetTy(Ty);
	}
	
	virtual BaseIR *Codegen();
};

//
class SelfExprAST : public ExprAST {
public:
	SelfExprAST(TypeAST Ty) { SetConst(true); SetTy(Ty); }
	virtual BaseIR *Codegen();
};

#endif
