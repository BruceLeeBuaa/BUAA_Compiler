#pragma once
#include "Lexical.h"
#include "get_midcode.h"

namespace Opt_Target{
	struct OptTarget
	{
		string op;
		string op_num1;
		string op_num2;
		string result;
	};
	void get_opt_target();
	void output_opt_target();
	void opt_const_string();
	void opt_text_label();
	void opt_j_main();
	void add_opt_target(string op, string result, string op_num1, string op_num2);
	void opt_para_declare();
	void opt_const_declare();
	void opt_vararr_declare();
	void opt_global_var();
	void opt_enter();
	bool is_block(Mid::op_type op);
	int get_temp_index(string temp_name);
	int get_reg_index(string reg_name);
	void opt_save_t89();
	void opt_calculate();
	void opt_calculate_i();
	void opt_neg();
	void opt_my_assign();
	void opt_assign();
	void opt_arr_assign();
	void opt_arr_take();
	void opt_input();
	void reset_relation_TSA();
}