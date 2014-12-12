#ifndef CPP_PARSER_FUNC
#define CPP_PARSER_FUNC 1

// BLOCK_DICTS ::= BLOCK_DICT | BLOCK_DICTS BLOCK_DICT
// BLOCK_DICT ::=  VAR_DICR | CONST_DICT | PROC_DICT

OperAST *parser::ParseBlockDicts(ExprAST *RetTy) {
	vector<OperAST*> Vars;
	vector<OperAST*> Consts;
	OperAST *ResultVar = 0;
	
	//VarDict = VD;
	//	
	/*if (!IsVoidTy(RetTy->GetTy())) {
		// New Var Result

		if (!RetTy->MemCpy) ResultVar = InsertVar("result", false, false, false, RetTy);
		else ResultVar = InsertVar("result", false, true, false, RetTy);

		Vars.push_back(ResultVar);
	}*/
	//
	//if (RetTy->MemCpy)
	//	ResultVar = InsertVar("result", false, true, false, RetTy);
	//
	while (1) {
		switch (Lexer->CurToken) {
			case tok_var : {
				OperAST* V = ParseVarBlock(false);
				Vars.push_back(V);
				
				continue;
			}
				
			case tok_const : {
				OperAST *C = ParseConstBlock();
				Consts.push_back(C);
				
				continue;
			}
		} // end switch
		
		break;
	}
	//
	return new BlockDictsOperAST(Vars, Consts, ResultVar);
}


// ARGS ::= ARGS | ARGS ',' ARGS
// ARG ::= DVAR | CONST DVAR | VAR DVAR | OUT DVAR

OperAST *parser::ParseArg(const bool istype) {
	vector<string> IdNames;
	int ty = 2;
	//
	bool Const = false;
	bool Var   = false;
	bool Out   = false;
	//
	switch (Lexer->CurToken) {
		case tok_const : Lexer->GetNextToken(); ty = 0; Const = true; break;
		case tok_var   : Lexer->GetNextToken(); ty = 1; Var   = true; break;
		case tok_out   : Lexer->GetNextToken(); ty = 2; Out   = true; break;
		case tok_id    : 			ty = 2; Out   = true; break;
		default        : return Error<OperAST*>(expected_expr);
	} // end switch
	
	//
	while (1) {
		if (Lexer->CurToken != tok_id)
			return Error<OperAST*>(expected_id);
	
		IdNames.push_back(Lexer->IdentifierStr);
		Lexer->GetNextToken();

		if (Lexer->CurToken == ':') break;
		if (Lexer->CurToken == ',') Lexer->GetNextToken();
		else return Error<OperAST*>(expected, ",");
	} // end arg
	//
	
	Lexer->GetNextToken();
	ExprAST *Ty = ParseType();
	//
	bool MemCpy = false;
	//
	if (IsClassTy(Ty->GetTy()))  Var = false;
	//
	if (OwnDataLayout) {
		if (IsStructTy (Ty->GetTy()) && !Var) { Var = true; ty = 1; MemCpy = true; }
		if (IsArrTy    (Ty->GetTy()) && !Var) { Var = true; ty = 1; MemCpy = true; }
		if (IsVecTy    (Ty->GetTy()) && !Var) { Var = true; ty = 1; MemCpy = true; }
	}
	
	//
	if (Lexer->CurToken == ',') {
		Lexer->GetNextToken();
		ExprAST *Align = ParseAlign();
		Ty->SetAlign(Align->GetAlign());
	}
	
	//
	ExprAST *Val = 0;
	//
	if (Lexer->CurToken == '=') {
		Lexer->GetNextToken();
		Val = ParseConstVal();
		//Val->SetTy(Ty);
	}
	
	//
	vector<struct Var> sArgs;
	vector<OperAST*> vArgs;
	
	//if (Var) Ty = new PtrTyExprAST(Ty);
	//if (!istype)
	for (int i = 0; i < IdNames.size(); i++) {
		ExprAST *E = new ArgExprAST(Ty, ty, IdNames[i]);
		//block->Append(new ExprOpAST(E));
		struct Var Arg;
		
		Arg.Name	= IdNames[i];
		Arg.Def		= true;
		Arg.Const	= Const;
		Arg.Var		= Var;
		Arg.Out		= Out;
		Arg.Global	= false;
		Arg.Type	= Ty->GetTy();
		Arg.TypeAST	= Ty;
		Arg.E		= E;
		Arg.DefaultVal	= Val;
		Arg.MemCpy	= MemCpy;
		//
		ExprAST *Ty1 = Ty;
		
		if (Var & !IsClassTy(Ty1->GetTy())) {
			Ty1 = new PtrTyExprAST(Ty1);
			Arg.Type = Ty1->GetTy();
		}
		
		if (!istype) {
			OperAST *V = InsertVar(IdNames[i], Const, Var, false, Ty1);
			V->Ty = Arg.Type;
			vArgs.push_back(V);
		} else {
			OperAST *V = new VarOperAST(IdNames[i], Ty1, false);
			V->Ty = Arg.Type;
			vArgs.push_back(V);
		}
		
		sArgs.push_back(Arg);
	}
	
	return new ArgsOperAST(sArgs, vArgs, Val);
}

