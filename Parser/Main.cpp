#ifndef CPP_PARSER_MAIN
#define CPP_PARSER_MAIN 1

// Main
#include <string>
#include <map>

using namespace std;

// Парсинг Операторов

OperAST	*parser::ParseOper(bool ifcond) {
	switch (Lexer->CurToken) {
		// break, continue, exit
		case tok_return   : {
			Lexer->GetNextToken();
			
			if (Lexer->CurToken == ';') {
				Lexer->GetNextToken();
				return new RetOperAST(0);
			}

			ExprAST *Val = ParseExpr();
			Lexer->GetNextToken();

			if (MayAssignOverload(CurRetTy, Val) != -1) {
				//				ExprAST *Ovr = TryAssignOverload(Id, Val);
				//if (Ovr != 0) new RetOperAST(Ovr);
				OperAST *Var = InsertVar("ret.tmp", false, false, false, CurRetTy);
				ExprAST *Id = new VarExprAST("ret.tmp", CurRetTy->GetTy(), VarDict);
				ExprAST *Ovr = TryAssignOverload(Id, Val);

				return new RetOvrOperAST(Id, Var, Ovr);
				//puts("!"); exit(-1);
			} else {
				Val = ConvertType(Val->GetTy(), CurRetTy->GetTy(), Val, true, CurRetTy);
				return new RetOperAST(Val);
			}

			break;
		}

		case tok_break    :
			Lexer->GetNextToken();
			
			if (Lexer->CurToken != ';' && Lexer->CurToken != tok_end) {
				return Error<OperAST*>(expected_s_after_statement, ";", "break");
			} else if (Lexer->CurToken != tok_end) Lexer->GetNextToken();
			
			return new BreakOperAST();
			
		case tok_continue :
			Lexer->GetNextToken();
			
			if (Lexer->CurToken != ';' && Lexer->CurToken != tok_end) {
				return Error<OperAST*>(expected_s_after_statement, ";", "continue");
			} else if (Lexer->CurToken != tok_end) Lexer->GetNextToken();
			
			return new ContinueOperAST();
			
		case tok_exit :
			Lexer->GetNextToken();
			
			if (Lexer->CurToken != ';' && Lexer->CurToken != tok_end) {
				return Error<OperAST*>(expected_s_after_statement, ";", "exit");
			} else if (Lexer->CurToken != tok_end) Lexer->GetNextToken();
			
			return new ExitOperAST();
			
		case ';' : Lexer->GetNextToken(); break;
		case tok_if     : return ParseIf();
		case tok_case   : return ParseCase();
		case tok_while  : return ParseWhile();
		case tok_with   : return ParseWith();
		case tok_for    : return ParseFor();
		//case tok_delete : return ParseDelete();
		case tok_end    : return 0;
		
		//
		case tok_begin :
			Lexer->GetNextToken();
			return ParseOpers(false, ifcond);
		
		//	
		case tok_id    : {
			Lexer->GetNextToken();
			
			if (Lexer->CurToken == ':' || Lexer->CurToken == ',') {
				Lexer->GetPrevToken();
				VarDict = VarStack[VarStack.size()-1];
				return ParseVar(false, true);
			}

			Lexer->GetPrevToken();
		}

		default :
			ExprAST *E = ParseExpr();
			exprty = "";

			if (E == 0) {
				Lexer->GetNextToken();
				return 0;
			}
			
			if (Lexer->CurToken != tok_end) {
				if (!ifcond)
				if (Lexer->CurToken != ';') {
					return Error<OperAST*>(expected_s_after_expr, ";");
				}

				if (!ifcond) Lexer->GetNextToken();
			}
			
			return new ExprOpAST(E);
	}
	
	Lexer->GetNextToken();
	return 0;
}

