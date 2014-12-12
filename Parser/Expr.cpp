#ifndef CPP_PARSER_EXPR
#define CPP_PARSER_EXPR 1

// Main
#include <string>
#include <map>

using namespace std;

//* EXPR

#define FLOAT_FTOI_MAGIC_NUM (float)(3<<21)
#define IT_FTOI_MAGIC_NUM 0x4ac00000

// Fast Float To Int
int64_t ff2i(float f) {
	f += FLOAT_FTOI_MAGIC_NUM;
	return (*((int64_t*)&f)-IT_FTOI_MAGIC_NUM) >> 1;
}

// Char
ExprAST *parser::ParseChar() {
	double val = Lexer->NumVal;
	Lexer->GetNextToken();
	return new IntExprAST(val, 8);
}

// Boolean
ExprAST *parser::ParseBool(bool val) {
	Lexer->GetNextToken();
	return new IntExprAST(val, 1, false);
}

// Number
ExprAST *parser::ParseNumber() {
	double val = Lexer->NumVal;
	int size = 32;
	
	//if (ExprTy.Ty != TNONE) size = ExprTy.Size;
		
	Lexer->GetNextToken();
	//
	if (val > 0x7fffffff) size = 64;
	
	int64_t ival = floor(val);
	
	if (IsIntTy(ExprTy)) return new IntExprAST(ival, size);
	else if (IsFloatTy(ExprTy)) return new FloatExprAST(val, size);
	
	if (val == ival) return new IntExprAST(ival, size);
	else return new FloatExprAST(val, size);
}

// String
ExprAST *parser::ParseString(bool SetConst) {
	//
	/*if (Lexer->StringConst.length() == 1) {
		Lexer->GetNextToken(); // ïîëó÷àåì ñèìâîë
		return new IntExprAST(Lexer->StringConst.c_str()[0], 8, true);
	}*/
	
	Lexer->GetNextToken(); // ïîëó÷àåì ñòðîêó
	return new StringExprAST(Lexer->StringConst, Lexer->GetStrLen(), SetConst);
}


// VECVAL ::= '<' VALS '>' | '<' VALS ',' ... '>' | '<' ... ',' VALS '>'
// VALS ::= EXPR | EXPR ',' VALS

ExprAST *parser::ParseVecVal() {
	vector<ExprAST*> Vals;
	Lexer->GetNextToken();
	int ConstN = 0;
	int ValN = 0;
	bool Left  = false;
	bool Right = false;
	bool Store = false;
	ArgsTyAST *ArgsE = 0;
	TypeAST Ty;
	//
	while (1) {
		ExprAST *E = ParseExpr();
		Ty = AssignId->GetTy();
		ExprTy = DerefTy(AssignId->GetTy());
		//
		if (dynamic_cast<ArgsTyAST*>(E)) {
			if (Store) {
				if (Left) return Error<ExprAST*>(expected_ss_in_vec_list, ",", ">");
				else return Error<ExprAST*>(expected_s_in_vec_list, ",");
			}
			
			if (Vals.size() == 0) Left = true;
			else Right = true;
			
			ArgsE = (ArgsTyAST*)E;
			Store = true;
		} else {
			E = ConvertType(E->GetTy(), ExprTy, E, true, AssignId);
			//
			if (E->IsConst()) constn++;
			else ValN++;
		
			if (!IsIntTy(E->GetTy()) && !IsFloatTy(E->GetTy()))
				Error<ExprAST*>(vec_ty_must_be_ty_int_or_fp);
		
			Vals.push_back(E);
		}
		
		ExprTy = Ty;
		
		if (Lexer->CurToken == '>') break;
		if (Lexer->CurToken != ',') 
			return Error<ExprAST*>(expected_ss_in_vec_list, ",", ">");
		
		Lexer->GetNextToken();
	}
	
	ConstN = constn;
	
	if (ArgsE != 0) {
		ValN = CountOfTy(ExprTy)-ConstN;
	}
	
	valinc = 1;
	Lexer->GetNextToken();
	
	ConstN = constn;
	constn = 0;

	if (ValN == 0) return new VecConstAST(Vals);
	else return new VecValAST(Vals, ConstN+ValN, Store, Left, AssignId);
}


// ARRVAL ::= '[' VALS ']' | '[' VALS ',' ... ']' | '[' ... ',' VALS ']'
// VALS ::= EXPR | EXPR ',' VALS

ExprAST *parser::ParseArrVal() {
	vector<ExprAST*> Vals;
	Lexer->GetNextToken();
	int ConstN = 0;
	int ValN = 0;
	bool Left  = false;
	bool Right = false;
	bool Store = false;
	ArgsTyAST *ArgsE = 0;
	int uni = 0;
	TypeAST Ty = AssignId->GetTy();
	TypeAST Ty1, Ty2;
	//ExprTy = AssignId->GetTy();
	
	//
	while (1) {
		Ty2 = ExprTy;
		ExprTy = DerefTy(ExprTy);
		Ty1 = ExprTy;

		unni.push_back(uni);
		ExprAST *E = ParseExpr();
		unni.pop_back();

		ExprTy = Ty1;
		
		if (!dynamic_cast<ArgsTyAST*>(E)) E = ConvertType(E->GetTy(), ExprTy, E);
		ExprTy = Ty2;
		//ExprTy = AssignId->GetTy();
		uni++;
		
		if (dynamic_cast<ArgsTyAST*>(E)) {
			if (Store) {
				if (Left) return Error<ExprAST*>(expected_ss_in_vec_list, ",", "]");
				else return Error<ExprAST*>(expected_s_in_vec_list, ",");
			}
			
			if (Vals.size() == 0) Left = true;
			else Right = true;
			
			ArgsE = (ArgsTyAST*)E;
			Store = true;
		} else {
			if (E->IsConst()) ConstN++;
			else ValN++;
		
			//if (ty == "") ty = E->GetTy();
			//else if (ty != E->GetTy()) return ExError(string("Type must be "+ty).c_str());
		
			Vals.push_back(E);
		}
		
		if (Lexer->CurToken == ']') break;
		if (Lexer->CurToken != ',')
			return Error<ExprAST*>(expected_ss_in_arr_list, ",", "]");
		
		Lexer->GetNextToken();
	}
	
	Lexer->GetNextToken();
	//ConstN = constn;
	//constn = 0;
	
	if (ArgsE != 0) {
		//ArgsE->N = 0;
		//ValN = StrToInt(GetBaseASTType(exprty))-ConstN;
		ValN = CountOfTy(AssignId->GetTy())-ConstN;
	}
	
	if (ValN == 0) return new ArrConstAST(Vals);
	else return new ArrValAST(Vals, ConstN+ValN, &unni, Store, Left, AssignId);
}


// PAREN ::= '(' EXPR ')'

ExprAST *parser::ParseParen() {
	Lexer->GetNextToken();  // Получаем '('
	ExprAST *V = ParseExpr();

	if (!V) return 0;
	if (Lexer->CurToken != ')')
		return Error<ExprAST*>(expected, ")");

	Lexer->GetNextToken(); // Получаем ')'
	V->SetConst(false);
	return V;
}


// IDX ::= '[' IDS ']'
// IDS ::= ID | IDS

