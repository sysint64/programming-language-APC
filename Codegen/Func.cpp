#ifndef CPP_CODEGEN_FUNC
#define CPP_CODEGEN_FUNC 1

//
BaseIR *BlockDictsOperAST::Codegen() {
	//
	for (int i = 0; i < Vars.size(); i++)
		Vars[i]->Codegen();
	
	for (int i = 0; i < Consts.size(); i++)
		Consts[i]->Codegen();
	
	return 0;
}

//* Директивы
BaseIR *DirectiveOperAST::Codegen() {
	return new CustomIR(Name); 
}

BaseIR *DirectivesOperAST::Codegen() {
	CCIR = CC->Codegen();
	
	// Attributes
	AttrIR = Attr->Codegen();
	
	if (AttrIR != 0)
	for (int i = 0; i < Attr->Res.size()-1; i++) {
		AttrIR = new CustomIR(AttrIR->GetEmit()+" "+Attr->Res[i]->GetEmit());
	}
	
	LinkIR = Linkage->Codegen();
	return 0;
}

//* Аргементы
BaseIR *ArgExprAST::Codegen() {
	BaseIR *ty = Type->Codegen();
	
	//if (IsClassTy(Type->GetTy())) ty = new PtrTyIR(ty);
	if (IsClassTy(Type->GetTy())/* & !Type->GetTy().MustCast*/) {
		ty = new PtrTyIR(ty);
		ty->SetAlign("8");
	} else
	
	if (Ty == 1) ty = new PtrTyIR(ty);
	
	return new ArgIR(ty, Name);
}

BaseIR *ArgsOperAST::Codegen() {
	//
	if (SRet != 0) {
		BaseIR *TypeIR = SRet->Codegen();
		FuncSRetIR = new ArgIR(TypeIR, "result", true);
		vArgsIR.push_back(FuncSRetIR);
	}
	//
	for (int i = 0; i < Args.size(); i++)
		vArgsIR.push_back(Args[i].E->Codegen());
	//
	if (CurClassIR != 0) {
		//BaseIR *TypeIR = new PtrTyIR(CurClassIR);
		BaseIR *TypeIR = new PtrTyIR(new IntTyIR(8));
		vArgsIR.push_back(new ArgIR(TypeIR, "this"));
	}
	//
	
	if (vArgsIR.size() > 0) return new ArgsIR(vArgsIR);
	return 0;
}

BaseIR *ArgsOperAST::CodegenDef() {
	//
	for (int i = 0; i < vArgs.size(); i++) {
		BaseIR *Alloca = vArgs[i]->Codegen();
		
		LIRInS = true;
		BaseIR *ArgIR = Args[i].E->Codegen();
		LIRInS = false;
		
		if (!IsClassTy(Args[i].E->GetTy())) {
			BaseIR *V = new StoreIR(ArgIR, Alloca);
			ManagerIR->Insert(V, Body);
		}
	}
	
	//
	if (CurClassIR != 0) {
		//BaseIR *Alloca = vArgs[i]->Codegen();
		BaseIR *TypeIR   = new PtrTyIR(CurClassIR);
		BaseIR *Cast     = new BitcastIR(vArgsIR[vArgsIR.size()-1], TypeIR);
		BaseIR *Alloca   = new AllocIR(TypeIR, "this");
		BaseIR *Allocai8 = new AllocIR(new PtrTyIR(new IntTyIR(8)), "this.ptr");
		//
		ManagerIR->Insert(Allocai8, Body);
		ManagerIR->Insert(Alloca, Body);
		ManagerIR->Insert(Cast, Body);
		
		//
		LIRInS = true;
		BaseIR *ArgIR = vArgsIR[vArgsIR.size()-1];
		LIRInS = false;
		//
		BaseIR *Store = new StoreIR(Cast, Alloca);
		BaseIR *Storei8 = new StoreIR(ArgIR, Allocai8);
		//
		ManagerIR->Insert(Store, Body);
		ManagerIR->Insert(Storei8, Body);
		//
		ThisClassIR = Alloca;
		ThisClassPtrIR = Allocai8;
	}
	
	//
	if ((*LocalVars)["result"].Var) {
		BaseIR *Store = new StoreIR(vArgsIR[0], (*LocalVars)["result"].Val);
		ManagerIR->Insert(Store, Body);
	}

	return 0;
}

