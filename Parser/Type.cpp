#ifndef JOBELL_PARSER_TYPE
#define JOBELL_PARSER_TYPE 1

// Main
#include <string>
#include <map>

using namespace std;

//
OperAST *parser::ParseAlias() {
	Lexer->GetNextToken(); // eat alias
   
	if (Lexer->CurToken != tok_id)
		return Error<OperAST*>(expected_id);

	string AliasName = Lexer->IdentifierStr;

	Lexer->GetNextToken();

	if (Lexer->CurToken != '=')
		return Error<OperAST*>(expected, "=");

	Lexer->GetNextToken();
	ExprAST *Ty = ParseType();

	if (Lexer->CurToken != ';')
		return Error<OperAST*>(expected, ";");

	ExprAST *DTy = new HTypeExprAST(AliasName, Ty);
	//
	(*TypeDict)[AliasName].Def  = true;
	(*TypeDict)[AliasName].Type = DTy;
	
	return new ExprOpAST(DTy);
}

ExprAST *parser::ParseTemplateType(const string Ty) {
	ExprAST *Res;
	Lexer->GetNextToken();
	bool DefaultSettings = false;
	
	if (Lexer->CurToken != '(')
		DefaultSettings = true;
	//	return Error<ExprAST*>(expected, "(");
	
	// Std Types
	// Signed integer
	if (StrCompare(Ty, "Int")) {		
		if (!DefaultSettings) {
			Lexer->GetNextToken();
			ExprAST *V = ParseConstVal();
			Res = new IntTyExprAST(V->GetVal());
		} else Res = new IntTyExprAST(32);
	}
	
	// Unsigned integer
	if (StrCompare(Ty, "UInt")) {		
		if (!DefaultSettings) {
			Lexer->GetNextToken();
			ExprAST *V = ParseConstVal();
			Res = new IntTyExprAST(V->GetVal(), true);
		} else Res = new IntTyExprAST(32, true);
	}
	
	// String
	if (StrCompare(Ty, "String")) {
		if (!DefaultSettings) {
			Lexer->GetNextToken();
			ExprAST *V = ParseConstVal();
			Res = new StringTyExprAST(V->GetVal());
		} else Res = new StringTyExprAST(0);
	}
	
	//Lexer->GetNextToken();
	
	if (!DefaultSettings) {
	if (Lexer->CurToken != ')')
		return Error<ExprAST*>(expected, "(");
	} else Lexer->GetPrevToken();
		
	return Res;
}
//
ExprAST *parser::ParseStdTy() {
	ExprAST *E = 0;
	int ptr = 0;
	
	// 
	while (Lexer->CurToken == '^') {
		Lexer->GetNextToken();
		ptr++;
	}
	
	string IdType = Lexer->IdentifierStr;
	
	if (IdType == CurClassName) {
		ExprAST *Type = new AClassTyExprAST(CurClassName);
		struct TypeAST Ty = CurClassTy->GetTy();
		Ty.MustCast = true;
		Ty.E = CurClassTy;
		Type->SetTy(Ty);
		
		E = Type;

		Lexer->GetNextToken();
	} else {
		if (!(*TypeDict)[IdType].Def)
			return Error<ExprAST*>(undecl_typename, IdType);

		E = new HTyExprAST((*TypeDict)[IdType].TypeIR, (*TypeDict)[IdType].Type);
		Lexer->GetNextToken();
	}
	
	//
	if (E != 0)
	for (int i = 0; i < ptr; i++)
		E = new PtrTyExprAST(E);
	
	return E;
}

//
ExprAST *parser::ParseArrTy() {
	ExprAST *E;
	Lexer->GetNextToken();
	vector<int> sizes;
	bool darr = false;
	//
	while (1) {
		if (Lexer->CurToken != tok_id) {
			if (Lexer->CurToken == '[') {
				darr = true;
				goto Exit;
			}
			//!
			E = ParseConstVal();
			
			if (E == 0) {
				darr = true;
				goto Exit;
			}
		} else {			
			if ((*VarDict)[Lexer->IdentifierStr].Const)
				E = (*VarDict)[Lexer->IdentifierStr].E;
			else {
				//Lexer->GetNextToken();
				darr = true;
				goto Exit;
			}
			//else return ExError("Size must be const"); FIXME
			
			Lexer->GetNextToken();
		}
		
		//if (GetBaseNameASTType(E->GetTy()) != "INT") { FIXME
			//return Error<ExprAST*>(size_of_arr_has_non_int_ty_s, GetNameASTType(E->GetTy()));
		//}
		
		sizes.push_back(E->GetVal());
		
		if (Lexer->CurToken == tok_id) {
			if (!StrCompare(Lexer->IdentifierStr, "x"))
				return Error<ExprAST*>(expected, "x");
		} else return Error<ExprAST*>(expected_expr);
		//
		Lexer->GetNextToken();
		break;
	}
	
Exit:
	ExprAST *Ty = ParseType();
	
	if (Lexer->CurToken != ']')
		return Error<ExprAST*>(expected, "]");
	
	Lexer->GetNextToken();

	if (!darr) return new ArrTyExprAST(sizes, Ty);
	else return new PtrTyExprAST(Ty);
}

