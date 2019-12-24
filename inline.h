#pragma once
#include "Lexical.h"
#include "get_midcode.h"

namespace In_Line {
	typedef struct
	{
		vector<Mid::MidCode> code;
		vector<Mid::MidCode> inline_code;
		vector<string> para_list;
		string func_name;
		bool is_inline;//true代表需要内联
		bool has_ret;
		int max_index;
	}InLineFunc;

	void add_func();
	void push_case();
	void call_case();
}