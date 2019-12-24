#pragma once

#include "Lexical.h"
#include <algorithm>

namespace SymbolTable{
	enum TYPE {
		int_array, char_array, int_para, char_para, int_const, char_const,
		int_var, char_var, int_func, char_func, void_func, none_type
	};
	
	typedef struct 
	{
		string name;
		TYPE type;
		int value;			//数值,或ASC码
	}CONST;

	typedef struct
	{
		string name;
		TYPE type;
		int num;			//若为数组，则为元素个数(若为普通变量则为0)
		string reg;
	}VAR;

	typedef struct 
	{
		string name;
		TYPE type;
		string reg;
	}PARA;

	typedef struct
	{
		string func_name;	//全局变量默认func_table[0]
		TYPE type;
		map<string, int> ident_addr;
		int arr_var_size = 0;
		int para_size = 0;
		int para_num = 0;  
		int temp_var_size = 0;
		int max_index = 0;
		vector<string> loop_var;
		vector<string> id_name;
		vector<PARA> paraTable;
		vector<CONST> constTable;
		vector<VAR> varTable;
	}GTABLE;

	int func_para_num();
	bool func_has_return(string name);
	void add_func(string name, Lexical::WORD type);
	void add_const(Lexical::WORD type, string id, int value);
	void add_var(Lexical::WORD type, string id, int num);
	void add_para(string name, Lexical::WORD type);
	void add_func_name(string name);
	
	bool is_element_in_vector(vector<string> v, string element);
	int is_func_define(string name);
	void is_ident_define(string name);
	void para_type_inspect(int index,int intorchar,string func_name);
	TYPE ident_type(string ident_name);
	TYPE func_type(string func_name);
	TYPE current_func_type();

	bool func_return();
	bool is_ident_const(string ident);
	//中间代码使用
	int const_ident_to_value(string ident_name);
	void current_func_temp_size();
	//中间代码优化使用
	TYPE mid_opt_func_type();
	TYPE mid_opt_ident_type(string ident_name);
	//目标代码使用
	bool is_local(string name);
	int para_addr(string name);
	int var_addr(string name);
	int local_addr(string name);//var或者para
	int global_var_addr(string name);
	bool is_para(string name);
	bool is_const(string name);
	bool is_int(string name);
	int sp_down_size();
	int T_reg_addr(int i);
	int S_reg_addr(int i);
	int ra_addr();
	int func_para_size();
	TYPE current_func_ident_type(string ident_name);
	//目标代码优化
	void add_S_reg_to_var(int index);
	void add_A_reg_to_para(int index);
	int A_reg_addr(int i);
	vector<string> loop_var();
	void add_loop_var(string var);
	//内联
	void add_max_index(int index);
	int get_max_index();
	void add_current_temp_size(int add_index);
	//窥孔
	bool ident_can_delete(string ident);
}
