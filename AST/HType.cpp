#ifndef CPP_AST_HTYPE
#define CPP_AST_HTYPE 1
//
static string CurClassName = "";
//
class StructTyExprAST : public ExprAST {
public:
	map<string, struct Var> mVars;
	vector<string> Names;
	BlockOperAST *SVars;
	ExprAST *Parent;
	BaseIR	*IR;
	string TypeName;
	bool Packed;
	
	//
	StructTyExprAST(string typeName, BlockOperAST *sVars, map<string, struct Var> VD, bool packed, ExprAST *parent = 0) :
		TypeName(typeName), SVars(sVars), mVars(VD), Packed(packed), Parent(parent) {
		
		string params;
		int Size = 0;
		int Offset = 0;
		IR = 0;
		//
			
		for (int i = 0; i < sVars->ops.size(); i++) {
			VarOperAST *pVar = (VarOperAST*)(sVars->ops[i]);
			
			for (int j = 0; j < pVar->IdNames.size(); j++) {
				params += pVar->Type->GetTy().Name;
				Size   += SizeOfTy(pVar->Type->GetTy());
				
				if (j != (pVar->IdNames.size()-1)) params += ", ";
				//if (j < pVar->IdNames.size()) {
					//printf("%s (offset: %d)\n", pVar->IdNames[j].c_str(), Offset);
				//	Offset++;
				//}
			}
			
			if (i != (sVars->ops.size()-1)) params += ", ";
		}
		
		string Name = "record ("+params+")";
		if (Packed) Name = "packed "+Name;
		//
		int tSize = Size >> 3;
		
		while (tSize % 4 != 0) {
			tSize++;
		}
		
		//Size = tSize << 3;
		SetTy(CreateASTType(TSTRUCT, Name, Size, this, 0, 0, false, false, this));
	}
	
	virtual BaseIR *Codegen();
};

// Class
class ClassTyExprAST : public ExprAST {
public:
	map<string, struct Var>		 mVars;
	map<string, struct Proc>	 mMethods;
	vector<BlockOperAST*>		 SBlock;
	vector<struct Var*>		 	 MustCastVars;
	ExprAST						*Constructors;
	ExprAST						*Destructors;
	BaseIR						*ThisIR;
	vector<string>				 MethodsNames;
	ExprAST						*CTy;
	//
	ClassTyExprAST() { SetTy(CreateASTType(TCLASS, CurClassName, 64, this, 0, 0, false, false, this)); }
	
	void UpdateTy() {
		string params = "";
		int Size = 0;
		int n = 1;
		CTy = 0;
		//int Offset = 0;
		
		//
		for (int i = 0; i < SBlock.size(); i++) {
			BlockOperAST *sVars = (BlockOperAST*)SBlock[i]->ops[0];
			
			for (int j = 0; j < sVars->ops.size(); j++) {
				VarOperAST *pVar = (VarOperAST*)(sVars->ops[j]);
				
				for (int k = 0; k < pVar->IdNames.size(); k++) {
					params += pVar->Type->GetTy().Name;
					Size += SizeOfTy(pVar->Type->GetTy());
					
					if (k != (pVar->IdNames.size()-1)) params += ", ";
				}
				
				if (j != (sVars->ops.size()-1)) params += ", ";
			}
			
			if (i != (SBlock.size()-1)) params += ", ";
		}
		
		string Name = "class ("+params+")";
		//puts(Name.c_str());
		//Size += 16;
		int tSize = Size;
		Size += 32;
		//Size *= n;
		
		while (Size % 32 != 0) {
			Size += 8;
		}
		
		//puts(("Size is "+IntToStr(Size)).c_str());
		SetTy(CreateASTType(TCLASS, CurClassName, Size, this, 0, 0, false, false, this));
	}
	//
	ClassTyExprAST(vector<BlockOperAST*> sBlock, map<string, struct Var> VD, map<string, struct Proc> FD) :
		SBlock(sBlock), mVars(VD), mMethods(FD) { UpdateTy(); }
	
	virtual BaseIR *Codegen();
};

// Inherited for class
class InheritedExprAST : public ExprAST {
public:
	ClassTyExprAST *ClassTy;
	InheritedExprAST(ClassTyExprAST *classTy) : ClassTy(classTy) {}
	virtual BaseIR *Codegen();
};

//
class HTypeExprAST : public ExprAST {
public:
	string  TypeName;
	ExprAST *Type;
	BaseIR	*IR;
	
	HTypeExprAST(string typeName, ExprAST *type) : TypeName(typeName), Type(type) {
		SetTy(Type->GetTy());
		IR = 0;
	}
	
	virtual BaseIR *Codegen();
};

class HTyExprAST : public ExprAST {
public:
	BaseIR *DY;
	HTyExprAST(BaseIR *IR, ExprAST* Type) : DY(IR) { SetTy(Type->GetTy()); }
	virtual BaseIR *Codegen();
};

#endif
