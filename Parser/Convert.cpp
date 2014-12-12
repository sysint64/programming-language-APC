#ifndef CPP_PARSER_CONVERT
#define CPP_PARSER_CONVERT 1

// Main
#include <string>
#include <map>

// Преобразование типа из from в to
ExprAST *parser::ConvertType(struct TypeAST from, struct TypeAST to, ExprAST* E, bool showerr, ExprAST *To) {
	//
	string toflag   = "false";
	string fromflag = "false";

	if (to.Flag)   toflag   = "true";
	if (from.Flag) fromflag = "true";
	
	//puts(("from : "+from.Name+"("+IntToStr(from.Size)+") "+fromflag).c_str());
	//puts(("to : "+to.Name+"("+IntToStr(to.Size)+") "+toflag).c_str());
	//puts("");
	
	if (EqTy(from, to)) return E;
	//if (from.Name == to.Name) return E;
	
	// Constant
	if (E->IsConst()) {
		// Numeric
		if (IsIntTy(from) && IsFloatTy(to))
			return new FloatExprAST(E->GetVal(), to.Size, to.Flag);
		
		if ((IsIntTy(from) || IsFloatTy(from)) && IsIntTy(to))
			return new IntExprAST(E->GetVal(), to.Size, to.Flag);

		if (IsFloatTy(from) && IsFloatTy(to))
			return new FloatExprAST(E->GetVal(), to.Size, to.Flag);
	}
	
	// Numeric
	//
	if (IsIntTy(from) && IsFloatTy(to))
		return new I2DExprAST(E, to);

	if (IsFloatTy(from) && IsIntTy(to))
		return new D2IExprAST(E, to);
	
	if (IsIntTy(from) && IsIntTy(to))
		return new IntConvExprAST(E, to);
		
	if (IsFloatTy(from) && IsFloatTy(to))
		return new FloatConvExprAST(E, to);

	//
	// Vector
	if (IsVecTy(from) && IsVecTy(to)) {
		if (IsFloatTy(DerefTy(from)) && IsIntTy(DerefTy(to)))
			return new VecD2IExprAST(E, to, CountOfTy(to));

		if (IsIntTy(DerefTy(from)) && IsFloatTy(DerefTy(to)))
			return new VecI2DExprAST(E, to, CountOfTy(to));
	}

	// REMOVE ME !!
	if (IsVPtrTy(from) && IsPtr2Ty(to)) {
		return new BitcastExprAST(E, To, to);
	}

	if (IsPtr2Ty(from) && IsVPtrTy(to)) {
		return new BitcastExprAST(E, To, to);
	}

	if (IsClassTy(from) && IsClassTy(to)) {
		return new BitcastExprAST(E, To, to);
	}

	if (IsPtrTy(from) && IsPtrTy(to) && dynamic_cast<NullExprAST*>(E)) {
		return new NullExprAST(to.TypeExpr);
	}
	//if (IsPtrTy(from) && IsPtrTy(to))
	//	return new BitcastExprAST(E, to.TypeExpr, to, true);

	//
	// Function to Pointer
	if ((IsFuncTy(from) & IsVPtrTy(to) & To != 0) == 1) {
		return new BitcastExprAST(E, To, to);
	}

	if (IsPtrTy(from) && IsFuncTy(to)) {
		return new BitcastExprAST(E, To, to);
	}

	if (IsFuncTy(from) && IsPtrTy(to)) {
		return new BitcastExprAST(E, To, to);
	}
	
	// Zero Integer to Pointer
	/*if (IsIntTy(from) & IsPtrTy(to)) {
		if (E->GetVal() == 0 && E->IsConst()) {
			return new NullExprAST(To);
		}
	}*/
	// Pointer to Function
	/*if ((IsFuncTy(to) & IsVPtrTy(from) & To != 0) == 1) {
		return new BitcastExprAST(E, To, to);
	}*/
		
	if (IsPtrTy(from) && IsPtrTy(to)) {
		puts("!!!!");
		return new BitcastExprAST(E, To, to, true, true);
	}

	if (E != 0 && showerr) {
		//
		for (int i = 0; i < OperOverloads.size(); i++) {
			struct Oper Item = OperOverloads[i];
			if (Item.op == tok_cast) {
				//
				//bool vVar = E->IsvVar();

				bool vVar = E->IsvVar();
				//
				struct TypeAST Ty = E->GetTy();
				//
				if (Item.LHS.Var) {;
					ExprAST *TyE = new PtrTyExprAST(Ty.TypeExpr);
					Ty = TyE->GetTy();
					//
					vVar = true;
				}
				
				if (EqTy(Item.Ty, to) & EqTy(Ty, Item.LHS.Type)) {
					E->SetvVar(vVar);
					E->MemCpy = Item.LHS.MemCpy;
					//
					return new CallOvrExprAST(i, E, 0, 0, to);
				}
				//
			}
		}
	}
	
	if (showerr)
		return Error<ExprAST*>(can_not_convert_to_type_ss, from.Name, to.Name);
	else return E;
}