ExprAST *parser::ParseIdx(ExprAST *E) {
	vector<ExprAST*> idx; // Список индексов
	
	while (1) {
		Lexer->GetNextToken(); // Получаем '['
		struct TypeAST T = ExprTy;
		ExprAST *Ty = new IntTyExprAST(32);
		ExprTy = Ty->GetTy();
		ExprAST *IDX = ParseExpr();
		
		if (idx.size() == 0) {
			ExprAST *IDXOVR = ParseBinaryOvrOpers(E, IDX, ']');

			if (IDXOVR != 0) {
				if (Lexer->CurToken == ']') {
					Lexer->GetNextToken();
					return IDXOVR;
				} else return Error<ExprAST*>(expected, "]");
			}
		}
		
		IDX = ConvertType(IDX->GetTy(), Ty->GetTy(), IDX);
		ExprTy = T;
		idx.push_back(IDX); // Ïîëó÷àåì èíäåêñ
		
		if (Lexer->CurToken != ']') {
			if (Lexer->CurToken == ',') {
				//Lexer->GetNextToken(); // ïîëó÷àåì ','
				continue;
			}
			
			return Error<ExprAST*>(expected_ss, "]", ",");
		}
		
		Lexer->GetNextToken(); // Ïîëó÷àåì ']'
		
		if (Lexer->CurToken == '[') continue;
		break;
	}
	
	struct TypeAST Ty = E->GetTy();
	
	for (int i = 0; i < idx.size(); i++) {
		if (Ty.E == 0)
			return Error<ExprAST*>(cant_deref_ty, E->GetTy().Name);
			
		Ty = DerefTy(Ty);
	}
	
	return new IdxExprAST(E, idx);
}

// PTR ::= PTRS
// PTRS ::= '^' | PTRS

ExprAST *parser::ParsePtr(ExprAST *E) {
	int n = 0;
	
	while (1) {
		n++;
		Lexer->GetNextToken();
		if (Lexer->CurToken != '^')
			break;
	}
	
	struct TypeAST Ty = E->GetTy();
	
	for (int i = 0; i < n; i++) {
		if (Ty.E == 0)
			return Error<ExprAST*>(cant_deref_ty, E->GetTy().Name);
			
		Ty = DerefTy(Ty);
	}

	return new PtrExprAST(E, n, OneDeref);
}

// PARAM ::= EXPR '.' SPARAM
// SPARAM ::= ID | CALL

ExprAST *parser::ParseParam(ExprAST *E, bool ShowErr, string IdName) {
	//
	if (IdName == "") {
		if (Lexer->CurToken != tok_id)
			return Error<ExprAST*>(expected_unqualified_id);
		//
		IdName = Lexer->IdentifierStr;
		Lexer->GetNextToken();
	}

	ExprAST *R = 0;
	//
	// TODO
	/*if (Lexer->CurToken == '^')
		E = ParsePtr(E);
		
	if (Lexer->CurToken == '[')
		E = ParseIdx(E);*/
	//
	if (IsStructTy(E->GetTy())) {
		StructTyExprAST *S = (StructTyExprAST*)E->GetTy().E;
		
		if (!S->mVars[IdName].Def) {
			if (!ShowErr) return 0;
			return Error<ExprAST*>(no_member_named_ss, IdName, E->GetTy().Name);
		}
		
		return new ParamExprAST(E, S->mVars[IdName].Offset, S->mVars[IdName].TypeAST, false, CurClassTyIR);
	}
	//
	if (IsClassTy(E->GetTy())) {
		ClassTyExprAST *S = (ClassTyExprAST*)E->GetTy().E;
		
		if (!S->mVars[IdName].Def) {
			if (S->mMethods[IdName].Declare) {
				//FIXME
				CurClassExpr = E;
				
				if (GlobalParam & !InClassMethod) {
					LIRInS = true;
					ThisClassIR = E->Codegen();
					ThisClassIR->SetTy(new PtrTyIR(ThisClassIR->GetTy()));
					LIRInS = false;
				}
				
				map<string, Proc> *FirstFD = FuncDict;
				FuncDict = &S->mMethods;
				
				bool noparen = false;
				
				if (Lexer->CurToken != '(') {
					noparen = true;

					if (AssignId != 0 && IsFuncTy(AssignId->GetTy()) && AssignId->GetTy().HaveClass) {
						//
						AssignId->Ty.Prot->HaveClass = true;
						AssignId->Ty.Prot->FuncClass = E;
						//
						int ai = GetPrototypeId(IdName, AssignId->GetTy().Prot->Args->Args, AssignId->GetTy().Prot->RetType);
						
						FuncDict = FirstFD;
						CurClassExpr = 0;

						return new FuncObjExprAST(IdName, &S->mMethods, S->mMethods[IdName].Overloads[ai].FuncAST->Prot->GetTy(), E, ai);
					}
					//
				}

				ExprAST *Call = ParseCall(IdName, noparen, 0, false);
				
				if (Call == 0) return 0;
				
				CurClassExpr = 0;
				FuncDict = FirstFD;
				//
				if (!GlobalParam | InClassMethod) {
					TypeAST Ty = Call->GetTy();
					Ty.NoThis = true;
					Call->SetTy(Ty);
				}
				//
				//
				return Call;
				//FIXME
			} else if (IdName == "assign") { // parse assign for object
				if (Lexer->CurToken != '(') 
					return Error<ExprAST*>(expected, "(");
				//
				Lexer->GetNextToken();
				ExprAST *Expr = ParseExpr();
				Expr = ConvertType(Expr->GetTy(), E->GetTy(), Expr, true, E);
				//
				if (Lexer->CurToken != ')') 
					return Error<ExprAST*>(expected, ")");
				
				Lexer->GetNextToken();
				//
				return new AssignObjExprAST(E, Expr);
			}
			
			if (!ShowErr) return 0;
			return Error<ExprAST*>(no_member_named_ss, IdName, E->GetTy().Name);
		}
		
		//
		ExprAST *R = new ParamExprAST(E, S->mVars[IdName].Offset, S->mVars[IdName].TypeAST, true, CurClassTyIR);
		//
		if (Lexer->CurToken == ';')
			if (IsFuncTy(R->GetTy()) && AssignId == 0)
				R = ParseCall(IdName, true, R);

		// ????????????????????????????????????????
		if (R->IsvVar()) {
			ExprTy = DerefTy(R->GetTy());
			R = new PtrExprAST(R, 1);
			//R->SetvVar(false);
		} else ExprTy = R->GetTy();
		//
		if (Lexer->CurToken == '(') {
			//
			if ((*TypeDict)[IdName].Def) R = ParseParenConvert(IdName);
			else if (!GetFuncDict(IdName)) R = ParseCall(IdName, false, R);
			else R = ParseCall(IdName);
			//return ParseCall(IdName);
		}
	
		return R;
	}
	
	return E;
}

// ASSIGN OVERLOAD

int parser::MayAssignOverload(ExprAST *Ty, ExprAST *Expr) {
	ExprAST *Ovr = ParseAssignOvrOpers (Ty, Expr, 0, tok_assign);
	
	if (Ovr != 0) {
		return ((CallOvrExprAST*)Ovr)->OverloadId;
	}

	return -1;
}

ExprAST *parser::TryAssignOverload(ExprAST *Id, ExprAST *Expr) {
	if (dynamic_cast<IdxExprAST*>(Id)) {
		ExprAST *OVR = ParseAssignOvrOpers (Id, Expr, ((IdxExprAST*)Id)->Idx[0], '[');
		
		if (OVR != 0) {
			ExprTy = CreateASTType(TNONE, "", 32);
			AssignId = 0;
			
			return OVR;
		}
	} else {
		ExprAST *OVR = ParseAssignOvrOpers (Id, Expr, 0, tok_assign);
		
		if (OVR != 0) {
			ExprTy = CreateASTType(TNONE, "", 32);
			AssignId = 0;

			return OVR;
		}
	}

	return 0;
}

// ASSIGN ::= EXPR ':=' EXPR

