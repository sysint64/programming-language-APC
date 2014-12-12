#ifndef JOBELL_AST_TYPE
#define JOBELL_AST_TYPE 1

// Main
#include <string>
#include <map>

using namespace std;

//
class PtrTyExprAST : public ExprAST {
public:
	ExprAST *Type;
	PtrTyExprAST(ExprAST *type) : Type(type) {
		struct TypeAST Ty = CreateASTType(TPTR, type->GetTy().Name+"^", PtrSize, type, 0, 0, false, false, this);
		Ty.MustCast = type->GetTy().MustCast;
		SetTy(Ty);
	}
	
	virtual BaseIR *Codegen();
};
//
class StringTyExprAST : public ExprAST {
public:
	int Size;
	
	StringTyExprAST(const int size = 0, const bool pchar = false) : Size(size) {
		/*SetTy("STRING."+IntToStr(size));
		if (pchar) SetTy("PCHAR");*/
	}
	
	virtual BaseIR *Codegen();
};
//
class ArrTyExprAST : public ExprAST {
public:
	ExprAST *Type;
	vector<int> Sizes;
	
	ArrTyExprAST(vector<int> sizes, ExprAST *type) : Sizes(sizes), Type(type) {
		struct TypeAST T = type->GetTy();
		string ty = "[";
		int Size = 0;
		
		for (int i = sizes.size()-1; i >= 0; --i) {
			Size += sizes[i];
			ty += IntToStr(Sizes[i])+" x ";
		}

		ty += type->GetTy().Name+"]";
		SetTy(CreateASTType(TARR, ty, Size*SizeOfTy(T), type, 0, 0, false, false, this));
	}

	//
	ArrTyExprAST(int size, ExprAST *type) : Type(type) {
		Sizes.push_back(size);
		struct TypeAST T = type->GetTy();
		string ty = "["+IntToStr(size)+" x "+type->GetTy().Name+"]";
		SetTy(CreateASTType(TARR, ty, size*SizeOfTy(T), type, 0, 0, false, false, this));
	}
	
	virtual BaseIR *Codegen();
};
//
class VecTyExprAST : public ExprAST {
public:
	ExprAST *Type;
	int Size;
	
	VecTyExprAST(int size, ExprAST *type) : Size(size), Type(type) {
		struct TypeAST T = type->GetTy();
		string ty = "<"+IntToStr(Size)+" x "+T.Name+">";
		SetTy(CreateASTType(TVEC, ty, Size*SizeOfTy(T), type, 0, 0, false, false, this));
	}
	
	virtual BaseIR *Codegen();
};
//
class VoidTyExprAST : public ExprAST {
public:
	VoidTyExprAST() {
		SetTy(CreateASTType(TVOID, "procedure", 8, 0, 0, 0, false, false, this));
	}
	
	virtual BaseIR *Codegen();
};



//
class ProtoTyExprAST : public ExprAST {
public:
	vector<ExprAST*> Args;
	ExprAST *RetType;
	ProtoTyExprAST(vector<ExprAST*> args, ExprAST *retty)
		: Args(args), RetType(retty)
	{
		string TypeName;
		int i;
		
		if (IsVoidTy(retty->GetTy())) TypeName = "procedure (";
		else TypeName = "function (";
		
		for (i = 0; i < Args.size()-1; i++) {
			TypeName += Args[i]->GetTy().Name+", ";
		}

		TypeName += Args[i]->GetTy().Name;
		
		if (IsVoidTy(retty->GetTy())) TypeName += ")";
		else TypeName += ") : "+RetType->GetTy().Name;
		
		//puts(TypeName.c_str());
		SetTy(CreateASTType(TFUNC, TypeName, 8, RetType, 0, this, false, false, this));
		//SetTy();
	}
	
	virtual BaseIR *Codegen();
};

//
class IntTyExprAST : public ExprAST {
public:
	int Base;
	
	IntTyExprAST(const int base, const bool Unsigned = false) : Base(base) {
		string name;

		if (Unsigned) name = "unsigned integer";
		else name = "integer";

		if (base == 1) name = "boolean";
		SetTy(CreateASTType(TINT, name, base, 0, 0, 0, Unsigned, false, this));
	}
	
	virtual BaseIR *Codegen();
};

// Float
class FloatTyExprAST : public ExprAST {
public:
	int Size;
	FloatTyExprAST(int size = 32) : Size(size) {
		switch (size) {
			case 32 : SetTy(CreateASTType(TFLOAT, "single", size, 0, 0, 0, false, false, this)); break;
			case 64 : SetTy(CreateASTType(TFLOAT, "double", size, 0, 0, 0, false, false, this)); break;
		}
	}

	virtual BaseIR *Codegen();
};

//
class ArgsTyAST : public ExprAST {
public:	
	ArgsTyAST() { SetTy(CreateASTType(TARGS, "...", 8, 0, 0, 0, false, false, this)); }
	virtual BaseIR *Codegen();
};

//
class AlignExprAST : public ExprAST {
public:
	AlignExprAST(const int align) { SetAlign(align); }
};

//
class AClassTyExprAST : public ExprAST {
public:
	string TypeName;
	AClassTyExprAST(string typeName) : TypeName(typeName) {}
	virtual BaseIR *Codegen();
};
//
class ATDExprAST : public ExprAST {
public:
	string TypeName;
	//
	ATDExprAST(string Name) : TypeName(Name) {
		SetTy(CreateASTType(TPTR, Name, PtrSize, 0, 0, 0, false, false, this));
		Ty.Flag = true;
	}
	
	virtual BaseIR *Codegen();
};

//
class FuncTyExprAST : public ExprAST {
public:
	ArgsOperAST *Args;
	ExprAST *RetType;
	bool Aligned;
	//
	FuncTyExprAST(ArgsOperAST *args, ExprAST *retType, bool aligned)
		: Args(args), RetType(retType), Aligned(aligned)
	{
		string TypeName;//
		
		if (IsVoidTy(RetType->GetTy())) TypeName = "procedure (";
		else TypeName = "function (";
		
		int i;

		if (Args != 0) {
			int tt = Args->Args.size();
			//
			for (i = 0; i < Args->Args.size(); i++) {
				if (i != (Args->Args.size()-1))
					TypeName += Args->vArgs[i]->Ty.Name+", ";
				else TypeName += Args->vArgs[i]->Ty.Name;
			}
			//
		}
		
		if (IsVoidTy(RetType->GetTy())) TypeName += ")";
		else TypeName += ") : "+RetType->GetTy().Name;
		
		SetTy(CreateASTType(TFUNC, TypeName, 8, RetType, 0, this, false, false, this));
	}

	virtual BaseIR *Codegen();
};

//
class PointerTyExprAST : public ExprAST {
public:
	bool aPtr;

	PointerTyExprAST() {
		aPtr = false;
		ExprAST *TyE = new PtrTyExprAST(new IntTyExprAST(8));
		SetTy(TyE->GetTy());
		Ty.Flag = true;
		//Ty.Ty = TVPTR; Ty.TypeExpr = this;
		//SetTy(CreateASTType(TVPTR, "pointer", 64, TyE, 0, 0, false, false, this));
	}

	PointerTyExprAST(bool aptr) : aPtr(aptr) {
		ExprAST *TyE = new PtrTyExprAST(new IntTyExprAST(8));
		SetTy(TyE->GetTy());
		Ty.Flag = true;
		//Ty.Ty = TVPTR; Ty.TypeExpr = this;
		//SetTy(CreateASTType(TVPTR, "pointer", 64, TyE, 0, 0, false, false, this));
	}
	
	virtual BaseIR *Codegen();
};

#endif
