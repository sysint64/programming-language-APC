#ifndef CPP_BUILDER_IR
#define CPP_BUILDER_IR 1

//#include "../Type.h"

using namespace std;

// +
BaseIR *CreateAdd(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = 0;
	
Begin:
	if (IsIntTy(Ty))   V = new AddIR(L, R);
	if (IsFloatTy(Ty)) V = new FAddIR(L, R);
	if (IsVecTy(Ty)) {
		Ty = DerefTy(Ty);
		goto Begin;
	}

	ManagerIR->Insert(V, Body);
	return V;
}

// -
BaseIR *CreateSub(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = 0;
	
Begin:
	if (IsIntTy(Ty))   V = new SubIR(L, R);
	if (IsFloatTy(Ty)) V = new FSubIR(L, R);
	if (IsVecTy(Ty)) {
		Ty = DerefTy(Ty);
		goto Begin;
	}
	
	ManagerIR->Insert(V, Body);
	return V;
}

// *
BaseIR *CreateMul(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = 0;
	
Begin:
	if (IsIntTy(Ty))   V = new MulIR(L, R);
	if (IsFloatTy(Ty)) V = new FMulIR(L, R);
	if (IsVecTy(Ty)) {
		Ty = DerefTy(Ty);
		goto Begin;
	}
	
	ManagerIR->Insert(V, Body);
	return V;
}

// Divide ('/')
BaseIR *CreateDiv(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = 0;

Begin:
	if (IsIntTy(Ty))   V = new DivIR(L, R);
	if (IsFloatTy(Ty)) V = new FDivIR(L, R);
	if (IsVecTy(Ty)) {
		Ty = DerefTy(Ty);
		goto Begin;
	}

	ManagerIR->Insert(V, Body);
	return V;
}

// Mod ('%')
BaseIR *CreateRem(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = 0;

Begin:
	if (IsIntTy(Ty))   V = new RemIR(L, R);
	if (IsFloatTy(Ty)) V = new FRemIR(L, R);
	if (IsVecTy(Ty)) {
		Ty = DerefTy(Ty);
		goto Begin;
	}

	ManagerIR->Insert(V, Body);
	return V;
}

// Shl ('<<')
BaseIR *CreateShl(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = new ShlIR(L, R);
	ManagerIR->Insert(V, Body);
	return V;
}

// Shr ('>>')
BaseIR *CreateShr(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = new ShrIR(L, R);
	ManagerIR->Insert(V, Body);
	return V;
}

// Xor
BaseIR *CreateXor(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = new XorIR(L, R);
	ManagerIR->Insert(V, Body);
	return V;
}

// Or
BaseIR *CreateOr(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = new OrIR(L, R);
	ManagerIR->Insert(V, Body);
	return V;
}

// And
BaseIR *CreateAnd(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = new AndIR(L, R);
	ManagerIR->Insert(V, Body);
	return V;
}

// Not
BaseIR *CreateNot(BaseIR *L, int Size, struct TypeAST Ty) {
	//
	BaseIR *Const = 0;
	//
	if (IsVecTy(Ty)) {
		vector<BaseIR*> consts;

		for (int i = 0; i < CountOfTy(Ty); i++)
			consts.push_back(new IntValIR(-1, SizeOfTy(DerefTy(Ty))));
		//
		Const = new VecValIR(consts);
		//
	} else Const = new IntValIR(-1, Size);
	//

	BaseIR *V = new XorIR(L, Const);
	ManagerIR->Insert(V, Body);
	return V;
}

// LogicNot
BaseIR *CreateLNot(BaseIR *L, int Size, struct TypeAST Ty) {
	//
	BaseIR *Const = 0;
	//
	if (IsVecTy(Ty)) {
		vector<BaseIR*> consts;

		for (int i = 0; i < CountOfTy(Ty); i++)
			consts.push_back(new IntValIR(1, SizeOfTy(DerefTy(Ty))));
		//
		Const = new VecValIR(consts);
		//
	} else Const = new IntValIR(1, Size);
	//

	BaseIR *V = new XorIR(L, Const);
	ManagerIR->Insert(V, Body);
	return V;
}