ExprAST *parser::ParseAssign(ExprAST *Id, bool cast) {
	Lexer->GetNextToken(); // Ïîëó÷àåì := ID
	AssignId = Id;
	
	if (Id->IsvVar()/* & !IsClassTy(Id->GetTy())*/) {
		ExprTy = DerefTy(Id->GetTy());
		Id = new PtrExprAST(Id, 1);
	} else ExprTy = Id->GetTy();
	
	ExprAST *Expr = ParseExpr();
	ExprTy = Id->GetTy();
	
	if (Id->IsConst() | Id->IsvConst())
		return Error<ExprAST*>(read_only_var_is_not_assignable);
	//
	//if (cast) {
		ExprAST *Ovr = TryAssignOverload(Id, Expr);
		if (Ovr != 0) return Ovr;
		//
		//if (Expr->MemCpy) {
			//Expr = new GetRetExprAST(Expr);
			//Expr->SetTy(DerefTy(Expr->GetTy()));
		//}
		//
		Expr = ConvertType(Expr->GetTy(), Id->GetTy(), Expr, true, Id);
	//}
	//
	ExprTy = CreateASTType(TNONE, "", 32);
	AssignId = 0;
	
	return new AssignExprAST(Id, Expr);
}

// CALL ::= ID | ID '(' ARGS ')' ';';
// ARGS ::= ARG | ARGS ',' ARG
// ARG  ::= EXPR

