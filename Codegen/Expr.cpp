#ifndef CPP_EXPR
#define CPP_EXPR 1

// Main
#include <string>
#include <map>

using namespace std;

//* EXPR

// Null Pointer Value
BaseIR *NullExprAST::Codegen() {
	return new NullValIR(Ty->Codegen());
}

// Öåëîå ÷èñëî
BaseIR *IntExprAST::Codegen() {
	return new IntValIR(Val, Size);
}

// ×èñëî ñ ïëàâàþùåé òî÷êîé
BaseIR *FloatExprAST::Codegen() {
	return new FloatValIR(Val, Size);
}

// Строковая константа
BaseIR *StringExprAST::Codegen() {
	if (!GetConst) {
		BaseIR *V = new StringValIR(Val, StrLen);
		ManagerIR->Insert(V, Head);

		BaseIR *Str = new GEPInBoundsIR(V, new IntValIR(0, 32), new IntValIR(0, 32));
		ManagerIR->Insert(Str, Body);
		
		return Str;
	} else {
		BaseIR *V = new StringValIR2(Val, StrLen);
		return V;
	}
}

// Vector value
BaseIR *VecValAST::Codegen() {
	BaseIR *Ty;
		
	if (IsIntTy(Vals[0]->GetTy()))
		Ty = new IntTyIR(SizeOfTy(Vals[0]->GetTy()));
		
	if (IsFloatTy(Vals[0]->GetTy()))
		Ty = new FloatTyIR(SizeOfTy(Vals[0]->GetTy()));
		
	//
	BaseIR *Tmp = new AllocIR(new VecTyIR(Size, Ty), "tmp");
	//ManagerIR->Insert(Tmp, Body);
	CurVarBlockIR->Elements.push_back(Tmp);
	//
	if (Store) {
		BaseIR *vec = Id->Codegen();
		BaseIR *Store1 = new StoreIR(vec, Tmp);
		ManagerIR->Insert(Store1, Body);
		//ManagerIR->Insert(new CustomIR(""), Body);
	}
	//
		
	for (int i = 0; i < Vals.size(); ++i) {
		int idx = i;
		if (Left) idx = Size-Vals.size()+i;
			
		BaseIR *Load  = new LoadIR(Tmp);
		BaseIR *Val = Vals[i]->Codegen();
		BaseIR *Insert = new InsertElementIR(Load, Val, new IntValIR(idx, 32));
		BaseIR *Store1 = new StoreIR(Insert, Tmp);
		//
		ManagerIR->Insert(Load  , Body);
		ManagerIR->Insert(Insert, Body);
		ManagerIR->Insert(Store1, Body);
		//ManagerIR->Insert(new CustomIR(""), Body);
	}
		
	BaseIR *Load1 = new LoadIR(Tmp);
	ManagerIR->Insert(Load1, Body);
	
	return Load1;
}

// Array value
BaseIR *ArrValAST::Codegen() {
	//
	BaseIR *Ty = Vals[0]->GetTy().TypeExpr->Codegen();//GetIRType(Vals[0]->GetTy());// = CreateIRType(Vals[0]->GetTy());
	BaseIR *Tmp = new AllocIR(new ArrTyIR(Size, Ty), "tmp");
	
	//ManagerIR->Insert(Tmp, Body);
	CurVarBlockIR->Elements.push_back(Tmp);
	//
	if (Store) {
		//
		BaseIR *arr = Id->Codegen();
		BaseIR *TAddr = arr;
		//
		for (int j = 0; j < (*Unni).size(); j++) {
			TAddr = new GEPInBoundsIR(TAddr, new IntValIR(0, 32), new IntValIR((*Unni)[j], 32));
			ManagerIR->Insert(TAddr, Body);
		}
		
		//BaseIR *Load = new LoadIR(TAddr);
		//ManagerIR->Insert(Load, Body);
	
		BaseIR *Store1 = new StoreIR(TAddr, Tmp);
		ManagerIR->Insert(Store1, Body);
	}
	//
	for (int i = 0; i < Vals.size(); ++i) {
		int idx = i;
		if (Left) idx = Size-Vals.size()+i;
		
		BaseIR *Val = Vals[i]->Codegen();
		BaseIR *GEP = new GEPInBoundsIR(Tmp, new IntValIR(0, 32), new IntValIR(idx, 32));
		BaseIR *Store1 = new StoreIR(Val, GEP);
		//
		ManagerIR->Insert(GEP, Body);
		ManagerIR->Insert(Store1, Body);
	}
		
	BaseIR *Load1 = new LoadIR(Tmp);
	ManagerIR->Insert(Load1, Body);
	
	return Load1;
}

