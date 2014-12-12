#ifndef CPP_CODEGEN_LOOP
#define CPP_CODEGEN_LOOP 1

// Main
#include <string>
#include <map>

using namespace std;

// Поток управления
// While
BaseIR *WhileOperAST::Codegen() {
	BaseIR *CondL  = new LabelIR("while");
	BaseIR *LoopL  = new LabelIR("while.do");
	BaseIR *AfterL = new LabelIR("while.end");
	
	BaseIR *CondBr = new BrIR(CondL);
	
	// Добавляем в очередь IR
	ManagerIR->Insert(CondBr, Body); // переход на метку CondL
	ManagerIR->Insert(CondL , Body); // сама меткка CondL

	BaseIR *CondV  = Cond->Codegen();
	BaseIR *CondBrLoop = new CondBrIR(CondV, LoopL, AfterL);  // условие
	
	ManagerIR->Insert(CondBrLoop, Body); // условный переход
	
	ManagerIR->Insert(LoopL , Body); // метка на тело цикла

	// Break Label
	AfterL->Prev = CurLAfter;
	CurLAfter = AfterL;
	
	// Continue Label
	CondL->Prev = CurLBegin;
	CurLBegin = CondL;
	
	// do
	Ops.Codegen();                   // тело цикла
	
	CurLAfter = CurLAfter->Prev;
	CurLBegin = CurLBegin->Prev;

	ManagerIR->Insert(CondBr, Body); // переход на метку с условием CondL
	ManagerIR->Insert(AfterL, Body); // метка после цикла
	
	return 0;
}

// For
BaseIR *ForOperAST::Codegen() {
	//
	if (Declare != 0)
		Declare->Codegen();
	//
	BaseIR *CondL  = new LabelIR("for");
	BaseIR *LoopL  = new LabelIR("for.do");
	BaseIR *IncL   = new LabelIR("for.inc");
	BaseIR *AfterL = new LabelIR("for.end");
	
	BaseIR *CondBr = new BrIR(CondL);
	
	// Добавляем в очередь IR
	
	BaseIR *A = Start->Codegen();
	
	ManagerIR->Insert(CondBr, Body); // переход на метку CondL
	ManagerIR->Insert(CondL , Body); // сама меткка CondL
	
	BaseIR *L = Load->Codegen();
	BaseIR *B = To->Codegen();
	
	BaseIR *CondV;
	

	if (IsFloatTy(Start->GetTy())) {
		//
		if (dynamic_cast<UnaryExprAST*>(Step)) {
			//
			if (((UnaryExprAST*)Step)->Op == '-') CondV = new FCmpIR(L, B, new OGEIR());
			else CondV = new FCmpIR(L, B, new OLEIR());
			//
		} else CondV = new FCmpIR(L, B, new OLEIR());
	} else {
		if (dynamic_cast<UnaryExprAST*>(Step)) {
			//
			if (((UnaryExprAST*)Step)->Op == '-') CondV = new ICmpIR(L, B, new SGEIR());
			else CondV = new ICmpIR(L, B, new SLEIR());
			//
		} else CondV = new ICmpIR(L, B, new SLEIR());
	}
	//
	BaseIR *CondBrLoop = new CondBrIR(CondV, LoopL, AfterL);  // условие
	
	ManagerIR->Insert(CondV     , Body);
	ManagerIR->Insert(CondBrLoop, Body); // условный переход
	
	ManagerIR->Insert(LoopL , Body); // метка на тело цикла
	
	CurLBegin = IncL; CurLAfter = AfterL;
	Do.Codegen();                    // тело цикла
	CurLAfter = 0; CurLBegin = 0;
	//
	ManagerIR->Insert(new BrIR(IncL), Body);
	ManagerIR->Insert(IncL  , Body);

	BaseIR *Inc;
	L = Load->Codegen();
	
	if (IsFloatTy(Start->GetTy())) Inc = new FAddIR(L, Step->Codegen());
	else Inc = new AddIR(L, Step->Codegen());
	
	BaseIR *Store = new StoreIR(Inc, A);

	ManagerIR->Insert(Inc   , Body);
	ManagerIR->Insert(Store , Body);
	ManagerIR->Insert(CondBr, Body); // переход на метку с условием CondL
	ManagerIR->Insert(AfterL, Body); // метка после цикла
	//
	return 0;
}

#endif
