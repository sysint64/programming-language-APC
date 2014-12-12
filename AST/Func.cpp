#ifndef CPP_AST_FUNC
#define CPP_AST_FUNC 1

//
class BlockDictsOperAST : public OperAST {
public:
	vector<OperAST*> Vars;
	vector<OperAST*> Consts;
	map<string, struct Var> VD;
	OperAST *ResultVar;
	
	//
	BlockDictsOperAST(vector<OperAST*> vars, vector<OperAST*> consts, OperAST *resultVar)
		: Vars(vars), Consts(consts), ResultVar(resultVar) {}
	
	virtual BaseIR *Codegen();
};

// Directives
class DirectiveOperAST : public OperAST {
public:
	string Name;
	bool External;
	string Lib, eName;
	//
	DirectiveOperAST(string name, bool external = false, string lib = "", string ename = "")
		: Name(name), External(external), Lib(lib), eName(ename) {}
	
	virtual BaseIR *Codegen();
};

class DirectivesOperAST : public OperAST {
public:
	BlockOperAST *CC, *Attr, *Linkage;
	BaseIR *AttrIR, *CCIR, *LinkIR;
	bool Overload, External, Virtual, Override;
	string Lib, eName;
	bool Swap;
	//
	DirectivesOperAST(BlockOperAST *cc, BlockOperAST *attr, BlockOperAST *link,
			  bool doverload = false, bool dvirtual = false, bool doverride = false,
			  bool external  = false, string lib = "", string ename = "", bool swap = false)
		: CC(cc), Attr(attr), Linkage(link), Overload(doverload),
		  Virtual(dvirtual), Override(doverride), Swap(swap),
		  External(external), Lib(lib), eName(ename) {}
	
	virtual BaseIR *Codegen();
};

// Prototype
class PrototypeExprAST : public ExprAST {
public:
	string Name;
	string NameReg;
	string LLVMName;
	DirectivesOperAST *Directives;
	ArgsOperAST *Args;
	ExprAST *RetType;
	ExprAST *SRet;
	BaseIR  *IRProto;
	ExprAST *FuncClass;
	bool Aligned;
	bool IsType;
	bool OfClass;
	bool HaveClass;
	//
	PrototypeExprAST(string protName, ArgsOperAST *args, DirectivesOperAST *directives, ExprAST *retType, bool aligned, bool istype)
		: Name(protName), Args(args), Directives(directives), RetType(retType), Aligned(aligned), IsType(istype)
	{
		string TypeName;//
		OfClass = false;
		HaveClass = false;
		SRet = RetType;
		FuncClass = new NullExprAST(new PointerTyExprAST(true));
		
		if (IsVoidTy(RetType->GetTy())) TypeName = "procedure (";
		else TypeName = "function (";
		
		int i;
		//puts("!!");
		if (Args != 0) {
			int tt = Args->Args.size();
			//
			for (i = 0; i < Args->Args.size(); i++) {
				if (i != (Args->Args.size()-1))
					TypeName += Args->vArgs[i]->Ty.Name+", ";
				else TypeName += Args->vArgs[i]->Ty.Name;
			}
		
			/*TypeName += Args->vArgs[i]->Ty.Name;*/
		}
		
		if (IsVoidTy(RetType->GetTy())) TypeName += ")";
		else TypeName += ") : "+RetType->GetTy().Name;
		
		SetTy(CreateASTType(TFUNC, TypeName, 8, RetType, 0, this, false, false, this));
	}

	//
	PrototypeExprAST (ArgsOperAST *args, ExprAST *retType, bool aligned, bool ofClass) {
		PrototypeExprAST("", args, 0, retType, aligned, true);
		OfClass = ofClass;
		if (OfClass) Ty.HaveClass = true;
	}
	
	BaseIR *GenCurFunc();
	virtual BaseIR *Codegen();
};

// Function
class FuncOperAST : public OperAST {
public:
	BlockDictsOperAST *Dicts;
	PrototypeExprAST *Prot;
	OperAST* Ops;
	bool InClass;
	//
	FuncOperAST(PrototypeExprAST *prot, BlockDictsOperAST *dicts, OperAST *body, bool inClass)
	: Prot(prot), Dicts(dicts), Ops(body), InClass(inClass) {}
	
	virtual BaseIR *Codegen();
};

#endif
