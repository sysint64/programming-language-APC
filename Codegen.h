#ifndef H_CODEGEN
#define H_CODEGEN 1

#include "Codegen/Codegen_IR.cpp"
#include "Codegen/Codegen_Haxe.cpp" // For mobile platforms

using namespace std;

enum IRBlock { Head, DType, Body, End };

string formatIRName (string name) {

	for (int i = 0; i < name.size(); i++) {

		if (name[i] == '/') name[i] = '.';

	}

	return name;

}

// Emit LLVM
class codegen {
private:
	vector<BaseIR*> BlockDType;
	vector<BaseIR*> BlockHead;
	vector<BaseIR*> BlockBody;
	vector<BaseIR*> BlockEnd;
	//
	FILE *f;
//
public:
	codegen() { f = fopen("a.out.ll", "w"); }
	~codegen() { if (f != NULL) fclose(f); }
	//
	void CloseF() { fclose(f); f = NULL; }
	void EmitIR(const string &line, const bool F = false) {
		if (EmitLLVM & OptLevel == 0) puts(line.c_str());
		fputs((line+"\n").c_str(), f);
	}
	
	// Выводим промежуточный код LLVM
	void Emit(const bool ASM = false) {
		EmitIR("; ModuleID = '"+OutName+".pas'");

		if (DataLayout) {
			if (arch == x32) EmitIR("target datalayout = \"e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-f80:128:128-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S32\"\n");
			if (arch == x64) EmitIR("target datalayout = \"e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128\"\n");
		}
		
		//
		EmitIR("; Type declares");
		//
		for (unsigned i = 0, e = BlockDType.size(); i != e; ++i) {
			EmitIR(BlockDType[i]->GetEmit());
		}
		//
		EmitIR("\n; Global vars");
		//
		for (unsigned i = 0, e = BlockHead.size(); i != e; ++i) {
			EmitIR(BlockHead[i]->GetEmit());
		}
		
		EmitIR("\n; Funcs declares");
		for (unsigned i = 0, e = BlockEnd .size(); i != e; ++i) {
			EmitIR(BlockEnd [i]->GetEmit());
		}
		
		EmitIR("\n; Implementation");
		
		// Тело программы
		
		for (unsigned i = 0, e = BlockBody.size(); i != e; ++i) {
			EmitIR(BlockBody[i]->GetEmit());
		}

		//
		EmitIR("\n; Metadata\n!0 = metadata !{ metadata !0 }");
	}
	
	// Добавляем элемент промежуточного кода в очередь
	void Insert(BaseIR *IR, IRBlock block) {
		if (LIRInS || NoInsIR) return;
		switch (block) {
			case DType : BlockDType.push_back(IR); break;
			case Head  : BlockHead .push_back(IR); break;
			case Body  : BlockBody .push_back(IR); break;
			case End   : BlockEnd  .push_back(IR); break;
		}
	}
};
//
codegen* ManagerIR;
//
#include "Codegen/Builder_IR.cpp"
#include "Error.h"
#include "Codegen/Convert.cpp"
#include "Codegen/Oper.cpp"
#include "Codegen/Var.cpp"
#include "Codegen/Type.cpp"
#include "Codegen/Const.cpp"
#include "Codegen/IfCase.cpp"
#include "Codegen/Loop.cpp"
#include "Codegen/Func.cpp"
#include "Codegen/HType.cpp"
#include "Declares.cpp"

#endif
