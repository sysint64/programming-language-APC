#ifndef CODEGEN_IR
#define CODEGEN_IR 1

//#include "../Type.h"

using namespace std;

//


static int StrToInt(const string str) {
	return strtod(str.c_str(), 0);
}

static string FloatToStr(float n) {
	union {
		uint64_t v;
		float f;
		double d;
	} fpir;

	char buff[16];

	fpir.f = n;
	fpir.d = n;

	sprintf(buff, "0x%llX", fpir.v);
	
	return buff;
}

static string DoubleToStr(double n) {
	char buff[13];
	sprintf(buff, "%e", n);

	return buff;
}

static string StrToUStr(string s) {
	return s+"\\00";
}


//
// Обозгачение
static const string sTmp  = "tmp";
static const string sStr  = "str";
static const string sPtr  = "ptr";
static const string sArg  = "arg";
static const string sCall = "call";

//
static map<string, int> Count;
static vector<string> CountStack;
//
bool LIRInS  = false;
bool NoInsIR = false;
bool Counter = true;
//
static string FastMath = "";
//
//
string GetIRCount (const string name, const bool i2s = true) {
	if (Count[name] == 0) {
		if (i2s) CountStack.push_back(name); // Добавляем в стэк имя
		if (!LIRInS) Count[name]++;
		return name;
	} else {
		string ret = name+'.'+IntToStr(Count[name]);
		if (!LIRInS) Count[name]++;
		return ret;
	}
}
//
void ResetIRCount() {
	for (int i = 0; i < CountStack.size(); i++) {
		Count[CountStack[i]] = 0;
	}
	
	CountStack.clear();
}

///

//
// Базовый класс генерации промежудочного кода LLVM-IR
class BaseIR {
private:
	string EmitStr;
	string Addr;
	BaseIR *Ty;
	bool   Const;
	string Align;
	
public:
	bool MemCpy;
	BaseIR *Prev;
	BaseIR() { Prev = 0; MemCpy = false; }
	virtual ~BaseIR() {}
	//
	void   SetEmit(const string str) { EmitStr = str; };
	string GetEmit() { return EmitStr; };
	
	void   SetAddr(const string str) { Addr = str; };
	string GetAddr() { return Addr; };
	
	void   SetAlign(const string str) { Align = str; };
	string GetAlign() { return Align; };
	
	void    SetTy(BaseIR *ty) { Ty = ty; };
	BaseIR *GetTy() { return Ty; };
	
	void    SetConst(bool c) { Const = c; };
	bool    IsConst() { return Const; };
};

class BlockIR : public BaseIR {
public:
	vector<BaseIR*> Elements;
	BlockIR() {}

	void Update() {
		for (int i = 0; i < Elements.size(); i++)
			SetEmit(GetEmit()+Elements[i]->GetEmit()+"\n");
	}
};

//
class CustomIR : public BaseIR {
public:
	CustomIR(const string Code) {
		SetEmit(Code);
	}
};

// Функция
class DefIR : public BaseIR {
public:
	DefIR(const string name, BaseIR *Ty, BaseIR *Proto = 0, BaseIR *ProtoTy = 0, BaseIR *CC = 0, BaseIR *Attr = 0, bool align = false) {
		//ResetIRDef();
		SetAddr("@"+GetIRCount(name)); SetTy(Ty);
		if (ProtoTy != 0) SetTy(ProtoTy);
		
		if (name == "main") SetEmit("; Main driver code\ndefine ");
		else SetEmit("define ");
		
		if (CC != 0) SetEmit(GetEmit()+CC->GetEmit()+" ");
		
		SetEmit(GetEmit()+Ty->GetEmit()+" "+GetAddr());

		if (Proto != 0) SetEmit(GetEmit()+"("+Proto->GetEmit()+") ");
		else SetEmit(GetEmit()+"() ");
		
		if (Attr != 0) SetEmit(GetEmit()+Attr->GetEmit()+" ");
		//if (align) SetEmit(GetEmit()+"align "+Ty->GetAlign()+" "); TODO
		
		SetEmit(GetEmit()+"{\ncode:");
	}
};

class RetVoidIR : public BaseIR {
public:
	RetVoidIR(const bool close = true) {
		string Ret = "  ret void";
		if (close) Ret += "\n}\n";
		SetEmit(Ret);
	}
};


class RetIR : public BaseIR {
public:
	RetIR(BaseIR *Val, const bool close = true) {
		string Ret = "  ret "+Val->GetTy()->GetEmit()+" "+Val->GetAddr();
		if (close) Ret += "\n}\n";
		SetEmit(Ret);
		
		//SetEmit("  ret "+Val->GetTy()->GetEmit()+" "+Val->GetAddr()+"\n}\n");
	}
};

