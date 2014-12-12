#ifndef CPP_CODEGEN_VAR
#define CPP_CODEGEN_VAR 1

// Main
#include <string>
#include <map>

using namespace std;

// Переменные
BaseIR *VarOperAST::Codegen() {
	BaseIR *vTy = Type->Codegen();
	
	if (IsClassTy(Type->GetTy())) {
		//
		vTy = new PtrTyIR(vTy);
		vTy->SetAlign("8");
		//
	}
	
	BaseIR *V = 0;
	
	for (int i = 0; i < IdNames.size(); ++i) {
		(*VD)[IdNames[i]].Def    = true;  // Эта переменная определена
		(*VD)[IdNames[i]].Const  = false; // Не константа
		(*VD)[IdNames[i]].Global = Global;
		//
		if ((*VD)[IdNames[i]].Val == 0) {
			if (!(*VD)[IdNames[i]].Global) {
				V = new AllocIR(vTy, IdNames[i]);

				if (CurVarBlockIR != 0) {
					//ManagerIR->Insert(V, Body);
					BaseIR *Store = new StoreIR(new UndefValIR(V->GetTy()->GetTy()), V);

					ManagerIR->Insert(Store, Body);
					CurVarBlockIR->Elements.push_back(V);
				} else {
					ManagerIR->Insert(V, Body);
				}
			} else {
				V = new ExternIR(vTy, IdNames[i]);
				ManagerIR->Insert(V, Head);
			}
			//
			(*VD)[IdNames[i]].Val  = V;
		} else {
			//if (!(*VD)[IdNames[i]].isAlloced) {
				if (!(*VD)[IdNames[i]].Global)	ManagerIR->Insert((*VD)[IdNames[i]].Val, Body);//CurVarBlockIR->Elements.push_back((*VD)[IdNames[i]].Val);
				else  							ManagerIR->Insert((*VD)[IdNames[i]].Val, Head);
			//}
		}
		
		(*VD)[IdNames[i]].Type = Type->GetTy();
	}
	//
	if (InitVals.size() == 1)  {
		//
		for (int i = 0; i < IdNames.size(); i++) {
			ExprAST *E = new VarExprAST(IdNames[i], Type->GetTy(), VD);

			if (ovrid == -1) {
				ExprAST *assign = new AssignExprAST (E, InitVals[0]);
				assign->Codegen();
			} else {
				ExprAST *ovr = new CallOvrExprAST(ovrid, E, InitVals[0], 0, Type->GetTy());
				GetAddr = true;
				ovr->Codegen();
				GetAddr = false;
			}
		}
	} else {
		//
		for (int i = 0; i < InitVals.size(); i++) {
			ExprAST *E = new VarExprAST(IdNames[i], Type->GetTy(), VD);

			if (ovrid == -1) {
				ExprAST *assign = new AssignExprAST (E, InitVals[i]);
				assign->Codegen();
			} else {
				ExprAST *ovr = new CallOvrExprAST(ovrid, E, InitVals[i], 0, Type->GetTy());
				GetAddr = true;
				ovr->Codegen();
				GetAddr = false;
			}
		}
	}
	
	return (*VD)[IdNames[IdNames.size()-1]].Val;
}

#endif
