#pragma once
#include "Lexical.h"
#include "get_midcode.h"

namespace opt_mid {

	typedef struct {
		string temp_name = "";
		string var_name = "";
	}REPLACE;

	void scan_inline_1();
	void scan_inline_2();
	void scan_zero();
	void scan_one();
	void add_relation(int index, string var_name);
	void remove_relation(string var_name);
	void remove_all_relation();
	int get_temp_from_var(string var_name);

	Mid::op_type new_op(Mid::op_type op);
	int get_temp_index(string temp_name);
	bool is_number(string var_name);
	bool is_alu_op(Mid::op_type op);
	bool is_branch_op(Mid::op_type op);
	bool is_baseblock_op(Mid::op_type op);
	bool need_use_later(string var_name, int begin);
	void push_back_none();
	void opt_midcode();
	bool need_use_later_local(string var_name,int k);
}