//
// 
int GetPrototypeId (const string Name, vector<struct Var> Args, ExprAST *RetType) {
	int  ai		= 0;
	int  begin	= 0;
	bool fail	= false;
	//
	vector<struct Var> fArgs;
	//puts(("get id in \""+Name+"\" size ovr is "+IntToStr((*FuncDict)[Name].Overloads.size())).c_str());
	
	//
	for (int i = 0; i < (*FuncDict)[Name].Overloads.size(); i++) {
		struct overloads ovr = (*FuncDict)[Name].Overloads[i];
		if (ovr.FuncAST->Prot->Args->Args.size() == Args.size()) {
			begin = i;
			fail = false;
			break;
		}
		
		fail = true;
	}
	//
	
	//
	fail = true;
	//
	for (int i = begin; i < (*FuncDict)[Name].Overloads.size(); i++) {
		struct overloads ovr = (*FuncDict)[Name].Overloads[i];
		fArgs = ovr.FuncAST->Prot->Args->Args;
		
		if (fArgs.size() != Args.size()) continue;
		bool a = false;
		
		if (fArgs.size() == Args.size() & fArgs.size() == 0) a = true;
		for (int j = 0; j < fArgs.size(); j++) {
			struct TypeAST Ty = Args[j].Type;
			
			if (EqTy(Ty, fArgs[j].Type)) {
				a = true;
			} else { a = false; break; }
		}
		//
		if (!ovr.Constructor) {
			if (EqTy(ovr.FuncAST->Prot->RetType->GetTy(), RetType->GetTy()) & a) {
				ai = i;
				fail = false;
				break;
			}
		} else if (a) {
			ai = i;
			fail = false;
			break;
		}
		//
	}
	//
	if (fail) { // Не нашили нужной функции
		return Error<int>(no_match_func_for_call_to_s, Name);
	}
	
	return ai;
}

// Прототип функции
BaseIR *PrototypeExprAST::Codegen() {
	bool a = LIRInS;
	
	LIRInS = true;
	Args->vArgsIR.clear();
	BaseIR *ArgsIR = Args->Codegen();
	LIRInS = a;
	Directives->Codegen();
	
	//
	if (!IsType) {
		BaseIR *RetTyIR = RetType->Codegen();
		
		if (IsClassTy(RetType->GetTy())) {
			RetTyIR = new PtrTyIR(RetTyIR);
		}
		
		string N = "";
		
		int n = 0;
		
		if (isFuncDef) n = GetPrototypeId(Name, Args->Args, RetType);
		else n = (*FuncDict)[Name].Overloads.size()-1;
			
		if (n != 0) N = "."+IntToStr(n);
		//
		if ((*FuncDict)[Name].ClassName.length() != 0) {
			LLVMName = /*CurNameSpace+"."+*/(*FuncDict)[Name].ClassName+"."+Name+N;
		} else {
			if (MainFile && Name == "main") LLVMName = Name;
			else LLVMName = /*CurNameSpace+"."+*/Name+N;
		}
		
		//
		if (Directives->External) {
			if (Directives->eName != "") LLVMName = Directives->eName;
			else LLVMName = Name;
			
			if (Directives->Lib == "llvm") {
				LLVMName = "llvm."+LLVMName;
			}
			
			LIRInS = true;
		}
		//
		(*FuncDict)[Name].Overloads[n].Name = LLVMName;
		(*FuncDict)[Name].Overloads[n].Prot = new ProtoIR (RetTyIR, Args->vArgsIR);
		
		//
		LIRInS = true;
		BaseIR *ProtTy = new ProtoTyIR(RetTyIR, Args->vArgsIR);
		BaseIR *Proto  = new DefIR((*FuncDict)[Name].Overloads[n].Name, RetTyIR, ArgsIR, ProtTy, Directives->CCIR, Directives->AttrIR, Aligned);
		(*FuncDict)[Name].Overloads[n].Func = Proto;
		IRProto = (*FuncDict)[Name].Overloads[n].Prot;
		LIRInS = false;
		
		return Proto;
	} else {
		//
		if (OfClass) {
			BaseIR *Ptr = new aPtrTyIR(new IntTyIR(8));
			Args->vArgsIR.push_back(Ptr);
		}
		//
		BaseIR *RetTyIR = RetType->Codegen();
		BaseIR *Prot = new ProtoTyIR(RetTyIR, Args->vArgsIR);
		
		return Prot;
	}
}

