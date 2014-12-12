#ifndef CPP_PARSER_HTYPE
#define CPP_PARSER_HTYPE 1

//
ExprAST *parser::ParseStruct(const bool packed) {
	//
	Lexer->GetNextToken();

	if (Lexer->CurToken != tok_id)
		Error<ExprAST*>(expected_id);

	string TypeName = Lexer->IdentifierStr;

	//
	ExprAST *ADTTy = new ATDExprAST(TypeName);
	ADTTy->Ty.Ty = TSTRUCT;
	ADTTy->Ty.MustCast = true;
	ExprAST *ADTDTy = new HTypeExprAST(TypeName, ADTTy);
	//
	(*TypeDict)[TypeName].Def  = true;
	(*TypeDict)[TypeName].Type = ADTDTy;
	(*TypeDict)[TypeName].TypeIR = ADTDTy->Codegen();
	//

	map<string, Var> *VD = new map<string, Var>(); // Переменные структуры
	VarDict = VD;
	ExprAST *Parent = 0;
	
	Lexer->GetNextToken();
	//
	if (Lexer->CurToken == '(') {
		Lexer->GetNextToken();
		
		Parent = ParseType();
		
		if (!IsStructTy(Parent->GetTy()))
			return Error<ExprAST*>(type_must_be_s, "record");
		
		if (Lexer->CurToken != ')')
			return Error<ExprAST*>(expected, ")");
	} else Lexer->GetPrevToken();
	//
	BlockOperAST* V = 0;
	//
	if (Lexer->CurToken == tok_end) {
		//Lexer->GetNextToken();
		V = new BlockOperAST();
		goto Exit;
	}

	V = (BlockOperAST*)ParseVarBlock(false);
	//
	//VarDict = &ExternVars;
	
	// Get parent vars
	if (Parent != 0) {
		StructTyExprAST *S = (StructTyExprAST*)Parent->GetTy().E;
		//
		/*for (int i = 0; i < S->Names.size(); i++) {
			
		}*/
		
		for (int i = 0; i < S->SVars->ops.size(); i++) {
			VarOperAST *pVar = (VarOperAST*)(S->SVars->ops[i]);
			VarOperAST *CurpVar = (VarOperAST*)(V->ops[i]);
			V->Append(S->SVars->ops[i]);
			
			//if (VD[pVar->IdNames[j]].Def) {
			//		V->ops[i] = S->SVars->ops[i];
			//	} else V->Append(S->SVars->ops[i]);
			
			for (int j = 0; j < pVar->IdNames.size(); j++) {
				(*VD)[pVar->IdNames[j]] = S->mVars[pVar->IdNames[j]];
			}
			//
			
		}
	}
	
Exit:
	//
	if (Lexer->CurToken != tok_end)
		return Error<ExprAST*>(expected, "end");
	
	Lexer->GetNextToken();
	
	//
	if (Lexer->CurToken != ';')
		return Error<ExprAST*>(expected, ";");
	
	Lexer->GetNextToken();
	StructTyExprAST *Ty = new StructTyExprAST(TypeName, V, *VD, packed);
	ExprAST *DTy = new HTypeExprAST(TypeName, (ExprAST*)Ty);
	//
	(*TypeDict)[TypeName].Def  = true;
	(*TypeDict)[TypeName].Type = DTy;
	
	return DTy;
}

//
OperAST *parser::ParseClassBlock(vector<string> *MethodsNames) {
	//
	BlockOperAST *Res   = new BlockOperAST();
	BlockOperAST *Prots = new BlockOperAST();
	//
	Lexer->GetNextToken();
	//
	if (Lexer->CurToken == tok_end | Lexer->CurToken == tok_public | Lexer->CurToken == tok_private | Lexer->CurToken == tok_protected) {
		OperAST* V = new BlockOperAST();
		LastVarBlock = (BlockOperAST*)V;
		Res->Append(V); // Vars
		goto Exit;
	}
	//
	if (Lexer->CurToken != tok_func) {
		Lexer->GetPrevToken();
		OperAST* V = ParseVarBlock(false);
		LastVarBlock = (BlockOperAST*)V;
		Res->Append(V); // Vars
	} else {
		OperAST* V = new BlockOperAST();
		LastVarBlock = (BlockOperAST*)V;
		Res->Append(V); // Vars
	}
	
	// Prototypes
	while (1) {
		PrototypeExprAST* V = 0;
		
		switch (Lexer->CurToken) {
			case tok_func : V = (PrototypeExprAST*) ParsePrototype (false, false, false, true); break; // function
			default : goto Exit;
		}
		//
		Lexer->GetNextToken();
		MethodsNames->push_back(V->Name);
		V->Codegen();
	}
	
	//Res->Append((OperAST*)Prots);
Exit:
	return Res;
}

