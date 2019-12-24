#pragma once

#include "Lexical.h"
#include "Globalvar.h"

namespace Syntax{
	void number();
	int factor();
	int term();
	int expression();
	bool statement_begin();
	void compound_statement();
	void statement_block();
	void statement();
	//void condition();
	void if_else_statement();
	void do_while_statement();
	void while_statement();
	void for_statement();
	void step();
	void assign_statement();
	void scanf_statement();
	void printf_statement();
	void printf_expression();
	void return_statement();
	void func_statement();
	void void_func_statement();

	void const_declare();
	void const_define();
	void var_declare();
	void var_define(Lexical::WORD type);
	void para_define();
	void complex_define();
	void complex_func_body(Lexical::WORD type);
	void func_define();
	void func_body();
	//void print1(Lexical::WORD temp, string temp_str);

	void syntax();
}