ExprAST *parser::ParseCall(string Name, bool noparen, ExprAST *E, bool GetFD) {
	bool fail = false;
	vector<ExprAST*> Args;
	vector<struct Var> fArgs;
	int ai = 0;
	int begin = 0;
	bool isVar = false;
	BaseIR *IR = 0;
	BaseIR *Prot = 0;
	ExprAST *ProtExpr = 0;
	struct TypeAST RetTy;
	bool cond = false;
	//
	bool Constructor = false;
	bool Destructor  = false;
	bool Override    = false;
	bool Virtual     = false;
	bool MemCpy	 = false;
	//
	int VirtFuncOffset = 0;
	//
	ExprAST *TyE  = 0;
	ExprAST *AssignE = 0;
	ExprAST *CurClassE = 0;
	BaseIR  *TyIR = 0;
	//
	if (noparen) goto Test;
	Lexer->GetNextToken(); // Ïîëó÷àåì '('
	//GetFuncDict(Name);
	//
	//if ((!(*FuncDict)[Name].Def & !(*FuncDict)[Name].Declare & E == 0)) {
	
	if (GetFD) cond = !GetFuncDict(Name) & E == 0;
	else cond = (!(*FuncDict)[Name].Def & !(*FuncDict)[Name].Declare & E == 0);
	
	if (cond) {
		return Error<ExprAST*>(undecl_id, Name);
	}
	
	if (E != 0) {
		if (IsFuncTy(E->GetTy())) {
			fArgs = E->GetTy().Prot->Args->Args;
		} else if (!IsVPtrTy(E->GetTy())) {
			return Error<ExprAST*>(type_must_be_ss, "function", "procedure");
		}
	}
	
	if (Lexer->CurToken != ')') {
		while (1) {
			if (ExprTy.Size == 0) ExprTy = CreateASTType(TNONE, "", 32);
			ExprAST *Arg = ParseExpr();
			
			if (!Arg) return 0;
			Args.push_back(Arg);

			if (Lexer->CurToken == ')') break;
			if (Lexer->CurToken != ',') {
				return Error<ExprAST*>(expected_ss_in_arg_list, ")", ",");
			}

			Lexer->GetNextToken();
		}
	}

Test:
	
	if (E != 0 && IsVPtrTy(E->GetTy())) {
		ExprAST *PTy;
		
		if (AssignId == 0) PTy = new PtrTyExprAST(new VoidTyExprAST());
		else PTy = AssignId;
		
		if (AssignId != 0) RetTy = AssignId->GetTy();
		ExprAST *Ty = new PtrTyExprAST(new ProtoTyExprAST(Args, PTy));
		E = new BitcastExprAST(E, Ty, Ty->GetTy());

		goto Exit;
	}
	
	if (Name == "write") {
		for (int i = 1; i < Args.size(); i++) {
			if (IsFloatTy(Args[i]->GetTy())) {
				Args[i] = ConvertType(Args[i]->GetTy(), CreateASTType(TFLOAT, "double", 64), Args[i]);
			}
		}
	}

	if (E != 0 && IsVPtrTy(E->GetTy())) {
		RetTy = E->GetTy();
		goto Exit;
	}
	
	if ((*FuncDict)[Name].inllvm) {
		goto Exit;
	}
	
	if (E == 0 & /*(*FuncDict)[Name].Overload*/ (*FuncDict)[Name].Overloads.size() > 1) {
	//{*FIXME*}//
		for (int i = 0; i < (*FuncDict)[Name].Overloads.size(); i++) {
			struct overloads ovr = (*FuncDict)[Name].Overloads[i];
			if (ovr.FuncAST->Prot->Args->Args.size() == Args.size()) {
				begin = i;
				break;
			}

		}
		//
		
		//
		fail = true;
		vector<bool> MemCpyArr;
		vector<bool> vVarArr;
		//
		for (int i = begin; i < (*FuncDict)[Name].Overloads.size(); i++) {
			struct overloads ovr = (*FuncDict)[Name].Overloads[i];
			fArgs = ovr.FuncAST->Prot->Args->Args;
			MemCpyArr.clear();
			vVarArr.clear();

			if (fArgs.size() != Args.size()) continue;
			bool a = false;
			
			for (int j = 0; j < fArgs.size(); j++) {
				//
				struct TypeAST Ty = Args[j]->GetTy();
				
				if (fArgs[j].Var) {
					ExprAST *TyE = new PtrTyExprAST(Ty.TypeExpr);
					Ty = TyE->GetTy();
				}
				//
				MemCpyArr.push_back(fArgs[j].MemCpy);
				vVarArr.push_back(fArgs[j].Var);
				//
				if (EqTy(Ty, fArgs[j].Type)) {
					a = true;
				} else { a = false; break; }
			}
			if (!a) continue;
			//
			/*if (AssignId != 0) {
				//
				TypeAST RTy = ovr.FuncAST->Prot->SRet->GetTy();
				
				if (ovr.FuncAST->Prot->SRet->MemCpy)
					RTy = DerefTy(RTy);

				//
				if (EqTy(RTy, AssignId->GetTy()) & a & !ovr.Constructor) {
					ai = i;
					fail = false;
					//
					for (int i = 0; i < MemCpyArr.size(); i++) {
						Args[i]->MemCpy = MemCpyArr[i];
						Args[i]->SetvVar(vVarArr[i]);
					}

					break;
				}
				//
			} else {
				if (a) {
					ai = i;
					fail = false;
					//
					for (int i = 0; i < MemCpyArr.size(); i++) {
						Args[i]->MemCpy = MemCpyArr[i];
						Args[i]->SetvVar(vVarArr[i]);
					}

					break;
				}
			}*/

			ai = i;
			fail = false;
			//
			for (int i = 0; i < MemCpyArr.size(); i++) {
				Args[i]->MemCpy = MemCpyArr[i];
				Args[i]->SetvVar(vVarArr[i]);
			}

			break;
		}
		//
		if (fail)
		for (int i = begin; i < (*FuncDict)[Name].Overloads.size(); i++) {
			struct overloads ovr = (*FuncDict)[Name].Overloads[i];
			fArgs = ovr.FuncAST->Prot->Args->Args;
			vector<ExprAST*> tArgs;
			vVarArr.clear();
			
			if (fArgs.size() != Args.size()) continue;
			bool a = false;
			
			for (int j = 0; j < fArgs.size(); j++) { 
				//
				struct TypeAST Ty = Args[j]->GetTy();
				
				if (fArgs[j].Var) {
					ExprAST *TyE = new PtrTyExprAST(Ty.TypeExpr);
					Ty = TyE->GetTy();
				}
				//
				ExprAST *cast = ConvertType(Ty, fArgs[j].Type, Args[j], false);
				cast->MemCpy = fArgs[j].MemCpy;
				//
				if (cast == Args[j])
					cast->SetTy(Ty);
				//
				vVarArr.push_back(fArgs[j].Var);
				//
				if (EqTy(cast->GetTy(), fArgs[j].Type)) {
					a = true;
				} else break;
				//
				tArgs.push_back(cast);
			}
			
			/*if (AssignId != 0) {
				//
				TypeAST RTy = ovr.FuncAST->Prot->SRet->GetTy();
				
				if (ovr.FuncAST->Prot->SRet->MemCpy)
					RTy = DerefTy(RTy);
				//
				if (EqTy(RTy, AssignId->GetTy()) & a & !ovr.Constructor) {
					ai = i;
					Args = tArgs;
					fail = false;

					for (int i = 0; i < MemCpyArr.size(); i++)
						Args[i]->SetvVar(vVarArr[i]);

					break;
				}
				//
			} else {
				if (a) {
					ai = i;
					Args = tArgs;
					fail = false;

					for (int i = 0; i < MemCpyArr.size(); i++)
						Args[i]->SetvVar(vVarArr[i]);

					break;
				}
			}*/

			if (a) {
				ai = i;
				Args = tArgs;
				fail = false;

				for (int i = 0; i < MemCpyArr.size(); i++)
					Args[i]->SetvVar(vVarArr[i]);

				break;
			}
			
			tArgs.clear();
		}
		//
		struct overloads ovr = (*FuncDict)[Name].Overloads[ai];
		fArgs = ovr.FuncAST->Prot->Args->Args;
	} else if (E == 0) {
		//
		ai = 0;
		struct overloads ovr = (*FuncDict)[Name].Overloads[0];
		fArgs = ovr.FuncAST->Prot->Args->Args;
		vector<ExprAST*> tArgs;

		if (Args.size() > fArgs.size())
			return Error<ExprAST*>(no_match_func_for_call_to_s, Name);
		
		//
		for (int j = 0; j < fArgs.size(); j++) {
			int n = j;
			//if (CurClassExpr != 0) n = j-1;
			//
			if (Args.size() <= n && fArgs[j].DefaultVal != 0) {
				Args.push_back(fArgs[j].DefaultVal);
			} else if (Args.size() <= n) return Error<ExprAST*>(no_match_func_for_call_to_s, Name);
			//

			struct TypeAST Ty = Args[j]->GetTy();
			//
			if (fArgs[j].Var) {
				ExprAST *TyE = new PtrTyExprAST(Ty.TypeExpr);
				Ty = TyE->GetTy();
				//
				Args[j]->SetvVar(true);
			}
			
			Args[j] = ConvertType(Ty, fArgs[j].Type, Args[j]);
			Args[j]->MemCpy = fArgs[j].MemCpy;
		}
	}
	
	if (E != 0) {
		fail = false;	
		RetTy = DerefTy(E->GetTy());
		
		if (fArgs.size() != Args.size())
			return Error<ExprAST*>(no_match_func_for_call_to_s, E->GetTy().Name);
		
		for (int i = 0; i < fArgs.size(); i++) {
			struct TypeAST Ty = Args[i]->GetTy();
			//
			if (fArgs[i].Var) {
				//Ty.Ty = TPTR; Ty.Size = 8;
				ExprAST *TyE = new PtrTyExprAST(Ty.TypeExpr);
				Ty = TyE->GetTy();
				//
				Args[i]->SetvVar(true);
			}
			
			Args[i] = ConvertType(Ty, fArgs[i].Type, Args[i]);
			Args[i]->MemCpy = fArgs[i].MemCpy;
			//Args[i] = ConvertType(Args[i]->GetTy(), fArgs[i].Type, Args[i]);
		}
	}

	if (fail) { // Íå íàøèëè íóæíîé ôóíêöèè
		return Error<ExprAST*>(no_match_func_for_call_to_s, Name);
	}
	
Exit:
	AssignE = AssignId;

	if (E == 0) {
		IR    = (*FuncDict)[Name].Overloads[ai].Func;
		Prot  = (*FuncDict)[Name].Overloads[ai].Prot;
		//
		Constructor = (*FuncDict)[Name].Overloads[ai].Constructor;
		Destructor  = (*FuncDict)[Name].Overloads[ai].Destructor;
		Virtual     = (*FuncDict)[Name].Overloads[ai].Virtual;
		//ProtExpr    = (*FuncDict)[Name].Overloads[ai].FuncAST->Prot;
		ProtExpr = (*FuncDict)[Name].Overloads[ai].ProtExpr;
		//(*FuncDict)[Name].Overloads[ai].FuncAST->Prot->GetTy();
		//
		MemCpy = (*FuncDict)[Name].Overloads[ai].FuncAST->Prot->RetType->MemCpy;
		if (!(*FuncDict)[Name].inllvm) RetTy = (*FuncDict)[Name].Overloads[ai].FuncAST->Prot->SRet->GetTy();

		if (MemCpy) RetTy = DerefTy(RetTy);
		//else RetTy = ExprTy; // FIXME!
		//
		if (Constructor & ClassTyD.Type != 0) {
			RetTy = ClassTyD.Type->GetTy();
			RetTy.Abstract = true;
			TyE = ClassTyD.Type;
			TyIR = ClassTyD.TypeIR;
			AssignE = AssignId;
			//RetIR = ClassTyD.TypeIR;
		}
		//
		CurClassE = CurClassExpr;
		//
		if (Virtual & CurClassExpr != 0) {
			//
			ClassTyExprAST* S = (ClassTyExprAST*)CurClassExpr->GetTy().E;
			VirtFuncOffset = S->mVars[Name+".virtual."+IntToStr(ai)].Offset;
			//
		} else if (Virtual & CurClassTy != 0) {
			//
			ClassTyExprAST* S = (ClassTyExprAST*)CurClassTy->GetTy().E;
			VirtFuncOffset = S->mVars[Name+".virtual."+IntToStr(ai)].Offset;
			CurClassE = new SelfExprAST(CurClassTy->GetTy());
		}
		//
	} else {
		TypeAST STy = E->GetTy();
		//
		if (IsFuncTy(STy) && STy.HaveClass) {
			PrototypeExprAST *Prot = STy.Prot;
			MemCpy = Prot->RetType->MemCpy;
			ExprAST *Arg = 0;

			if (Prot->HaveClass) {
				//
				if (dynamic_cast<SelfExprAST*>(Prot->FuncClass) && !InClassMethod) {
					Arg = new BitcastExprAST(CurClassDot, new PtrTyExprAST(new PointerTyExprAST()), CurClassDot->GetTy());
				} else {
					Arg = new BitcastExprAST(Prot->FuncClass, new PtrTyExprAST(new PointerTyExprAST()), Prot->FuncClass->GetTy());
				}
				//
			} else Arg = Prot->FuncClass;

			Args.push_back(Arg);
		}
		//
		if (IsFuncTy(STy)) {
			PrototypeExprAST *Prot = STy.Prot;
			ProtExpr = Prot;
			MemCpy   = Prot->RetType->MemCpy;
			RetTy    = Prot->SRet->GetTy();

			if (MemCpy) RetTy = DerefTy(RetTy);
		}
	}
	
	//
	if (CurClassExpr != 0) {
		ExprAST *Arg = new BitcastExprAST(CurClassExpr, new PtrTyExprAST(new PointerTyExprAST()), CurClassExpr->GetTy());
		Args.push_back(Arg);
	}
	
	// Ïîëó÷àåì ')'
	if (!noparen) Lexer->GetNextToken();
	
Ret:
	
	if (E == 0) return new CallExprAST(Name, IR, Prot, Args, RetTy, Constructor, Destructor, TyIR, TyE, AssignE, Virtual, VirtFuncOffset, CurClassE, ProtExpr, MemCpy);
	return new CallExprAST(Name, E, Prot, Args, RetTy, Constructor, Destructor, TyIR, TyE, AssignE, Virtual, VirtFuncOffset, CurClassE, ProtExpr, MemCpy);
}

