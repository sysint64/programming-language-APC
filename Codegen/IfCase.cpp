#ifndef CPP_CODEGEN_IFCASE
#define CPP_CODEGEN_IFCASE 1

// Main
#include <string>
#include <map>

using namespace std;

//* Поток управления
// Условие
BaseIR *IfOperAST::Codegen() {
	BaseIR *ElseL;
	BaseIR *ThenL   = new LabelIR("if");
	BaseIR *AfterL  = new LabelIR("if.end");
	BaseIR *AfterBr = new BrIR(AfterL);
	
	if (ElseB) ElseL = new LabelIR("if.else");
	
	BaseIR *CondBr;
	BaseIR *CondV  = Cond->Codegen();
	
	if (ElseB) CondBr = new CondBrIR(CondV, ThenL, ElseL);
	else CondBr = new CondBrIR(CondV, ThenL, AfterL);
	
	// Добавляем в очередь IR
	ManagerIR->Insert(CondBr, Body);
	ManagerIR->Insert(ThenL, Body);
	ThenOps.Codegen();
	ManagerIR->Insert(AfterBr, Body);
	//
	if (ElseB) {
		ManagerIR->Insert(ElseL , Body);
		ElseOps.Codegen();
		ManagerIR->Insert(AfterBr, Body);
	}
	
	ManagerIR->Insert(AfterL, Body);
	
	return 0;
}

// Case
BaseIR *TdOperAST::Codegen() {
	return 0;
}

BaseIR *CaseOperAST::Codegen() {
	vector<BaseIR*> TdsIR;
	//
	for (int i = 0; i < Tds.size(); ++i) {
		Tds[i]->Label = new LabelIR("case");
		TdsIR.push_back(new TdIR(Tds[i]->Val->Codegen(), Tds[i]->Label));
	}
	//
	BaseIR *AfterL = new LabelIR("switch.end");
	BaseIR *DefL   = new LabelIR("switch.default");
	BaseIR *Case;
	
	AfterL->Prev = CurLAfter;
	CurLAfter = AfterL;
	
	if (ElseOp != 0) Case = new CaseIR(Id->Codegen(), TdsIR, DefL);
	else Case = new CaseIR(Id->Codegen(), TdsIR, AfterL);
	//
	ManagerIR->Insert(Case, Body); // Добавляем таблицу переходов
	//
	for (int i = 0; i < Tds.size(); ++i) {
		ManagerIR->Insert(Tds[i]->Label, Body);
		Tds[i]->Ops->Codegen();
		ManagerIR->Insert(new BrIR(AfterL), Body);
	}
	//
	if (ElseOp != 0) {
		ManagerIR->Insert(DefL, Body);
		ElseOp->Codegen();
		ManagerIR->Insert(new BrIR(AfterL), Body);
	}
	//
	ManagerIR->Insert(AfterL, Body);
	//
	CurLAfter = CurLAfter->Prev;
	//
	return Case;
}

#endif