// Number
BaseIR *NumBinaryExprAST::Codegen() {
	BaseIR *L = LHS->Codegen();
	BaseIR *R = RHS->Codegen();
	
	if (L == 0 || R == 0) return 0;
	
	switch (Op) {
		case '+'	: return CreateAdd (L, R, LHS->GetTy());
		case '-'	: return CreateSub (L, R, LHS->GetTy());
		case '*'	: return CreateMul (L, R, LHS->GetTy());
		case '/'	: return CreateDiv (L, R, LHS->GetTy());
		case '%'	: return CreateRem (L, R, LHS->GetTy());
		
		// Int
		//case tok_div	: return CreateDiv (L, R, LHS->GetTy());
		//case tok_mod	: return CreateRem (L, R, LHS->GetTy());
		case tok_shl	: return CreateShl (L, R, LHS->GetTy());
		case tok_shr	: return CreateShr (L, R, LHS->GetTy());
		case tok_xor	: return CreateXor (L, R, LHS->GetTy());
		case tok_and	: return CreateAnd (L, R, LHS->GetTy());
		case tok_or		: return CreateOr  (L, R, LHS->GetTy());
		case '&'		: return CreateAnd (L, R, LHS->GetTy());
		case '|'		: return CreateOr  (L, R, LHS->GetTy());
		
		// Cmp
		case tok_eq	: return CreateCmpEQ (L, R, LHS->GetTy());
		case '<'	: return CreateCmpLT (L, R, LHS->GetTy());
		case '>'	: return CreateCmpGT (L, R, LHS->GetTy());
		
		case tok_le	: return CreateCmpLE (L, R, LHS->GetTy());
		case tok_ge	: return CreateCmpGE (L, R, LHS->GetTy());
		case tok_ne	: return CreateCmpNE (L, R, LHS->GetTy());
	}
}

// Ptr
BaseIR *PtrBinaryExprAST::Codegen() {
	BaseIR *L;
	BaseIR *R;
	struct TypeAST ty;
	
	//GetAddr = true;
	
	if (IsPtrTy(LHS->GetTy())) {
		L  = LHS->Codegen();
		R  = RHS->Codegen();
		ty = RHS->GetTy();
	} else {
		L  = RHS->Codegen();
		R  = LHS->Codegen();
		ty = LHS->GetTy();
	}
	
	//GetAddr = false;
	
	if (L == 0 || R == 0) return 0;
	if (Op == '-')
		R = CreateSub(new IntValIR(0, SizeOfTy(ty)), R, ty);
	
	//
	BaseIR *ptr = 0;
	
	if (IsRelop(Op)) {
		if (Op == tok_eq) ptr = CreateCmpEQ(L, R, LHS->GetTy());
		if (Op == tok_ne) ptr = CreateCmpNE(L, R, LHS->GetTy());
	} else {
		ptr = new GEPInBoundsIR(L, R, 0, true);
		ManagerIR->Insert(ptr, Body);
	}
	
	return ptr;
}

// Óíàðíîå âûðàæåíèå
BaseIR *UnaryExprAST::Codegen() {	
	if (Op == '@') GetAddr = true;
	BaseIR *L = Val->Codegen();;
	GetAddr = false;
	struct TypeAST ty = Val->GetTy();
	
	if (L == 0) return 0;
	switch (Op) {
		case '-' :
			if (IsIntTy(ty))   return CreateSub(new IntValIR   (0, SizeOfTy(ty)), L, ty);
			if (IsFloatTy(ty)) return CreateSub(new FloatValIR (0, SizeOfTy(ty)), L, ty);
			
			break;
		
		case tok_not : {
			if (SizeOfTy(ty) == 1) return CreateLNot(L, SizeOfTy(ty), ty);
			else return CreateNot(L, SizeOfTy(ty), ty);
		}

		case '@'     : return L;
	}
	
	return 0;
}

