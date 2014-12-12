//===-----------------------------------------------------------===//
// Дополнительные функции 2012г.
// Автор : Кабылин Андрей Николаевич (sys.int64@gmail.com)
//===-----------------------------------------------------------===//

#ifndef H_DEF
#define H_DEF 1

#ifdef WIN32
#define strcasecmp stricmp
#endif

#if _WIN64 || __amd64__
	#define PORTABLE_64_BIT
#else
	#define PORTABLE_32_BIT
#endif

using namespace std;

// Функции
static bool StrCompare(const string &s1, const string &s2) {
	if (strcasecmp(s1.c_str(), s2.c_str()) == 0)
		return true;
	
	return false;
}

float FastSqrt (float f) {
	int t = *(int*)&f;
	t -= 0x3f800000;
	t >>= 1;
	t += 0x3f800000;
	return *(float*)&t;
}

/*float FastInvSqrt(float x) {
	int tmp = ((0x3f800000 << 1) +
		    0x3f800000 - *(long*)&x) >> 1;
	float y = *(float*)&tmp;
	return y*(1.47f-0.47f*x*y*y);
}*/

// Main
#include <string>

enum vvisible { v_public, v_private, v_protected };
// Структуры
// Переменные
struct Var {
	BaseIR *Val;
	ExprAST *E;
	OperAST *Stmt;
	string Name;
	struct TypeAST Type;
	ExprAST *TypeAST;
	int Offset; // Offset for record & class type
	ExprAST *DefaultVal;
	
	bool Vec, Arr, Ptr;
	bool Def, Dyn;
	bool Global;
	bool Const, Arg;
	bool Out, Var;
	bool MemCpy;
	bool isAlloced;
	//
	vvisible Visible;
	//
	int sPtr,sDyn;
	PrototypeExprAST *Prot;
} typedef Var;

enum cconv   { stdcc, fastcc, ccc, coldcc, cc };
enum linkage { stdlink, dllimport, dllexport, llvmimport };
enum attr    { doverride, dvirtual, dabstract };

struct overloads {
	string Name; // Имя функции в LLVM
	FuncOperAST *FuncAST;
	PrototypeExprAST *ProtExpr;
	BaseIR *Prot;
	BaseIR *Func;
	ExprAST *ClassTy;
	BaseIR *ClassIR;
	vvisible Visible;
	//
	bool External;
	bool Def;
	bool inClass;
	bool Constructor;
	bool Destructor;
	bool Virtual;
	bool Override;
	//
	struct overloads *old_ovr;
} typedef overloads;

// Функции 
struct Proc {
	bool	Def;
	bool	Declare;
	bool	inllvm;
	bool	Overload;
	bool	Virtual;
	bool	MayDeclare;
	//
	string	NSName;
	string	ClassName;
	BaseIR *Proto;
	//
	vector<overloads> Overloads;
} typedef Proc;
//
struct Label {
	bool Def;
	string Name;
} typedef Label;
//
struct Typed {
	ExprAST* Type;
	BaseIR* TypeIR;
	bool Def;
	bool Declare;
} typedef Typed;
//
struct Unit {

} typedef Unit;
//
struct Oper {
	int op;
	bool binary;
	bool def;
	bool assign;
	bool Swap;
	TypeAST Ty;
	struct Var RHS;
	struct Var LHS;
	struct Var IDX;
	FuncOperAST *FuncAST;
	PrototypeExprAST *Prot;
} typedef Oper;
//
bool NoErrors = true;
bool Implementation = false;
bool OneDeref = false; // Maybe fixme, i'm confused :o (for cast and deref ptr)
//
//static map<int, Oper> OperOverloads;
static vector<Oper> OperOverloads;
//
static map<string, Var>   ExternVars;  // Глобальные переменные
static map<string, Proc>  MainFuncDict;
static map<string, Typed> MainTypeDict;
//
static Typed ClassTyD;
//
map<string, Var>	*VarDict = &ExternVars; // Текущая таблица переменных // FIXME!
map<string, Var>	*LocalVars = 0; // Локальные переменные // FIXME!
map<string, Var>	VarsMem[1000];
int vi = 0;

