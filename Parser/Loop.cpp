#ifndef CPP_PARSER_LOOP
#define CPP_PARSER_LOOP 1

// Main
#include <string>
#include <map>

using namespace std;

//* WHILE ::= WHILE EXPR DO OPS
OperAST *parser::ParseWhile() {
	//
	Lexer->GetNextToken(); // Получаем While
	ExprAST *TyE = new IntTyExprAST(1);
	ExprTy = TyE->GetTy();
	ExprAST *Cond = ParseExpr();
	if (!Cond) return 0;

	//
	map<string, Var> *VD = new map<string, Var>(); // переменные цикла
	VarStack.push_back(VD);
	VarDict = VD;
	
	struct TypeAST Ty = Cond->GetTy();
	Cond = ConvertType(Ty, CreateBoolTy(), Cond);
	
	if (Lexer->CurToken != tok_do)
		return Error<OperAST*>(expected_s_after_expr, "do");
	
	Lexer->GetNextToken();	 // Получаем do
	OperAST *Do = ParseOpers(false);
	//Do = new LifetimeVarsExprAST(VD, Do);

	VarStack.pop_back();
	VarDict = VarStack[VarStack.size()-1];
	
	return new WhileOperAST(Cond, Do);
}

//* FOR ::= FOR EXPR TO NUM DO OPS
OperAST *parser::ParseFor() {
	//
	Lexer->GetNextToken(); // Получаем For
	OperAST *Declare = 0;

	map<string, Var> *VD = new map<string, Var>(); // переменные цикла
	VarStack.push_back(VD);
	VarDict = VD;
	//
	ExprAST *Start, *Load = 0;
	//
	if (Lexer->CurToken == tok_id) {
		Lexer->GetNextToken();

		if (Lexer->CurToken == ':') { // new variable
			Lexer->GetPrevToken();
			Declare = ParseVar(false, true, true);
			Lexer->GetPrevToken();

			if (Lexer->CurToken != '=')
				return Error<OperAST*>(expected, "=");

			Lexer->GetNextToken();
			string IdName = ((VarOperAST*)Declare)->IdNames[0];
			ExprAST *Var = new VarExprAST(IdName, (*VarDict)[IdName].Type, VarDict);
			ExprTy = Var->GetTy();

			ExprAST *Val = ParseExpr();
			Val = ConvertType(Val->GetTy(), ((VarOperAST*)Declare)->Type->GetTy(), Val, true, ((VarOperAST*)Declare)->Type);

			Start = new AssignExprAST(Var, Val);

			//Lexer->GetPrevToken();
		} else Lexer->GetPrevToken();
	}

	if (Declare == 0)
		Start = ParseExpr(); // Получаем начальное значение переменной
	
	Load = new LoadExprAST(Start);

	//
	if (!dynamic_cast<AssignExprAST*>(Start))
		return Error<OperAST*>(expected_assign_stmt);
	/*} else {
		string IdName = ((VarOperAST*)Declare)->IdNames[0];
		Start = new VarExprAST(IdName, (*VarDict)[IdName].Type, VarDict);
		Start->SetvVar((*VarDict)[IdName].Var);
		Load = new LoadExprAST(Start);
	}*/
	
	if (Lexer->CurToken != ',')
		return Error<OperAST*>(expected, ",");

	bool dt = false;
	if (Lexer->CurToken == tok_downto) dt = true;
	
	Lexer->GetNextToken(); // Получаем to | downto
	ExprAST *To = ParseExpr(); // получаем конечное число

	To = ConvertType(To->GetTy(), Start->GetTy(), To);
	
	if (Lexer->CurToken != tok_do && Lexer->CurToken != ',')
		return Error<OperAST*>(expected_ss, "do", ",");
	
	//
	ExprAST *Step = new IntExprAST(1, 32);
	//
	if (Lexer->CurToken == ',') {
		Lexer->GetNextToken();
		Step = ParseExpr();
	}
	//
	Step = ConvertType(Step->GetTy(), Start->GetTy(), Step);
	//
	Lexer->GetNextToken(); // Получаем do
	OperAST *Do = ParseOpers(); // Тело цикла

	VarStack.pop_back();
	VarDict = VarStack[VarStack.size()-1];
	//
	return new ForOperAST(Start, To, Step, Load, Do, Declare);
}

#endif