// Ïåðåìåííàÿ â âûðàæåíèè
BaseIR *VarExprAST::Codegen() {
	//GetVarDict(Name);
	// Èùåì ýòó ïåðåìåííóþ â òåêóùåé ôóíêöèè.
	BaseIR *V = (*VD)[Name].Val;
	BaseIR *R = 0;
	//
	if (!V) return 0;
	if (!GetAddr) {
		R = new LoadIR(V);
		ManagerIR->Insert(R, Body);
	} else R = V;
	
	return R;
}

//
BaseIR *IdxExprAST::Codegen() {
	bool ga = GetAddr;
	
	GetAddr = true;
	BaseIR *arr = E->Codegen();
	GetAddr = false;
	
	for (int i = 0; i < Idx.size(); i++) {
		BaseIR *IDX = Idx[i]->Codegen();
		
		if (IsPtrTy(E->GetTy())) { // Pointer deref
			arr = new LoadIR(arr);
			ManagerIR->Insert(arr, Body);
			arr = new GEPInBoundsIR(arr, IDX, 0, true);
			ManagerIR->Insert(arr, Body);
			//arr = new LoadIR(arr);
			//ManagerIR->Insert(arr, Body);
		} else if (IsArrTy(E->GetTy())) {

			arr = new GEPInBoundsIR(arr, new IntValIR(0, 32), IDX);
			ManagerIR->Insert(arr, Body);

		} else if (IsVecTy(E->GetTy())) {
			//
			if (ga) {
				arr = new GEPInBoundsIR(arr, new IntValIR(0, 32), IDX);
				ManagerIR->Insert(arr, Body);
			} else
			
			if (!this->IsAssign) {
				arr = new LoadIR(arr);
				ManagerIR->Insert(arr, Body);
				arr = new ExtractElementIR(arr, IDX);
				ManagerIR->Insert(arr, Body);
			} else {
				return arr;
			}
			//
			ga = true;
		}
	}
	
	BaseIR *R;
	
	if (!ga) {
		R = new LoadIR(arr);
		ManagerIR->Insert(R, Body);
	} else R = arr;
	
	return R;
}

//
BaseIR *PtrExprAST::Codegen() {
	BaseIR *R = E->Codegen();
		
	// DerefPtr
	for (int i = 0; i < N; i++) {
		bool l = true;

		//if (GetAddr) {
		//	l = false;
		//} else if (i == 0) l = false;
		
		if (ccp) {
			ccp = false;
			continue;
		}
		
		R = new LoadIR(R, false);
		ManagerIR->Insert(R, Body);
	}
			
	return R;
}

//
BaseIR *AssignExprAST::Codegen() {
	BaseIR  *V = 0;
	TypeAST mTy = DefaultTy();
	BaseIR *IDX = new IntValIR(0, 32);
	//
	if (dynamic_cast<IdxExprAST*>(Id)) {
		mTy = ((IdxExprAST*)Id)->E->GetTy();
		IDX = ((IdxExprAST*)Id)->Idx[0]->Codegen();
	}
	//
	if (IsVecTy(mTy)) {
		//
		BaseIR *R = Expr->Codegen();
		BaseIR *I = Id->Codegen();
		BaseIR *Load = new LoadIR(I);
		BaseIR *Insert = new InsertElementIR(Load, R, IDX);
		//
		ManagerIR->Insert(Load, Body);
		ManagerIR->Insert(Insert, Body);
		//
		V = new StoreIR(Insert, I);
		ManagerIR->Insert(V, Body);
		//
	} else if ((!IsClassTy(GetTy())) | Expr->GetTy().Abstract) {
		//
		BaseIR *R = Expr->Codegen();
		if (R->GetEmit() == "") return 0;
		GetAddr = true;
		BaseIR *I = Id->Codegen();
		GetAddr = false;
		V = new StoreIR(R, I);
		ManagerIR->Insert(V, Body);
		//
	} else {
		BaseIR *R = Expr->Codegen();

		GetAddr = true;
		BaseIR *I = Id->Codegen();
		GetAddr = false;
		//
		V = new StoreIR(R, I);
		ManagerIR->Insert(V, Body);
	}
	
	return V;
}

