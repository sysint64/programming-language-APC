#ifndef CPP_CODEGEN_CONST
#define CPP_CODEGEN_CONST 1

// Main
#include <string>
#include <map>

using namespace std;

// Array constant
BaseIR *ArrConstAST::Codegen() {
	vector<BaseIR*> ConstsIR;
	//
	for (int i = 0; i < Vals.size(); ++i) {
		ConstsIR.push_back(Vals[i]->Codegen());
	}
	
	return new ArrValIR(ConstsIR);
}

// Vector constant
BaseIR *VecConstAST::Codegen() {
	vector<BaseIR*> ConstsIR;
	//
	for (int i = 0; i < Vals.size(); ++i) {
		ConstsIR.push_back(Vals[i]->Codegen());
	}
	
	return new VecValIR(ConstsIR);
}

// Константы
BaseIR *ConstOperAST::Codegen() {
	VarDict = &ExternVars;
	(*VarDict)[IdName].Def    = true;
	(*VarDict)[IdName].Global = true;
	(*VarDict)[IdName].Const  = true;
	//
	BaseIR *vTy, *ArrTy, *VecTy;
	
	ArrTy = 0;
	VecTy = 0;
	
	BaseIR *V = 0;
	//

	if ((*VarDict)[IdName].Val == 0) {
		V = new ConstIR(Val->Codegen(), IdName);
		(*VarDict)[IdName].Val  = V;
		ManagerIR->Insert(V, Head);
	} else ManagerIR->Insert((*VarDict)[IdName].Val, Head);
	
	return V;
}

#endif