// Явное преобразование типов
ExprAST *parser::ExplictConvertType(struct TypeAST from, struct TypeAST to, ExprAST* E, ExprAST* To) {
	//
	//puts(("from : "+from.Name+"("+IntToStr(from.Size)+")").c_str());
	//puts(("to : "+to.Name+"("+IntToStr(to.Size)+")").c_str());
	//puts("");
	
	if (EqTy(from, to)) return E;
	
	// Constant
	if (E->IsConst()) {
		// Numeric
		if (IsIntTy(from) && IsFloatTy(to))
			return new FloatExprAST(E->GetVal(), to.Size, to.Flag);
		
		if ((IsIntTy(from) || IsFloatTy(from)) && IsIntTy(to))
			return new IntExprAST(E->GetVal(), to.Size, to.Flag);

		if (IsFloatTy(from) && IsFloatTy(to))
			return new FloatExprAST(E->GetVal(), to.Size, to.Flag);
	}
	
	// Numeric
	//
	if (IsIntTy(from) && IsFloatTy(to))
		return new I2DExprAST(E, to);

	if (IsFloatTy(from) && IsIntTy(to))
		return new D2IExprAST(E, to);
	
	if (IsIntTy(from) && IsIntTy(to))
		return new IntConvExprAST(E, to);
		
	if (IsFloatTy(from) && IsFloatTy(to))
		return new FloatConvExprAST(E, to);
	//
	// Vector
	if (IsVecTy(from) && IsVecTy(to)) {
		if (IsFloatTy(DerefTy(from)) && IsIntTy(DerefTy(to)))
			return new VecD2IExprAST(E, to, CountOfTy(to));

		if (IsIntTy(DerefTy(from)) && IsFloatTy(DerefTy(to)))
			return new VecI2DExprAST(E, to, CountOfTy(to));
	}

	//
	if (IsVPtrTy(from) && IsPtr2Ty(to)) {
		return new BitcastExprAST(E, To, to, true);
	}

	if (IsPtr2Ty(from) && IsVPtrTy(to)) {
		return new BitcastExprAST(E, To, to, true);
	}
	//
	if (IsPtrTy(from) && IsPtrTy(to) && dynamic_cast<NullExprAST*>(E)) {
		return new NullExprAST(to.TypeExpr);
	}
	//
	// Function to Pointer
	if ((IsFuncTy(from) & IsVPtrTy(to) & To != 0) == 1) {
		return new BitcastExprAST(E, To, to);
	}
	
	//
	if (IsIntTy(from) && IsPtrTy(to)) {
		return new Int2PtrExprAST(E, to.TypeExpr);
	}

	//
	if (IsPtrTy(from) && IsIntTy(to)) {
		return new Ptr2IntExprAST(E, to.TypeExpr);
	}

	if (IsPtrTy(from) && IsPtrTy(to))
		return new BitcastExprAST(E, to.TypeExpr, to, true);

	//if (IsPtrTy(from) && IsPtrTy(to))
	//	return new BitcastExprAST(E, To, to);

	// Zero Integer to Pointer
	/*if (IsIntTy(from) & IsPtrTy(to)) {
		if (E->GetVal() == 0) {
			return new NullExprAST(To);
		}
	}*/
	
	return Error<ExprAST*>(can_not_convert_to_type_ss, from.Name, to.Name);
}

#endif