OperAST *parser::ParseArgs(const bool istype) {
	vector<BlockOperAST*> Args;
	vector<struct Var> sArgs;
	vector<OperAST*> vArgs;
	bool HaveConst = false;
	
	if (Lexer->CurToken == ')') {
		Lexer->GetNextToken();
		goto Exit;
	}
	
	//
	if (Lexer->CurToken != tok_var &
	    Lexer->CurToken != tok_const &
	    Lexer->CurToken != tok_out &
	    Lexer->CurToken != tok_id) return 0;
	
	//
	while (1) {
		ArgsOperAST *Arg = (ArgsOperAST*)ParseArg(istype);

		if (Arg->Val == 0 && HaveConst) {
			string parametrs = "";

			for (int i = 0; i < Arg->Args.size(); i++) {
				if (i < Arg->Args.size()-1) parametrs += Arg->Args[i].Name+"', '";
				else parametrs += Arg->Args[i].Name;
			}

			return Error<OperAST*>(missing_default_arg, parametrs);
		}

		if (Arg->Val != 0) HaveConst = true;
		
		for (int i = 0; i < Arg->Args.size(); i++) {
			sArgs.push_back(Arg->Args[i]);
			vArgs.push_back(Arg->vArgs[i]);
		}
		//Args.push_back((BlockOperAST*)Arg);
		
		if (Lexer->CurToken == ';') {
			Lexer->GetNextToken();
			continue;
		}
		
		if (Lexer->CurToken == ')') {
			Lexer->GetNextToken();
				
			break;
		}
	} // end while
	
	//return Args;
Exit:
	return new ArgsOperAST(sArgs, vArgs, 0);
}

// DIRECTIVES ::= | DIRECTIVES ';' | DIRECTIVES ';' DIRECTIVE
// DIRECTIVE ::=  LINKAGE | CCONV | ATTR

OperAST *parser::ParseDirective() {
	int    drty  = Lexer->CurToken;
	string dname = Lexer->IdentifierStr;
	
	Lexer->GetNextToken();
	
	//
	switch (drty) {
		case tok_cc : {
			//if (Lexer->CurToken != ';')
			//	return Error<OperAST*>(expected, ";");
			
			//Lexer->GetNextToken();
			return new DirectiveOperAST(dname);
		}
		
		//
		case tok_attr : {
			if (dname == "alignstack") {
				//Lexer->GetNextToken();
				
				if (Lexer->CurToken != '(')
					return Error<OperAST*>(expected, "(");
				
				Lexer->GetNextToken(); // Получаем '('
				ExprAST *Const = ParseConstVal();
				
				if (Lexer->CurToken != ')')
					return Error<OperAST*>(expected, ")");
				
				if (IsIntTy(Const->GetTy())) {
					dname += "("+IntToStr(Const->GetVal())+")";
				} else return Error<OperAST*>(type_must_be_s, "integer");
				
				Lexer->GetNextToken(); // Получаем ')'
			}

			//
			//if (Lexer->CurToken != ';')
			//	return Error<OperAST*>(expected, ";");
				
			//Lexer->GetNextToken();
			return new DirectiveOperAST(dname);
		}
		
		//
		case tok_linkage : {
			bool external = false;
			string lib = "";
			string ename = ProtNameReg;
			
			if (dname == "external") {
				external = true;
				//

				if (Lexer->CurToken == tok_string) {
					ename = Lexer->StringConst;
				} else Lexer->GetPrevToken();
				//
				Lexer->GetNextToken();
				//ParseString();
				
				/*if (Lexer->IdentifierStr == "name") {
					Lexer->GetNextToken();
					ename = Lexer->StringConst;
					Lexer->GetNextToken();
					//ParseString();
				}*/
			}
			
			//if (Lexer->CurToken != ';')
			//	return Error<OperAST*>(expected, ";");
				
			//Lexer->GetNextToken();
			return new DirectiveOperAST(dname, external, lib, ename);
			//return 0;
		}
	}
}

