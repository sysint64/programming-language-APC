//===-----------------------------------------------------------===//
// Synax analyzer 2012г.
// Author : Kabylin Andrey Nikolaevich (andrey@kabylin.ru)
//===-----------------------------------------------------------===//

#ifndef H_PARSER
#define H_PARSER 1

class parser {
public:
	lexer *Lexer; // Lexer - Лексический анализатор
	map<char, int> BinopPrecedence; // Содержит приоритеты для бинарных операторов
	bool NoErrors;
	
	parser() {
		Lexer = new lexer();
		NoErrors = true;
		ExprTy = DefaultTy();
	}
	
	~parser() { delete Lexer; }
	
	// GetTokPrecedence - Возвращает приоритет текущего бинарного оператора.
	int GetTokPrecedence() {
		//if (!isascii(Lexer->CurToken))
		//	return -1;
		
		// Удостоверимся, что это объявленный бинарный оператор.
		int TokPrec = BinopPrecedence[Lexer->CurToken];
		if (TokPrec <= 0) return -1;
		return TokPrec;
	}
	//
	void PrintToken() {
		printf("code: %d; char: %c; id: %s\n", Lexer->CurToken, Lexer->CurToken, Lexer->IdentifierStr.c_str());
	}
	
	// Recover After Error
	void RecoverAfterOperErr() {
	}
	
	void RecoverAfterExprErr() {
	}
	
	// Parsing
	//* Val
	ExprAST		*ParseVal(); // +
	ExprAST		*FoldingBinary(char BinOp, ExprAST *LHS, ExprAST *RHS); // +
	ExprAST		*ParseBinary  (int ExprPrec, ExprAST *LHS); // +
	ExprAST		*ParseBinaryOvrOpers (ExprAST *LHS, ExprAST *RHS, int BinOp, bool Swap = false); // +
	ExprAST		*ParseUnaryOvrOpers (ExprAST *Val, int Op); // +
	ExprAST		*ParseAssignOvrOpers (ExprAST *Id, ExprAST *Expr, ExprAST *Idx, int Op); // +/- (todo idx)
	ExprAST		*ParseExpr();	// +
	ExprAST		*ParseBool(bool val); // +
	ExprAST		*ParseChar(); // +
	ExprAST		*ParseNumber(); // +
	ExprAST		*ParseParen(); // +
	ExprAST		*ParseUnary(); // +
	ExprAST		*ParseDoubleChar(); // +
	ExprAST		*ParseId(); // +
	ExprAST		*ParseIdx(ExprAST *E); // +
	ExprAST		*ParsePtr(ExprAST *E); // +
	ExprAST		*ParseParam(ExprAST *E, bool ShowErr = true, string IdName = ""); // +
	ExprAST		*ParseArrVal(); // +
	ExprAST		*ParseVecVal(); // +
	ExprAST		*ParseAssign(ExprAST *Id, bool cast = true); // +
	ExprAST		*ParseString(bool SetConst = false); // +
	ExprAST		*ParseCast();	// +
	
	//* Statemets
	OperAST		*ParseIf(); 	// +
	OperAST		*ParseFor(); 	// +
	OperAST		*ParseWhile(); 	// +
	OperAST		*ParseRepeat(); // -
	
	// Таблица переходов
	OperAST 	*ParseTd();     // +
	OperAST 	*ParseTds();    // +
	OperAST		*ParseCase();   // +
	OperAST		*ParseWith();	// +
	//
	OperAST		*ParseOper(bool ifcond = false); // +
	OperAST		*ParseOpers(const bool main = false, bool ifcond = false); // +
	
	//* Прасинг переменных
	// Парсинг типов
	ExprAST		*ParseTemplateType(const string Ty); // -
	ExprAST		*ParseStdTy(); // +
	ExprAST		*ParseArrTy(); // +
	ExprAST		*ParseVecTy(); // +
	ExprAST		*ParseFuncTy(); // +
	//
	ExprAST		*ParseType();  // +
	ExprAST		*ParseAlign(); // +
	ExprAST		*ParseSizeOf(); // +
	
