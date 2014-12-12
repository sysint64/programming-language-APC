#ifndef H_TYPE
#define H_TYPE 1

using namespace std;

enum TT { TVOID, TFUNC, TARGS  , TINT  , TFLOAT, TARR ,
	  TVEC , TPTR , TSTRUCT, TCLASS, TNONE , TVPTR, TCPTR  };
//
class BaseIR;
class ExprAST;
class PrototypeExprAST;
struct TypeAST;

//
const int TyExtNone	= 0; //
const int TyExtAbstract	= 1; //
const int TyExtUnsigned	= 2; //
const int TyExtMustCast	= 4; // For class in the class
static int LastTypeID = 0;
//
struct TypeAST {
	int Size;    // Ðàçìåð òèïà â áèòàõ
	int id;
	string Name; // Èìÿ òèïà
	string LLVMName;
	TT Ty;
	BaseIR *TyIR;
	ExprAST *E, *TypeExpr;
	bool Flag; /* DELME */
	unsigned char ExtProps;
	PrototypeExprAST *Prot; // Äëÿ òèïà ôóíêöèè
	bool Abstract; /* FIXME */
	bool MustCast; /* FIXME */
	bool Global;   /* FIXME */
	bool NoThis;   /* FIXME */
	bool HaveClass;
} typedef TypeP;

//
struct TypeAST NullTy, BoolTy;
//
#define IsIntTy(T)	(T.Ty == TINT)
#define IsFloatTy(T)	(T.Ty == TFLOAT)
#define IsArrTy(T)	(T.Ty == TARR)
#define IsVecTy(T)	(T.Ty == TVEC)
#define IsPtrTy(T)	(T.Ty == TPTR | T.Ty == TVPTR)
#define IsPtr2Ty(T)	(T.Ty == TPTR)
#define IsVPtrTy(T)	(T.Ty == TVPTR)
#define ISCPtrTy(T)	(T.Ty == TCPTR) // универсальный указатель, может быть чем угодно ))
#define IsStructTy(T)	(T.Ty == TSTRUCT)
#define IsClassTy(T)	(T.Ty == TCLASS)
#define IsVoidTy(T)	(T.Ty == TVOID)
#define IsStringTy(T)	(T.Ty == TSTRING)
#define IsFuncTy(T)	(T.Ty == TFUNC)
#define TyHaveClass(T)	T.HaveClass
#define SizeOfTy(T)	T.Size
#define CountOfTy(T)	T.Size / DerefTy(T).Size
#define GetIRType(T)	T.TyIR

//
static string IntToStr(int64_t n) {
	char buff[13];
	sprintf(buff, "%lld", n);

	return buff;
}

#endif