OperAST *parser::ParseDirectives() {
	BlockOperAST *cc   = new BlockOperAST();
	BlockOperAST *attr = new BlockOperAST();
	BlockOperAST *link = new BlockOperAST();
	bool overload, external, Virtual, Override;
	overload = true; external = false; Virtual = false;
	Override = false;
	bool swap = false;
	string lib, ename;
	//
	//
	if (Lexer->CurToken != tok_linkage &&
	    Lexer->CurToken != tok_cc &&
	    Lexer->CurToken != tok_attr) return new DirectivesOperAST(cc, attr, link); // Нет директив
	
	int DTy = 1;
	//
	while (1) {
		if (Lexer->CurToken == tok_cc)      DTy = 1;
		if (Lexer->CurToken == tok_attr)    DTy = 2;
		if (Lexer->CurToken == tok_linkage) DTy = 3;
		//
		OperAST *Attr;
		
		if ((Lexer->CurToken == tok_attr & Lexer->IdentifierStr == "overload") == 1) {
			//
			overload = true;
			Lexer->GetNextToken();
			
			//if (Lexer->CurToken != ';')
			//	return Error<OperAST*>(expected, ";");
			
			//Lexer->GetNextToken();
			Attr = 0;
			//
		} else if ((Lexer->CurToken == tok_attr & Lexer->IdentifierStr == "swap") == 1) {
			//
			swap = true;
			Lexer->GetNextToken();
			
			//if (Lexer->CurToken != ';')
			//	return Error<OperAST*>(expected, ";");
			
			//Lexer->GetNextToken();
			Attr = 0;
			//
		} else if ((Lexer->CurToken == tok_attr & Lexer->IdentifierStr == "virtual") == 1) {
			//
			Virtual = true;
			Lexer->GetNextToken();
			
			//if (Lexer->CurToken != ';')
			//	return Error<OperAST*>(expected, ";");
			
			//Lexer->GetNextToken();
			Attr = 0;
			//
		} else if ((Lexer->CurToken == tok_attr & Lexer->IdentifierStr == "override") == 1) {
			//
			Override = true;
			Lexer->GetNextToken();
			
			//if (Lexer->CurToken != ';')
			//	return Error<OperAST*>(expected, ";");
			
			//Lexer->GetNextToken();
			Attr = 0;
			//
		} else Attr = ParseDirective();
		
		if (Attr != 0) {
			if (((DirectiveOperAST*)Attr)->External) {
				external = true;
				lib   = ((DirectiveOperAST*)Attr)->Lib;
				ename = ((DirectiveOperAST*)Attr)->eName;
			}
		
			switch (DTy) {
				case 1 : cc  ->Append(Attr); break;
				case 2 : attr->Append(Attr); break;
				case 3 : link->Append(Attr); break;
			}
		}
		
		//
		if (Lexer->CurToken == tok_cc |
		    Lexer->CurToken == tok_linkage |
		    Lexer->CurToken == tok_attr) continue;
		
		break;
	}
	
	return new DirectivesOperAST(cc, attr, link, overload, Virtual, Override, external, lib, ename, swap);
}