vector<map<string, Var>*>	VarStack; // Заменить на это

map<string, Label>	*LabelsDict; // Метки
map<string, Proc>	*FuncDict = &MainFuncDict;
map<string, Proc>	*LocalFuncs = 0;
map<string, Typed>	*TypeDict = &MainTypeDict;
//
ClassTyExprAST	*CurClassTy   = 0; // <
ExprAST			*CurClassDot  = 0; // <
BaseIR			*CurClassIR   = 0; // < Бддло код, просто пиздец какой-то!
BaseIR			*CurClassTyIR = 0; // < :(((((((
ExprAST			*CurClassExpr = 0; // <
ExprAST			*CurClassCTy  = 0; // <
static string	 CurTypeName;
ExprAST			*CurRetTy = 0;
BlockIR			*CurVarBlockIR = 0;
//
//
bool GetVarDict(const string name) {
	//
	for (int i = VarStack.size()-1; i >= 0; i--) {
		if ((*VarStack[i])[name].Def) {
			VarDict = VarStack[i];
			return true;
		}
	}

	return false;
}
//
static bool isClass = false;
//
bool GetFuncDict(const string name) {
	FuncDict = &MainFuncDict;
	
	if (LocalFuncs != 0) {
		if ((*LocalFuncs)[name].Def | (*LocalFuncs)[name].Declare) { FuncDict = LocalFuncs; }
		else if (MainFuncDict[name].Def) FuncDict = &MainFuncDict;
		else return false;
	} else {
		if (MainFuncDict[name].Def | MainFuncDict[name].Declare) FuncDict = &MainFuncDict;
		else return false;
	}
	
	return true;
}

// Флаги
static string FileName;
static string CurFN;
static string OutName;
static vector<string> Flags;
static int  OptLevel = 0;
static bool EmitLLVM;
static bool EmitASM;
static bool Native = false;
static bool Vorbis = false;
static bool IntelSyntax;
static bool Skip = false;
static bool GetAddr = false;
static bool LoadPtr = true;
static bool bie = false;
static bool debug = true;
static bool ClcMath = false;
static ExprAST *AssignId = 0;
static vector<ExprAST*> WithParams;
static vvisible CurVisibility = v_public;
static bool isFuncDef = false;
static vector<VarOperAST*> MustCasts;
static string CurFuncName;
static int CurOvrID = 0;
static bool BranchingIf = false; // Расчет условий путем ветвления
static BlockOperAST *LastVarBlock = 0;
static bool AssignIdx = false;
static string ProtNameReg = "";
static string MacroFlags = "";
// Стандартные типы
enum btype     { bin_num, bin_bool, bin_str, bin_ptr };
enum earch     { x32, x64 };
enum eplatfoms { win_os, linux_os, apple_osx, android, apple_ios };
//
BaseIR *CurLAfter = 0; // Current Label with after code (afeter cods, after loops), for tok break;
BaseIR *CurLBegin = 0; // Current label with begin code (begin conds, begin loops), for tok continue;
BaseIR *CurFunc   = 0; // Current function for recursive FIXME
BaseIR *ExitVar   = 0;

BaseIR *TmpVal;
string exprty, uety;
string assignvname = "";
struct TypeAST ExprTy;
BaseIR *ThisClassPtrIR = 0;
BaseIR *FuncSRetIR = 0;

int constn = 0;
vector<int> unni;
vector<FILE*> FileStack;
string CurNameSpace = "main";
btype bintype = bin_num;
earch arch = x32;
string iPtrSize = "i32";
int intSize = 32;
eplatfoms platform = win_os;
static bool condif = false;
static bool ifend = false;
static bool GlobalParam = false;
static bool InClassMethod = false; // WTF
static bool InsideClass = false;   // FIXME!!
static bool MainFunc = false;
static bool OwnDataLayout = true;
static bool DataLayout = true;
static bool InOperator = false;
static bool MainFile = true;
static bool Implement = true;
static bool DebugMode = false;

// Codegen config
static bool ClassHow_i8 = true;  // TODO

#endif
