#ifndef CPP_CODEGEN_CONVERT
#define CPP_CODEGEN_CONVERT 1

// Main
#include <string>
#include <map>

using namespace std;

//* Преобразование типов

// Int to Double
BaseIR *I2DExprAST::Codegen() {
	BaseIR *VE = Expr->Codegen();
	BaseIR *V = new SIToFP(VE, new FloatTyIR(SizeOfTy(GetTy())));
	ManagerIR->Insert(V, Body);

	return V;
}

// Double to Int
BaseIR *D2IExprAST::Codegen() {	
	BaseIR *VE = Expr->Codegen();
	BaseIR *V = new FPToSI(VE, new IntTyIR(SizeOfTy(GetTy())));
	ManagerIR->Insert(V, Body);

	return V;
}

// Vector Double to Vector Int
BaseIR *VecD2IExprAST::Codegen() {	
	BaseIR *VE = Expr->Codegen();
	BaseIR *V = new FPToSI(VE, new VecTyIR(Size, new IntTyIR(SizeOfTy(DerefTy(GetTy())))));
	ManagerIR->Insert(V, Body);

	return V;
}

// Vector Int to Vector Double
BaseIR *VecI2DExprAST::Codegen() {	
	BaseIR *VE = Expr->Codegen();
	BaseIR *V = new SIToFP(VE, new VecTyIR(Size, new FloatTyIR(SizeOfTy(DerefTy(GetTy())))));
	ManagerIR->Insert(V, Body);

	return V;
}


// Int To Int
BaseIR *IntConvExprAST::Codegen() {
	BaseIR *VE = Expr->Codegen();
	BaseIR *V;
	//
	int Size = SizeOfTy(GetTy());
	int b2 = SizeOfTy(Expr->GetTy());
	//
	if (Size == 1)  V = new ICmpIR (VE, new IntValIR(0, b2), new NEIR());
	else if (b2 == 1) V = new ZExtIR (VE, new IntTyIR(Size));
	else if (Size > b2) V = new SExtIR (VE, new IntTyIR(Size));
	else V = new TruncIR (VE, new IntTyIR(Size));
	//
	ManagerIR->Insert(V, Body);
	return V;
}

// Float To Float
BaseIR *FloatConvExprAST::Codegen() {
	BaseIR *VE = Expr->Codegen();
	BaseIR *V;
	
	int Size1 = SizeOfTy(GetTy());
	int Size2 = SizeOfTy(Expr->GetTy());
	
	if (Size1 > Size2) V = new FPExtIR(VE, new FloatTyIR(Size1));
	else V = new FPTruncIR(VE, new FloatTyIR(Size1));
	
	ManagerIR->Insert(V, Body);
	return V;
}

// Bitcast
BaseIR *BitcastExprAST::Codegen() {
	//
	if (NoLoad) GetAddr = true; // ?????????????????????????????
	BaseIR *FromIR = From->Codegen(); 
	BaseIR *ToIR   = 0;
	GetAddr = false;
	//
	if (TypeIR != 0) ToIR = TypeIR;
	else if (To != 0) ToIR = To->Codegen();
	else { ToIR = new PtrTyIR(GetTy().E->Codegen()); } // ?????????????????????????????

	BaseIR *V      = 0;
	//
	if (NoDeref) V = new BitcastIR(FromIR, ToIR);
	else V = new BitcastIR(FromIR, ToIR->GetTy());

	ManagerIR->Insert(V, Body);
	return V;
}

// IsCast
BaseIR *IsCastExprAST::Codegen() {
	BaseIR *CIR = E->Codegen();
	BaseIR *TyIR = Ty->Codegen();
	BaseIR *V = new BitcastIR(CIR, TyIR);
	ManagerIR->Insert(V, Body);
	
	return CreateCmpEQ(V, new NullValIR(CIR), E->GetTy());
}

BaseIR *NotNullExprAST::Codegen() {
	BaseIR *ValIR = Val->Codegen();
	BaseIR *Cmp = new ICmpIR(ValIR, new NullValIR(ValIR->GetTy()), new NEIR(), 1);
	ManagerIR->Insert(Cmp, Body);

	return Cmp;
}

BaseIR *Int2PtrExprAST::Codegen() {
	BaseIR *VE = Expr->Codegen();
	BaseIR *V = new IntToPtrIR(VE, Ty->Codegen());
	ManagerIR->Insert(V, Body);

	return V;
}

BaseIR *Ptr2IntExprAST::Codegen() {
	BaseIR *VE = Expr->Codegen();
	BaseIR *V = new PtrToIntIR(VE, Ty->Codegen());
	ManagerIR->Insert(V, Body);

	return V;
}

#endif
