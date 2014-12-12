//===-----------------------------------------------------------===//
// Aphalin Compiler v1.0. 2012-2014 г.
//===-----------------------------------------------------------===//

// Main
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "string.h"
#include "math.h"
#include <sys/types.h>
#include <sys/stat.h>

static int PtrSize = 32;

// Основные модули компилятора
#include "Type.h"
#include "AST.h"
#include "Error.h"
#include "Lexer.h"

lexer *mLexer;

#include "Codegen.h"
#include "Parser.h"
#include "Codegen/Expr.cpp"

#include "Error.cpp"

//===----------------------------------------------------------------------===//
// Compiler
//===----------------------------------------------------------------------===//

// compiler - Компилятор
class compiler {
public:
	parser* Parser; // Parser - Синтаксический анализатор

	void Compile(); // Компиляция

	 compiler() { Parser = new parser(); }
	~compiler() { delete Parser; }
};
//
void compiler::Compile() {
	// Задаём бинарные операторы.
	Parser->BinopPrecedence[tok_le]  = 1;
	Parser->BinopPrecedence[tok_ge]  = 1;
	Parser->BinopPrecedence[tok_ne]  = 1;
	Parser->BinopPrecedence['>']     = 1;
	Parser->BinopPrecedence['<']     = 1;
	Parser->BinopPrecedence[tok_eq]  = 1;
	Parser->BinopPrecedence[tok_in]  = 1;
	Parser->BinopPrecedence[tok_is]  = 1;
	//
	Parser->BinopPrecedence['+']     = 2;
	Parser->BinopPrecedence['-']     = 2;
	Parser->BinopPrecedence['|'] = 3;
	Parser->BinopPrecedence[tok_xor] = 2;
	Parser->BinopPrecedence['*']     = 3;
	Parser->BinopPrecedence['/']     = 3;
	Parser->BinopPrecedence['%']     = 3;
	//Parser->BinopPrecedence[tok_div] = 3;
	//Parser->BinopPrecedence[tok_mod] = 3;
	Parser->BinopPrecedence['&'] = 3;
	Parser->BinopPrecedence[tok_shl] = 3;
	Parser->BinopPrecedence[tok_shr] = 3;
	Parser->BinopPrecedence[tok_as]  = 4;
	Parser->BinopPrecedence[tok_or]	 = 5;
	Parser->BinopPrecedence[tok_and] = 5; // Наивысший приоритет
	
	//*
	//Parser->Lexer->GetNextToken();
	//Parser->Lexer->TokenStack.clear();
	Parser->MainLoop();
}

//
string SumS(const string a, const string b) {
	string result = "";
	unsigned int m,n;
	int x = 0;
	int y = 0;
	int hi = 0;
	int hj = 0;
	//
	if (a.length() > b.length()) {
		hi = a.length()-b.length();
	}
	
	if (a.length() < b.length()) {
		hj = b.length()-a.length();
	}
	
	//
	if (a.length() > b.length()) n = a.length();
	else n = b.length();
	//
	char *res = (char*)malloc(sizeof(char)*(n+2));
	res[n+1] = '\0';
	//
	for (int i = n-1; i >= 0; i--) {
		int c, d;
		
		if (i+hi >= a.length()) c = 48;
		else c = a[hi+i];
		
		if (i+hj >= b.length()) d = 48;
		else d = b[hj+i];
		
		x = c+(d-48)+y;		
		y = 0;
		
		if (x >= 58) {
			x = x-10;
			y = 1;
		}

		char ch = x;
		
		result += ch;
		res[i+1] = ch;
	}
	
	if (y == 1) res[0] = '1';
	else {
		memmove(res, res+1, sizeof(char)*(n));
		res[n] = '\0';
	}
	
	return res;
}

#ifdef _WIN64
	#define _WIN32
#endif

//===----------------------------------------------------------------------===//
// Main driver code (Код основной программы)
//===----------------------------------------------------------------------===//