// Типы данных
class IntTyIR : public BaseIR {
public:
	IntTyIR(const int size) {
		SetEmit("i"+IntToStr(size));
		SetAddr(GetEmit());
		SetAlign(IntToStr(size/8));
		if (size < 8) SetAlign("8");
		SetTy(this);
	}
};

// Floating point type
class FloatTyIR : public BaseIR {
public:
	FloatTyIR(const int size = 32, const bool ppc = false) {
		switch (size) {
			case 16  : SetEmit("half");     SetAlign("2");  break;
			case 32  : SetEmit("float");    SetAlign("4");  break;
			case 64  : SetEmit("double");   SetAlign("8");  break;
			case 80  : SetEmit("x86_fp80"); SetAlign("10"); break;
			case 128 : {
				if (ppc) SetEmit("fp128");
				else SetEmit("ppc_fp128");
				SetAlign("16");
				break;
			}
			
			default : SetEmit("float"); SetAlign("4");
		}
		
		SetTy(this);
		SetAddr(GetEmit());
	}
};

// Void
class VoidTyIR : public BaseIR {
public:
	VoidTyIR() { SetEmit("void"); SetAlign("4"); SetTy(this); SetAddr(GetEmit()); }
};

// Array
class ArrTyIR : public BaseIR {
public:
	ArrTyIR(const int n, BaseIR *Ty) {
		SetTy(Ty); SetAlign(Ty->GetAlign());
		SetEmit("["+IntToStr(n)+" x "+Ty->GetEmit()+"]");
		SetAddr(GetEmit());
	}
};

// SIMD Vector
class VecTyIR : public BaseIR {
public:
	VecTyIR(const int n, BaseIR *Ty) {
		SetTy(Ty); SetAlign(Ty->GetAlign());
		SetEmit("<"+IntToStr(n)+" x "+Ty->GetEmit()+">");
		SetAddr(GetEmit());
	}
};

// Указатель
class PtrTyIR : public BaseIR {
public:
	PtrTyIR(BaseIR *Ty) {
		SetTy(Ty); SetAlign("4");
		SetEmit(Ty->GetAddr()+"*");
		SetAddr(GetEmit());
	}
};

class aPtrTyIR : public BaseIR {
public:
	aPtrTyIR(BaseIR *Ty) {
		SetAlign("4");
		SetEmit(Ty->GetAddr()+"*");
		SetTy(this);
		SetAddr(GetEmit());
	}
};

// Declare Types
class DeclTyIR : public BaseIR {
public:
	DeclTyIR(BaseIR *Type, string Name) {
		SetAddr("%"+GetIRCount(Name, false));
		SetTy(Type->GetTy());
		SetEmit(GetAddr()+" = type "+Type->GetEmit());
		//SetAddr(Type->GetEmit());
	}
};

// Переменное количество аргументов
class ArgsTyIR : public BaseIR {
public:
	ArgsTyIR() { SetEmit("..."); SetTy(this); SetAddr(GetEmit()); }
};

// Константы
//

// zero initializer
class ZeroValIR : public BaseIR {
public:
	ZeroValIR() {
		SetConst(true);
		SetEmit("zeroinitializer");
	}
};

//
// undef val
class UndefValIR : public BaseIR {
public:
	UndefValIR(BaseIR *Ty) {
		SetTy(Ty);
		SetEmit("undef");
		SetAddr("undef");
		SetConst(true);
	}
};
//

// Null
class NullValIR : public BaseIR {
public:
	NullValIR(BaseIR *Ty) {
		SetTy(Ty);
		SetEmit("null");
		SetAddr("null");
		SetConst(true);
	}
};

// Целове число
class IntValIR : public BaseIR {
public:
	IntValIR(const int64_t val, const int base) {
		SetTy(new IntTyIR(base)); SetConst(true);
		
		if (base == 1) { // Булевая константа
			string s;
			
			switch (val) {
				case 0 : s = "false";  break;
				case 1 : s = "true";   break;
				default :
					if (val > 1) s = "true";
					else s = "false";
			}
			
			SetAddr(s);
			SetEmit(s);
		} else {
			SetAddr(IntToStr(val));
			SetEmit(IntToStr(val));
		}
	}
};

// Вещественное число Double, Float
class FloatValIR : public BaseIR {
public:
	FloatValIR(const double val, const int size = 32, const bool ppc = false) {
		string n;
		
		if (size == 64) n = DoubleToStr(val);
		else n = FloatToStr(val);
		
		SetTy(new FloatTyIR(size, ppc)); SetAddr(n);
		SetEmit(n); SetConst(true);
	}
};

// Константа массив
class ArrValIR : public BaseIR {
public:
	ArrValIR(vector<BaseIR*> Consts) {
		SetTy(new ArrTyIR(Consts.size(), Consts[0]->GetTy()));
		string consts = ""; SetConst(true);
		
		//
		for (int i = 0; i < Consts.size(); ++i) {
			consts += Consts[i]->GetTy()->GetEmit()+" "+Consts[i]->GetAddr();
			if (i != Consts.size()-1) consts += ", ";
		}
		
		SetEmit("["+consts+"]");
		SetAddr(GetEmit());
	}
};