// Cmp
// =
BaseIR *CreateCmpEQ(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = 0;
	int size = 1;

	if (IsVecTy(Ty)) {
		size = CountOfTy(Ty);
		Ty = DerefTy(Ty);
	}

	if (IsIntTy(Ty))   V = new ICmpIR(L, R, new EQIR() , size);
	if (IsFloatTy(Ty)) V = new FCmpIR(L, R, new OEQIR(), size);
	if (IsPtrTy(Ty))   V = new ICmpIR(L, R, new EQIR() , size);
	if (IsClassTy(Ty)) V = new ICmpIR(L, R, new EQIR() , size);
	
	ManagerIR->Insert(V, Body);
	return V;
}

// <
BaseIR *CreateCmpLT(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = 0;
	int size = 1;

	if (IsVecTy(Ty)) {
		size = CountOfTy(Ty);
		Ty = DerefTy(Ty);
	}
	
	if (IsIntTy(Ty)) {
		if (Ty.Flag) V = new ICmpIR(L, R, new ULTIR(), size);
		else V = new ICmpIR(L, R, new SLTIR(), size);
	}

	if (IsFloatTy(Ty)) V = new FCmpIR(L, R, new OLTIR(), size);
	
	ManagerIR->Insert(V, Body);
	return V;
}

// >
BaseIR *CreateCmpGT(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = 0;
	int size = 1;

	if (IsVecTy(Ty)) {
		size = CountOfTy(Ty);
		Ty = DerefTy(Ty);
	}

	if (IsIntTy(Ty)) {
		if (Ty.Flag) V = new ICmpIR(L, R, new UGTIR(), size);
		else V = new ICmpIR(L, R, new SGTIR(), size);
	}
	
	if (IsFloatTy(Ty)) V = new FCmpIR(L, R, new OGTIR(), size);
	
	ManagerIR->Insert(V, Body);
	return V;
}

// <=
BaseIR *CreateCmpLE(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = 0;
	int size = 1;

	if (IsVecTy(Ty)) {
		size = CountOfTy(Ty);
		Ty = DerefTy(Ty);
	}

	if (IsIntTy(Ty)) {
		if (Ty.Flag) V = new ICmpIR(L, R, new ULEIR(), size);
		else V = new ICmpIR(L, R, new SLEIR(), size);
	}

	if (IsFloatTy(Ty)) V = new FCmpIR(L, R, new OLEIR(), size);
	
	ManagerIR->Insert(V, Body);
	return V;
}

// >=
BaseIR *CreateCmpGE(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = 0;
	int size = 1;

	if (IsVecTy(Ty)) {
		size = CountOfTy(Ty);
		Ty = DerefTy(Ty);
	}

	if (IsIntTy(Ty)) {
		if (Ty.Flag) V = new ICmpIR(L, R, new UGEIR(), size);
		else V = new ICmpIR(L, R, new SGEIR(), size);
	}

	if (IsFloatTy(Ty)) V = new FCmpIR(L, R, new OGEIR(), size);
	
	ManagerIR->Insert(V, Body);
	return V;
}

// <>
BaseIR *CreateCmpNE(BaseIR *L, BaseIR *R, struct TypeAST Ty) {
	BaseIR *V = 0;
	int size = 1;

	if (IsVecTy(Ty)) {
		size = CountOfTy(Ty);
		Ty = DerefTy(Ty);
	}

	if (IsIntTy(Ty))   V = new ICmpIR(L, R, new NEIR() , size);
	if (IsFloatTy(Ty)) V = new FCmpIR(L, R, new ONEIR(), size);
	if (IsPtrTy(Ty))   V = new ICmpIR(L, R, new NEIR() , size);
	if (IsClassTy(Ty)) V = new ICmpIR(L, R, new NEIR() , size);
	
	ManagerIR->Insert(V, Body);
	return V;
}

#endif
