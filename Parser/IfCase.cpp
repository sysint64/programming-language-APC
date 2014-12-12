#ifndef CPP_PARSER_IFCASE
#define CPP_PARSER_IFCASE 1

// Main
#include <string>
#include <map>

using namespace std;

// COND ::= IF EXPR THEN OPS |
//          IF EXPR THEN OPS ELSE OPS
OperAST *parser::ParseIf() {
	// IF
	
	Lexer->GetNextToken(); // Получаем If
	//bintype = bin_bool;
	//ExprTy = BoolTy;
	ExprAST *TyE = new IntTyExprAST(1);
	ExprTy = TyE->GetTy();
	//AssignId = new IntTyExprAST(1);
	ExprAST *Cond = ParseExpr();
	
	if (!Cond) return 0;
	
	struct TypeAST Ty = Cond->GetTy();
	Cond = ConvertType(Ty, CreateBoolTy(), Cond);
	//Cond = ConvertType(Ty, AssignId->GetTy(), Cond);
	
	if (Lexer->CurToken != tok_then)
		return Error<OperAST*>(expected_s_after_expr, "then");
	
	// THEN
	Lexer->GetNextToken(); // Получаем Then

	// VARS ---
	map<string, Var> *VD = new map<string, Var>();
	VarStack.push_back(VD);
	VarDict = VD;
	// --------
	
	condif = true;
	OperAST *Then = ParseOpers(false, true);
	//Then = new LifetimeVarsExprAST(VD, Then);
	bool ifend = false;
	
	if (Lexer->CurToken == ';') {
		ifend = true;
		Lexer->GetNextToken();
	}

	// VARS ---
	VarStack.pop_back();
	VarDict = VarStack[VarStack.size()-1];
	// --------
	
	if (Lexer->CurToken != tok_else && Lexer->CurToken != tok_elsif) {
		//condif = false; ifend  = false;
		//Lexer->GetNextToken();
		
		return new IfOperAST(Cond, Then, 0, false);
		//return 0;
	} else if (ifend) return Error<OperAST*>(expected_expr);
	
	// ELSE
	//if (ifend) return Error<OperAST*>(expected_s_after_statement, ";");
	
	BlockOperAST *ElseBlock = new BlockOperAST();
	//

	while (Lexer->CurToken == tok_elsif) {
		//
		ExprAST *TyE = new IntTyExprAST(1);
		ExprTy = TyE->GetTy();
		//
		Lexer->GetNextToken(); // Получаем elsif
		ExprAST *elsifcond = ParseExpr();
	
		if (!elsifcond) return 0;
		struct TypeAST Ty = elsifcond->GetTy();

		if (Lexer->CurToken != tok_then)
			return Error<OperAST*>(expected_s_after_expr, "then");

		Lexer->GetNextToken(); // Получаем Then
		//
		// VARS ---
		map<string, Var> *VD = new map<string, Var>();
		VarStack.push_back(VD);
		VarDict = VD;
		// --------
		//
		OperAST *elsifthen = ParseOpers (false, true);
		//elsifthen = new LifetimeVarsExprAST(VD, elsifthen);
		OperAST *ifstmt = new IfOperAST(elsifcond, elsifthen, 0, false);

		if (elsifthen != 0) 
			ElseBlock->Append(ifstmt);

		// VARS ---
		VarStack.pop_back();
		VarDict = VarStack[VarStack.size()-1];
		// --------
	}
	//
	OperAST *Else = 0;

	if (Lexer->CurToken == tok_else) {
		// VARS ---
		map<string, Var> *VD = new map<string, Var>();
		VarStack.push_back(VD);
		VarDict = VD;
		// --------

		Lexer->GetNextToken(); // Получаем Else
		
		condif = false; ifend  = false;
		Else = ParseOpers();
		//Else = new LifetimeVarsExprAST(VD, Else);

		// VARS ---
		VarStack.pop_back();
		VarDict = VarStack[VarStack.size()-1];
		// --------
	} else Lexer->GetNextToken();

	ElseBlock->Append(Else);

	return new IfOperAST(Cond, Then, ElseBlock, true);
}

// Таблица переходов

// BRTD  ::= VAL : OPS
OperAST *parser::ParseTd() {
	ExprAST *Val = ParseVal();

	if (Lexer->CurToken != ':')
		return Error<OperAST*>(expected, ":");

	map<string, Var> *VD = new map<string, Var>();
	VarStack.push_back(VD);
	VarDict = VD;
		
	Lexer->GetNextToken();
	OperAST *Ops = ParseOper();

	VarStack.pop_back();
	VarDict = VarStack[VarStack.size()-1];

	return new TdOperAST(Val, Ops);
}

// BRTDS ::= BRT | BRTD BRTDS
OperAST *parser::ParseTds() {
	BlockOperAST *block = new BlockOperAST();
	
	while (1) {		
		if (Lexer->CurToken == tok_end) {
			Lexer->GetNextToken();
			
			if (Lexer->CurToken != ';')
				return Error<OperAST*>(expected, ";");
			
			Lexer->GetNextToken();
			return block;
		}
		
		OperAST* V = ParseTd();
		block->Append(V);
	}
}

// CASE  ::= CASE EXPR OF BRTDS
OperAST *parser::ParseCase() {
	Lexer->GetNextToken();   // Получаем case
	ExprAST *Id = ParseExpr(); // Получаем выражение

	//Lexer->GetNextToken();
	
	if (Lexer->CurToken != tok_of)
		return Error<OperAST*>(expected, "of");
		
	Lexer->GetNextToken();  // Получаем of
	vector<TdOperAST*> Tds;
	OperAST *ElseOp = 0;
	
	// Таблица
	while (1) {
		if (Lexer->CurToken == tok_end) {
			Lexer->GetNextToken();
			
			if (Lexer->CurToken != ';')
				return Error<OperAST*>(expected_s_after_statement, ";", "end");
			
			Lexer->GetNextToken();
			break;
		} else if (Lexer->CurToken == tok_else) {
			map<string, Var> *VD = new map<string, Var>();
			VarStack.push_back(VD);
			VarDict = VD;
			//
			Lexer->GetNextToken();
			ElseOp = ParseOper();

			VarStack.pop_back();
			VarDict = VarStack[VarStack.size()-1];
			//
			if (Lexer->CurToken == tok_end) {
				Lexer->GetNextToken();
				
				if (Lexer->CurToken != ';')
				return Error<OperAST*>(expected_s_after_statement, ";", "end");
			
				Lexer->GetNextToken();
			} else return Error<OperAST*>(expected, "end");
			
			break;
		}
		
		OperAST* V = ParseTd();
		Tds.push_back((TdOperAST*)V);
	}
	
	/*// else
	if (Lexer->CurToken == tok_else) {
		puts("!");
		Lexer->GetNextToken();
	}*/
	
	return new CaseOperAST(Id, Tds, ElseOp);
}

#endif