// Константа вектор
class VecValIR : public BaseIR {
public:
	VecValIR(vector<BaseIR*> Consts) {
		SetTy(new VecTyIR(Consts.size(), Consts[0]->GetTy()));
		string consts = ""; SetConst(true);
		
		//
		for (int i = 0; i < Consts.size(); ++i) {
			consts += Consts[i]->GetTy()->GetEmit()+" "+Consts[i]->GetAddr();
			if (i != Consts.size()-1) consts += ", ";
		}
		
		SetEmit("<"+consts+">");
		SetAddr(GetEmit());
	}
};

// Строковая константа
class StringValIR : public BaseIR {
public:
	StringValIR(string val, const int size) {
		SetTy(new ArrTyIR(size+1, new IntTyIR(8)));
		SetAddr("@"+GetIRCount("str", false)); 
		SetEmit(GetAddr()+" = private unnamed_addr constant "+GetTy()->GetEmit()+" c\""+StrToUStr(val)+"\"");
		SetTy(new PtrTyIR(GetTy()));
	}
};

class StringConstIR : public BaseIR {
public:
	StringConstIR(string val, const int size) {
		SetTy(new ArrTyIR(size+1, new IntTyIR(8)));
		SetEmit("c\""+StrToUStr(val)+"\"");
		SetAddr(GetEmit());
		SetTy(new PtrTyIR(GetTy()));
	}
};

//
class StringValIR2 : public BaseIR {
public:
	StringValIR2(string val, const int size) {
		SetTy(new ArrTyIR(size+1, new IntTyIR(8)));
		SetAlign("1");
		SetAddr("c\""+StrToUStr(val)+"\" ");
		SetEmit("c\""+StrToUStr(val)+"\" ");
	}
};

// Выделение памяти в стеке
class AllocIR : public BaseIR {
public:
	AllocIR(BaseIR *Ty, string name) {
		SetAddr("%"+GetIRCount(name)); SetTy(Ty);
		//SetEmit("  "+GetAddr()+" = alloca "+Ty->GetAddr()+", align "+Ty->GetAlign()); TODO
		SetEmit("  "+GetAddr()+" = alloca "+Ty->GetAddr());
		SetTy(new PtrTyIR(Ty));
	}
};

// Загрузка из стека
class LoadIR : public BaseIR {
public:
	LoadIR(BaseIR *V, const bool load = false) {
		string name;
		
		for (int i = 1; i < V->GetAddr().length(); i++)
			name += V->GetAddr().c_str()[i];
			
		name += ".load";
			
		SetAddr("%"+GetIRCount(name)); SetTy(V->GetTy());
		
		if (dynamic_cast<ArrTyIR*>(V->GetTy())) {
			SetTy(V->GetTy());
		} else if (load) SetTy(V->GetTy()->GetTy()->GetTy());
		else SetTy(V->GetTy()->GetTy());
		
		//SetEmit("  "+GetAddr()+" = load "+V->GetTy()->GetEmit()+" "+V->GetAddr()+", align "+V->GetTy()->GetAlign()); TODO
		SetEmit("  "+GetAddr()+" = load "+V->GetTy()->GetEmit()+" "+V->GetAddr());
	}
};

// Глобальная переменная
class ExternIR : public BaseIR {
public:
	ExternIR(BaseIR *Ty, string Name, bool undef = true) {
		SetAddr("@"+GetIRCount(Name, false)); SetTy(new PtrTyIR(Ty));
		string init = "zeroinitializer";
		if (undef) init = "undef";
		//SetEmit(GetAddr()+" = global "+Ty->GetEmit()+" "+init+", align "+Ty->GetAlign()); TODO
		SetEmit(GetAddr()+" = global "+Ty->GetEmit()+" "+init);
	}
};

// Глобальная константа
class ConstIR : public BaseIR {
public:
	ConstIR(BaseIR *Val, string Name) {
		SetAddr("@"+GetIRCount(Name, false)); SetTy(Val->GetTy());
		SetEmit(GetAddr()+" = private unnamed_addr constant "+GetTy()->GetEmit()+" "+Val->GetEmit());
		SetTy(new PtrTyIR(Val->GetTy()));
	}
};

// Изменение значения переменной
class StoreIR : public BaseIR {
public:
	StoreIR(BaseIR *V1, BaseIR *V2) {
		//SetEmit("  store "+V1->GetTy()->GetEmit()+" "+V1->GetAddr()+", "+V2->GetTy()->GetEmit()+" "+V2->GetAddr()+", align "+V1->GetTy()->GetAlign()); TODO
		SetEmit("  store "+V1->GetTy()->GetAddr()+" "+V1->GetAddr()+", "+V2->GetTy()->GetAddr()+" "+V2->GetAddr());
		SetAddr(V2->GetAddr()); SetTy(V2->GetTy());
	}
};