//
BaseIR *AssignObjExprAST::Codegen() {
	BaseIR *V = 0;
	//
	string ClassSize = IntToStr(SizeOfTy(GetTy()) >> 3);
	//
	BaseIR *R = Expr->Codegen();
	BaseIR *I = Id->Codegen();
	//
	BaseIR *Cast1 = I;
	BaseIR *Cast2 = R;
	
	//
	Cast2 = new BitcastIR(R, new PtrTyIR(new IntTyIR(8)));
	Cast1 = new BitcastIR(I, new PtrTyIR(new IntTyIR(8)));

	ManagerIR->Insert(Cast1, Body);
	ManagerIR->Insert(Cast2, Body);

	BaseIR *Memcpy = new CustomIR("  call void @llvm.memcpy.p0i8.p0i8."+iPtrSize+"("+Cast1->GetTy()->GetAddr()+" "+Cast1->GetAddr()+", "+Cast2->GetTy()->GetAddr()+" "+Cast2->GetAddr()+", "+iPtrSize+" "+ClassSize+", i32 4, i1 false)");
	ManagerIR->Insert(Memcpy, Body);
	
	return V;
}

// Load
BaseIR *LoadExprAST::Codegen() {
	if (IR == 0) {
		NoInsIR = true;
		BaseIR *L = new LoadIR(E->Codegen());
		NoInsIR = false;
		
		ManagerIR->Insert(L, Body);
		
		return L;
	} else {
		BaseIR *L = new LoadIR(IR);
		ManagerIR->Insert(L, Body);
		
		return L;
	}
}

//
	/*bool ga = GetAddr;
	GetAddr = true;
	BaseIR *Param = 0;
	//
	if (E == 0 && ThisClassIR != 0) {
		Param = ThisClassIR;
	} else { Param = E->Codegen(); }
	
	GetAddr = false;
	//
	if (IsClass | E == 0) {
		Param = new LoadIR(Param);
		ManagerIR->Insert(Param, Body);
	}
	//
	Param = new GEPInBoundsIR(Param, new IntValIR(0, 32), new IntValIR(Offset+1, 32));
	//
	if (IsClassTy(GetTy())) {
		Param->SetTy(new PtrTyIR(new PtrTyIR(Ty->Codegen())));
	} else {
		Param->SetTy(new PtrTyIR(Ty->Codegen()));
	}
	
	ManagerIR->Insert(Param, Body);
	BaseIR *R;
	
	if (!ga) {
		R = new LoadIR(Param);
		ManagerIR->Insert(R, Body);
	} else R = Param;
	//
	return R;*/
//