//
ExprAST *parser::ParseParenConvert(string IdName) {
	Lexer->GetPrevToken();
	ExprAST *Ty = ParseType();
	ExprAST *E  = ParseParen();
	
	return ExplictConvertType(E->GetTy(), Ty->GetTy(), E, Ty);
	//return ExplictConvertType(E->GetTy(), (*TypeDict)[IdName].Type->GetTy(), E);
}

// ID

ExprAST *parser::ParseId() {
	//
	// Class | Struct Param
	if (WithParams.size() != 0) {
		ExprAST *PR = 0;
		int n = WithParams.size();
		
		while (PR == 0 && n != 0) {
			PR = ParseParam(WithParams[n-1], false);
			n--;
			
			if (PR != 0) return PR;
			else Lexer->GetPrevToken();
			//
		}
	}
	//	
	// Usually id
	string IdName = Lexer->IdentifierStr;
	Lexer->GetNextToken();
	ExprAST *R = 0;
	map<string, Var> *VD = VarDict;
	
	//
	if ((!GetVarDict(IdName) & Lexer->CurToken != '(') == 1) {
		//
		if (CurClassTy != 0) { // Class Param
			ClassTyExprAST *S = (ClassTyExprAST*)CurClassTy->GetTy().E;
		
			if (S->mVars[IdName].Def) {
				ExprAST *R = new ParamExprAST(0, S->mVars[IdName].Offset, S->mVars[IdName].TypeAST, false, CurClassTyIR);
				//
				if (Lexer->CurToken == ';')
					if (IsFuncTy(R->GetTy()) && AssignId == 0)
						R = ParseCall(IdName, true, R);
					
				// ????????????????????????????????????????
				if (R->IsvVar()) {
					ExprTy = DerefTy(R->GetTy());
					R = new PtrExprAST(R, 1);
				} else ExprTy = R->GetTy();
				//
				if (Lexer->CurToken == '(') {
					//
					if ((*TypeDict)[IdName].Def)   R = ParseParenConvert(IdName);
					else if (!GetFuncDict(IdName)) R = ParseCall(IdName, false, R);
					else                           R = ParseCall(IdName);
				}

				return R;
			}
			//
			if (S->mMethods[IdName].Declare) {
				if (Lexer->CurToken != '(') {
					//
					if (AssignId != 0 && IsFuncTy(AssignId->GetTy()) && AssignId->GetTy().HaveClass) {
						//
						AssignId->Ty.Prot->HaveClass = true;
						AssignId->Ty.Prot->FuncClass = new SelfExprAST(CurClassTy->GetTy());
						
						map<string, Proc> *FirstFD = FuncDict;
						FuncDict = &S->mMethods;

						int ai = GetPrototypeId(IdName, AssignId->GetTy().Prot->Args->Args, AssignId->GetTy().Prot->RetType);
						
						FuncDict = FirstFD;
						return new FuncObjExprAST(IdName, &S->mMethods, S->mMethods[IdName].Overloads[ai].FuncAST->Prot->GetTy(), S, ai);
					}
				}
			}
			//
		}
		//
		if (GetFuncDict(IdName)) { // Ïðîöåäóðà
			if ((IsFuncTy(ExprTy) | IsVPtrTy(ExprTy)) == 1) { // Функция для переменной
				int ai = GetPrototypeId(IdName, ExprTy.Prot->Args->Args, ExprTy.Prot->RetType);
				return new FuncExprAST(IdName, (*FuncDict)[IdName].Overloads[ai].FuncAST->Prot->GetTy(), ai);
			} else return ParseCall(IdName, true);
		}
		
		// From class type
		/*if ((*TypeDict)[IdName].Def) {
			if (AssignId != 0 && IsClassTy(AssignId->GetTy())) {
				Lexer->GetNextToken();
				ClassTyD = (*TypeDict)[IdName];
				ExprAST *Id = new BitcastExprAST(AssignId, new PtrTyIR((*TypeDict)[IdName].TypeIR), (*TypeDict)[IdName].Type->GetTy(), true);
				ExprAST *PR = ParseParam(Id);
				return PR;
			}
		}*/
		
		return Error<ExprAST*>(undecl_id, IdName);
	}
		//
	GetVarDict(IdName);
	R = new VarExprAST(IdName, (*VarDict)[IdName].Type, VarDict);
	//((VarExprAST*)R)->VD = *VarDict;
	R->SetvConst((*VarDict)[IdName].Const);
	R->SetvVar((*VarDict)[IdName].Var);
	//
	if (Lexer->CurToken == ';')
		if (IsFuncTy(R->GetTy()) && AssignId == 0)
			R = ParseCall(IdName, true, R);
	//
	if (R->IsvVar()) {
		ExprTy = DerefTy(R->GetTy());
		R = new PtrExprAST(R, 1);
		//R->SetvVar(false);
	} else ExprTy = R->GetTy();
	//
	if (Lexer->CurToken == '(') {
		//
		if ((*TypeDict)[IdName].Def) R = ParseParenConvert(IdName);
		else if (!GetFuncDict(IdName)) R = ParseCall(IdName, false, R);
		else R = ParseCall(IdName);
	}
	
	VarDict = VD;
	return R;
}

// SIZEOF ::= SIZEOF '(' TYPE ')'
ExprAST *parser::ParseSizeOf() {
	Lexer->GetNextToken(); // Ïîëó÷àåì SizeOf
	
	if (Lexer->CurToken != '(')
		return Error<ExprAST*>(expected, "(");
	
	Lexer->GetNextToken();
	ExprAST *Type = ParseType();
	
	if (Lexer->CurToken != ')')
		return Error<ExprAST*>(expected, ")");
	
	Lexer->GetNextToken();
	
	return new IntExprAST(SizeOfTy(Type->GetTy()) >> 3, 32);
}

// VAL ::= NUM | PAREN | ID | MY

ExprAST *parser::ParseVal() {
	//
	switch (Lexer->CurToken) {
		case tok_number	: return ParseNumber();
		//
		case tok_true	: return ParseBool(true);
		case tok_false	: return ParseBool(false);
		//
		case tok_string	: return ParseString();
		case tok_char   : return ParseChar();
		//
		case '('	: return ParseParen();
		case tok_id	: return ParseId();
		case '['	: return ParseArrVal();
		case '<'	: return ParseVecVal();
		case tok_sizeof : return ParseSizeOf();
		case tok_args	: Lexer->GetNextToken(); return new ArgsTyAST();
		case tok_cast	: return ParseCast();
		case tok_self   :
			//
			if (CurClassTy != 0) {
				Lexer->GetNextToken();
				return new SelfExprAST(CurClassTy->GetTy());
			} else return Error<ExprAST*>(invalid_use_s_in_non_member_func, "self");
			
			break;
		//
		case tok_inherited :
			//
			if (CurClassTy != 0) {
				Lexer->GetNextToken();
				return new InheritedExprAST(CurClassTy);
			} else return Error<ExprAST*>(invalid_use_s_in_non_member_func, "inherited");
			
			break;

		case tok_nil : {
			//
			ExprAST *TyE = new PtrTyExprAST(new IntTyExprAST(8));
			//TyE->Ty.Ty = TCPTR;
			Lexer->GetNextToken();
			return new NullExprAST(TyE);
			//
			break;
		}

		//
		default		:
			SetCurLine();
			return Error<ExprAST*>(expected_expr);
	}
}

