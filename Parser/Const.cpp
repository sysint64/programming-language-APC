#ifndef CPP_CONST
#define CPP_CONST 1

//
ExprAST *parser::ParseArrConstVal() {
	vector<ExprAST*> Consts;
	Lexer->GetNextToken();
	struct TypeAST ty = NullTy;
	
	//
	while (1) {
		ExprAST *E = ParseConstVal();
		
		if (ty.Name == "") ty = E->GetTy();
		else if (ty.Name != E->GetTy().Name) return Error<ExprAST*>(invalid_op_ty_for_instr, ty.Name);
		
		Consts.push_back(E);
		
		if (Lexer->CurToken == ']') break;
		if (Lexer->CurToken != ',') return Error<ExprAST*>(expected, ",");
		
		Lexer->GetNextToken();
	}
	
	Lexer->GetNextToken();
	return new ArrConstAST(Consts);
}
//
ExprAST *parser::ParseVecConstVal() {
	vector<ExprAST*> Consts;
	Lexer->GetNextToken();
	
	//
	while (1) {
		if (Lexer->CurToken == '<' || Lexer->CurToken == '[') {
			return Error<ExprAST*>(vec_ty_must_be_ty_int_or_fp);
		}
		
		ExprAST *E = ParseConstVal();
		Consts.push_back(E);
		
		if (Lexer->CurToken == '>') break;
		if (Lexer->CurToken != ',') {
			//OpError("Expected ',' after const");
			return Error<ExprAST*>(expected, ",");
		}
		
		Lexer->GetNextToken();
	}
	
	Lexer->GetNextToken();
	return new VecConstAST(Consts);
}
//
ExprAST *parser::ParseConstVal() {
	switch (Lexer->CurToken) {
		case tok_number	: return ParseNumber();
		case tok_true	: return ParseBool(true);
		case tok_false	: return ParseBool(false);
		case tok_string : return ParseString(true);
		case tok_char	: return ParseChar();
		case tok_nil	: {
			ExprAST *TyE = new PtrTyExprAST(new IntTyExprAST(8));
			Lexer->GetNextToken();
			return new NullExprAST(TyE);
		}
		
		case '+' : Lexer->GetNextToken(); return ParseConstVal(); break;
		case '-' : Lexer->GetNextToken(); return ParseConstVal(); break;
		
		case '[' : return ParseArrConstVal();
		case '<' : return ParseVecConstVal();
		
		case tok_id :
			ExprAST *E = ParseExpr();
			return new IntExprAST(0, 32);
			/*if (E->IsConst()) *///return E->GetVal();
			//else return Error<ExprAST*>();
			//if (IsIntTy(ExprTy)) return new IntExprAST(E->GetVal(), 32);
			//else if (IsFloatTy(ExprTy)) return new FloatExprAST(E->GetVal(), 32);

			break;
	}
	
	return 0;
}
//
OperAST *parser::ParseConstBlock(const bool global) {
	BlockOperAST *block = new BlockOperAST();
	Lexer->GetNextToken();
	
	//
	if (Lexer->CurToken != tok_id)
		return Error<OperAST*>(expected_id);
	
	//*
	string IdName, IdType;
	//
	while(1) {
		IdName = Lexer->IdentifierStr;
		
		if ((*VarDict)[IdName].Def) {
			Lexer->GetNextToken();
			return Error<OperAST*>(redefinition_id);
		}
		
		Lexer->GetNextToken();
		//
		if (Lexer->CurToken == ':') {
			Lexer->GetNextToken(); // получаем ':'
			ExprAST *Ty = ParseType();
			
			if (Lexer->CurToken == '=') {
				ExprTy = Ty->GetTy();
				Lexer->GetNextToken(); // получаем '='
				ExprAST *E = ParseConstVal();
				
				(*VarDict)[IdName].Def  = true;
				(*VarDict)[IdName].Type = E->GetTy();
				(*VarDict)[IdName].E    = E;
				
				block->Append(new ConstOperAST(IdName, E, ""));
			} else return Error<OperAST*>(expected, "=");
		} else if (Lexer->CurToken == '=') {
			Lexer->GetNextToken(); // получаем '='
			ExprTy = NullTy;
			ExprAST *E = ParseConstVal();
			
			(*VarDict)[IdName].Def = true;
			(*VarDict)[IdName].Type = E->GetTy();
			(*VarDict)[IdName].E   = E;
			
			block->Append(new ConstOperAST(IdName, E, ""));
		} else {
			return Error<OperAST*>(expected_ss, ":", "=");
		}
		
		ExprTy = NullTy;
		Lexer->GetNextToken();
		if (Lexer->CurToken != tok_id) break;
	}
	
	return block;
}

#endif