// Бинарные операции
// Сложение
class AddIR : public BaseIR {
public:
	AddIR(BaseIR *A, BaseIR *B, const bool nsw = false, const bool nuw = false) {
		SetAddr("%"+GetIRCount("add")); SetTy(A->GetTy());
		string nw = " ";
		
		if (nsw) nw += "nsw ";
		else if (nuw) nw += "nuw ";
		else nw = "";
		
		SetEmit("  "+GetAddr()+" = add "+nw+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

class FAddIR : public BaseIR {
public:
	FAddIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("add")); SetTy(A->GetTy());
		SetEmit("  "+GetAddr()+" = fadd "+FastMath+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

// Вычитание
class SubIR : public BaseIR {
public:
	SubIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("sub")); SetTy(A->GetTy());
		SetEmit("  "+GetAddr()+" = sub "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

class FSubIR : public BaseIR {
public:
	FSubIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("sub")); SetTy(A->GetTy());
		SetEmit("  "+GetAddr()+" = fsub "+FastMath+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

// Умножение
class MulIR : public BaseIR {
public:
	MulIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("mul")); SetTy(A->GetTy());
		SetEmit("  "+GetAddr()+" = mul "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

class FMulIR : public BaseIR {
public:
	FMulIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("mul")); SetTy(A->GetTy());
		SetEmit("  "+GetAddr()+" = fmul "+FastMath+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

// Деление
class DivIR : public BaseIR {
public:
	DivIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("div")); SetTy(A->GetTy());
		SetEmit("  "+GetAddr()+" = sdiv "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

class UDivIR : public BaseIR { // Unsigned div
public:
	UDivIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("div")); SetTy(A->GetTy());
		SetEmit("  "+GetAddr()+" = udiv "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

class FDivIR : public BaseIR {
public:
	FDivIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("div")); SetTy(A->GetTy());
		SetEmit("  "+GetAddr()+" = fdiv "+FastMath+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

// Остаток
class RemIR : public BaseIR {
public:
	RemIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("rem")); SetTy(A->GetTy());
		SetEmit("  "+GetAddr()+" = srem "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

class URemIR : public BaseIR { // Unsigned rem
public:
	URemIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("rem")); SetTy(A->GetTy());
		SetEmit("  "+GetAddr()+" = urem "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

class FRemIR : public BaseIR { // Floating rem
public:
	FRemIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("rem")); SetTy(A->GetTy());
		SetEmit("  "+GetAddr()+" = frem "+FastMath+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

// Операторы сравнения

class ICmpIR : public BaseIR {
public:
	ICmpIR(BaseIR *A, BaseIR *B, BaseIR *Cond, int size = 1) {
		SetAddr("%"+GetIRCount("cmp"));
		
		if (size == 1) SetTy(new IntTyIR(1));
		else SetTy(new VecTyIR(size, new IntTyIR(1)));
		
		SetEmit("  "+GetAddr()+" = icmp "+Cond->GetEmit()+" "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

class FCmpIR : public BaseIR {
public:
	FCmpIR(BaseIR *A, BaseIR *B, BaseIR *Cond, int size = 1) {
		SetAddr("%"+GetIRCount("cmp"));

		if (size == 1) SetTy(new IntTyIR(1));
		else SetTy(new VecTyIR(size, new IntTyIR(1)));

		SetEmit("  "+GetAddr()+" = fcmp "+Cond->GetEmit()+" "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

// CONDS

// '='
class EQIR  : public BaseIR { public: EQIR()  { SetEmit("eq");  } };
class OEQIR : public BaseIR { public: OEQIR() { SetEmit("oeq"); } };

// '<>'
class NEIR  : public BaseIR { public: NEIR()  { SetEmit("ne");  } };
class ONEIR : public BaseIR { public: ONEIR() { SetEmit("one"); } };

// '<'
class SLTIR : public BaseIR { public: SLTIR() { SetEmit("slt"); } };
class OLTIR : public BaseIR { public: OLTIR() { SetEmit("olt"); } };
class ULTIR : public BaseIR { public: ULTIR() { SetEmit("ult"); } };

// '>'
class SGTIR : public BaseIR { public: SGTIR() { SetEmit("sgt"); } };
class OGTIR : public BaseIR { public: OGTIR() { SetEmit("ogt"); } };
class UGTIR : public BaseIR { public: UGTIR() { SetEmit("ugt"); } };

// '<='
class SLEIR : public BaseIR { public: SLEIR() { SetEmit("sle"); } };
class OLEIR : public BaseIR { public: OLEIR() { SetEmit("ole"); } };
class ULEIR : public BaseIR { public: ULEIR() { SetEmit("ule"); } };

// '>='
class SGEIR : public BaseIR { public: SGEIR() { SetEmit("sge"); } };
class OGEIR : public BaseIR { public: OGEIR() { SetEmit("oge"); } };
class UGEIR : public BaseIR { public: UGEIR() { SetEmit("uge"); } };

// And
class AndIR : public BaseIR {
public:
	AndIR(BaseIR *A, BaseIR *B, const bool Bool = false) {
		if (Bool) SetTy(new IntTyIR(1));
		else SetTy(A->GetTy());
		
		SetAddr("%"+GetIRCount("and"));
		SetEmit("  "+GetAddr()+" = and "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

// Or
class OrIR : public BaseIR {
public:
	OrIR(BaseIR *A, BaseIR *B, const bool Bool = false) {
		if (Bool) SetTy(new IntTyIR(1));
		else SetTy(A->GetTy());
		
		SetAddr("%"+GetIRCount("or"));
		SetEmit("  "+GetAddr()+" = or "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

// xor
class XorIR : public BaseIR {
public:
	XorIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("xor")); SetTy(A->GetTy());	
		SetEmit("  "+GetAddr()+" = xor "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

// shl
class ShlIR : public BaseIR {
public:
	ShlIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("shl")); SetTy(A->GetTy());		
		SetEmit("  "+GetAddr()+" = shl "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

// shr
class ShrIR : public BaseIR {
public:
	ShrIR(BaseIR *A, BaseIR *B) {
		SetAddr("%"+GetIRCount("shr")); SetTy(A->GetTy());		
		SetEmit("  "+GetAddr()+" = ashr "+A->GetTy()->GetEmit()+" "+A->GetAddr()+", "+B->GetAddr());
	}
};

// Преобразование типов
// Целое число в вещественное
class SIToFP : public BaseIR {
public:
	SIToFP(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = sitofp "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

// Вещественное число в целое
class FPToSI : public BaseIR {
public:
	FPToSI(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = fptosi "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

// Вещественное число в безнаковое целое
class FPToUI : public BaseIR {
public:
	FPToUI(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = fptoui "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

// Безнаковое целое число в вещественное
class UIToFP : public BaseIR {
public:
	UIToFP(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = uitofp "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

// 


// Указатель в число
class PtrToInt : public BaseIR {
public:
	PtrToInt(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = ptrtoint "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

class BitcastIR : public BaseIR {
public:
	BitcastIR(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = bitcast "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

// Число в указатель
class IntToPtrIR : public BaseIR {
public:
	IntToPtrIR(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = inttoptr "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

class PtrToIntIR : public BaseIR {
public:
	PtrToIntIR(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = ptrtoint "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

// sext low to high
class SExtIR : public BaseIR {
public:
	SExtIR(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = sext "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

// zext i1 to high
class ZExtIR : public BaseIR {
public:
	ZExtIR(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = zext "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

// trunc high to low
class TruncIR : public BaseIR {
public:
	TruncIR(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = trunc "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

// floating convert
class FPExtIR : public BaseIR {
public:
	FPExtIR(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = fpext "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

class FPTruncIR : public BaseIR {
public:
	FPTruncIR(BaseIR *A, BaseIR *Ty) {
		SetAddr("%"+GetIRCount("cast")); SetTy(Ty);
		SetEmit("  "+GetAddr()+" = fptrunc "+A->GetTy()->GetEmit()+" "+A->GetAddr()+" to "+Ty->GetEmit());
	}
};

//
// Метки
class LabelIR : public BaseIR {
public:
	LabelIR(const string name) {
		SetAddr(GetIRCount(name));
		SetEmit("\n"+GetAddr()+":");
		SetAddr("%"+GetAddr());
	}
};

// Безусловный переход
class BrIR : public BaseIR {
public:
	BrIR(BaseIR *L, string metadata = "") {
		if (metadata != "") metadata = ", "+metadata;
		SetEmit("  br label "+L->GetAddr()+metadata);
	}
};

// Условный переход
class CondBrIR : public BaseIR {
public:
	CondBrIR(BaseIR *Cond, BaseIR *L1, BaseIR *L2, string metadata = "") {
		if (metadata != "") metadata = ", "+metadata;
		SetEmit("  br "+Cond->GetTy()->GetEmit()+" "+Cond->GetAddr()+", label "+L1->GetAddr()+", label "+L2->GetAddr()+metadata);
	}
};

// GEP
class GEPInBoundsIR : public BaseIR {
public:
	GEPInBoundsIR(BaseIR *V1, BaseIR *V2, BaseIR *V3, const bool low = false, const bool low1 = false) {
		string name;
		
		//for (int i = 1; i < V1->GetAddr().length(); i++)
		//	name += V1->GetAddr().c_str()[i];
		
		name = "cast";
		SetAddr("%"+GetIRCount(name));
		//
		if (low) SetTy(V1->GetTy());
		else if (low1) SetTy(V1->GetTy()->GetTy());
		else SetTy(new PtrTyIR(V1->GetTy()->GetTy()->GetTy()));
		
		if (V3 != 0) {
			SetEmit("  "+GetAddr()+" = getelementptr inbounds "+V1->GetTy()->GetEmit()+" "+V1->GetAddr()+", "
						+V2->GetTy()->GetEmit()+" "+V2->GetAddr()+", "+V3->GetTy()->GetEmit()+" "+V3->GetAddr());
		} else {
			SetEmit("  "+GetAddr()+" = getelementptr inbounds "+V1->GetTy()->GetEmit()+" "+V1->GetAddr()+", "
						+V2->GetTy()->GetEmit()+" "+V2->GetAddr());
		}
	
	}
};

// Vector Operations
//'extractelement' Instruction
class ExtractElementIR : public BaseIR {
public:
	ExtractElementIR(BaseIR *V1, BaseIR *V2, const bool low = false) {
		string name;
		
		//for (int i = 1; i < V1->GetAddr().length(); i++)
		//	name += V1->GetAddr().c_str()[i];
		
		name = "element";
		//
		if (low) { SetAddr("%"+GetIRCount(name)); SetTy(V1->GetTy()); }
		else { SetAddr("%"+GetIRCount(name)); SetTy(V1->GetTy()->GetTy()); }
		
		SetEmit("  "+GetAddr()+" = extractelement "+V1->GetTy()->GetEmit()+" "+V1->GetAddr()+", "
					+V2->GetTy()->GetEmit()+" "+V2->GetAddr());
	}
};

//'insertelement' Instruction
class InsertElementIR : public BaseIR {
public:
	InsertElementIR(BaseIR *V1, BaseIR *V2, BaseIR *V3) {
		string name;
		
		//for (int i = 1; i < V1->GetAddr().length(); i++)
		//	name += V1->GetAddr().c_str()[i];
		
		name = "insert";
		//
		SetAddr("%"+GetIRCount(name)); SetTy(V1->GetTy());
		SetEmit("  "+GetAddr()+" = insertelement "+V1->GetTy()->GetEmit()+" "+V1->GetAddr()+", "
					+V2->GetTy()->GetEmit()+" "+V2->GetAddr()+", "+V3->GetTy()->GetEmit()+" "+V3->GetAddr());
	}
};

//'shufflevector' Instruction
class ShuffleVectorIR : public BaseIR {
public:
	ShuffleVectorIR(BaseIR *V1, BaseIR *V2, BaseIR *V3) {
		string name;
		
		//for (int i = 1; i < V1->GetAddr().length(); i++)
		//	name += V1->GetAddr().c_str()[i];
		
		name = "insert";
		//
		SetAddr("%"+GetIRCount(name)); SetTy(V1->GetTy());
		SetEmit("  "+GetAddr()+" = shufflevector "+V1->GetTy()->GetEmit()+" "+V1->GetAddr()+", "
					+V2->GetTy()->GetEmit()+" "+V2->GetAddr()+", "+V3->GetTy()->GetEmit()+" "+V3->GetAddr());
	}
};

// Inline Assembler
class ASMIR : public BaseIR {
public:
	ASMIR(const string ASM, BaseIR *Metadata) {
		SetEmit("  call void asm sideeffect \""+ASM+"\", \"~{dirflag},~{fpsr},~{flags}\"() nounwind, !srcloc "+Metadata->GetEmit());
	}
};

// Arg
class ArgIR : public BaseIR {
public:
	ArgIR(BaseIR *Ty, const string name, bool ret = false) {
		SetTy(Ty); SetAddr("%"+GetIRCount(name+".arg"));
		
		if (!ret) SetEmit(Ty->GetAddr()+" "+GetAddr());
		else SetEmit(Ty->GetAddr()+" sret "+GetAddr());
	}
};

// Args
class ArgsIR : public BaseIR {
public:
	ArgsIR(vector<BaseIR*> Args) {
		string args = "";
		
		for (int i = 0; i < Args.size(); i++) {
			args += Args[i]->GetEmit();
			if (i != Args.size()-1) args += ", ";
		}
		
		SetEmit(args);
	}
};

// Structure type
class StructTyIR : public BaseIR {
public:
	StructTyIR(vector<BaseIR*> Vars, string Name) {
		string argsty = "";
		
		//
		for (int i = 0; i < Vars.size(); ++i) {
			string  ptr = "";
			BaseIR *ty = Vars[i]->GetTy()->GetTy();
			//
			argsty += ty->GetEmit()+ptr;
			
			if (i != Vars.size()-1) argsty += ", ";
		}
		
		if (Vars.size() > 0) SetEmit("{"+argsty+"}");
		else SetEmit("{i8*}");

		SetTy(this);
	}
};

// Packed type
class PackedTyIR : public BaseIR {
public:
	PackedTyIR(BaseIR* Ty) {
		SetEmit("<"+Ty->GetEmit()+">");
		SetTy(this);
	}
};

// Prototype
class ProtoIR : public BaseIR {
public:
	ProtoIR(BaseIR *Ty, vector<BaseIR*> Args) {
		SetTy(Ty);
		string argsty = "";
		
		//
		for (int i = 0; i < Args.size(); ++i) {
			string  ptr = "";
			BaseIR *ty = Args[i]->GetTy();
			//
			/*while (dynamic_cast<ArrTyIR*>(ty)) { // ty is ArrTyIR
				ptr += "*";
				ty = ty->GetTy();
			}*/
			//
			argsty += ty->GetAddr()+ptr;
			
			if (i != Args.size()-1) argsty += ", ";
		}
		
		/*if (Args.size() > 0)*/ SetEmit("("+argsty+")");
	}
};

// Prototype Type
class ProtoTyIR : public BaseIR {
public:
	ProtoTyIR(BaseIR *Ty, vector<BaseIR*> Args) {
		SetTy(Ty);
		SetAlign("8");
		string argsty = "";
		
		//
		for (int i = 0; i < Args.size(); ++i) {
			string  ptr = "";
			BaseIR *ty = Args[i]->GetTy();
			//
			/*while (dynamic_cast<ArrTyIR*>(ty)) { // ty is ArrTyIR
				ptr += "*";
				ty = ty->GetTy();
			}*/
			//
			argsty += ty->GetAddr()+ptr;
			
			if (i != Args.size()-1) argsty += ", ";
		}
		
		/*if (Args.size() > 0)*/ SetEmit(Ty->GetAddr()+" ("+argsty+")*"); // !
	}
	
	//
	ProtoTyIR(ProtoIR *Prot) {
		SetTy(Prot->GetTy());
		SetAlign(Prot->GetAlign());
		SetEmit(Prot->GetTy()->GetAddr()+" "+Prot->GetEmit());
	}
};

// Attributes

// Calling Conventions
class FastCCIR : public BaseIR { public: FastCCIR()  { SetEmit("fastcc"); } }; // The fast calling convention
class CCCIR    : public BaseIR { public: CCCIR()     { SetEmit("ccc");    } }; // The C calling convention
class ColdCCIR : public BaseIR { public: ColdCCIR()  { SetEmit("coldcc"); } }; // The cold calling convention
class CCIR     : public BaseIR { public: CCIR(int n) { SetEmit("cc "+IntToStr(n)); } }; // GHC convention

// Linkage Types
// dllimport
class DLLImportIR : public BaseIR {
public:
	DLLImportIR() {
		SetEmit("dllimport");
	}
};

// dllexport
class DLLExportIR : public BaseIR {
public:
	DLLExportIR() {
		SetEmit("dllexport");
	}
};

// linkonce_odr
class LinkonceODRIR : public BaseIR {
public:
	LinkonceODRIR() {
		SetEmit("linkonce_odr");
	}
};

// Attrs
class AttrsIR : public BaseIR {
public:
	AttrsIR(vector<BaseIR*> Attrs) {
		string attrs = "";
		
		for (int i = 0; i < Attrs.size(); i++) {
			attrs += Attrs[i]->GetEmit()+" ";
		}
		
		SetEmit(attrs);
	}
};

// Объявление функци
class DeclareIR : public BaseIR {
public:
	DeclareIR(const string Name, BaseIR *Proto, BaseIR *CC = 0, BaseIR *Attr = 0) {
		SetAddr("@"+GetIRCount(Name)); SetTy(Proto->GetTy());
		SetEmit("declare ");
		
		if (CC != 0) SetEmit(GetEmit()+CC->GetEmit()+" ");

		if (Proto != 0) SetEmit(GetEmit()+GetTy()->GetEmit()+" "+GetAddr()+Proto->GetEmit());
		else SetEmit(GetEmit()+GetTy()->GetEmit()+" "+GetAddr()+"()");
		
		if (Attr != 0) SetEmit(GetEmit()+" "+Attr->GetEmit()+" ");
		
		//SetEmit("declare "+GetTy()->GetEmit()+" "+GetAddr()+Proto->GetEmit());
	}
};

// Call
class CallIR : public BaseIR {
public:
	CallIR(const string Name, BaseIR *Proto, vector<BaseIR*> Args, BaseIR *CC = 0, bool SRet = false, BaseIR *Ty = 0) {
		bool voidty = false;
		
		if (Ty != 0) SetTy(Ty);
		else SetTy(Proto->GetTy());
		
		if (dynamic_cast<VoidTyIR*>(Proto->GetTy())) voidty = true;
		if (!voidty) SetAddr("%"+GetIRCount("call"));
		
		//SetEmit("  "+GetAddr()+" = call"+IntToStr(GetIRCall())+" "+GetTy()->GetEmit()+" "+Proto->GetEmit());
		//
		string args   = "";
		bool isDynArg = false;
		//
		for (int i = 0; i < Args.size(); ++i) {
			string  ptr = "";
			BaseIR *argty = Args[i]->GetTy();
			//
			if (dynamic_cast<ArgsTyIR*>(argty)) isDynArg = true;
			
			/*while (dynamic_cast<ArrTyIR*>(argty)) { // ty is ArrTyIR
				ptr += "*";
				argty = argty->GetTy();
			}*/
			//
			//args   += argty->GetAddr()+ptr+" "+Args[i]->GetAddr();

			if (i == 0) {
				if (!SRet) args += argty->GetAddr()+ptr+" "+Args[i]->GetAddr();
				else args += argty->GetAddr()+ptr+" sret "+Args[i]->GetAddr();
			} else args += argty->GetAddr()+ptr+" "+Args[i]->GetAddr();
			
			if (i != Args.size()-1) args += ", ";
		}
		
		//if (Args.size() > 0) args = "("+args+")";
		//else args = "";
		string proto = Proto->GetEmit()+" *";
		string cc = "";
		
		if (CC != 0) cc = CC->GetEmit()+" ";
		
		if (!voidty) SetEmit(GetAddr()+" = ");
		//else proto = "";
		if (Args.size() <= 0) proto = "";
		//SetEmit("  "+GetEmit()+"call "+GetTy()->GetEmit()+" @"+Name+"("+args+")");
		//if (!isDynArg) SetEmit("  "+GetEmit()+"call "+GetTy()->GetEmit()+" @"+Name+"("+args+")");
		/*else*/ SetEmit("  "+GetEmit()+"call "+cc+Proto->GetTy()->GetAddr()+proto+" @"+Name+"("+args+")");
	}
};

//
class CallVarIR : public BaseIR {
public:
	CallVarIR(BaseIR *Id, BaseIR *Proto, vector<BaseIR*> Args, BaseIR *CC = 0, bool SRet = false) {
		bool voidty = false;
		
		if (Proto != 0) SetTy(Proto->GetTy());
		//else SetTy(Id->GetTy());
		else SetTy(Id->GetTy()->GetTy());
		
		if (dynamic_cast<VoidTyIR*>(GetTy())) voidty = true;
		if (!voidty) SetAddr("%"+GetIRCount("call"));
		
		//
		string args   = "";
		string proto  = "";
		bool isDynArg = false;
		//
		for (int i = 0; i < Args.size(); ++i) {
			string  ptr = "";
			BaseIR *argty = Args[i]->GetTy();
			//
			if (dynamic_cast<ArgsTyIR*>(argty)) isDynArg = true;

			//
			if (i == 0) {
				if (!SRet) args += argty->GetAddr()+ptr+" "+Args[i]->GetAddr();
				else args += argty->GetAddr()+ptr+" sret "+Args[i]->GetAddr();
			} else args += argty->GetAddr()+ptr+" "+Args[i]->GetAddr();
			//
			proto += argty->GetAddr()+ptr;
			
			if (i != Args.size()-1) { args += ", "; proto += ", "; }
		}
		
				
		//else proto = " "+Id->GetTy()->GetEmit()+" ";
		
		string cc = "";
		
		if (CC != 0) cc = CC->GetEmit()+" ";
		//if (voidty)  proto = "";		
		if (!voidty) SetEmit(GetAddr()+" = ");
		//else proto = "";
		if (Args.size() <= 0) proto = "";
		else proto = " ("+proto+")";

		if (proto != "") proto += " *";
		if (Proto != 0) proto = Proto->GetEmit()+" *";

		SetEmit("  "+GetEmit()+"call "+cc+GetTy()->GetAddr()+proto+" "+Id->GetAddr()+"("+args+")");
	}
};

// Td
class TdIR : public BaseIR {
public:
	TdIR(BaseIR *Val, BaseIR *Label) {
		SetTy(Val->GetTy());
		SetEmit(GetTy()->GetEmit()+" "+Val->GetEmit()+", label "+Label->GetAddr());
	}
};

// Case
class CaseIR : public BaseIR {
public:
	CaseIR(BaseIR *Id, vector<BaseIR*> Tds, BaseIR *AfterL) {
		SetEmit("\n  switch "+Id->GetTy()->GetEmit()+" "+Id->GetAddr()+", label "+AfterL->GetAddr()+" [");
		
		for (int i = 0; i < Tds.size(); ++i) {
			SetEmit(GetEmit()+"\n    "+Tds[i]->GetEmit());
		}
		
		SetEmit(GetEmit()+"\n  ]");
	}
};

#endif