OperAST	*parser::ParseOpers(const bool main, bool ifcond) {
	BlockOperAST *block = new BlockOperAST();
	//

	while (Lexer->CurToken != tok_end && Lexer->CurToken != tok_else && Lexer->CurToken != tok_elsif && Lexer->CurToken != tok_eof) {
		OperAST *Op = ParseOper();
		if (Op != 0) block->Append(Op);
	}
	
	if (Lexer->CurToken == tok_end || Lexer->CurToken == tok_else || Lexer->CurToken == tok_elsif) {

		if (Lexer->CurToken != tok_else && Lexer->CurToken != tok_elsif)
			Lexer->GetNextToken();
		
		/*if (main) {
			if (Lexer->CurToken != '.')
				return Error<OperAST*>(expected_s_after_statement, ".", "end");
				
		} else {
			if (!ifcond) {
				if (Lexer->CurToken != ';') {
					if (Lexer->CurToken != tok_end) {
						return Error<OperAST*>(expected_s_after_statement, ";", "end");
					}
				}
			}
		}*/
		
		if (!ifcond || Lexer->CurToken == tok_end) Lexer->GetNextToken();
	} else return Error<OperAST*>(expected, "end");

	return block;
}

void parser::MainLoop() {
	//
	Lexer->GetNextToken();
	Lexer->TokenStack.clear();
	//Implement = true;
	//
	while (1) {
		//if (MainFile) {
			switch (Lexer->CurToken) {
				case tok_eof		 : return;
				case ';'			 : Lexer->GetNextToken(); break;
				case tok_include	 : HandleInclude(MainFile); break;
				//
				case tok_var		 : HandleVar(true);  break;				
				case tok_const		 : HandleConst(); break;
				//case tok_record	 	: Implementation = false; HandleRecord(); break;
				//case tok_type		 : Implementation = false; HandleType(); break;
				case tok_func		 : Implementation = true; HandleFunc(false, false); break; // function
				
				// Types
				case tok_alias		 : HandleAlias(); break;
				case tok_record		 : HandleStruct(false); break;
				case tok_packed		 :
					Lexer->GetNextToken();

					if (Lexer->CurToken != tok_record)
						Error<OperAST*>(expected, "struct");

					HandleStruct(true);  break;
				case tok_class		 : HandleClass(); break;
				//
				//case tok_begin		 : VarDict = &ExternVars; Implementation = true; HandleBlock(true); Lexer->GetNextToken(); break;
				default	:
					SetCurLine();
					Error<OperAST*>(expected_expr);
					Lexer->GetNextToken();
					//RecoverAfterOperErr();
			}
			//
		/*} else {
			//
			switch (Lexer->CurToken) {
				case tok_eof		: return;
				case ';'		: Lexer->GetNextToken(); break;
				case tok_with		: HandleInclude(false); break;
				//
				case tok_interface	: HandleInterface(); break;
				case tok_implementation	: HandleImplementation(); break;

				default	:
					Error<int>(expected_expr);
					Lexer->GetNextToken();
					return;
			}
		}*/
		//
	}
}


void parser::HandleInterface() {
	//
	Lexer->GetNextToken();
	Implementation = false;
	Implement = false;

	while (1) {
		switch (Lexer->CurToken) {
			case tok_implementation	: return;
			case tok_eof		: return;
			//
			case tok_var		: HandleVar(true);  break;				
			case tok_const		: HandleConst(); break;
			case tok_type		: HandleType(); break;
			//
			case tok_func		: HandleFunc(false, false); break; // function
			//
			case tok_end :
				Lexer->GetNextToken();
				
				if (Lexer->CurToken != '.')
					Error<int>(expected_s_after_statement, ".", "end");

				Lexer->GetNextToken();
				return;

			default	:
				Error<int>(expected_expr);
				Lexer->GetNextToken();
		}
	}
}

void parser::HandleImplementation() {
	//
	Lexer->GetNextToken();
	Implementation = true;
	Implement = true;

	while (1) {
		Implementation = true;
		switch (Lexer->CurToken) {
			case tok_end	:
				Lexer->GetNextToken();
				
				if (Lexer->CurToken != '.')
					Error<int>(expected_s_after_statement, ".", "end");

				Lexer->GetNextToken();

				return;

			case tok_eof	:
				Error<int>(expected_s_stmt, "end");
				return;
			
			//
			case tok_var		: HandleVar(true);  break;				
			case tok_const		: HandleConst(); break;
			//
			case tok_proc		: HandleFunc(true , false); break; // procedure
			case tok_func		: HandleFunc(false, false); break; // function
			case tok_constructor	: HandleFunc(true , false); break; // constructor
			case tok_destructor	: HandleFunc(true , false); break; // destructor
			case tok_oper		: HandleFunc(false, true ); break; // operator
			//
			default	:
				Error<int>(expected_expr);
				Lexer->GetNextToken();
		}
	}
}