//
BaseIR *PrototypeExprAST::GenCurFunc() {
	LIRInS = true;
	BaseIR *ArgsIR  = Args->Codegen();
	BaseIR *RetTyIR = RetType->Codegen();
	string N = "";
	
	int n = 0;
		
	if (isFuncDef) n = GetPrototypeId(Name, Args->Args, RetType);
	else n = (*FuncDict)[Name].Overloads.size();

	if (n != 0) N = "."+IntToStr(n);
	if ((*FuncDict)[Name].ClassName.length() != 0) {
		LLVMName = /*CurNameSpace+"."+*/(*FuncDict)[Name].ClassName+"."+Name+N;
	} else {
		if (MainFile && Name == "main") LLVMName = Name;
		else LLVMName = /*CurNameSpace+"."+*/Name+N;
	}
	
	//
	if (Directives->External) {
		if (Directives->eName != "") LLVMName = Directives->eName;
		else LLVMName = Name;
		
		if (Directives->Lib == "llvm") {
			LLVMName = "llvm."+LLVMName;
		}
		
		LIRInS = true;
	}
	
	LIRInS = true;
	//BaseIR *ProtTy = new ProtoTyIR(RetTyIR, Args->vArgsIR);
	BaseIR *Func = new DefIR(LLVMName, RetTyIR);
	LIRInS = false;
	return Func;
}

// Функция
BaseIR *FuncOperAST::Codegen() {
	//
	LocalVars = &Dicts->VD;
	InsideClass = InClass;
	//
	if (CurClassTy != 0) {
		ClassTyExprAST* S = (ClassTyExprAST*)(CurClassTy->GetTy().E);
		
		LocalFuncs = &S->mMethods;
		FuncDict   = &S->mMethods;
	}
	
	if (Ops == 0 && !Prot->Directives->External)
		goto Exit;
	//
	if (!Prot->Directives->External) {
		isFuncDef = true;
		BaseIR *ProtoIR = Prot->Codegen();
		CurVarBlockIR = new BlockIR();
		ManagerIR->Insert(ProtoIR, Body);
		ManagerIR->Insert(CurVarBlockIR, Body);
		isFuncDef = false;
		
		//
		VarDict = &Dicts->VD;
		Dicts->Codegen();
		
		Prot->Args->CodegenDef();
		ExitVar = (*LocalVars)["result"].Val;

		if (ExitVar != 0)
			ExitVar->MemCpy = Prot->RetType->MemCpy;
		
		// Body
		Ops->Codegen();
		
		// Return
		if (IsVoidTy(Prot->RetType->GetTy())) {
			ManagerIR->Insert(new RetVoidIR(), Body);
		} else {
			/*BaseIR *RetVar = (*LocalVars)["result"].Val;//Dicts->ResultVar->Codegen();
			BaseIR *Ret = 0;
			//
			if (!Prot->RetType->MemCpy) {
				BaseIR *Load = new LoadIR(RetVar);
				ManagerIR->Insert(Load, Body);
				Ret = Load;
			} else Ret = RetVar;
			//
			ManagerIR->Insert(new RetIR(Ret), Body);*/
			BaseIR *Ret = new UndefValIR(Prot->RetType->Codegen());
			ManagerIR->Insert(new RetIR(Ret), Body);
		}
	} else {
		string Name = "";
		
		if (Prot->Directives->eName != "") Name = Prot->Directives->eName;
		else Name = Prot->Name;
		
		if (Prot->Directives->Lib == "llvm") {
			Name = "llvm."+Name;
		}
		//
		BaseIR *ProtoIR = Prot->Codegen();
		int n = (*FuncDict)[Prot->Name].Overloads.size()-1;
		BaseIR *Declare = new DeclareIR(Name, (*FuncDict)[Prot->Name].Overloads[n].Prot, Prot->Directives->CCIR, Prot->Directives->AttrIR);
		ManagerIR->Insert(Declare, End);
	}
	//
Exit:
	//
	InsideClass = false;
	//
	LocalVars      = 0;
	LocalFuncs     = 0;
	ExitVar        = 0;
	ThisClassIR    = 0;
	ThisClassPtrIR = 0;
	CurClassExpr   = 0;
	//
	CurClassIR = 0;
	CurClassTy = 0;
	FuncSRetIR = 0;
	//
	if (CurVarBlockIR != 0)
		CurVarBlockIR->Update();

	CurVarBlockIR = 0;
	
	return 0;
}

#endif
