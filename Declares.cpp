#ifndef CPP_DECLARES
#define CPP_DECLARES 1

using namespace std;

// Объявление функции
void DeclareFunc(const char *Name, const char *LLVMName, BaseIR *Ty, vector<BaseIR*> Args) {
	struct overloads ovr;
	//
	ovr.Prot = new ProtoIR(Ty, Args);
	ovr.Name = LLVMName;
	ovr.Func = new CustomIR("");
	ovr.Func->SetAddr("@"+ovr.Name);
	ovr.Constructor = false;
	ovr.Destructor  = false;
	
	FuncDict = &MainFuncDict;
	//
	(*FuncDict)[Name].Overloads.push_back(ovr);
	(*FuncDict)[Name].Def     = true;
	(*FuncDict)[Name].Proto   = new ProtoIR(Ty, Args);
	(*FuncDict)[Name].inllvm  = true;
	//
	BaseIR *Declare = new DeclareIR(LLVMName, (*FuncDict)[Name].Proto);
	ManagerIR->Insert(Declare, End);
}
//
void DeclarePrintf() {
	vector<BaseIR*> Args;					 	  // Аргументы
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));  // Тип аргумента i8*
	Args.push_back(new ArgsTyIR());		   		  // Тип аргумента ...
	
	DeclareFunc("write", "printf", new IntTyIR(32), Args);
	//
}
//
void DeclareScanf() {
	vector<BaseIR*> Args;					 	  // Аргументы
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));  // Тип аргумента i8*
	Args.push_back(new ArgsTyIR());		   		  // Тип аргумента ...
	
	DeclareFunc("read", "scanf", new IntTyIR(32), Args);
	//
}
//
void DeclarePuts() {
	vector<BaseIR*> Args;					 // Аргументы
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));			 // Тип аргумента i8*
	
	DeclareFunc("writeln", "put1s", new IntTyIR(32), Args);	
}

// Exceptions Handler
void DeclareAllocExcept() {
	vector<BaseIR*> Args;
	Args.push_back(new IntTyIR(64));
	
	DeclareFunc("llvm.allocate_exception", "__cxa_allocate_exception", new aPtrTyIR(new IntTyIR(8)), Args);	
}

void DeclarePersonalityV0() {
	vector<BaseIR*> Args;
	Args.push_back(new ArgsTyIR());
	
	DeclareFunc("llvm.personality_v0", "__gxx_personality_v0", new IntTyIR(32), Args);	
}

void DeclareFreeExcept() {
	vector<BaseIR*> Args;
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	
	DeclareFunc("llvm.free_exception", "__cxa_free_exception", new VoidTyIR(), Args);	
}

void DeclareThrow() {
	vector<BaseIR*> Args;
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	
	DeclareFunc("llvm.throw", "__cxa_throw", new VoidTyIR(), Args);	
}

void DeclareBeginCatch() {
	vector<BaseIR*> Args;
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	
	DeclareFunc("llvm.begin_catch", "__cxa_begin_catch", new aPtrTyIR(new IntTyIR(8)), Args);	
}

void DeclareEndCatch() {
	vector<BaseIR*> Args;
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	
	DeclareFunc("llvm.end_catch", "__cxa_end_catch", new VoidTyIR(), Args);	
}
//

// Mem
// new pointer
void DeclareNew() {
	//
	vector<BaseIR*> Args;

	if (arch == x32) {
		Args.push_back(new IntTyIR(32));
		DeclareFunc("new", "new", new aPtrTyIR(new IntTyIR(8)), Args);
	} else

	if (arch == x64) {
		Args.push_back(new IntTyIR(64));
		DeclareFunc("new", "new", new aPtrTyIR(new IntTyIR(8)), Args);
	}
}

// delete pointer (dispose)
void DeclareDelete() {
	vector<BaseIR*> Args;
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	
	DeclareFunc("dispose", "delete", new VoidTyIR(), Args);	
}
//
void DeclareMemset() {
	vector<BaseIR*> Args;
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	Args.push_back(new IntTyIR(8));
	Args.push_back(new IntTyIR(64));
	Args.push_back(new IntTyIR(32));
	Args.push_back(new IntTyIR(1));
	
	DeclareFunc("llvm.memset.p0i8.i64", "llvm.memset.p0i8.i64", new VoidTyIR(), Args);	
}
//
void DeclareMemcpy() {
	/*vector<BaseIR*> Args;
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	Args.push_back(new IntTyIR(64));
	Args.push_back(new IntTyIR(32));
	Args.push_back(new IntTyIR(1));
	
	DeclareFunc("_llvm_memcpy", "llvm.memcpy.p0i8.p0i8.i64", new VoidTyIR(), Args);*/
}

//
void DeclareLifetimeStart() {
	vector<BaseIR*> Args;
	Args.push_back(new IntTyIR(64));
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	
	DeclareFunc("_llvm_lifetime_start", "llvm.lifetime.start", new VoidTyIR(), Args);
}

//
void DeclareLifetimeEnd() {
	vector<BaseIR*> Args;
	Args.push_back(new IntTyIR(64));
	Args.push_back(new aPtrTyIR(new IntTyIR(8)));
	
	DeclareFunc("_llvm_lifetime_end", "llvm.lifetime.end", new VoidTyIR(), Args);
}

//

inline void DeclareType(string TypeName, ExprAST *Ty) {
	(*TypeDict)[TypeName].Def    = true;
	(*TypeDict)[TypeName].Type   = Ty;
	(*TypeDict)[TypeName].TypeIR = Ty->Codegen();
}
//
void DeclareStdTypes() {
	//
	// Integer
	DeclareType("bool"    , new IntTyExprAST(1));
	DeclareType("shortint", new IntTyExprAST(8));
	DeclareType("smallint", new IntTyExprAST(16));
	DeclareType("int" 	  , new IntTyExprAST(32));
	DeclareType("longint" , new IntTyExprAST(32));
	DeclareType("int64"   , new IntTyExprAST(64));
	
	// Unsigned
	DeclareType("char"    , new IntTyExprAST(8 , true));
	DeclareType("byte"    , new IntTyExprAST(8 , true));
	DeclareType("wchar"   , new IntTyExprAST(16, true));
	DeclareType("word"    , new IntTyExprAST(16, true));
	DeclareType("widechar", new IntTyExprAST(16, true));
	DeclareType("cardinal", new IntTyExprAST(32, true));
	DeclareType("longword", new IntTyExprAST(32, true));
	DeclareType("uint64"  , new IntTyExprAST(64, true));

	if (arch == x32) DeclareType("size_t", new IntTyExprAST(32, true));
	if (arch == x64) DeclareType("size_t", new IntTyExprAST(64, true));

	if (arch == x32) DeclareType("uintptr_t", new IntTyExprAST(32, true));
	if (arch == x64) DeclareType("uintptr_t", new IntTyExprAST(64, true));

	if (arch == x32) DeclareType("off_t", new IntTyExprAST(32));
	if (arch == x64) DeclareType("off_t", new IntTyExprAST(64));
	
	// Float
	DeclareType("float" , new FloatTyExprAST(32));
	DeclareType("double", new FloatTyExprAST(64));
	
	// Pointer
	DeclareType("ptr", new PointerTyExprAST());
	//
}

#endif
