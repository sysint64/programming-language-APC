#ifndef CPP_STRING_AST
#define CPP_STRING_AST 1

// StringExprAST - Класс узла выражения для строковых литералов
class StringExprAST : public ExprAST {
public:
	string Val;
	int StrLen;
	bool GetConst;
	
	StringExprAST(const string val, const int sLen, const bool getConst = false)
		: Val(val), StrLen(sLen), GetConst(getConst)
	{
			SetConst(true);
			ExprAST *Ty = new PtrTyExprAST(new IntTyExprAST(8, false));
			SetTy(Ty->GetTy());
	}
	
	virtual BaseIR *Codegen();
};

#endif