//
ExprAST *parser::ParseClass() {
	//
	Lexer->GetNextToken();

	if (Lexer->CurToken != tok_id)
		Error<ExprAST*>(expected_id);

	string TypeName = Lexer->IdentifierStr;

	Lexer->GetNextToken();
	
	if (Lexer->CurToken == ';') {
		ExprAST *Ty = new ATDExprAST(TypeName);
		Lexer->GetNextToken();

		ExprAST *DTy = new HTypeExprAST(TypeName, Ty);
		//
		(*TypeDict)[TypeName].Def  = true;
		(*TypeDict)[TypeName].Type = DTy;

		return DTy;
	} else Lexer->GetPrevToken();
	//
	map<string, Var>  *VD = new map<string, Var>(); // Переменные класса
	map<string, Proc>  FD; // Методы класса
	
	VarDict  = VD;
	FuncDict = &FD;
	//
	ClassTyExprAST *ThisClass = new ClassTyExprAST();
	CurClassIR = new CustomIR("%"+CurNameSpace+"."+CurClassName);
	CurClassIR->SetAlign("8");
	CurClassIR->SetTy(CurClassIR);
	CurClassTy = ThisClass;
	//puts(CurClassIR->GetEmit().c_str());
	//CurClassTy = ThisClass;
	//
	isClass = true;
	vector<BlockOperAST*> Blocks;
	bool finish = false;
	ExprAST *Parent = 0;
	
	Lexer->GetNextToken();
	PrintToken();
	//
	if (Lexer->CurToken == '(') {
		Lexer->GetNextToken();
		
		//if (Lexer->CurToken != tok_id) 
		//	return Error<ExprAST*>(expected_unqualified_id);
		Parent = ParseType();
		
		if (!IsClassTy(Parent->GetTy()))
			return Error<ExprAST*>(type_must_be_s, "class");
		
		//Lexer->GetNextToken();
		
		if (Lexer->CurToken != ')')
			return Error<ExprAST*>(expected, ")");
	} else Lexer->GetPrevToken();
	
	Lexer->GetNextToken();
	//
	// Get Parent Params
	if (Parent != 0) {
		ClassTyExprAST *S = (ClassTyExprAST*)Parent->GetTy().E;
		
		// Vars
		for (int i = 0; i < S->SBlock.size(); i++) {
			BlockOperAST *sVars = (BlockOperAST*)S->SBlock[i]->ops[0];
			Blocks.push_back(S->SBlock[i]);
			
			for (int j = 0; j < sVars->ops.size(); j++) {
				VarOperAST *pVar = (VarOperAST*)(sVars->ops[j]);
				
				for (int k = 0; k < pVar->IdNames.size(); k++) {
					(*VD)[pVar->IdNames[k]] = S->mVars[pVar->IdNames[k]];
				}
				//
			}
			//
		}
		
		//puts(CurClassName.c_str());
		// Methods
		for (int i = 0; i < S->MethodsNames.size(); i++) {
			FD[S->MethodsNames[i]] = S->mMethods[S->MethodsNames[i]];
			FD[S->MethodsNames[i]].MayDeclare = true;
			//
			ThisClass->MethodsNames.push_back(S->MethodsNames[i]);
			//
			//puts(("  "+S->MethodsNames[i]).c_str());
			FD[S->MethodsNames[i]].ClassName = CurClassName;
		}
		//puts("<--");
	}
	//
	//
	
	while (!finish) {
		switch (Lexer->CurToken) {
			case tok_public    :
				CurVisibility = v_public;
				break;
				
			case tok_private   :
				CurVisibility = v_private;
				break;
				
			case tok_protected :
				CurVisibility = v_protected;
				break;
				
			default: finish = true;
		}
		//
		if (!finish) {
			Lexer->GetNextToken();
				
			if (Lexer->CurToken != ':')
				return Error<ExprAST*>(expected, ":");
					
			OperAST *V = ParseClassBlock(&ThisClass->MethodsNames);
			Blocks.push_back((BlockOperAST*)V);
		}
	}
	//
	isClass = false;
	
	//
	if (Lexer->CurToken != tok_end)
		return Error<ExprAST*>(expected, "end");
	
	Lexer->GetNextToken();
	
	//
	if (Lexer->CurToken != ';')
		return Error<ExprAST*>(expected, ";");
	
	Lexer->GetNextToken();
	//
	ThisClass->SBlock   = Blocks;
	ThisClass->mVars    = *VD;
	ThisClass->mMethods = FD;
	ThisClass->UpdateTy();
	//SBlock(sBlock), mVars(VD), mMethods(FD)
	
	//
	/*ClassTyExprAST *CastClass = new ClassTyExprAST();
	memcpy(CastClass, ThisClass, sizeof(ClassTyExprAST));
	//
	TypeAST Ty = ThisClass->GetTy();
	Ty.MustCast = false;
	CastClass->SetTy(Ty);
	
	HTypeExprAST *CastHType = new HTypeExprAST(CurClassName, ThisClass);
	ThisClass->SetTy(Ty);
	//CastClass->GetTy().Name = CurNameSpace+"."+CurClassName;
	//CastClass->GetTy().MustCast = true;
	
	//
	for (int i = 0; i < MustCasts.size(); i++) {
		VarOperAST *V = MustCasts[i];
		//
		for (int j = 0; j < V->IdNames.size(); j++) {
			ThisClass->mVars[V->IdNames[j]].Type    = CastHType->GetTy();
			ThisClass->mVars[V->IdNames[j]].TypeAST = CastHType;
		}
		
		//V->SetTy(ThisClass);
		//V->Type = ThisClass;
	}*/
	//
	//
	//MustCasts.clear();
	//CurClassIR = 0;
	//CurClassTy = 0;
	//CurClassTy = 0;

	ExprAST *DTy = new HTypeExprAST(TypeName, ThisClass);
	//
	(*TypeDict)[TypeName].Def  = true;
	(*TypeDict)[TypeName].Type = DTy;

	//exit(0);
	return DTy;

	return ThisClass;
	//return new ClassTyExprAST(Blocks, VD, FD);
}

