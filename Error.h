#ifndef JOBELL_ERROR_AST
#define JOBELL_ERROR_AST 1

// LLVM
#include "AST.h"
#include "Def.h"
//#include "Lexer.h"

enum messages {
	no_input_file = 1,
	err_compile = 2,
	expected = 3,
	expected_expr = 4,
	expected_s_after_decl = 5,
	expected_s_after_expr = 6,
	expected_s_after_statement = 7,
	//
	expected_unqualified_id = 8,
	undecl_id = 9,
	redefinition_id = 10,
	expected_ss_after_decl = 11,
	expected_id = 12,
	expected_ss = 13,
	expected_ss_in_arg_list = 14,
	subscripted_value_not_arr_ptr_vec = 15,
	invalid_suffix_s_on_int_const = 16,
	read_only_var_is_not_assignable = 17,
	invalid_operands_to_bin_expr = 18,
	can_not_convert_to_type_ss = 19,
	size_of_arr_has_non_int_ty_s = 20,
	vec_ty_must_be_ty_int_or_fp = 21,
	expected_ss_in_vec_list = 22,
	expected_ss_in_arr_list = 23,
	expected_s_in_vec_list = 24,
	expected_s_in_arr_list = 25,
	invalid_op_ty_for_instr = 26,
	break_stmt_not_within_ls = 27,
	continue_stmt_not_within_s = 28,
	align_is_not_pow_2 = 29,
	expected_s_stmt = 30,
	expected_assign_stmt = 31,
	expected_ss_assign_stmt = 32,
	type_must_be_s = 33,
	type_must_be_ss = 34,
	no_match_func_for_call_to_s = 35,
	expected_oper = 36,
	size_argument_list_s = 37,
	cant_deref_ty = 38,
	no_member_named_ss = 39,
	const_and_destr_must_be_methods = 40,
	missing_default_arg = 41,
	undecl_typename = 42,
	invalid_use_s_in_non_member_func = 43,
	directive_for_oper_s = 44,
	assign_2_s_from_incompatible_ty_s = 45,
	s_is_not_polymorphic = 46,
	include_file_s_not_found = 47,
	
	//* Warnings
	decl_does_not_declare = 1,
	div_by_zero = 2
};

static map<int, string> Errors;
static map<int, string> Warnings;

FILE *errf;
bool EE = false;

void LoadMessage(const string FN);
template<class T> T Error (const int errn, const string s1 = "", const string s2 = "");
void Warning (const int errn, const string s1 = "", const string s2 = "");

#endif