// ASCAST ::= VAL AS TYPE
ExprAST *parser::ParseAsCast (ExprAST *Val) {
	//
	Lexer->GetNextToken();
	ExprAST *Ty = ParseType();
	//
	if (IsClassTy(Ty->GetTy())) {
		//
		if (AssignId == 0) Val = new BitcastExprAST(Val, new PtrTyExprAST(new PtrTyExprAST(Ty)), Ty->GetTy(), true);
		else Val = new BitcastExprAST(Val, new PtrTyExprAST(Ty), Ty->GetTy(), true);
		//
	} else if (IsPtrTy(Ty->GetTy())) {
		Val = new BitcastExprAST(Val, Ty, Ty->GetTy(), true);
		//
	} else if (IsIntTy(Ty->GetTy()) || IsFloatTy(Ty->GetTy())) {
		//
		if (IsVecTy(Val->GetTy())) {
			//
			if (SizeOfTy(Val->GetTy()) != SizeOfTy(Ty->GetTy()))
				return Error<ExprAST*>(can_not_convert_to_type_ss, Val->GetTy().Name, Ty->GetTy().Name);

			Val = new BitcastExprAST(Val, Ty, Ty->GetTy(), true);
		} else return Error<ExprAST*>(type_must_be_s, "vector");
		//
	} else if (IsVecTy(Ty->GetTy())) {
		//
		if (IsIntTy(Val->GetTy()) || IsFloatTy(Val->GetTy())) {
			//
			if (SizeOfTy(Val->GetTy()) != SizeOfTy(Ty->GetTy()))
				return Error<ExprAST*>(can_not_convert_to_type_ss, Val->GetTy().Name, Ty->GetTy().Name);

			Val = new BitcastExprAST(Val, Ty, Ty->GetTy(), true);
		} else return Error<ExprAST*>(type_must_be_s, "vector");
		//
	} else return Error<ExprAST*>(type_must_be_ss, "pointer", "class");
	//
	return Val;
}

// UNARY ::= not VAL | '-' VAL | '@' VAL | VAL PTR | VAL IDXS | VAL

ExprAST	*parser::ParseUnary() {
	char op = Lexer->CurToken;
	//
	if (op != tok_not & op != '-' & op != '@') {
		ExprAST *Val = ParseVal();
		//
		if (Lexer->CurToken == '^')
			Val = ParsePtr(Val);
		
		if (Lexer->CurToken == '[')
			Val = ParseIdx(Val);
		
		GlobalParam = Val->GetTy().Global;
		
		if (IsClassTy(Val->GetTy())) {
			CurClassDot = Val;
		}

		//		
		while (Lexer->CurToken == '.') {
			Lexer->GetNextToken(); // Ïîëó÷àåì '.'
			Val = ParseParam(Val);
			
			if (Lexer->CurToken == '^')
				Val = ParsePtr(Val);
		
			if (Lexer->CurToken == '[')
				Val = ParseIdx(Val);
		}

		//
		if (Lexer->CurToken == tok_as)
			Val = ParseAsCast (Val);
		
		//
		if (Lexer->CurToken == tok_is) {
			//
			Lexer->GetNextToken();			
			ExprAST *Ty = ParseType();
			
			if (IsPtrTy(Ty->GetTy()))
				Val = new BitcastExprAST(Val, Ty, Ty->GetTy(), true);
			else if (IsClassTy(Ty->GetTy())) {
				if (AssignId == 0) Val = new BitcastExprAST(Val, new PtrTyExprAST(new PtrTyExprAST(Ty)), Ty->GetTy(), true);
				else Val = new BitcastExprAST(Val, new PtrTyExprAST(Ty), Ty->GetTy(), true);
			}

			else return Error<ExprAST*>(type_must_be_ss, "pointer", "class");
			//
			Val = new NotNullExprAST (Val);
		}
		//
		
		return Val;
	}
	
	//
	
	Lexer->GetNextToken();
	ExprAST *Val = ParseUnary();

	Val = ConvertType(Val->GetTy(), ExprTy, Val, false);

	ExprAST *OVR = ParseUnaryOvrOpers (Val, op);
	if (OVR != 0) return OVR;
	
	return new UnaryExprAST(op, Val, ExprTy);
}

// BINARTOVR
//
ExprAST *parser::ParseBinaryOvrOpers (ExprAST *LHS, ExprAST *RHS, int BinOp, bool Swap) {
	//
	if (InOperator) return 0;
	if (AssignId == 0) {
		ExprAST *TyE = new IntTyExprAST(1);
		ExprTy = TyE->GetTy();
	} else ExprTy = AssignId->GetTy();
	
	// 
	for (int i = 0; i < OperOverloads.size(); i++) {
		struct Oper Item = OperOverloads[i];
		bool cond = false;
		
		//if (Swap && Item.Swap) cond = true;
		//if (!Swap) cond = true;

		if (Item.op == BinOp && Item.binary && (!Swap || (Swap && Item.Swap))) {
			//
			bool lvVar = LHS->IsvVar();
			bool rvVar = RHS->IsvVar();
			//
			struct TypeAST lTy = LHS->GetTy();
			struct TypeAST rTy = RHS->GetTy();
			//
			if (Item.LHS.Var) {;
				ExprAST *TyE = new PtrTyExprAST(lTy.TypeExpr);
				lTy = TyE->GetTy();
				//
				lvVar = true;
			}
			//
			if (Item.RHS.Var) {
				ExprAST *TyE = new PtrTyExprAST(rTy.TypeExpr);
				rTy = TyE->GetTy();
				//
				rvVar = true;
			}
			//
			LHS = ConvertType(lTy, Item.LHS.Type, LHS, false);
			RHS = ConvertType(rTy, Item.RHS.Type, RHS, false);
			//
			struct TypeAST RetTy = Item.Ty;
			bool MemCpy = Item.Prot->RetType->MemCpy;
			RetTy = Item.Prot->SRet->GetTy();

			if (MemCpy) RetTy = DerefTy(RetTy);
			//
			if (EqTy(RetTy, ExprTy) & EqTy(lTy, Item.LHS.Type) &
			    EqTy(rTy, Item.RHS.Type)) {
				//
				LHS->SetvVar(lvVar);
				RHS->SetvVar(rvVar);
				//
				LHS->MemCpy = Item.LHS.MemCpy;
				RHS->MemCpy = Item.RHS.MemCpy;
				//
				return new CallOvrExprAST(i, LHS, RHS, 0, RetTy);
			}
			//
		}
	}
	
	return 0;
}

// UNARYOVR
//
ExprAST *parser::ParseUnaryOvrOpers (ExprAST *Val, int Op) {
	//
	if (InOperator) return 0;
	if (AssignId == 0) {
		ExprAST *TyE = new IntTyExprAST(1);
		ExprTy = TyE->GetTy();
	} else ExprTy = AssignId->GetTy();
	//
	for (int i = 0; i < OperOverloads.size(); i++) {
		struct Oper Item = OperOverloads[i];
		if (Item.op == Op & !Item.binary) {
			//
			bool vVar = Val->IsvVar();
			//
			struct TypeAST Ty = Val->GetTy();
			//
			if (Item.LHS.Var) {;
				ExprAST *TyE = new PtrTyExprAST(Ty.TypeExpr);
				Ty = TyE->GetTy();
				//
				vVar = true;
			}
			//
			Val = ConvertType(Ty, Item.LHS.Type, Val, false);
			//
			//
			if (EqTy(Item.Ty, ExprTy) & EqTy(Ty, Item.LHS.Type)) {
				Val->SetvVar(vVar);
				Val->MemCpy = Item.LHS.MemCpy;
				//
				return new CallOvrExprAST(i, Val, 0, 0, ExprTy);
			}
			//
		}
	}
	//
	return 0;
}