// Вызов функции
BaseIR *CallExprAST::Codegen() {
	//
	vector<BaseIR*> ArgsV;
	vector<BaseIR*> EndLTVar;
	BaseIR *R = 0;
	BaseIR *CC = 0;
	bool invCast = false;
	GetAddr = false;
	//
	if (ProtExpr != 0)
		CC = ((PrototypeExprAST*)ProtExpr)->Directives->CCIR;
	//
	if (TyExpr != 0 && Constructor) {
		ClassTyD.Type = 0;
		string ClassSize = IntToStr(SizeOfTy(TyExpr->GetTy()) >> 3);
		
		// FIXME //
		BaseIR *New = new BaseIR();
		New->SetAddr("%"+GetIRCount("new"));

		//
		if (arch == x32) New->SetEmit("  "+New->GetAddr()+" = call noalias i8* @new(i32 "+ClassSize+")");
		if (arch == x64) New->SetEmit("  "+New->GetAddr()+" = call noalias i8* @new(i64 "+ClassSize+")");

		New->SetTy(new PtrTyIR(new IntTyIR(8)));
		//
		BaseIR *CTyIR = AssignE->Codegen()->GetTy();
		BaseIR *Cast1 = new BitcastIR(New, CTyIR);
		//BaseIR *Cast2 = new BitcastIR(Cast1, new PtrTyIR(new IntTyIR(8)));
		//
		
		R = Cast1;
		Prot->SetTy(new VoidTyIR());
		BaseIR *Param = new GEPInBoundsIR(R, new IntValIR(0, 32), new IntValIR(0, 32));
		Param->SetTy(new PtrTyIR(new IntTyIR(intSize)));

		BaseIR *StoreID = new StoreIR(new IntValIR(TyExpr->GetTy().id, intSize), Param);

		//intSize
		//
		ManagerIR->Insert (New  , Body);
		ManagerIR->Insert (Cast1, Body);
		ManagerIR->Insert (Param, Body);
		ManagerIR->Insert (StoreID, Body);
		//
		if (invCast) {
			R = new BitcastIR(New, new PtrTyIR(new IntTyIR(8)));
			ManagerIR->Insert(R, Body);
		}
		
		//
		GetAddr = true;
		BaseIR *Val = AssignE->Codegen();
		GetAddr = false;
		BaseIR *Store = new StoreIR(R, Val);
		ManagerIR->Insert(Store, Body);

		//
		// Init virtual functions
		//
		
		ClassTyExprAST *S = (ClassTyExprAST*)ClassTy->GetTy().E;
		//
		for (int i = 0; i < S->MethodsNames.size(); i++) {
			//
			for (int j = 0; j < S->mMethods[S->MethodsNames[i]].Overloads.size(); j++) {
				//
				struct overloads ovr = S->mMethods[S->MethodsNames[i]].Overloads[j];
				
				if (ovr.Virtual) {
					//
					int Offset = S->mVars[S->MethodsNames[i]+".virtual."+IntToStr(j)].Offset;
					//
					BaseIR *TyClass = ClassTy->Codegen();
					BaseIR *Param = new GEPInBoundsIR(TyClass, new IntValIR(0, 32), new IntValIR(Offset, 32));
					Param->SetTy(new PtrTyIR(new PtrTyIR(new IntTyIR(8))));
					BaseIR *Cast = new BitcastIR(Param, new PtrTyIR(new PtrTyIR(new ProtoTyIR((ProtoIR*)Prot))));
					BaseIR *Store = new StoreIR(ovr.Func, Cast);
					//
					ManagerIR->Insert(Param, Body);
					ManagerIR->Insert(Cast, Body);
					ManagerIR->Insert(Store, Body);
				}
				//
			}
			//
		}
		//
	}
	//
	BaseIR *SRet = 0;
	//
	if (((PrototypeExprAST*)ProtExpr)->RetType->MemCpy) {
		//
		if (AssignE == 0) {
			BaseIR *Id = new AllocIR(((PrototypeExprAST*)ProtExpr)->SRet->Codegen()->GetTy(), "tmp");
			//
			//ManagerIR->Insert(Id, Body);
			CurVarBlockIR->Elements.push_back(Id);
			//
			SRet = new LoadIR(Id);
			ArgsV.push_back(Id);
			//
		} else {
			GetAddr = true;
			BaseIR *Id = AssignE->Codegen();
			GetAddr = false;
			ArgsV.push_back(Id);
			
			SRet = new CustomIR("");
		}
	}

	//
	for (int i = 0; i < Args.size(); i++) {
		BaseIR *Arg = 0;
		
		//if (IsPtrTy(Args[i]->GetTy())) GetAddr = true;
		if (Args[i]->IsvVar()) { GetAddr = true; }
		if (Name == "read" | Name == "readln") GetAddr = true;
		
		Arg = Args[i]->Codegen();

		if (Args[i]->MemCpy) {
			BaseIR *Id = new AllocIR(Arg->GetTy()->GetTy(), "tmp");
			//
			BaseIR *Cast    = new BitcastIR(Arg, new PtrTyIR(new IntTyIR(8)));
			BaseIR *CastId  = new BitcastIR(Id , new PtrTyIR(new IntTyIR(8)));
			BaseIR *CastId2 = new BitcastIR(Id , new PtrTyIR(new IntTyIR(8)));

			//
			int Size = SizeOfTy(Args[i]->GetTy()) >> 3;
			BaseIR *LifeTimeStart = new CustomIR("  call void @llvm.lifetime.start(i64 -1, "+CastId->GetTy()->GetAddr()+" "+CastId->GetAddr()+")");
			BaseIR *LifeTimeEnd = new CustomIR("  call void @llvm.lifetime.end(i64 -1, "+CastId2->GetTy()->GetAddr()+" "+CastId2->GetAddr()+")");

			BaseIR *Memcpy = new CustomIR("  call void @llvm.memcpy.p0i8.p0i8."+iPtrSize+"("+CastId->GetTy()->GetAddr()+" "+CastId->GetAddr()+", "+Cast->GetTy()->GetAddr()+" "+Cast->GetAddr()+", "+iPtrSize+" "+IntToStr(Size)+", i32 4, i1 false)");

			//ManagerIR->Insert(Id, Body);
			CurVarBlockIR->Elements.push_back(Id);
			//
			ManagerIR->Insert(Cast, Body);
			ManagerIR->Insert(CastId, Body);
			ManagerIR->Insert(LifeTimeStart, Body);
			ManagerIR->Insert(Memcpy, Body);
			//
			EndLTVar.push_back(CastId2);
			EndLTVar.push_back(LifeTimeEnd);
			//
			Arg = Id;
			//
		}
		
		ArgsV.push_back(Arg);
		GetAddr = false;
	}
	
	//
	GetFuncDict(Name);
	//
	if (ThisClassPtrIR != 0 & FuncDict != &MainFuncDict) {
		if (CurClassTy != 0) {
			ClassTyExprAST* S = (ClassTyExprAST*)(CurClassTy->GetTy().E);
			FuncDict = &S->mMethods;
		}
		//
		if (!GetTy().NoThis) {
			BaseIR *load = new LoadIR(ThisClassPtrIR);
			//BaseIR *Cast = new BitcastIR(load, new PtrTyIR(new IntTyIR(8)));
			ManagerIR->Insert(load, Body);
			//ManagerIR->Insert(Cast, Body);
			ArgsV.push_back(load);
		}
	}
	
	//
	if ((!Constructor | TyExpr == 0) & (!Virtual)) {
		//
		if (IR == 0) IR = Expr->Codegen();
		if (Constructor & TyExpr == 0) Prot->SetTy(new VoidTyIR());
		R = new CallVarIR(IR, Prot, ArgsV, CC, SRet != 0);
		ManagerIR->Insert(R, Body);
		//
	} else if (!Virtual) {
		//
		if (IR == 0) IR = Expr->Codegen();
		BaseIR *Call = new CallVarIR(IR, Prot, ArgsV, CC, SRet != 0);
		ManagerIR->Insert(Call , Body);
		//
	} else if (Virtual) {		
		//
		if (IR == 0) IR = Expr->Codegen();
		if (Constructor & TyExpr == 0) Prot->SetTy(new VoidTyIR());
		//
		BaseIR *TyClass = ClassTy->Codegen();
		BaseIR *Param = new GEPInBoundsIR(TyClass, new IntValIR(0, 32), new IntValIR(Offset, 32));
		Param->SetTy(new PtrTyIR(new PtrTyIR(new IntTyIR(8))));
		BaseIR *Load = new LoadIR(Param);
		BaseIR *Cast = new BitcastIR(Load, new PtrTyIR(new ProtoTyIR((ProtoIR*)Prot)));
		BaseIR *Call = new CallVarIR(Cast, Prot, ArgsV, CC, SRet != 0);
		//
		ManagerIR->Insert(Param, Body);
		ManagerIR->Insert(Load, Body);
		ManagerIR->Insert(Cast, Body);
		ManagerIR->Insert(Call, Body);
		//
	}
	//
	
	/*if (Destructor) {
		// FIXME //
		BaseIR *Cast = new BitcastIR(ArgsV[ArgsV.size()-1], new PtrTyIR(new IntTyIR(8)));
		BaseIR *Destroy = new CustomIR("  call void @delete("+Cast->GetTy()->GetAddr()+" "+Cast->GetAddr()+")");
		//
		ManagerIR->Insert(Cast   , Body);
		ManagerIR->Insert(Destroy, Body);
	}*/

	//
	for (int i = 0; i < EndLTVar.size(); i++)
		ManagerIR->Insert(EndLTVar[i], Body);
	//
	if (SRet != 0) {
		R = SRet;

		if (AssignE == 0) {
			ManagerIR->Insert(R, Body);
		}
	}
	//
	return R;
}

