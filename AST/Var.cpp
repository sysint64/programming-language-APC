#ifndef CPP_VAR_AST
#define CPP_VAR_AST 1

// Main
#include <string>
#include <map>

using namespace std;

// VarOperAST - Класс узла переменных
class VarOperAST : public OperAST {
public:
	//string IdName;
	//vector<int> Sizes;
	//string Ty;
	vector<string> IdNames;
	vector<ExprAST*> InitVals;
	ExprAST *Type;
	map<string, Var> *VD;
	int ovrid;
	bool Global;
		
	//VarOperAST(const string &idName, const string &ty, vector<int> &sizes)
	//	: IdName(idName), Ty(ty), Sizes(sizes) {}
	VarOperAST(vector<string> idNames, ExprAST *type, map<string, Var> *vd, const bool global = false, int ovrid = -1) : IdNames(idNames), Type(type), Global(global), ovrid(ovrid)  {
		TypeAST Ty;
		Ty = Type->GetTy();
		Ty.Global = Global;
		Type->SetTy(Ty);
		VD = vd;
	}

	VarOperAST(vector<string> idNames, ExprAST *type, vector<ExprAST*> initVals, map<string, Var> *vd, const bool global = false, int ovrid = -1) : IdNames(idNames), Type(type), Global(global), InitVals(initVals), ovrid(ovrid)  {
		TypeAST Ty;
		Ty = Type->GetTy();
		Ty.Global = Global;
		Type->SetTy(Ty);
		VD = vd;
	}
	
	VarOperAST(string idName, ExprAST *type, map<string, Var> *vd, const bool global = false, int ovrid = -1) : Type(type), Global(global), ovrid(ovrid) {
		IdNames.push_back(idName);
		TypeAST Ty;
		Ty = Type->GetTy();
		Ty.Global = Global;
		Type->SetTy(Ty);
		VD = vd;
	}
		
	virtual BaseIR *Codegen();
};

#endif
