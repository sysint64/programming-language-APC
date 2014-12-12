#ifndef JOBELL_CODEGEN_TYPE
#define JOBELL_CODEGEN_TYPE 1

// Main
#include <string>
#include <map>

using namespace std;

BaseIR *PtrTyExprAST::Codegen() {
	BaseIR *Ty = new PtrTyIR(Type->Codegen());
	if (GetAlign() != 0) Ty->SetAlign(IntToStr(GetAlign()));

	TypeAST mTy = GetTy();
	mTy.TyIR = Ty;
	SetTy(mTy);

	return Ty;
}

BaseIR *ArrTyExprAST::Codegen() {
	BaseIR *ArrTy = 0;
	BaseIR *Ty0 = Type->Codegen();
	
	if (IsClassTy(Type->GetTy())) Ty0 = new PtrTyIR(Ty0);
	
	for (int i = Sizes.size()-1; i >= 0; --i) {
		if (ArrTy != 0) {
			ArrTy = new ArrTyIR(Sizes[i], ArrTy);
		} else {
			ArrTy = new ArrTyIR(Sizes[i], Ty0);
		}
	}
	
	if (GetAlign() != 0) ArrTy->SetAlign(IntToStr(GetAlign()));	
	
	TypeAST mTy = GetTy();
	mTy.TyIR = ArrTy;
	SetTy(mTy);

	//SetTy(CreateASTType(mTy.Ty, mTy.Name, SizeOfTy(mTy), mTy.E, ArrTy));
	
	return ArrTy;
}

BaseIR *StringTyExprAST::Codegen() {
	if (Size == 0) return new PtrTyIR(new IntTyIR(8));
	return new ArrTyIR(Size, new IntTyIR(8));
}

BaseIR *VecTyExprAST::Codegen() {
	BaseIR *VecTy = new VecTyIR(Size, Type->Codegen());
	
	if (GetAlign() != 0) VecTy->SetAlign(IntToStr(GetAlign()));	
	return VecTy;
}

//

BaseIR *ArgsTyAST::Codegen() {
	return new ArgsTyIR();
}

//

BaseIR *VoidTyExprAST::Codegen() {
	return new VoidTyIR();
}

BaseIR *PointerTyExprAST::Codegen() {
	BaseIR *VoidTy = 0;

	if (aPtr) VoidTy = new aPtrTyIR(new IntTyIR(8));
	else VoidTy = new PtrTyIR(new IntTyIR(8));
	
	if (GetAlign() != 0) VoidTy->SetAlign(IntToStr(GetAlign()));
	return VoidTy;
}

//
BaseIR *ProtoTyExprAST::Codegen() {
	NoInsIR = true;
	BaseIR *RetTyIR = RetType->Codegen();
	vector<BaseIR*> vArgsIR;
	
	//
	for (int i = 0; i < Args.size(); i++) {
		vArgsIR.push_back(Args[i]->Codegen());
	}
	
	NoInsIR = false;
	BaseIR *Prot = new ProtoTyIR(RetTyIR->GetTy(), vArgsIR);
	//puts(Prot->GetEmit().c_str());
	
	return Prot;
}

BaseIR *IntTyExprAST::Codegen() {
	BaseIR *TyIR = new IntTyIR(Base);
	Ty.TyIR = TyIR;
	
	return TyIR;
}

// Floating types

BaseIR *FloatTyExprAST::Codegen() {
	return new FloatTyIR(Size);
}

BaseIR *AClassTyExprAST::Codegen() {
	BaseIR *vTy = new BaseIR();
	vTy->SetEmit("%"+TypeName);
	vTy->SetAlign("8");
	vTy->SetTy(vTy);
	
	return vTy;
}

BaseIR *ATDExprAST::Codegen() {
	BaseIR *TyIR = new BaseIR();
	TyIR->SetEmit(TypeName);
	TyIR->SetAddr(TyIR->GetEmit());
	TyIR->SetTy(TyIR);
	
	return TyIR;
}

BaseIR *FuncTyExprAST::Codegen() {
	bool a = LIRInS;
	
	LIRInS = true;
	Args->vArgsIR.clear();
	BaseIR *ArgsIR = Args->Codegen();
	LIRInS = a;
	//
	BaseIR *RetTyIR = RetType->Codegen();
	BaseIR *Prot = new ProtoTyIR(RetTyIR, Args->vArgsIR);

	return Prot;
}

#endif