	//
	OperAST		*ParseVar(const bool global = false, const bool isexpr = false, const bool forloop = false); // +
	OperAST		*ParseVarBlock(const bool global = false); // +
	OperAST		*ParseConstBlock(const bool global = false); // +
	ExprAST		*ParseConstVal();     // +
	ExprAST		*ParseArrConstVal();  // +
	ExprAST		*ParseVecConstVal();  // +
	OperAST		*InsertVar(string IdName, bool Const, bool Var, bool Global, ExprAST *Ty); // +
	//
	ExprAST		*ParseCall(string Name, bool noparen = false,  ExprAST *E = 0, bool GetFD = true);
	
	//* Подпрограммы
	OperAST		*ParseFunc(const bool proc, bool oper, const bool classdef = false); // +
	ExprAST		*ParsePrototype(const bool proc, const bool oper, const bool istype = false, const bool inclass = false); // +
	OperAST		*ParseDirectives(); // +
	OperAST		*ParseDirective(); // +
	OperAST		*ParseBlockDicts(ExprAST *RetTy); // +
	OperAST		*ParseArgs(const bool istype = false); // +
	OperAST		*ParseArg(const bool istype = false); // +
	//int		 GetPrototypeId(const string Name, vector<ExprAST*> Args, ExprAST *RetType); //+
	//int		 GetPrototypeId(const string Name, vector<struct Var> Args); //+
	
	//* Объявление типов
	ExprAST		*ParseHType(); // +
	OperAST		*ParseHTypeBlock(); // +
	
	// NEW
	OperAST		*ParseAlias(); // -
	ExprAST		*ParseStruct(const bool packed = false); // -
	ExprAST		*ParseClass(); // -
	OperAST		*ParseClassBlock(vector<string> *MethodsNames); // +

	// Преобразование типов
	ExprAST	 	*ConvertType(struct TypeAST from, struct TypeAST to, ExprAST* E, bool showerr = true, ExprAST *To = 0); // +
	ExprAST	 	*ExplictConvertType(struct TypeAST from, struct TypeAST to, ExprAST* E, ExprAST *To = 0); // +
	ExprAST		*ParseParenConvert(string IdName); // +
	ExprAST		*ParseAsCast(ExprAST *Val); // +
	ExprAST		*ParseIsCast(ExprAST *Val); // -
	ExprAST		*TryAssignOverload(ExprAST *Id, ExprAST *Expr); // +
	inline int	 MayAssignOverload(ExprAST *Ty, ExprAST *Expr); // +

	//
	ExprAST		*ParseNew(); // +
	ExprAST		*ParseDelete(); // +
	
	//* Handles
	void HandleHeadProgram();
	void HandleBlock(const bool main);
	void HandleProc();
	void HandleFunc(const bool proc, const bool oper);
	void HandleVar(const bool global = false);
	void HandleInclude(const bool mf);
	void HandleConst();
	void HandleType();
	void HandleAlias();
	void HandleStruct(const bool packed = false);
	void HandleClass();
	//
	void Handle(parser *obj, OperAST* (parser::*func)());
	//
	void HandleInterface();
	void HandleImplementation();
	
	//*
	void MainLoop();
	void MainProgBlock(); // Что это?
};

parser *mParser;

//
#include "AST.h"
#include "Lexer.h"
#include "Parser/Convert.cpp"
#include "Parser/Expr.cpp"
#include "Parser/Const.cpp"
#include "Parser/Type.cpp"
#include "Parser/Var.cpp"
#include "Parser/HType.cpp"
#include "Parser/IfCase.cpp"
#include "Parser/Main.cpp"
#include "Parser/Loop.cpp"
#include "Parser/Func.cpp"

#endif
