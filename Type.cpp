#ifndef CPP_TYPE
#define CPP_TYPE 1

// Dereference type
TypeAST inline DerefTy(TypeAST Ty) {
	return Ty.E->GetTy();
}

//
TypeAST inline CreateASTType(TT Ty, string Name, int Size, ExprAST *E = 0, BaseIR *IR = 0, ExprAST *Prot = 0, bool Flag = false, bool Abstract = false, ExprAST *TypeExpr = 0) {
//TypeAST inline CreateASTType(TT Ty, string Name, int Size, ExprAST *E, BaseIR *IR, ExprAST *Prot, bool Flag, bool Abstract, ExprAST *TypeExpr) {
	TypeAST R;
	//
	R.Size = Size;  R.Name = Name;
	R.Ty   = Ty;    R.E    = E;
	R.Flag = Flag;  R.TyIR = IR;
	R.Prot = (PrototypeExprAST*)Prot;
	R.TypeExpr = TypeExpr;
	R.Abstract = Abstract;
	R.MustCast = false;
	R.Global = false;
	R.NoThis = false;
	R.HaveClass = false;
	R.id = LastTypeID;
	LastTypeID++;
	//
	return R;
}
//
TypeAST inline CreateIntTy() {
	return CreateASTType(TINT, "integer", 32);
}

TypeAST inline CreateBoolTy(const int size = 1) {
	//
	if (size == 1) return CreateASTType(TINT, "boolean", 1);
	else return CreateASTType(TVEC, "<"+IntToStr(size)+" x boolean>", size);
}
//


//
bool inline EqTy(TypeAST A, TypeAST B) {
	if (A.Size == B.Size & A.Ty == B.Ty) {
		if (A.Ty == TPTR) {
			if (EqTy(DerefTy(A), DerefTy(B)))
			return true;
		} else

		return true;
	}
		
	return false;
}

TypeAST inline DefaultTy() {
	return CreateASTType(TNONE, "", 32, 0, 0, 0, false, false, 0);
}



#endif