// ASSIGNOVR
//
ExprAST	*parser::ParseAssignOvrOpers (ExprAST *Id, ExprAST *Expr, ExprAST *Idx, int Op) {
	//
	if (InOperator) return 0;
	ExprTy = Id->GetTy();
	
	// 
	for (int i = 0; i < OperOverloads.size(); i++) {
		struct Oper Item = OperOverloads[i];
		if (Item.op == Op) {
			//
			bool lvVar = Id->IsvVar();
			bool rvVar = Expr->IsvVar();
			//
			struct TypeAST lTy = Id->GetTy();
			struct TypeAST rTy = Expr->GetTy();
			//
			if (Item.LHS.Var) {;
				ExprAST *TyE = new PtrTyExprAST(lTy.TypeExpr);
				lTy = TyE->GetTy();
				//
				lvVar = true;
			}
			//
			if (Item.RHS.Var) {
				ExprAST *TyE = new PtrTyExprAST(rTy.TypeExpr);
				rTy = TyE->GetTy();
				//
				rvVar = true;
			}
			//
			Id   = ConvertType(lTy, Item.LHS.Type, Id  , false);
			Expr = ConvertType(rTy, Item.RHS.Type, Expr, false);
			//
			if (EqTy(rTy, Item.RHS.Type) && EqTy(lTy, Item.LHS.Type)) {
				Id->SetvVar(lvVar);
				Expr->SetvVar(rvVar);
				//
				Id  ->MemCpy = Item.LHS.MemCpy;
				Expr->MemCpy = Item.RHS.MemCpy;
				//
				return new CallOvrExprAST (i, Id, Expr, Idx, ExprTy);
			}
			//
		}
	}
	
	return 0;
}

//
ExprAST *parser::FoldingBinary(char BinOp, ExprAST *LHS, ExprAST *RHS) {
	//
	struct TypeAST lty = LHS->GetTy();
	struct TypeAST rty = RHS->GetTy();

	// Свертка строковых констант --->
	if (dynamic_cast<StringExprAST*>(LHS) && dynamic_cast<StringExprAST*>(RHS)) {
		//
		StringExprAST *S1 = (StringExprAST*)LHS;
		StringExprAST *S2 = (StringExprAST*)RHS;
		//
		return new StringExprAST(S1->Val+S2->Val, S1->StrLen+S2->StrLen);
	}
	//
	if (dynamic_cast<StringExprAST*>(LHS) && dynamic_cast<IntExprAST*>(RHS)) {
		//
		StringExprAST *S1 = (StringExprAST*)LHS;
		IntExprAST    *S2 =    (IntExprAST*)RHS;
		//
		return new StringExprAST(S1->Val+(char)S2->Val, S1->StrLen+1);
	}
	//
	if (dynamic_cast<StringExprAST*>(RHS) && dynamic_cast<IntExprAST*>(LHS)) {
		//
		StringExprAST *S1 = (StringExprAST*)RHS;
		IntExprAST    *S2 =    (IntExprAST*)LHS;
		//
		return new StringExprAST(S1->Val+(char)S2->Val, S1->StrLen+1);
	}
	// <---

	// Пытаемся перегрузить оператор
	ExprAST *OVR = ParseBinaryOvrOpers(LHS, RHS, BinOp);
	if (OVR == 0) OVR = ParseBinaryOvrOpers(RHS, LHS, BinOp, true);
	if (OVR != 0) { LHS = OVR; return LHS; }
	// 
	if ((IsPtrTy(lty) || IsPtrTy(rty) || IsArrTy(lty) || IsArrTy(rty)) && (IsIntTy(lty) || IsIntTy(rty))) {
		//
		if (IsRelop(BinOp)) {
			if (IsIntTy(lty) && IsPtrTy(rty)) LHS = ConvertType(lty, rty, LHS, true, LHS); else
			if (IsIntTy(rty) && IsPtrTy(lty)) RHS = ConvertType(rty, lty, RHS, true, RHS);
		}
	}
	else if (!EqTy(lty, rty)) {
		//
		if (IsIntTy(lty) && IsFloatTy(rty)) LHS = ConvertType(lty, rty, LHS); else
		if (IsIntTy(rty) && IsFloatTy(lty)) RHS = ConvertType(rty, lty, RHS); else
		if (SizeOfTy(lty) >  SizeOfTy(rty)) RHS = ConvertType(rty, lty, RHS); else
		if (SizeOfTy(rty) >  SizeOfTy(lty)) LHS = ConvertType(lty, rty, LHS);
		//
		if (IsArrTy(lty)) return Error<ExprAST*>(can_not_convert_to_type_ss, lty.Name, rty.Name);
		if (IsArrTy(rty)) return Error<ExprAST*>(can_not_convert_to_type_ss, rty.Name, lty.Name);
		//
		if (LHS == 0) return Error<ExprAST*>(invalid_op_ty_for_instr);;
		if (RHS == 0) return Error<ExprAST*>(invalid_op_ty_for_instr);;
	}
	
	// Пытаемся перегрузить оператор
	OVR = ParseBinaryOvrOpers(LHS, RHS, BinOp);
	if (OVR == 0) OVR = ParseBinaryOvrOpers(RHS, LHS, BinOp, true);
	if (OVR != 0) { LHS = OVR; return LHS; }
	//
	if (!IsIntTy(lty) && !IsFloatTy(lty) && !IsVecTy(lty) && !IsPtrTy(lty) && !IsClassTy(lty))
		return Error<ExprAST*>(invalid_op_ty_for_instr);
	//
	if (!IsIntTy(rty) && !IsFloatTy(rty) && !IsVecTy(rty) && !IsPtrTy(rty) && !IsClassTy(rty))
		return Error<ExprAST*>(invalid_op_ty_for_instr);
	// 
	if (IsPtrTy(lty) && IsPtrTy(rty) && BinOp != tok_eq && BinOp != tok_ne)
		return Error<ExprAST*>(invalid_op_ty_for_instr);

	//
	if ((IsClassTy(lty) || IsClassTy(rty)) && BinOp != tok_eq && BinOp != tok_ne)
		return Error<ExprAST*>(invalid_op_ty_for_instr);

	// Возвращаем бинарный оператор
	if (IsPtrTy(lty) || IsPtrTy(rty) || IsClassTy(lty) || IsClassTy(rty)) {
		return new PtrBinaryExprAST(BinOp, LHS, RHS);
	} else return new NumBinaryExprAST(BinOp, LHS, RHS);
	//
}

// BINARY ::= ('+' NUM)* >

ExprAST *parser::ParseBinary(int ExprPrec, ExprAST *LHS) {
	bool Relop = false;
	
	while (1) {
		//
		int TokPrec = GetTokPrecedence();
		
		//
		if (TokPrec < ExprPrec) {
			if (LHS == 0) return 0;	
			return LHS;
		}
			
		int BinOp = Lexer->CurToken;

		Lexer->GetNextToken();
		
		if (Lexer->CurToken == ';') {
			Lexer->GetPrevToken();
			return LHS;
		} else if (GetTokPrecedence() != -1 && Lexer->CurToken != '<') {
			Lexer->GetPrevToken();
			return LHS;
		}
		
		
		//
		ExprAST *RHS = ParseUnary();//ParseVal();
		//if (RHS == 0) return 0;
		if (RHS == 0) {
			return LHS;
		}
		
		//
		int NextPrec = GetTokPrecedence();
		if (TokPrec < NextPrec) {
			RHS = ParseBinary(TokPrec+1, RHS);
			if (RHS == 0) {
				//Lexer->GetPrevToken();
				return LHS;
			}
		}
		//		
		LHS = FoldingBinary(BinOp, LHS, RHS);
	} // end while
}