//
ExprAST *parser::ParseVecTy() {
	ExprAST *E;
	Lexer->GetNextToken();
	int size;
	//
	if (Lexer->CurToken != tok_id) {
		//!
		E = ParseConstVal();
	} else {			
		if ((*VarDict)[Lexer->IdentifierStr].Const)
			E = (*VarDict)[Lexer->IdentifierStr].E;
		//else return ExError("Size must be const");
		
		Lexer->GetNextToken();
	}
		//! FIXME
	//if (GetBaseNameASTType(E->GetTy()) != "INT") {
		//return Error<ExprAST*>(size_of_arr_has_non_int_ty_s, GetNameASTType(E->GetTy()));
	//}
		
	size = (int)E->GetVal();
		
	if (Lexer->CurToken == tok_id) {
		if (!StrCompare(Lexer->IdentifierStr, "x"))
			return Error<ExprAST*>(expected, "x");
	} else return Error<ExprAST*>(expected_expr);
	//
	Lexer->GetNextToken();
	ExprAST *Ty = ParseType();
	
	// ! FIXME
	//if (GetBaseNameASTType(Ty->GetTy()) != "INT" && GetBaseNameASTType(Ty->GetTy()) != "FLOAT")
	//	return ExError("Type must be numeric (int, float)");
	
	if (Lexer->CurToken != '>')
		return Error<ExprAST*>(expected, ">");
	
	Lexer->GetNextToken();
	return new VecTyExprAST(size, Ty);
}

//
ExprAST *parser::ParseFuncTy() {
	//
	bool proc	= false;
	bool Aligned    = false;
	bool ofclass	= false;
	DirectivesOperAST *Directives = 0;

	ArgsOperAST *Args = new ArgsOperAST();
	ExprAST *RetType, *SRet;
	string ProtName;
	//
	if (Lexer->CurToken == tok_proc) {
		//RetType = new VoidTyExprAST();
		proc = true;
	}
	RetType = new VoidTyExprAST();
	//
	Lexer->GetNextToken(); // получаем : function | procedure
	//
	if (Lexer->CurToken == ':') { // Нет аргументов
		Lexer->GetNextToken(); // получаем ':'
		
		//if (proc) return Error<ExprAST*>(expected_ss, ";", "("); 
		
		RetType = ParseType();
		
		if (Lexer->CurToken == ',') { // Parse alignment
			Lexer->GetNextToken();
			ExprAST *Align = ParseAlign();
			RetType->SetAlign(Align->GetAlign());
			Aligned = true;
		}
		
		//if (Lexer->CurToken != ';')
		//	return Error<ExprAST*>(expected, ";");
		
		Lexer->GetNextToken();
	} else if (Lexer->CurToken == ';' | Lexer->CurToken == tok_of) { // Нет аргументов
		Lexer->GetNextToken(); // получаем ';'
		
		/*if (!proc)
			return Error<ExprAST*>(expected_ss, ":", "(");*/
		
	} else if (Lexer->CurToken == '(') {
		Lexer->GetNextToken(); // получаем '('
		Args = (ArgsOperAST*)ParseArgs(true);
		//
		/*if (proc) {
			//if (Lexer->CurToken != ';')
			//	return Error<ExprAST*>(expected, ";");
		} else {*/
			/*if (Lexer->CurToken != ':')
				return Error<ExprAST*>(expected, ":");*/
			
		if (Lexer->CurToken == ':') {
			Lexer->GetNextToken(); // получаем ':'
			RetType = ParseType();
			
			if (Lexer->CurToken == ',') {
				Lexer->GetNextToken();
				ExprAST *Align = ParseAlign();
				RetType->SetAlign(Align->GetAlign());
				Aligned = true;
			}
		}
			
			//if (Lexer->CurToken != ';')
			//	return Error<ExprAST*>(expected, ";");
		//}
		
		Lexer->GetNextToken();
	} //else return Error<ExprAST*>(expected_ss, ";", "(");
	//
	Lexer->GetPrevToken();
	//
	if (Lexer->CurToken == tok_of) { // of class
		Lexer->GetNextToken();

		if (Lexer->CurToken != tok_class)
			return Error<ExprAST*>(expected, "class");

		Lexer->GetNextToken();
		ofclass = true;
	}

	Lexer->GetNextToken();
	//
	Directives = (DirectivesOperAST*)ParseDirectives();
	Lexer->GetPrevToken();
	//FuncTyExprAST *Ty = new FuncTyExprAST(Args, RetType, Aligned);

	//
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
	//

	PrototypeExprAST *Ty = new PrototypeExprAST("", Args, Directives, RetType, Aligned, true);
	Ty->SRet = SRet;

	Ty->OfClass = ofclass;
	if (ofclass) Ty->Ty.HaveClass = true;
	//
	return Ty;
}
//
ExprAST *parser::ParseType() {
	switch (Lexer->CurToken) {
		case tok_id	: return ParseStdTy();
		case '^'	: return ParseStdTy();
		case '['	: return ParseArrTy();
		case '<'	: return ParseVecTy();
		case tok_proc	: return ParseFuncTy();
		case tok_func	: return ParseFuncTy();
	}
}

bool Step2(const int n) {
	int a = 2;
	
	while (a < n) a *= 2;
	
	if (a == n) return true;
	else return false;
}

ExprAST *parser::ParseAlign() {
	if (Lexer->CurToken == tok_align) {
		Lexer->GetNextToken();
		
		if (Lexer->CurToken != '(')
			return Error<ExprAST*>(expected, "(");

		Lexer->GetNextToken();
		ExprAST *Align = ParseConstVal();
		int al = Align->GetVal();
		
		if (!Step2(al) && al != 1)
			return Error<ExprAST*>(align_is_not_pow_2);
		
		if (Lexer->CurToken != ')')
			return Error<ExprAST*>(expected, ")");
			
		Lexer->GetNextToken();
		
		return new AlignExprAST(al);
	}
}

#endif
