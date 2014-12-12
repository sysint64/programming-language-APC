#ifndef CPP_CODEGEN_OPER
#define CPP_CODEGEN_OPER 1

// Main
#include <map>

using namespace std;

//* Операторы

// Оператор выражения
BaseIR *ExprOpAST::Codegen() {
	return Expr->Codegen();
} 

// Блок операторов
BaseIR *BlockOperAST::Codegen() {
	BaseIR *result = 0;
	
	//foreach (i, ops) {
	for (int i = 0; i < ops.size(); i++) {
		result = ops[i]->Codegen();
		Res.push_back(result);
		//result = (*i)->Codegen();
		//Res.push_back(result);
	}
	//}
	
	return result;
}

// break, continue, exit
BaseIR *BreakOperAST::Codegen() {
	if (CurLAfter == 0) 
		return Error<BaseIR*>(break_stmt_not_within_ls);
	
	BaseIR *Break = new BrIR(CurLAfter);
	ManagerIR->Insert(Break, Body);
	
	return Break;
}

BaseIR *ContinueOperAST::Codegen() {
	if (CurLBegin == 0)
		return Error<BaseIR*>(continue_stmt_not_within_s);
	
	BaseIR *Continue = new BrIR(CurLBegin);
	ManagerIR->Insert(Continue, Body);
	
	return Continue;
}

BaseIR *ExitOperAST::Codegen() {
	BaseIR *Ret;
	
	if (ExitVar == 0) {
		Ret = new RetVoidIR(false);
	} else {
		if (!ExitVar->MemCpy) {
			BaseIR *Load = new LoadIR(ExitVar);
			Ret = new RetIR(Load, false);
			ManagerIR->Insert(Load, Body);
		} else Ret = new RetVoidIR(false);
	}
	
	ManagerIR->Insert(Ret, Body);
	return Ret;
}

// RetOperAST
BaseIR *RetOperAST::Codegen() {
	//
	if (FuncSRetIR != 0) {
		//GetAddr = true;
		BaseIR *RetStore = new StoreIR(Val->Codegen(), FuncSRetIR);
		BaseIR *Ret 	 = new RetVoidIR(false);
		//GetAddr = false;
		
		ManagerIR->Insert(RetStore, Body);
		ManagerIR->Insert(Ret, Body);

		return Ret;
	} else if (Val != 0) {
		BaseIR *ValIR = Val->Codegen();
		BaseIR *Ret = new RetIR(ValIR, false);
		ManagerIR->Insert(Ret, Body);
		return Ret;
	} else {
		BaseIR *Ret = new RetVoidIR(false);
		ManagerIR->Insert(Ret, Body);
		return Ret;
	}
}

BaseIR *RetOvrOperAST::Codegen() {
	BaseIR *ValIR = Var->Codegen();
	ValIR = new LoadIR(ValIR);

	Ovr->Codegen();

	ManagerIR->Insert(ValIR, Body);
	//
	if (FuncSRetIR != 0) {
		//GetAddr = true;
		BaseIR *RetStore = new StoreIR(ValIR, FuncSRetIR);
		BaseIR *Ret 	 = new RetVoidIR(false);
		//GetAddr = false;
		
		ManagerIR->Insert(RetStore, Body);
		ManagerIR->Insert(Ret, Body);

		return Ret;
	} else {
		BaseIR *Ret = new RetIR(ValIR, false);
		ManagerIR->Insert(Ret, Body);
		return Ret;
	}
}

//
BaseIR *StoreOperAST::Codegen() {
	NoInsIR = true;
	LIRInS = true;
	BaseIR *R = E->Codegen();
	BaseIR *I = V->Codegen();
	LIRInS = false;
	NoInsIR = false;
	
	BaseIR *V = new StoreIR(R, I);
	ManagerIR->Insert(V, Body);

	return V;
}

BaseIR *LifetimeVarsExprAST::Codegen() {
	//
	for (map<string, Var>::const_iterator it = VD->begin(); it != VD->end(); it++) {
		if (!(*VD)[it->first].Def) continue;
		string IdName = it->first;

		VarOperAST *Var = (VarOperAST*)((*VD)[IdName].Stmt);
		BaseIR *TypeIR  = Var->Type->Codegen();

		if (IsClassTy(Var->Type->GetTy())) {
			//
			TypeIR = new PtrTyIR(TypeIR);
			TypeIR->SetAlign("8");
		}

		BaseIR *Id = new AllocIR(TypeIR, IdName);
		BaseIR *CastId  = new BitcastIR(Id , new PtrTyIR(new IntTyIR(8)));
		BaseIR *LifeTimeStart = new CustomIR("  call void @llvm.lifetime.start(i64 -1, "+CastId->GetTy()->GetAddr()+" "+CastId->GetAddr()+")");

		(*VD)[IdName].isAlloced = true;
		(*VD)[IdName].Val		= Id;
		(*VD)[IdName].Type		= Var->Type->GetTy();

		ManagerIR->Insert(Id, Body);
		ManagerIR->Insert(CastId, Body);
		ManagerIR->Insert(LifeTimeStart, Body);
	}

	BaseIR *OpsIR = Ops->Codegen();

	for (map<string, Var>::const_iterator it = VD->begin(); it != VD->end(); it++) {
		if (!(*VD)[it->first].Def) continue;
		string IdName = it->first;

		VarOperAST *Var = (VarOperAST*)((*VD)[IdName].Stmt);
		BaseIR *TypeIR  = Var->Type->Codegen();

		if (IsClassTy(Var->Type->GetTy())) {
			//
			TypeIR = new PtrTyIR(TypeIR);
			TypeIR->SetAlign("8");
		}

		BaseIR *Id			= (*VD)[IdName].Val;
		BaseIR *CastId		= new BitcastIR(Id , new PtrTyIR(new IntTyIR(8)));
		BaseIR *LifeTimeEnd	= new CustomIR("  call void @llvm.lifetime.end(i64 -1, "+CastId->GetTy()->GetAddr()+" "+CastId->GetAddr()+")");

		ManagerIR->Insert(CastId, Body);
		ManagerIR->Insert(LifeTimeEnd, Body);
	}

	return OpsIR;
}

#endif