// TYOP	     ::= BINARY | UNARY | ASSIGN | CAST
// PROTOTYPE ::= PROCEDURE ID ';' DIRECTIVES | 
//		 PROCEDURE ID '(' ARGS ')' ';' DIRECTIVES |
//		 FUNCTION ID ':' TYPE ';' DIRECTIVES |
//		 FUNCTION ID '(' ARGS ')' ':' TYPE ';' DIRECTIVES |
//		 CONSTRUCTOR ID '(' ARGS ')' ';' DIRECTIVES |
//		 DESTRUCTOR  ID '(' ARGS ')' ';' DIRECTIVES |
//		 OPERATOR TYOP OP '(' ARGS ')' ':' TYPE ';' DIRECTIVES

ExprAST *parser::ParsePrototype (const bool proc, bool oper, const bool istype, const bool inclass) {
	DirectivesOperAST *Directives;
	DirectivesOperAST *DCC;
	ArgsOperAST *Args = new ArgsOperAST();
	ExprAST *RetType, *SRet;
	bool Aligned     = false;
	bool Binary      = false;
	bool Assign	 = false;
	bool Cast	 = false;
	bool Constructor = false;
	bool Destructor  = false;
	int Oper;
	string ProtName;
	
	//
	//if (Lexer->CurToken == tok_constructor) Constructor = true;
	//if (Lexer->CurToken == tok_destructor)  Destructor  = true;
	//
	//if (proc/* & !Constructor*/) RetType = new VoidTyExprAST();
	//
	Lexer->GetNextToken(); // получаем : function | procedure | operator | constructor | destructor

	if (Lexer->CurToken == tok_binary || Lexer->CurToken == tok_unary)
		oper = true;

	if (Lexer->CurToken == tok_cast)
		oper = true;

	if (Lexer->CurToken == tok_id) {
		if (Lexer->IdentifierStr == "assign" || Lexer->IdentifierStr == "cast")
			oper = true;
	}
	//
	if (!istype) {
		DCC = (DirectivesOperAST*)ParseDirectives();
	
		if (oper) DCC->Overload = true;
		//
	} else Lexer->GetPrevToken();
	//
	RetType = new VoidTyExprAST();
	//
	if (!oper) {
		if (!istype) {
			if (Lexer->CurToken != tok_id)
				return Error<ExprAST*>(expected_id);
			
			ProtName    = Lexer->IdentifierStr;
			ProtNameReg = Lexer->IdentifierStrReg;
		}
	} else {
		//
		if (Lexer->CurToken == tok_id && Lexer->IdentifierStr == "assign") {
			Assign = true;
			Binary = false;
			Oper = tok_assign;
			ProtName = "assign";

			Lexer->GetNextToken();

			if (Lexer->CurToken == '[') {
				Lexer->GetNextToken();

				if (Lexer->CurToken != ']')
					return Error<ExprAST*>(expected, "]");

				Oper = '['; // idx operator
			} else Lexer->GetPrevToken();
			//
		} else if (Lexer->CurToken == tok_id && Lexer->IdentifierStr == "cast") {
			Cast = true;
			Binary = false;
			Oper = tok_cast;
			ProtName = "cast";
			//
		} else {
			//
			if ((Lexer->CurToken != tok_binary & Lexer->CurToken != tok_unary &
		     	     Lexer->CurToken != '[' & Lexer->CurToken != ']'))
				return Error<ExprAST*>(expected_ss, "binary", "unary");
		
			if (Lexer->CurToken == tok_binary) Binary = true;
			//
			ProtName = Lexer->IdentifierStr+".";
			Lexer->GetNextToken(); // получаем токен
			Oper = Lexer->CurToken;
		}
		
		//
		if ((Binary || Oper == ']') && !Assign && !Cast) {
			switch (Oper) {
				// Compare
				case tok_le	: ProtName += "le"; break;
				case tok_ge	: ProtName += "ge"; break;
				case tok_ne	: ProtName += "ne"; break;
				case '>'	: ProtName += "gt"; break;
				case '<'	: ProtName += "lt"; break;
				case '='	: ProtName += "eq"; break;
				case tok_in	: ProtName += "in"; break;
				
				// Арифетические операторы
				case '+'	: ProtName += "add";  break;
				case '-'	: ProtName += "sub";  break;
				case '*'	: ProtName += "mul";  break;
				case '/'	: ProtName += "div";  break;
				case tok_div	: ProtName += "sdiv"; break;
				case tok_mod	: ProtName += "mod";  break;
				case tok_and	: ProtName += "and";  break;
				case tok_shl	: ProtName += "shl";  break;
				case tok_shr	: ProtName += "shr";  break;
				case ']'	: ProtName += "idx";  Binary = true; break;
				
				//
				default	: return Error<ExprAST*>(expected_oper);
			}
		} else if (!Binary && !Assign && !Cast) {
			switch (Oper) {
				// Арифетические операторы
				case '-'	: ProtName += "neg";  break;
				case tok_not	: ProtName += "not";  break;
				case '@'	: ProtName += "addr"; break;
				//
				default	: return Error<ExprAST*>(expected_oper);
			}
		} else if (Assign) {
			switch (Oper) {
				// Арифетические операторы
				case tok_assign	: break;
				case '['	: ProtName += ".idx";  Binary = true; break;
				//
				default	: return Error<ExprAST*>(expected_oper);
			}
		}
	}
	//
	if (!istype) {
		Lexer->GetNextToken(); // получаем id
		//
		if (Lexer->CurToken == '.') {
			if (!(*TypeDict)[ProtName].Def) 
				return Error<ExprAST*>(undecl_id, ProtName);
			//
			CurClassTy = (ClassTyExprAST*)((*TypeDict)[ProtName].Type);
			CurClassIR = (*TypeDict)[ProtName].TypeIR;
			//
			if (!IsClassTy((*TypeDict)[ProtName].Type->GetTy()))
				return Error<ExprAST*>(type_must_be_s, "class");
			
			Lexer->GetNextToken(); // получаем '.'
			Lexer->GetNextToken(); // получаем имя
			
			ClassTyExprAST* S = (ClassTyExprAST*)((*TypeDict)[ProtName].Type->GetTy().E);
			
			//* FIXME *//
			if (!S->mMethods[Lexer->IdentifierStr].Declare)
				return Error<ExprAST*>(undecl_id, Lexer->IdentifierStr);
			
			LocalFuncs = &S->mMethods;
			FuncDict   = &S->mMethods;
			S->mMethods[Lexer->IdentifierStr].ClassName = ProtName;
			ProtName = Lexer->IdentifierStr;
			InClassMethod = true;
			//
			if (ProtName == "create")  Constructor = true;
			if (ProtName == "destroy") Destructor  = true;
		}
	}

	if (inclass) {
		if (ProtName == "create")  Constructor = true;
		if (ProtName == "destroy") Destructor  = true;
	}

	//
	if (Destructor & CurClassTy == 0)
		return Error<ExprAST*>(const_and_destr_must_be_methods);
	
	//
	if (Lexer->CurToken == ':') { // Нет аргументов
		Lexer->GetNextToken(); // получаем ':'
		
		/*if (proc)
			return Error<ExprAST*>(expected_ss, ";", "(");*/
		
		RetType = ParseType();
		
		if (Lexer->CurToken == ',') {
			Lexer->GetNextToken();
			ExprAST *Align = ParseAlign();
			RetType->SetAlign(Align->GetAlign());
			Aligned = true;
		}
		
		//if (Lexer->CurToken != ';')
		//	return Error<ExprAST*>(expected, ";");
		
		//Lexer->GetNextToken();
	} else if (Lexer->CurToken == ';') { // Нет аргументов
		Lexer->GetNextToken(); // получаем ';'
		
		/*if (!proc)
			return Error<ExprAST*>(expected_ss, ":", "("); */
		
		//Directives = (DirectivesOperAST*)ParseDirectives();
	} else if (Lexer->CurToken == '(') {
		Lexer->GetNextToken(); // получаем '('

		if ((*FuncDict)[ProtName].Declare) {
			Args = (ArgsOperAST*)ParseArgs(istype);
		} else Args = (ArgsOperAST*)ParseArgs(istype);
		//
		if (/*proc ||*/ Assign) {
			//if (Lexer->CurToken != ';')
			//	return Error<ExprAST*>(expected, ";");
		} else if (Lexer->CurToken == ':') {
			/*if (Lexer->CurToken != ':') {
				return Error<ExprAST*>(expected, ":");
			}*/
			
			Lexer->GetNextToken(); // получаем ':'
			RetType = ParseType();
			
			if (Lexer->CurToken == ',') {
				Lexer->GetNextToken();
				ExprAST *Align = ParseAlign();
				RetType->SetAlign(Align->GetAlign());
				Aligned = true;
			}
			
			//if (Lexer->CurToken != ';')
			//	return Error<ExprAST*>(expected, ";");
		}
		
		//Lexer->GetNextToken();
		//Directives = (DirectivesOperAST*)ParseDirectives();
	} else return Error<ExprAST*>(expected_ss, "(");
	
	//
	if (!istype) {
		Directives = (DirectivesOperAST*)ParseDirectives();
		if (oper) Directives->Overload = true;
	} else Lexer->GetPrevToken();
	//
	Directives->CC = DCC->CC;
	//bool withdef = Lexer->CurToken == tok_var | Lexer->CurToken == tok_const | Lexer->CurToken == tok_begin;
	bool withdef = Lexer->CurToken != ';' || !inclass;
	//
	if ((!(*FuncDict)[ProtName].Declare | Directives->Overload | (*FuncDict)[ProtName].Overload) & Constructor & !withdef & !istype) {
		//
		if (CurClassTy == 0) {
			return Error<ExprAST*>(const_and_destr_must_be_methods);
		} else RetType = CurClassTy;
	}

	//
	CurRetTy = RetType;
	if (OwnDataLayout) {
		if (IsStructTy (RetType->GetTy())) { RetType = new PtrTyExprAST(RetType); RetType->MemCpy = true; }
		if (IsArrTy    (RetType->GetTy())) { RetType = new PtrTyExprAST(RetType); RetType->MemCpy = true; }
		if (IsVecTy    (RetType->GetTy())) { RetType = new PtrTyExprAST(RetType); RetType->MemCpy = true; }
	}

	//
	SRet = RetType;
	//
	if (RetType->MemCpy) {
		Args->SRet = RetType;
		RetType = new VoidTyExprAST();
		RetType->MemCpy = true;
	}

	PrototypeExprAST *Prot = new PrototypeExprAST(ProtName, Args, Directives, RetType, Aligned, istype);
	Prot->SRet = SRet;

	//
	if (Directives->Swap && (!oper || !Binary))
		return Error<ExprAST*>(directive_for_oper_s, "swap");

	if (istype) goto Exit;
	if (oper) {
		//
		// NOTE : '[' - idx operator for assign
		//        ']' - idx operator for expr
		//
		if (Binary && Oper != '[' && Args->Args.size() != 2) {
			return Error<ExprAST*>(size_argument_list_s, "2");
		}
		//
		if (!Binary && !Assign && Args->Args.size() != 1) {
			return Error<ExprAST*>(size_argument_list_s, "1");
		}
		//
		if (Oper == '[' && Args->Args.size() != 3) {
			return Error<ExprAST*>(size_argument_list_s, "3");
		}

		//
		struct Oper Item;
		Item.op  = Oper;
		Item.binary = Binary;
		Item.Ty  = SRet->GetTy();
		Item.Prot = (PrototypeExprAST*)Prot;
		Item.LHS = Args->Args[0];
		Item.Swap = Directives->Swap;
		
		if (Binary || Assign) Item.RHS = Args->Args[1];
		if (Oper == '[') Item.IDX = Args->Args[2];
		
		OperOverloads.push_back(Item);
	}

	//
	//if ((!Implementation | !(*FuncDict)[Prot->Name].Declare) & !istype) {
	if (((!(*FuncDict)[ProtName].Declare | Directives->Overload | (*FuncDict)[ProtName].Overload) & (!withdef | !InClassMethod)) & !Directives->Override) {
		// Add to funcs table
		vector<OperAST*> vars;
		vector<OperAST*> consts;
		OperAST *resultVar = 0;
		//
		LIRInS = true;
		BaseIR *ArgsIR = Prot->Args->Codegen();
		
		// Fill func data
		struct overloads ovr;
		//
		ovr.External    = Prot->Directives->External;
		ovr.FuncAST     = new FuncOperAST(Prot, new BlockDictsOperAST(vars, consts, 0), 0, InClassMethod); // TODO
		
		//BaseIR *RetTyIR;
		
		//if (!Constructor) RetTyIR = Prot->RetType->Codegen();
		//else RetTyIR = new VoidTyIR();
		
		ovr.Prot        = new ProtoIR(RetType->Codegen(), Prot->Args->vArgsIR);
		ovr.Func        = Prot->GenCurFunc();
		ovr.ClassIR     = CurClassIR;
		ovr.Constructor = Constructor;
		ovr.Destructor  = Destructor;
		ovr.Virtual     = Directives->Virtual;
		ovr.ProtExpr    = Prot;
		//
		(*FuncDict)[ProtName].Virtual = Directives->Virtual;
		//
		Prot->Args->vArgsIR.clear();
		
		(*FuncDict)[ProtName].Overloads.push_back(ovr);
	
		//
		if ((*FuncDict)[ProtName].ClassName.length() == 0) {
			(*FuncDict)[ProtName].ClassName = CurClassName;
		}
		//
		if (ovr.Virtual) {
			//OperAST *E = InsertVar(ProtName+".virtual."+IntToStr((*FuncDict)[ProtName].Overloads.size()-1), false, false, false, new PtrTyExprAST(new IntTyExprAST(8)));
			//LastVarBlock->Append(E);
			OperAST *E = InsertVar(ProtName+".virtual."+IntToStr((*FuncDict)[ProtName].Overloads.size()-1), false, false, false, new PtrTyExprAST(new IntTyExprAST(8)));
			LastVarBlock->Append(E);
			//puts((ProtName+".virtual."+IntToStr((*FuncDict)[ProtName].Overloads.size()-1)).c_str());
		}
		//
	} else if (/*(*FuncDict)[ProtName].Def &*/ Directives->Override && (!withdef | !InClassMethod)) { // Override function
		    //( /*(*FuncDict)[ProtName].Virtual &*/ Directives->Override) {
		vector<OperAST*> vars;
		vector<OperAST*> consts;
		OperAST *resultVar = 0;
		//
		LIRInS = true;
		BaseIR *ArgsIR = Prot->Args->Codegen();
		
		// Fill func data
		int n = GetPrototypeId(ProtName, Args->Args, RetType);
		struct overloads ovr = (*FuncDict)[ProtName].Overloads[n];
		//
		ovr.old_ovr = new struct overloads;//&old_ovr;
		memcpy(ovr.old_ovr, &(*FuncDict)[ProtName].Overloads[n], sizeof(struct overloads));
		//
		ovr.External    = Prot->Directives->External;
		ovr.FuncAST     = new FuncOperAST(Prot, new BlockDictsOperAST(vars, consts, 0), 0, InClassMethod); // TODO
		//
		ovr.Prot        = new ProtoIR(RetType->Codegen(), Prot->Args->vArgsIR);
		(*FuncDict)[ProtName].ClassName = CurClassName; isFuncDef = true;
		ovr.Func        = Prot->GenCurFunc(); isFuncDef = false;
		ovr.ClassIR     = CurClassIR;
		ovr.Constructor = Constructor;
		ovr.Destructor  = Destructor;
		ovr.Virtual = true;
		ovr.ProtExpr    = Prot;
		//
		Prot->Args->vArgsIR.clear();
		(*FuncDict)[ProtName].Overloads[n] = ovr;
		(*FuncDict)[ProtName].Def = false;
		//
		//OperAST *E = InsertVar(ProtName+".virtual."+IntToStr((*FuncDict)[ProtName].Overloads.size()-1), false, false, false, new PtrTyExprAST(new IntTyExprAST(8)));
		//LastVarBlock->Append(E);
	} //else if ((*FuncDict)[ProtName].Virtual & !Directives->Override)
		//return Error<ExprAST*>(redefinition_id, "FIXME"); // FIXME
	//

	// 
	//if ((*FuncDict)[ProtName].Declare | (!(*FuncDict)[ProtName].Overload & !Directives->Overload & !withdef))
	//if ((*FuncDict)[ProtName].Declare &

	if ((((*FuncDict)[ProtName].Declare & !(*FuncDict)[ProtName].MayDeclare) &
	   (!(*FuncDict)[ProtName].Overload & !Directives->Overload & !Directives->Override & !withdef)) |
	   ((*FuncDict)[ProtName].Def & (!(*FuncDict)[ProtName].Overload & !Directives->Overload & !Directives->Override))) {
		//
		return Error<ExprAST*>(redefinition_id, ProtName);
	}
	//
	(*FuncDict)[ProtName].Declare  = true;
	(*FuncDict)[ProtName].Overload = true;
	//if (!(*FuncDict)[ProtName].Overload)
	//	(*FuncDict)[ProtName].Overload = Directives->Overload;

Exit:
	//
	LIRInS = false;
	return (ExprAST*)Prot;
}