// Operator new

ExprAST *parser::ParseNew() {
	Lexer->GetNextToken();
	string TypeName = Lexer->IdentifierStr;
	//
	if ((*TypeDict)[TypeName].Def) {
		if (AssignId != 0) {

			if (IsClassTy(AssignId->GetTy())) {
				Lexer->GetNextToken();

				ClassTyD = (*TypeDict)[TypeName];
				ExprAST *Id = new BitcastExprAST(AssignId, new PtrTyIR((*TypeDict)[TypeName].TypeIR), (*TypeDict)[TypeName].Type->GetTy(), true);
				ExprAST *PR = ParseParam(Id, true, "create");

				return PR;
			} else {
				Lexer->GetNextToken();

				if (Lexer->CurToken != '(')
					return Error<ExprAST*>(expected, "(");

				Lexer->GetNextToken();
				vector<ExprAST*> Sizes;

				while (Lexer->CurToken != tok_eof) {
					ExprAST *Size = ParseExpr();
					Sizes.push_back(Size);

					if (Lexer->CurToken != ',' && Lexer->CurToken != ')')
						return Error<ExprAST*>(expected_ss, ",", ")");
					
					if (Lexer->CurToken == ')') {
						Lexer->GetNextToken();
						break;
					}

					Lexer->GetNextToken();
				}

				//Lexer->GetNextToken();
				return new NewExprAST((*TypeDict)[TypeName].Type, Sizes, AssignId);
			}
		}
	}
	//
}

// Operator Delete
ExprAST *parser::ParseDelete() {
	Lexer->GetNextToken(); // eat delete
	vector<ExprAST*> Ids;
	vector<ExprAST*> Calls;
	Ids.push_back(ParseExpr());
	//
	while (Lexer->CurToken == ',') {
		Lexer->GetNextToken();
		Ids.push_back(ParseExpr());
	}
	//
	for (int i = 0; i < Ids.size(); i++) {
		if (IsClassTy(Ids[i]->GetTy()))
			Calls.push_back(ParseParam(Ids[i], true, "destroy"));
	}

	return new DeleteExprAST (Ids, Calls);
}

ExprAST	*parser::ParseExpr() {
	unni.clear();
	//WithParams.clear();

	//*
	//ExprAST *Alloc = ParseVarIn();
	//return new IntExprAST(1,1);
	/*if (Alloc != 0)*/ //return Alloc;

	if (Lexer->CurToken == tok_new)    return ParseNew();
	if (Lexer->CurToken == tok_delete) return ParseDelete();
	//
	ExprAST *LHS = ParseUnary();
	if (LHS == 0) return 0;
	//*
	
	int op = Lexer->CurToken;
	//
	if (BinopPrecedence[op] == 2 || BinopPrecedence[op] == 3) {
		//
		// inc
		if (Lexer->CurToken == '+') {
			Lexer->GetNextToken();

			if (Lexer->CurToken == '+') {
				Lexer->GetNextToken();
				ExprAST *val = new IntExprAST(1, 32);
				val = ConvertType(MainTypeDict["int"].Type->GetTy(), LHS->GetTy(), val);

				return new AssignExprAST (LHS, new NumBinaryExprAST('+', LHS, val));
			}

			Lexer->GetPrevToken();
		}

		// dec
		if (Lexer->CurToken == '-') {
			Lexer->GetNextToken();

			if (Lexer->CurToken == '-') {
				Lexer->GetNextToken();
				ExprAST *val = new IntExprAST(1, 32);
				val = ConvertType(MainTypeDict["int"].Type->GetTy(), LHS->GetTy(), val);

				return new AssignExprAST (LHS, new NumBinaryExprAST('-', LHS, val));
			}

			Lexer->GetPrevToken();
		}
		//
		Lexer->GetNextToken();
		// 
		if (Lexer->CurToken == '=') {
			//
			//AssignExprAST *AssignEE = (AssignExprAST*)ParseAssign (LHS);
			//AssignEE->Expr = FoldingBinary (op, LHS, AssignEE->Expr);
			//
			Lexer->GetNextToken();
			AssignId = LHS;
			ExprTy = LHS->GetTy();
			OneDeref = false;
			ExprAST *RHS = ParseExpr();
			ExprAST *LI = FoldingBinary (op, LHS, RHS);
			//	
			ExprTy = CreateASTType(TNONE, "", 32);
			AssignId = 0;
			LI = ConvertType(LI->GetTy(), LHS->GetTy(), LI, true, LHS);
			//return AssignEE;
			return new AssignExprAST(LHS, LI);
			//
		} else Lexer->GetPrevToken();
	}

	// Assigned
	if (Lexer->CurToken == '=') {

		if (AssignId != 0)
			return Error<ExprAST*>(expected_expr);
		
		LHS->IsAssign = true; OneDeref = false;
		return ParseAssign(LHS);
	}
	
	return ParseBinary(0, LHS);
}
/*
ExprAST *parser::ParseAssign(ExprAST *Id, bool cast) {
	Lexer->GetNextToken(); // Ïîëó÷àåì := ID
	AssignId = Id;
	
	if (Id->IsvVar()) {
		ExprTy = DerefTy(Id->GetTy());
		Id = new PtrExprAST(Id, 1);
	} else ExprTy = Id->GetTy();
	
	ExprAST *Expr = ParseExpr();
	ExprTy = Id->GetTy();
	
	if (Id->IsConst() | Id->IsvConst())
		return Error<ExprAST*>(read_only_var_is_not_assignable);
	//
	//if (cast) {
		ExprAST *Ovr = TryAssignOverload(Id, Expr);
		if (Ovr != 0) return Ovr;
		//
		//if (Expr->MemCpy) {
			//Expr = new GetRetExprAST(Expr);
			//Expr->SetTy(DerefTy(Expr->GetTy()));
		//}
		//
		Expr = ConvertType(Expr->GetTy(), Id->GetTy(), Expr, true, Id);
	//}
	//
	ExprTy = CreateASTType(TNONE, "", 32);
	AssignId = 0;
	
	return new AssignExprAST(Id, Expr);
}*/

// CAST ::= CAST (TYPE) UNARY

ExprAST *parser::ParseCast() {
	Lexer->GetNextToken(); // eat cast

	if (Lexer->CurToken != '(')
		return Error<ExprAST*>(expected, "(");

	Lexer->GetNextToken(); // eat '('
	ExprAST *Ty = ParseType();

	if (Lexer->CurToken != ')')
		return Error<ExprAST*>(expected, ")");

	Lexer->GetNextToken(); // eat ')'
	ExprAST *E = ParseUnary();

	if (AssignId == 0) {
		TypeAST Type = E->GetTy();
		E = new LoadExprAST(E);
		E->Ty = Type;
		OneDeref = true;
	}

	return ExplictConvertType(E->GetTy(), Ty->GetTy(), E, Ty);
}

// WITH ::= WITH EXPR DO OPS

OperAST *parser::ParseWith() {
	//
	Lexer->GetNextToken(); 
	
	ExprAST *id = ParseExpr();
	
	if (!IsStructTy(id->GetTy()) & !IsClassTy(id->GetTy()))
		return Error<OperAST*>(type_must_be_ss, "record", "class");
		
	if (Lexer->CurToken != tok_do)
		return Error<OperAST*>(expected, "do");
	
	Lexer->GetNextToken();
	WithParams.push_back(id);
	OperAST *Body = ParseOper();
	WithParams.pop_back();
	
	return Body;
}

#endif
