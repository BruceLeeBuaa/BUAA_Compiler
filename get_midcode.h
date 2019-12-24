#pragma once
#include "Lexical.h"

//my_assign:是对寄存器的赋值
//bgt:大于跳转，bge:大于等于跳转,blt:小于跳转,ble:小于等于跳转
namespace Mid {
	enum op_type {
		con_declare, var_declare, array_declare, para_declare, func_declare,
		add, sub, mul, div, neg, my_assign, assign, arr_assign, arr_take,
		output_str, output, output_c, input, call, push, ret, receive, set_label,
		_goto, _beq, _bne, _bgt, _bge, _blt, _ble, _beqz, _bnez, _addi, _subi,
		addi, subi, none, muli, divi
	};

	typedef struct
	{
		op_type op;
		string op_num1;
		string op_num2;
		string result;
	} MidCode;

	void add_mid_const(int value);
	void add_mid_arr_take(string ident_name, int index_temp);
	void add_mid_receive();
	void add_mid_ident(string ident_name);
	void add_mid_cal(Lexical::WORD op, int temp1, int temp2);
	void add_mid_neg(int temp);
	void add_mid_label(int label_num);
	void add_mid_goto(int label_num);
	void add_mid_condition_whileforif(Lexical::WORD op, int temp1, int temp2, int label_num);//不满足条件跳转
	void add_mid_condition_dowhile(Lexical::WORD op, int temp1, int temp2, int label_num);	 //满足条件跳转
	void add_mid_assign(string ident_name, int temp1);
	void add_mid_arr_assign(string ident_name, int temp1, int temp2);
	void add_mid_scanf(string ident_name);
	void add_mid_print_str(int index,int have_n);
	void add_mid_print(int index);
	void add_mid_print_c(int index);
	void add_mid_return(int index);
	void add_mid_push(int temp);
	void add_mid_call(string func_name);
	void add_mid_con_declare(string const_ident_name, int value);
	void add_mid_var_declare(string var_ident_name);
	void add_mid_arr_declare(string arr_ident_name, int arr_size);
	void add_mid_para_declare(string para_ident_name);
	void add_mid_func_declare(string func_ident_name);
	void add_mid_for_step(string ident_name1,string ident_name2, Lexical::WORD op, int step);
}