// self
BaseIR *SelfExprAST::Codegen() {
	if (!GetAddr) {
		BaseIR *Load = new LoadIR(ThisClassIR);
		ManagerIR->Insert(Load, Body);
		return Load;
	} else  return ThisClassIR;
}

// Вызов перегруженного оператора
BaseIR *CallOvrExprAST::Codegen() {
	struct Oper Item = OperOverloads[OverloadId];
	vector<BaseIR*> ArgsV;
	vector<BaseIR*> EndLTVar;
	vector<ExprAST*> Args;
	//
	if (LHS != 0) Args.push_back(LHS);
	if (RHS != 0) Args.push_back(RHS);
	if (RES != 0) Args.push_back(RES);

	//
	BaseIR *SRet = 0;
	//
	if (Item.Prot->RetType->MemCpy) {
		//
		BaseIR *Id = new AllocIR(Item.Prot->SRet->Codegen()->GetTy(), "tmp");
		//
		//ManagerIR->Insert(Id, Body);
		CurVarBlockIR->Elements.push_back(Id);
		//
		if (!GetAddr && Item.op != tok_cast) SRet = new LoadIR(Id);
		else SRet = Id;

		ArgsV.push_back(Id);
		SRet->MemCpy = true;
		/*if (AssignE == 0) {
			
			BaseIR *Id = new AllocIR(((PrototypeExprAST*)ProtExpr)->SRet->Codegen()->GetTy(), "tmp");
		//
		ManagerIR->Insert(Id, Body);
		//
		SRet = new LoadIR(Id);
		ArgsV.push_back(Id);
			//
		} else {
			GetAddr = true;
			BaseIR *Id = AssignE->Codegen();
			GetAddr = false;
			ArgsV.push_back(Id);
			
			SRet = new CustomIR("");
		}*/
	}

	//
	bool ga = GetAddr;
	//
	for (int i = 0; i < Args.size(); i++) {
		//
		
		if (Args[i]->IsvVar()) GetAddr = true;
		BaseIR *Arg = Args[i]->Codegen();
		GetAddr = false;
		//
		if (Args[i]->MemCpy) {
			//
			BaseIR *Id = new AllocIR(Arg->GetTy()->GetTy(), "tmp");
			//
			BaseIR *Cast    = new BitcastIR(Arg, new PtrTyIR(new IntTyIR(8)));
			BaseIR *CastId  = new BitcastIR(Id , new PtrTyIR(new IntTyIR(8)));
			BaseIR *CastId2 = new BitcastIR(Id , new PtrTyIR(new IntTyIR(8)));

			//
			int Size = SizeOfTy(Args[i]->GetTy()) >> 3;
			BaseIR *LifeTimeStart = new CustomIR("  call void @llvm.lifetime.start(i64 -1, "+CastId->GetTy()->GetAddr()+" "+CastId->GetAddr()+")");
			BaseIR *LifeTimeEnd = new CustomIR("  call void @llvm.lifetime.end(i64 -1, "+CastId2->GetTy()->GetAddr()+" "+CastId2->GetAddr()+")");

			BaseIR *Memcpy = new CustomIR("  call void @llvm.memcpy.p0i8.p0i8."+iPtrSize+"("+CastId->GetTy()->GetAddr()+" "+CastId->GetAddr()+", "+Cast->GetTy()->GetAddr()+" "+Cast->GetAddr()+", "+iPtrSize+" "+IntToStr(Size)+", i32 4, i1 false)");

			//ManagerIR->Insert(Id, Body);
			CurVarBlockIR->Elements.push_back(Id);
			ManagerIR->Insert(Cast, Body);
			ManagerIR->Insert(CastId, Body);
			ManagerIR->Insert(LifeTimeStart, Body);
			ManagerIR->Insert(Memcpy, Body);
			//
			EndLTVar.push_back(CastId2);
			EndLTVar.push_back(LifeTimeEnd);
			//
			Arg = Id;
			//
		}
		//
		ArgsV.push_back(Arg);
		//
	}
	
	BaseIR *R = new CallIR(Item.Prot->LLVMName, Item.Prot->IRProto, ArgsV, 0, SRet != 0/*, Item.Prot->SRet->Codegen()*/);
	ManagerIR->Insert(R, Body);

	//
	for (int i = 0; i < EndLTVar.size(); i++)
		ManagerIR->Insert(EndLTVar[i], Body);
	
	if (SRet != 0) {
		R = SRet;
		//
		if (!ga && Item.op != tok_cast) // FIXME!
			ManagerIR->Insert(R, Body);
	}

	return R;
}