void parser::HandleInclude (const bool mf) {
	//
	Lexer->GetNextToken();

	if (Lexer->CurToken != tok_string) {
		Error<int>(expected, "file");
		return;
	}

	string FN = Lexer->StringConst;
	Lexer->GetNextToken();

	if (Lexer->CurToken != ';') {
		Error<int>(expected, ";");
		return;
	}

	//
	string LastFN = CurFN;
	FileName = FN;
	CurFN = FN;

	string LastNameSpace = CurNameSpace;
	CurNameSpace = FN.substr(0, FN.find("."));

	//
	int lpos  = Lexer->pos;
	int lline = Lexer->line;
	//
	Lexer->OpenFile(FileName.c_str());
	Lexer->pos = 1; Lexer->line = 1;
	//
	MainFile = false;
	MainLoop();
	MainFile = mf;
	CurNameSpace = LastNameSpace;

	FileName = LastFN;
	CurFN = LastFN;
	//

	Lexer->TokenStack.clear();
	Lexer->nts = 0;
	Lexer->ReOpenFile(FileName.c_str());
	
	Lexer->TokenStack.clear();
	Lexer->nts = 0;

	int Tok = Lexer->GetToken();
	Lexer->GetNextToken();
	//
	Lexer->pos  = lpos;
	Lexer->line = lline;
}

void parser::HandleType() {
	if (OperAST *Op = ParseHTypeBlock()) {
		Op->Codegen();
	} else {
		// Пропускаем токен для восстановления после ошибки.
		Lexer->GetNextToken();
	}
}

/*void parser::Handle() {
	if (OperAST *Op = ParseAlias()) {
		Op->Codegen();
	} else {
		// Пропускаем токен для восстановления после ошибки.
		Lexer->GetNextToken();
	}
}*/

void parser::Handle(parser *obj, OperAST* (parser::*func)()) {
	//
	if (OperAST *Op = (obj->*func)()) Op->Codegen();
	else Lexer->GetNextToken();
}

void parser::HandleAlias() {
	//
	//ParseAlias();
	if (OperAST *Op = ParseAlias()) Op->Codegen();
	else Lexer->GetNextToken();
}

void parser::HandleStruct(const bool packed) {
	if (ExprAST *Op = ParseStruct(packed)) Op->Codegen();
	else Lexer->GetNextToken();	
}

void parser::HandleClass() {
	//
	//ParseAlias();
	if (ExprAST *Op = ParseClass()) Op->Codegen();
	else Lexer->GetNextToken();
}

void parser::HandleVar(const bool global) {
	if (global) VarDict = &ExternVars;
	if (OperAST *Op = ParseVarBlock(global)) {
		Op->Codegen();
	} else {
		// Пропускаем токен для восстановления после ошибки.
		Lexer->GetNextToken();
	}
}

void parser::HandleConst() {
	if (OperAST *Op = ParseConstBlock())
		Op->Codegen();
	else Lexer->GetNextToken();
}

void parser::HandleProc() {
	/*if (OperAST *Op = ParseFunc()) {
		Op->Codegen();
	} else {
		// Пропускаем токен для восстановления после ошибки.
		Lexer->GetNextToken();
	}*/
}

void parser::HandleFunc(const bool proc, const bool oper) {
	if (OperAST *Op = ParseFunc(proc, oper)) {
		Op->Codegen();
	} else {
		// Пропускаем токен для восстановления после ошибки.
		Lexer->GetNextToken();
	}
}

void parser::HandleBlock(const bool main = false) {
	Lexer->GetNextToken();
	MainFunc = main;

	if (OperAST *Op = ParseOpers(main)) {
		// Генерация промежуточного кода
		if (main) {
			ResetIRCount();
			//LocalVars = 0;
			//VarDict = &ExternVars; // Выбираем таблицу с глобальными переменными
		
			ManagerIR->Insert(new DefIR("main", new VoidTyIR()), Body);
			Op->Codegen();
			ManagerIR->Insert(new RetVoidIR(), Body);
		} else Op->Codegen();
	} else {
		// Пропускаем токен для восстановления после ошибки.
		Lexer->GetNextToken();
	}
}

#endif
