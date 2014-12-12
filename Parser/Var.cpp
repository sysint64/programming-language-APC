#ifndef JOBELL_PARSER_VAR
#define JOBELL_PARSER_VAR 1

// Main
#include <string>
#include <map>

using namespace std;

//* Head
// ParseVar
OperAST *parser::ParseVar(const bool global, const bool isexpr, const bool forloop) {
	vector<string> IdNames;
	bool skip = false;
	bool noid = false;
	int	 ovrid = -1;
	vector <ExprAST*> InitVals;

	//if (isexpr) Lexer->GetNextToken();
	if (Lexer->CurToken == tok_id)
	while(1) {
		string idname = Lexer->IdentifierStr;
		
		if ((*VarDict)[idname].Def) {
			skip = true;
			Error<OperAST*>(redefinition_id, idname);
		}
		
		for (int i = 0; i < IdNames.size(); i++)
			if (idname == IdNames[i]) {
				skip = true;
				Error<OperAST*>(redefinition_id, idname);
			}
			
		if (!skip) IdNames.push_back(idname);
		else skip = false;
			
		Lexer->GetNextToken();
		
		if (Lexer->CurToken == ',') {
			Lexer->GetNextToken();
			
			if (Lexer->CurToken != tok_id) {
				Error<OperAST*>(expected_unqualified_id);
				break;
			}
				
		} else break;
	}
	
	//
	if (Lexer->CurToken != ':') {
		skip = true;
		IdNames.clear();
		
		if (Lexer->CurToken != ';' && Lexer->CurToken == tok_id)
			Error<OperAST*>(expected_ss_after_decl, ",", ":");
	} else Lexer->GetNextToken();
	
	ExprAST *Ty = 0;
	ExprAST *Align = 0;

	if (Lexer->CurToken == tok_id || Lexer->CurToken == '^' || Lexer->CurToken == '[' || Lexer->CurToken == '<') {
		Ty = ParseType();
		
		if (Lexer->CurToken == ',') {
			Lexer->GetNextToken();
			Align = ParseAlign();
			Ty->SetAlign(Align->GetAlign());
		}
	} else {
		skip = true;
		
		if (Lexer->CurToken == ';' || Lexer->CurToken != tok_id) {
			Warning(decl_does_not_declare);
		} else {
			Error<OperAST*>(expected_unqualified_id);
			Lexer->GetNextToken();
		}
	}
	
	if (!isexpr) {
		if (Lexer->CurToken != ';') {
			skip = true;
			IdNames.clear();
			
			if (Lexer->CurToken == ':' || Lexer->CurToken == ',') {
				Lexer->GetPrevToken();
				Error<OperAST*>(expected_s_after_decl, ";");
				
				return ParseVar(global);
			}
			
			Error<OperAST*>(expected_s_after_decl, ";");
		} else Lexer->GetNextToken();
	} else {
		//
		if (forloop) {
			/*if (Lexer->CurToken == '=') {
				Lexer->GetNextToken();
				ExprAST *Val = ParseUnary();
				Val = ConvertType(Val->GetTy(), Ty->GetTy(), Val, true);
				InitVals.push_back (Val);
			} else
				return Error<OperAST*>(expected_s_after_decl, "=");

			Lexer->GetNextToken();*/
		} else if (Lexer->CurToken == '=') {
			while (Lexer->CurToken != ';') {
				Lexer->GetNextToken();

				ExprAST *Val = ParseExpr();
				ovrid = MayAssignOverload(Ty, Val);
				
				if (ovrid == -1)
					Val = ConvertType(Val->GetTy(), Ty->GetTy(), Val, true);

				InitVals.push_back (Val);

				if (Lexer->CurToken != ',' && Lexer->CurToken != ';')
					return Error<OperAST*>(expected_ss, ",", ";");
			}

			//Lexer->GetNextToken();
		}
		//
		Lexer->GetNextToken();
	}
	//
	if (InitVals.size() != 0) {
		//
		if (InitVals.size() != IdNames.size() && InitVals.size() != 1) {
			if (InitVals.size() > IdNames.size())
				return Error<OperAST*> (expected_s_after_decl, ";");
			//
			if (InitVals.size() < IdNames.size())
				return Error<OperAST*> (expected, ",");
		}
	}
	//
	OperAST *E = new VarOperAST(IdNames, Ty, InitVals, VarDict, global, ovrid);
	
	if (!skip)
	for (int i = 0; i < IdNames.size(); ++i)
		InsertVar(IdNames[i], false, false, global, Ty);
	
	if (!skip) {
		if (Ty->GetTy().MustCast) MustCasts.push_back((VarOperAST*)E);
		return E;
	}
	else return 0;
}

OperAST *parser::InsertVar(string IdName, bool Const, bool Var, bool Global, struct ExprAST *Ty) {
	OperAST *E = new VarOperAST(IdName, Ty, VarDict, Global);
	//
	(*VarDict)[IdName].Stmt		= E;
	(*VarDict)[IdName].Def 		= true;
	(*VarDict)[IdName].Const	= Const;
	(*VarDict)[IdName].Var 		= Var;
	(*VarDict)[IdName].Global	= Global;
	(*VarDict)[IdName].Type		= Ty->GetTy();
	(*VarDict)[IdName].TypeAST	= Ty;
	(*VarDict)[IdName].Val		= 0;
	(*VarDict)[IdName].Visible	= CurVisibility;
	(*VarDict)[IdName].MemCpy	= false;
	//
	return E;
}

//
OperAST *parser::ParseVarBlock(const bool global) {
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
	
	while (1) {
		OperAST *V = ParseVar(global);
		
		if (!skip && V != 0) block->Append(V);
		if (Lexer->CurToken != tok_id) break;
	}
	
	return block;
}

#endif