//
ExprAST *parser::ParseHType() {
	string TypeName = Lexer->IdentifierStr;
	CurClassName = TypeName;
	Lexer->GetNextToken();
	
	//
	if (Lexer->CurToken != '=') {
		Error<OperAST*>(expected, "=");
		
		while (Lexer->CurToken != tok_id) {
			Lexer->GetNextToken();
		}
	}
		
	Lexer->GetNextToken();
	bool packed = false;
	
	if (Lexer->CurToken == tok_packed) {
		Lexer->GetNextToken();
		packed = true;
	}
	
	ExprAST *Ty;
	//
	switch (Lexer->CurToken) {
		case tok_set	: break;
		case '('	: break;
		case tok_record	: Ty = ParseStruct(packed); break;
		case tok_class	: {
			Lexer->GetNextToken();
			
			if (Lexer->CurToken == ';') {
				Ty = new ATDExprAST(TypeName);
				Lexer->GetNextToken();
				break;
			} else Lexer->GetPrevToken();
			
			LIRInS = true;
			ExprAST *CTy = new HTypeExprAST(TypeName, new PointerTyExprAST());
			CurClassIR = new CustomIR(CTy->Codegen()->GetAddr());
			CurClassIR->SetAlign("8");
			LIRInS = false;
			
			CurClassName = TypeName;
			Ty = ParseClass();
			((ClassTyExprAST*)Ty)->CTy = CTy;
			CurClassName = "";
			
			break;
		}
		//
		default:
			Ty = ParseType();
			
			if (Lexer->CurToken != ';')
				return Error<ExprAST*>(expected, ";");
			
			Lexer->GetNextToken();
			
			break;
	}
	
	//if (Lexer->CurToken != ';')
	//	return Error<ExprAST*>(expected, ";");
	
	//Lexer->GetNextToken();
	
	ExprAST *DTy = new HTypeExprAST(TypeName, Ty);
	//
	(*TypeDict)[TypeName].Def  = true;
	(*TypeDict)[TypeName].Type = DTy;
	//
	NoInsIR = true; LIRInS = true;
	BaseIR *TIR = DTy->Codegen();
	ExprAST *E = new HTyExprAST(TIR, DTy);
	
	TypeAST aTy = E->GetTy();
	aTy.MustCast = true; LIRInS = false;
	
	E->SetTy(aTy);
	NoInsIR = false;
	//
	if (CurClassTy != 0) {
		for (int i = 0; i < MustCasts.size(); i++) {
			VarOperAST *V = MustCasts[i];
			//
			for (int j = 0; j < V->IdNames.size(); j++) {
				CurClassTy->mVars[V->IdNames[j]].Type    = E->GetTy();
				CurClassTy->mVars[V->IdNames[j]].TypeAST = E;
			}
		}
	}
	//
	MustCasts.clear();
	CurClassName = "";
	CurClassIR = 0;
	CurClassTy = 0;
	//
	return DTy;
}

//
OperAST *parser::ParseHTypeBlock() {
	BlockOperAST *block = new BlockOperAST();
	Lexer->GetNextToken();
		
	bool skip = false;
	//
	if (Lexer->CurToken != tok_id) {
		skip = true;
		Error<OperAST*>(expected_id);
		
		while (Lexer->CurToken != tok_id) {
			Lexer->GetNextToken();
		}
	}
	//
	//
	
	while (1) {
		ExprAST *V = ParseHType();
		
		if (!skip && V != 0) block->Append(new ExprOpAST(V));
		if (Lexer->CurToken != tok_id) break;
	}
	
	return block;
}

#endif