int main(int argc, char *argv[]) {
	NullTy = CreateASTType(TNONE, "", 32, 0);
	BoolTy = CreateASTType(TINT, "boolean", 1, 0);
	ExprTy = NullTy;
	VarStack.push_back(&ExternVars);
	
	// Читаем флаги
	OutName = "a.out";
	IntelSyntax = false;
	string errfn = "lang/en.txt";

	//

	#ifdef _WIN32 
		platform = win_os;
		arch = x32;
		PtrSize = 32;
		iPtrSize = "i32";
	#endif

	#ifdef _WIN64 || __amd64__
		platform = win_os;
		arch = x64;
		PtrSize = 64;
		iPtrSize = "i64";
	#endif

	#ifdef __x86_64__ || __ppc64__
		platform = linux_os;
		arch = x64;
		PtrSize = 64;
		iPtrSize = "i64";
	#endif

	#ifdef __APPLE__
		platform = apple_osx;
	#endif

	#ifdef __linux__
		platform = linux_os;
	#endif

	char curdir[256];
	getcwd(&curdir[0], 256);
	string currentDirectory = curdir;
	
	//
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-o") == 0) {
			i++;
			
			if (i < argc) OutName = argv[i];
			else return Error<int>(no_input_file);
			
			continue;
		} else if (strcmp(argv[i], "emit=asm") == 0) {
			EmitASM  = true;
			EmitLLVM = false;
			continue;
		} else if (strcmp(argv[i], "emit=llvm") == 0) {
			EmitLLVM = true;
			EmitASM  = false;
			continue;
		} else if (strcmp(argv[i], "asm=intel") == 0) {
			IntelSyntax = true;
			continue;
		} else if (strcmp(argv[i], "asm=att") == 0) {
			IntelSyntax = false;
			continue;
		} else if (strcmp(argv[i], "math=fast") == 0) {
			FastMath = "fast ";
			continue;
		} else if (strcmp(argv[i], "math=opencl") == 0) {
			ClcMath = true;
			continue;
		} else if (strcmp(argv[i], "arch=x32") == 0) {
			arch = x32;
			PtrSize = 32;
			iPtrSize = "i32";
			continue;

		} else if (strcmp(argv[i], "arch=x64") == 0) {
			arch = x64;
			PtrSize = 64;
			iPtrSize = "i64";
			continue;
		} else if (strcmp(argv[i], "-v") == 0) {
			Vorbis = true;
			continue;
		} else if (strcmp(argv[i], "-g") == 0) {
			DebugMode = true;
			continue;
		// Platforms
		} else if (strcmp(argv[i], "os=win") == 0) {
			platform = win_os;
			continue;
			
		} else if (strcmp(argv[i], "os=linux") == 0) {
			platform = linux_os;
			continue;
			
		} else if (strcmp(argv[i], "os=osx") == 0) {
			platform = apple_osx;
			continue;
			
		} else if (strcmp(argv[i], "os=android") == 0) {
			platform = android;
			continue;
			
		} else if (strcmp(argv[i], "os=ios") == 0) {
			platform = apple_ios;
			continue;
		
		//
		} else if (strcmp(argv[i], "-bie") == 0) {
			bie = true;
			continue;
		} else if (strcmp(argv[i], "-native") == 0) {
			Native = true;
			continue;
		} else if (argv[i][0] == 'l' & argv[i][1] == 'a' & argv[i][2] == 'n' & argv[i][3] == 'g' & argv[i][4] == '=') {
			errfn = "lang/";
			errfn += &argv[i][5];
			errfn += ".txt";
			continue;
		} else if (argv[i][0] == '-' & argv[i][1] == 'O') {
			if (isdigit(argv[i][2])) {
				char c[1];
				c[0] = argv[i][2];
				OptLevel = strtod(c, 0);
				
				continue;
			}
		}
		
		// Другие флаги
		if (argv[i][0] == '-') {
			Flags.push_back(argv[i]);
		} else {
			int n = strlen(argv[i]);
			if (argv[i][n-4] == '.') FileName = argv[i];
		}
	}
	
	CurFN = FileName;

	// Initialize macroprocessor
	if (arch == x64) MacroFlags += "-d x86_64 ";
	else MacroFlags += "-d x86 ";

	//
	if (platform == win_os   ) MacroFlags += "-d win ";
	if (platform == linux_os ) MacroFlags += "-d linux ";
	if (platform == apple_osx) MacroFlags += "-d macos ";
	if (platform == android  ) MacroFlags += "-d android ";
	if (platform == apple_ios) MacroFlags += "-d ios ";

	//
	
	if (FileName == "")
		return Error<int>(no_input_file);

	// Инициализация
	ManagerIR = new codegen();
	
	// Стандартные функции
	//
	//DeclarePrintf();
	//DeclareScanf();
	//DeclarePuts();
	//
	DeclareStdTypes();
	
	// Memory
	DeclareNew();
	DeclareDelete();
	//DeclareMemset();
	//DeclareMemcpy();
	DeclareLifetimeStart();
	DeclareLifetimeEnd();
	
	// Сообщения
	
	errf = fopen("log.txt", "w"); // Лог об ошибках
	LoadMessage(errfn);
	
	// Транслятор
	compiler* Compiler = new compiler();
	mLexer = Compiler->Parser->Lexer;
	mParser = Compiler->Parser;
	Compiler->Compile();

	// Если при генерации кода, не нашлось ошибок, то создаем бинарник
	/*if (NoErrors) {
		ManagerIR->Emit();
		ManagerIR->CloseF();
	}*/

	if (ManagerIR) {
		ManagerIR->Emit();
		//puts ("!!!");
		ManagerIR->CloseF();
		string cmd;
		//puts ("!!!");
		//exit (0);

		// links llvm files
		
		/*#ifdef _WIN32
			cmd = currentDirectory+"\\bin\\llvm-link.exe";
		#else
			cmd = currentDirectory+"/bin/llvm-link";
		#endif
		
		if (system((cmd+" a.out.ll libsll/mman0.ll -S -o a.link.ll").c_str()) == -1)
			return Error<int>(err_compile);*/
		
		#ifdef _WIN32
			cmd = currentDirectory+"\\bin\\llc.exe";
		#else
			cmd = currentDirectory+"/bin/llc";
		#endif
		
		if (!IntelSyntax) {
			if (system((cmd+" a.out.ll -x86-asm-syntax=att -o debug.S").c_str()) == -1)
				return Error<int>(err_compile);
		} else {
			if (system((cmd+" a.out.ll -x86-asm-syntax=intel -o debug.S").c_str()) == -1)
				return Error<int>(err_compile);
		}
		
		string opt = "";
		// Pre opt
		
		#ifdef _WIN32
			cmd = currentDirectory+"\\bin\\opt.exe";
			//string PollyLib = "LLVMPolly.dll";
		#else
			cmd = currentDirectory+"/bin/opt";
			//string PollyLib = "./LLVMPolly.so";
		#endif
		
		//opt = "-correlated-propagation -mem2reg -instcombine -loop-simplify -indvars -globalopt -globalopt -globalsmodref -adce";
		//if (Vorbis) puts((cmd+" a.out.ll "+opt+" -S -o a.preopt.ll").c_str());
		opt = "";
		
		if (system((cmd+" a.out.ll "+opt+" -S -o a.opt.ll").c_str()) == -1)
			return Error<int>(err_compile);
		
		if (OptLevel <= 3 & OptLevel > 0) {
			if (OptLevel > 0 & OptLevel < 3)
				opt = "-O"+IntToStr(OptLevel);
			
			if (OptLevel == 3)
				opt = " -O3 -mem2reg -loop-simplify -inline -indvars -globalopt -globaldce -adce -da -lcssa -memcpyopt -loop-unroll -loop-vectorize -vectorize-slp-aggressive -vectorize-loops -bb-vectorize -regalloc=fast -strip-dead-prototypes -memcpyopt";				
			
			string Polly = "-polly -polly-codegen -polly-opt-isl -polly-codegen-isl -polly-cloog ";
			Polly = "-O3 -polly -polly-codegen -enable-polly-openmp -polly-vectorizer=polly -strip";
			
			//if (OptLevel == 4)
			//	opt = Polly+opt;
			// S
			cmd = cmd+" a.out.ll "+opt+" -S -o a.opt.ll";
			
			if (Vorbis) puts(("\n"+cmd).c_str());
			if (system(cmd.c_str()) == -1)
				return Error<int>(err_compile);
			
			if (!Native) {
				#ifdef _WIN32
					cmd = currentDirectory+"\\bin\\llvm-as.exe";
				#else
					cmd = currentDirectory+"/bin/llvm-as";
				#endif
				
				// S
				cmd = cmd+" a.opt.ll -o core.bc";
				
				if (Vorbis) puts(cmd.c_str());
				if (system(cmd.c_str()) == -1)
					return Error<int>(err_compile);
			}
			//
		} //else OpError("too much optlv");
		//
		if (Native) {
			//cmd = cmdld+" a.opt.obj --sysroot=/ --build-id --no-add-needed --as-needed --eh-frame-hdr -m elf_x86_64 --hash-style=gnu -dynamic-linker /lib64/ld-linux-x86-64.so.2 -z relro /usr/lib/gcc/x86_64-linux-gnu/4.6/../../../x86_64-linux-gnu/crt1.o /usr/lib/gcc/x86_64-linux-gnu/4.6/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/4.6/crtbegin.o -L/usr/lib/gcc/x86_64-linux-gnu/4.6 -L/usr/lib/gcc/x86_64-linux-gnu/4.6/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/4.6/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/4.6/../../.. -lstdc++ -lm -lgcc_s -lgcc -lc -lgcc_s -lgcc /usr/lib/gcc/x86_64-linux-gnu/4.6/crtend.o /usr/lib/gcc/x86_64-linux-gnu/4.6/../../../x86_64-linux-gnu/crtn.o -static -lpthread -o "+OutName;
			//cmd = cmdld+" core.o --as-needed --eh-frame-hdr -m elf_x86_64 ";
			if (OptLevel > 0) {
				#ifdef _WIN32 
					cmd = currentDirectory+"\\bin\\llc.exe";
				#else
					cmd = currentDirectory+"/bin/llc";
				#endif
				//
				cmd = cmd+" a.opt.ll -o a.opt.S";
				
				if (Vorbis) puts(cmd.c_str());
				if (system(cmd.c_str()) == -1)
					return Error<int>(err_compile);
				//
			}

			string flags_str;

			for (int i = 0; i < Flags.size(); i++) {
				flags_str += Flags[i]+" ";
			}

			/*#ifdef _WIN32
				cmd = currentDirectory+"\\bin\\lld.exe";
			#else
				cmd = currentDirectory+"/bin/lld";
			#endif*/

			#ifdef _WIN32 
				string cmd = currentDirectory+"\\bin\\lld.exe";
			#else
				string cmd = currentDirectory+"/bin/lld";
			#endif
			
			//
			string optfile = "";
			//
			if (OptLevel == 0) optfile = "debug.S";
			else optfile = "a.opt.S";
			//
			cmd = cmd+" "+optfile+" -O3 "+flags_str+"-o "+OutName;

			if (Vorbis) puts(cmd.c_str());
			if (system(cmd.c_str()) == -1)
				return Error<int>(err_compile);
			
			/*cmd = "gcc a.opt.S ";
			
			for (int i = 0; Flags.size(); i++) {
				cmd += Flags[i]+" ";
			}
			
			cmd += "-o "+OutName;
			
			if (Vorbis) puts(cmd.c_str());
			if (system(cmd.c_str()) == -1)
				return Error<int>(err_compile);*/
		}
		
		// Emit
		if (EmitASM) {
			FILE *sf;
			char  buf;
			
			sf = fopen("a.opt.S", "r");
			
			while (!feof(sf)) {
				fread(&buf, sizeof(char), 1, sf);
				printf("%c", buf);
			}
			
			fclose(sf);
		}
		
		//
		if (EmitLLVM & OptLevel > 0) {
			FILE *sf;
			char  buf;
			
			sf = fopen("a.opt.ll", "r");
			
			while (!feof(sf)) {
				fread(&buf, sizeof(char), 1, sf);
				printf("%c", buf);
			}
			
			fclose(sf);
		}
		
		//if (OutName == "-") ManagerIR->Emit(true);
		delete ManagerIR;

	} else delete ManagerIR;
	
	delete Compiler;
	fclose(errf); // лог

	// Free Memory
	//for (int i = 0; i < VarStack.size(); i++)
	//	free (VarStack[i]);
	
	//using namespace mcld;
	
	//Initialize();
	//Linker linker;
	
	//LinkerConfig config("armv7-none-linux-gnueabi"); ///< --mtriple="armv7-none-linux-gnueabi"
	//linker.config(config);
	//config.setCodeGenType(LinkerConfig::Exec);
	//config.options().directories().insert("/usr/local/lib");  ///< -L/usr/local/lib
	
	/*Module module("hello");
	IRBuilder builder(module, config);

	builder.ReadInput("hello", "./hello.o"); ///< ./hello.o
	builder.ReadInput("c"); ///< -lc

	if (linker.link(module, builder)) {
		linker.emit("hello.exe"); ///< -o hello.exe
	}*/
	
	return 0;
}
