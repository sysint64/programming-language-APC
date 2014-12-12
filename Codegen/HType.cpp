#ifndef CPP_CODEGEN_HTYPE
#define CPP_CODEGEN_HTYPE 1

//
BaseIR *HTypeExprAST::Codegen() {
	//
	if (IR != 0) return IR;
	//
	if (dynamic_cast<ATDExprAST*>(Type)) LIRInS = true;
	//LIRInS = true;
	CurTypeName = TypeName;
	BaseIR *TyIR = Type->Codegen();
	BaseIR *DTy = new DeclTyIR(TyIR, TypeName);
	
	if (!dynamic_cast<ATDExprAST*>(Type)) ManagerIR->Insert(DTy, DType);
	
	BaseIR *TypeIR = new CustomIR(DTy->GetAddr());
	TypeIR->SetTy(DTy->GetTy());
	TypeIR->SetAlign(TyIR->GetAlign());
	TypeIR->SetAddr(DTy->GetAddr());
	
	(*TypeDict)[TypeName].TypeIR = TypeIR;
	LIRInS = false;
	IR = DTy;

	return DTy;
}

//
BaseIR *HTyExprAST::Codegen() {
	BaseIR *DT = new CustomIR(DY->GetAddr());
	DT->SetAddr(DY->GetAddr());
	DT->SetTy(DY->GetTy());
	DT->SetAlign(DY->GetTy()->GetAlign());
	
	return DT;
}

//
BaseIR *StructTyExprAST::Codegen() {
	if (IR != 0) return IR;
	VarDict = &mVars;
	
	vector<BaseIR*> Res;
	NoInsIR = true;
	int Offset = 0;

	// Empty
	Res.push_back(new IntTyIR(8));
	//printf("%d\n", SVars->ops.size()); exit(0);
	//
	for (int i = 0; i < SVars->ops.size(); i++) {
		VarOperAST *pVar = (VarOperAST*)(SVars->ops[i]);
		
		for (int j = 0; j < pVar->IdNames.size(); j++) {
			mVars[pVar->IdNames[j]].Offset = Offset;
			Res.push_back(pVar->Codegen());
			
			if (j < pVar->IdNames.size()) Offset++;
		}
	}

	//
	for (int i = 0; i < SVars->ops.size(); i++) {
		VarOperAST *pVar = (VarOperAST*)(SVars->ops[i]);
		TypeAST Ty = pVar->Type->GetTy();

		//if (IsPtrTy(Ty))
		int nderef = 0;
		while (Ty.E != 0 && Ty.Ty != TSTRUCT) {
			Ty = Ty.E->GetTy();
			nderef++;
			puts("!");
		}

		if (!dynamic_cast<ATDExprAST*>(Ty.TypeExpr))
		//if (pVar->Type->GetTy().Name != "test^")
			continue;

		//puts("!"); exit(0);
		//puts(Ty.E->GetTy().Name.c_str()); exit(0);
		ExprAST *Type = (*TypeDict)[TypeName].Type;

		for (int i = 0; i < nderef; i++)
			Type = new PtrTyExprAST(Type);

		pVar->Type = Type;
		map<string, Var> *VD = pVar->VD;
		
		for (int j = 0; j < pVar->IdNames.size(); j++) {
			/*puts(pVar->IdNames[j].c_str());
			mVars[pVar->IdNames[j]].Offset = Offset;
			Res.push_back(pVar->Codegen());
			
			if (j < pVar->IdNames.size()) Offset++;*/
			OperAST *E = new VarOperAST(pVar->IdNames[j], Type, VD, false);
			//puts(pVar->IdNames[i].c_str());
			(*VD)[pVar->IdNames[j]].Stmt	= E;
			(*VD)[pVar->IdNames[j]].Type	= Type->GetTy();
			(*VD)[pVar->IdNames[j]].TypeAST	= Type;
			//
			mVars[pVar->IdNames[j]].TypeAST = Type;
		}
	}
	
	NoInsIR = false;
	
	BaseIR *DT = new StructTyIR(Res, "");
	if (Packed) DT = new PackedTyIR(DT);
	DT->SetAlign("8");
	DT->SetAddr(DT->GetEmit());
	DT->SetTy(DT);
	
	IR = DT;

	return DT;
}

//
BaseIR *ClassTyExprAST::Codegen() {
	VarDict = &mVars;
	
	vector<BaseIR*> Res;
	NoInsIR = true;
	int Offset = 0;
	//
	// TypeID
	if (arch == x32) Res.push_back(new IntTyIR(32));
	else Res.push_back(new IntTyIR(64));
	//
	for (int i = 0; i < SBlock.size(); i++) {
		BlockOperAST *sVars = (BlockOperAST*)SBlock[i]->ops[0];
		//
		for (int j = 0; j < sVars->ops.size(); j++) {
			VarOperAST *pVar = (VarOperAST*)(sVars->ops[j]);
			//
			for (int k = 0; k < pVar->IdNames.size(); k++) {
				mVars[pVar->IdNames[k]].Offset = Offset;
				Res.push_back(pVar->Codegen());
				
				if (k < pVar->IdNames.size()) Offset++;
			}
		}
	}
	//
	NoInsIR = false;
	
	BaseIR *DT = new StructTyIR(Res, "");
	DT->SetAlign("8");
	DT->SetAddr(DT->GetEmit());
	DT->SetTy(DT);
	
	return DT;
}

// Inherited
BaseIR *InheritedExprAST::Codegen() {
	vector<BaseIR*> ArgsV;
	//
	struct overloads ovr = *(*FuncDict)[CurFuncName].Overloads[CurOvrID].old_ovr;
	//
	BaseIR *IR    = ovr.Func;
	BaseIR *Prot  = ovr.Prot;
	//
	for (int i = 0; i < ovr.FuncAST->Prot->Args->vArgs.size(); i++) {
		NoInsIR = true;
		BaseIR *ArgIR = ovr.FuncAST->Prot->Args->vArgs[i]->Codegen();
		BaseIR *load = new LoadIR(ArgIR);
		NoInsIR = false;
		ManagerIR->Insert(load, Body);
		ArgsV.push_back(load);
	}
	//
	BaseIR *load = new LoadIR(ThisClassPtrIR);
	ManagerIR->Insert(load, Body);
	ArgsV.push_back(load);
	//
	if (ovr.Constructor) Prot->SetTy(new VoidTyIR());
	//
	BaseIR *Call = new CallVarIR(IR, Prot, ArgsV);
	ManagerIR->Insert(Call, Body);
	
	return 0;
}

#endif