BaseIR *FuncExprAST::Codegen() {
	GetFuncDict(IdName);
	return (*FuncDict)[IdName].Overloads[Idx].Func;
}

BaseIR *FuncObjExprAST::Codegen() {
	return (*pMethods)[IdName].Overloads[Idx].Func;
}

// Get Structur Param
BaseIR *ParamExprAST::Codegen() {
	bool ga = GetAddr;
	GetAddr = true;
	BaseIR *Param = 0;
	puts("a");

	if (InsideClass) ClassTyIR = ThisClassIR->GetTy()->GetTy();
	//
	if (E == 0 && ThisClassIR != 0) {
		Param = ThisClassIR;
	} else { Param = E->Codegen(); }
	puts("b");
	GetAddr = false;
	//
	if (IsClass | E == 0) {
		Param = new LoadIR(Param);
		ManagerIR->Insert(Param, Body);
	}
	//
	Param = new GEPInBoundsIR(Param, new IntValIR(0, 32), new IntValIR(Offset+1, 32));
	//
	if (IsClassTy(GetTy())) {
		Param->SetTy(new PtrTyIR(new PtrTyIR(Ty->Codegen())));
	} else {
		puts("c");
		Param->SetTy(new PtrTyIR(Ty->Codegen()));
		puts("c");
	}
	
	ManagerIR->Insert(Param, Body);
	BaseIR *R;
	
	if (!ga) {
		R = new LoadIR(Param);
		ManagerIR->Insert(R, Body);
	} else R = Param;
	puts("d");
	//
	return R;
}

