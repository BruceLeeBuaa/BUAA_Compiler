#pragma once
#include "Lexical.h"

namespace Target {
	struct TargetCode
	{
		string op;
		string op_num1;
		string op_num2;
		string result;
	};

	void get_target_code();
	void output_target();
	void add_const_string();
	void add_text_label();
	void add_j_main();

	void add_target_code(string op, string result, string op_num1, string op_num2);
	void ignore_para_declare();
	void ignore_const_declare();
	void ignore_vararr_declare();
	void add_calculate();
	void add_calculate_i();
	void add_neg();
	void add_my_assign();
	void add_assign();
	void add_arr_assign();
	void add_arr_take();
	void add_input();
	void add_enter();
}