// FUNC ::= PROTOTYPE BLOCK_DICT BEGIN OPS END ';'

OperAST *parser::ParseFunc(const bool proc, const bool oper, const bool classdef) {
	map<string, Var> *VD = new map<string, Var>(); // Локальные переменные функции
	VarStack.push_back(VD);
	VarDict = VD;
	FuncDict = &MainFuncDict;
	LocalFuncs = 0;
	//
	PrototypeExprAST  *Prot  = (PrototypeExprAST*)ParsePrototype(proc, oper);
	//BlockDictsOperAST *Dicts = (BlockDictsOperAST*)ParseBlockDicts(Prot->SRet);
	vector<OperAST*> Vars;
	vector<OperAST*> Consts;
	OperAST *ResultVar = 0;

	/*if (!IsVoidTy(Prot->RetType->GetTy())) {
		// New Var Result

		//if (!Prot->RetType->MemCpy) ResultVar = InsertVar("result", false, false, false, Prot->RetType);
		//else ResultVar = InsertVar("result", false, true, false, Prot->RetType);
		if (Prot->RetType->MemCpy) {
			ResultVar = InsertVar("result", false, true, false, Prot->RetType);
			Vars.push_back(ResultVar);
		}
	}*/

	BlockDictsOperAST *Dicts = new BlockDictsOperAST(Vars, Consts, ResultVar);
	//puts ("!!!");
	//
	if (oper) InOperator = true;
	//
	Dicts->VD = *VD;
	OperAST *Body = 0;
	//
	FuncOperAST *FuncAST = new FuncOperAST (Prot, Dicts, Body, InClassMethod); // TODO

	if (Lexer->CurToken == ';') {
		Lexer->GetNextToken();
		(*FuncDict)[Prot->Name].Def = true;
		return FuncAST;
	}
	//
	map<string, Proc> *FirstFD = FuncDict;
	//
	if (!Prot->Directives->External && Implement /*&& !Implementation*/) {
		LocalVars = &Dicts->VD;
		Body = ParseOpers();
	}
	//
	FuncAST->Ops = Body;
	//
	CurFunc = 0;
	FuncDict = FirstFD;
	if (Implement || Prot->Directives->External) (*FuncDict)[Prot->Name].Def = true;
	//(*FuncDict)[Prot->Name].Overloads[(*FuncDict)[Prot->Name].Overloads.size()-1].FuncAST = FuncAST;
	int n = GetPrototypeId(Prot->Name, Prot->Args->Args, Prot->RetType);
	CurOvrID = n;
	CurFuncName = Prot->Name;
	(*FuncDict)[Prot->Name].Overloads[n].FuncAST = FuncAST;
	//(*FuncDict)[Prot->Name].Overloads[(*FuncDict)[Prot->Name].Overloads.size()-1].Func    = Prot->GenCurFunc();;
	FuncDict = &MainFuncDict;
	LocalFuncs = 0;
	InClassMethod = false;
	InOperator = false;

	VarStack.pop_back();
	VarDict = &ExternVars;
	return FuncAST;
}

#endif