// Get Ret
BaseIR *GetRetExprAST::Codegen() {
	BaseIR *Call = Expr->Codegen();
	BaseIR *Load = new LoadIR(Call);

	ManagerIR->Insert(Load,  Body);
	return Load;
}

// New
BaseIR *NewExprAST::Codegen() {
	//
	BaseIR *New = new BaseIR();
	New->SetAddr("%"+GetIRCount("new"));

	int sum = 0;
	BaseIR *Mul = 0;
	string SumSize = IntToStr(TypeSize);
	//
	if (Sizes.size() != 0) {
		ExprAST *Ty = new IntTyExprAST(PtrSize);
		Sizes[0] = mParser->ConvertType (Sizes[0]->GetTy(), Ty->GetTy(), Sizes[0], true, Ty);
		Mul = Sizes[0]->Codegen();

		for (int i = 1; i < Sizes.size(); i++) {
			Mul = CreateMul (Mul, Sizes[i]->Codegen(), Sizes[i]->GetTy());
		}

		SumSize = Mul->GetAddr();
	}

	if (arch == x32) New->SetEmit("  "+New->GetAddr()+" = call noalias i8* @new(i32 "+SumSize+")");
	if (arch == x64) New->SetEmit("  "+New->GetAddr()+" = call noalias i8* @new(i64 "+SumSize+")");

	New->SetTy(new PtrTyIR(new IntTyIR(8)));
	//
	BaseIR *Cast1 = new BitcastIR(New, AssignE->Codegen()->GetTy());
	//BaseIR *Cast2 = new BitcastIR(Cast1, new PtrTyIR(new IntTyIR(8)));

	ManagerIR->Insert(New  ,  Body);
	ManagerIR->Insert(Cast1,  Body);
	//ManagerIR->Insert(Cast2,  Body);

	GetAddr = true;
	BaseIR *Val = AssignE->Codegen();
	GetAddr = false;

	return Cast1;
}

// Delete
BaseIR *DeleteExprAST::Codegen() {
	//
	for (int i = 0; i < Calls.size(); i++)
		Calls[i]->Codegen();
	//
	for (int i = 0; i < Ids.size(); i++) {
		bool ga = GetAddr;
		
		//if (Ids[i]->IsvVar())
		//	GetAddr = true;

		GetAddr = true;
		BaseIR *IR = Ids[i]->Codegen();

		GetAddr = ga;

		BaseIR *Cast = new BitcastIR(IR, new PtrTyIR(new IntTyIR(8)));


		BaseIR *Destroy = new CustomIR("  call void @delete("+Cast->GetTy()->GetAddr()+" "+Cast->GetAddr()+")");
		//
		ManagerIR->Insert(Cast   , Body);
		ManagerIR->Insert(Destroy, Body);
	}

	return 0;
}

#endif
