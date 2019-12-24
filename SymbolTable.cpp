#include "Lexical.h"
#include "SymbolTable.h"
#include "Globalvar.h"
#include "error.h"

int SymbolTable::func_para_num()
{
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == ident_string) {
			return func_table[i].para_num;
		}
	}
	return 0;
}

void SymbolTable::add_const(Lexical::WORD type, string id, int value)
{
	//每进入一个函数时(包括程序最开头)都要add一个空GTABLE
	GTABLE temp = func_table.back();
	func_table.pop_back();
	SymbolTable::TYPE real_type;
	if (type == Lexical::CHARTK) {
		real_type = SymbolTable::char_const;
	}
	else {
		real_type = SymbolTable::int_const;
	}
	CONST temp_const = {id, real_type, value};
	temp.constTable.push_back(temp_const);
	if (is_element_in_vector(temp.id_name, id)) {
		Error::error_print(Error::ident_redefine);
	}
	temp.id_name.push_back(id);
	func_table.push_back(temp);
}

void SymbolTable::add_var(Lexical::WORD type, string id, int num)
{
	GTABLE temp = func_table.back();
	func_table.pop_back();
	SymbolTable::TYPE real_type;
	if (type == Lexical::CHARTK) {
		if (num == 0) {
			real_type = SymbolTable::char_var;
		}
		else {
			real_type = SymbolTable::char_array;
		}
	}
	else {
		if (num == 0) {
			real_type = SymbolTable::int_var;
		}
		else {
			real_type = SymbolTable::int_array;
		}
	}
	VAR temp_var = {id, real_type, num, ""};
	temp.varTable.push_back(temp_var);
	if (is_element_in_vector(temp.id_name, id)) {
		Error::error_print(Error::ident_redefine);
	}
	temp.id_name.push_back(id);
	if (num == 0) {
		temp.ident_addr[id] = temp.arr_var_size;
		temp.arr_var_size = temp.arr_var_size + 4;
	}
	else {
		temp.ident_addr[id] = temp.arr_var_size;
		temp.arr_var_size = temp.arr_var_size + 4 * num;
	}
	func_table.push_back(temp);
}

bool SymbolTable::func_has_return(string name) {
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == name) {
			if (func_table[i].type == void_func) {
				return false;
			}
			else {
				return true;
			}
		}
	}
	return false;
}

void SymbolTable::add_func(string name, Lexical::WORD type) 
{
	table_num++;
	GTABLE temp;
	if (type == Lexical::CHARTK) {
		temp.type = char_func;
	}
	else if (type == Lexical::INTTK) {
		temp.type = int_func;
	}
	else {
		temp.type = void_func;
	}
	temp.func_name = name;
	func_table.push_back(temp);
}

void SymbolTable::add_para(string name, Lexical::WORD type)
{
	GTABLE temp = func_table.back();
	func_table.pop_back();
	SymbolTable::TYPE real_type;
	if (type == Lexical::CHARTK) {
		real_type = SymbolTable::char_para;
	}
	else {
		real_type = SymbolTable::int_para;
	}
	PARA temp_para = {name, real_type, ""};
	temp.paraTable.push_back(temp_para);
	if (is_element_in_vector(temp.id_name, name)) {
		Error::error_print(Error::ident_redefine);
	}
	temp.id_name.push_back(name);
	temp.para_num = temp.para_num + 1;
	temp.para_size = temp.para_size + 4;
	temp.ident_addr[name] = temp.para_size;//第一个para的偏移是4
	func_table.push_back(temp);
}

bool SymbolTable::is_element_in_vector(vector<string> v, string element) {
	vector<string>::iterator it;
	it = find(v.begin(), v.end(), element);
	if (it != v.end()) {
		return true;
	}
	else {
		return false;
	}
}

void SymbolTable::add_func_name(string name)
{
	if (is_element_in_vector(func_name_list, name)) {
		Error::error_print(Error::ident_redefine);
	}
	func_name_list.push_back(name);
}

int SymbolTable::is_func_define(string name) //每次函数调用时都要运行此函数
{
	if (!is_element_in_vector(func_name_list, name)) {
		Error::error_print(Error::ident_undefine);
		return -1;
	}
	if (name == "main") {
		Error::error_print(Error::ident_undefine);
		return -1;
	}
	return 0;
}

void SymbolTable::is_ident_define(string name) //每次出现标识符时都要运行此函数
{
	GTABLE temp = func_table.back();
	if (!is_element_in_vector(temp.id_name, name)) {
		GTABLE temp2 = func_table[0];
		if (!is_element_in_vector(temp2.id_name, name)) {
			Error::error_print(Error::ident_undefine);
		}
	}
}

void SymbolTable::para_type_inspect(int index, int intorchar, string func_name)
{
	//intorchar:0字符 1整数
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == func_name) {
			temp = func_table[i];
			break;
		}
	}
	vector<PARA> para_v = temp.paraTable;
	if ((para_v[index].type == int_para) && (intorchar == 0)) {
		Error::error_print(Error::paratype_error);
	}
	if ((para_v[index].type == char_para) && (intorchar == 1)) {
		Error::error_print(Error::paratype_error);
	}
}

SymbolTable::TYPE SymbolTable::ident_type(string ident_name)
{
	GTABLE temp = func_table.back();
	if (is_element_in_vector(temp.id_name, ident_name)) {
		vector<CONST> v1 = temp.constTable;
		vector<VAR> v2 = temp.varTable;
		vector<PARA> v3 = temp.paraTable;
		for (int i = 0; i < v1.size(); i++) {
			if (v1[i].name == ident_name) {
				return v1[i].type;
			}
		}
		for (int i = 0; i < v2.size(); i++) {
			if (v2[i].name == ident_name) {
				return v2[i].type;
			}
		}
		for (int i = 0; i < v3.size(); i++) {
			if (v3[i].name == ident_name) {
				return v3[i].type;
			}
		}
	}
	else {
		GTABLE temp2 = func_table[0];
		vector<CONST> v4 = temp2.constTable;
		vector<VAR> v5 = temp2.varTable;
		vector<PARA> v6 = temp2.paraTable;
		for (int i = 0; i < v4.size(); i++) {
			if (v4[i].name == ident_name) {
				return v4[i].type;
			}
		}
		for (int i = 0; i < v5.size(); i++) {
			if (v5[i].name == ident_name) {
				return v5[i].type;
			}
		}
		for (int i = 0; i < v6.size(); i++) {
			if (v6[i].name == ident_name) {
				return v6[i].type;
			}
		}
	}
	return none_type;
}

SymbolTable::TYPE SymbolTable::func_type(string func_name)
{
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == func_name) {
			return func_table[i].type;
		}
	}
	return none_type;
}

SymbolTable::TYPE SymbolTable::current_func_type()
{
	GTABLE temp = func_table.back();
	return temp.type;
}

bool SymbolTable::func_return()
{
	GTABLE temp = func_table.back();
	if (temp.type == char_func || temp.type == int_func) {
		return true;
	}
	else {
		return false;
	}
}

bool SymbolTable::is_ident_const(string ident)
{
	GTABLE temp = func_table.back();
	vector<CONST> const_v = temp.constTable;
	for (int i = 0; i < const_v.size(); i++) {
		if (const_v[i].name == ident) {
			return true;
		}
	}
	if (is_element_in_vector(temp.id_name, ident)) {
		return false;
	}
	GTABLE temp2 = func_table[0];
	vector<CONST> const_v2 = temp2.constTable;
	for (int i = 0; i < const_v2.size(); i++) {
		if (const_v2[i].name == ident) {
			return true;
		}
	}
	return false;
}
//midcode --- --- --- --- part
int SymbolTable::const_ident_to_value(string ident_name)
{
	GTABLE temp = func_table.back();
	vector<CONST> const_v = temp.constTable;
	for (int i = 0; i < const_v.size(); i++) {
		if (const_v[i].name == ident_name) {
			return (int)const_v[i].value;
		}
	}
	GTABLE temp2 = func_table[0];
	vector<CONST> const_v2 = temp2.constTable;
	for (int i = 0; i < const_v2.size(); i++) {
		if (const_v2[i].name == ident_name) {
			return (int)const_v2[i].value;
		}
	}
	return 2019;
}

void SymbolTable::current_func_temp_size()
{
	GTABLE temp = func_table.back();
	func_table.pop_back();
	temp.temp_var_size = max_num_index * 4;
	func_table.push_back(temp);
}

bool SymbolTable::is_local(string name)//这里的name是变量名
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	if (is_element_in_vector(temp.id_name, name)) {
		return true;
	}
	else {
		return false;
	}
}
int SymbolTable::A_reg_addr(int i)//a1 对应第0个参数
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	int addr = temp.temp_var_size + temp.arr_var_size + 80 + (i-1) * 4;
	return addr;
}
int SymbolTable::para_addr(string name)
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	int addr = temp.temp_var_size + temp.arr_var_size + 80 + temp.para_size - temp.ident_addr[name];
	//int addr = temp.temp_var_size + temp.arr_var_size + 80 + temp.ident_addr[name] - 4;
	return addr;
}

int SymbolTable::var_addr(string name)
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	int addr = temp.temp_var_size + temp.ident_addr[name];
	return addr;
}
//这里数组和var变量是一样的，只能加载数组首地址
int SymbolTable::global_var_addr(string name)
{
	GTABLE temp = func_table[0];
	int addr = temp.ident_addr[name];
	return addr;
}

int SymbolTable::local_addr(string name)
{
	if (current_func_ident_type(name) == int_para || current_func_ident_type(name) == char_para) {
		return para_addr(name);
	}
	else {
		return var_addr(name);
	}
}

bool SymbolTable::is_para(string name)
{
	if (current_func_ident_type(name) == int_para || current_func_ident_type(name) == char_para) {
		return true;
	}
	else {
		return false;
	}
}

bool SymbolTable::is_const(string name)
{
	if (current_func_ident_type(name) == int_const || current_func_ident_type(name) == char_const) {
		return true;
	}
	else {
		return false;
	}
}

bool SymbolTable::is_int(string name)
{
	if (current_func_ident_type(name) == int_para || current_func_ident_type(name) == int_var) {
		return true;
	}
	else {
		return false;
	}
}

int SymbolTable::sp_down_size()
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	int addr = temp.arr_var_size + temp.temp_var_size + 80;
	return addr;
}

int SymbolTable::T_reg_addr(int i)
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	int addr = temp.temp_var_size + temp.arr_var_size + i * 4;
	return addr;
}

int SymbolTable::S_reg_addr(int i)
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	int addr = temp.temp_var_size + temp.arr_var_size + 40 + i * 4;
	return addr;
}

int SymbolTable::ra_addr()
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	int addr = temp.temp_var_size + temp.arr_var_size + 72;
	return addr;
}

int SymbolTable::func_para_size()
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	return temp.para_size;
}

SymbolTable::TYPE SymbolTable::current_func_ident_type(string ident_name)
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	if (is_element_in_vector(temp.id_name, ident_name)) {
		vector<CONST> v1 = temp.constTable;
		vector<VAR> v2 = temp.varTable;
		vector<PARA> v3 = temp.paraTable;
		for (int i = 0; i < v1.size(); i++) {
			if (v1[i].name == ident_name) {
				return v1[i].type;
			}
		}
		for (int i = 0; i < v2.size(); i++) {
			if (v2[i].name == ident_name) {
				return v2[i].type;
			}
		}
		for (int i = 0; i < v3.size(); i++) {
			if (v3[i].name == ident_name) {
				return v3[i].type;
			}
		}
	}
	else {
		GTABLE temp2 = func_table[0];
		vector<CONST> v4 = temp2.constTable;
		vector<VAR> v5 = temp2.varTable;
		vector<PARA> v6 = temp2.paraTable;
		for (int i = 0; i < v4.size(); i++) {
			if (v4[i].name == ident_name) {
				return v4[i].type;
			}
		}
		for (int i = 0; i < v5.size(); i++) {
			if (v5[i].name == ident_name) {
				return v5[i].type;
			}
		}
		for (int i = 0; i < v6.size(); i++) {
			if (v6[i].name == ident_name) {
				return v6[i].type;
			}
		}
	}
	return none_type;
}

SymbolTable::TYPE SymbolTable::mid_opt_func_type()
{
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == opt_mid_current_func) {
			return func_table[i].type;
		}
	}
	return none_type;
}
SymbolTable::TYPE SymbolTable::mid_opt_ident_type(string ident_name)
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == opt_mid_current_func) {
			temp = func_table[i];
		}
	}
	if (is_element_in_vector(temp.id_name, ident_name)) {
		vector<CONST> v1 = temp.constTable;
		vector<VAR> v2 = temp.varTable;
		vector<PARA> v3 = temp.paraTable;
		for (int i = 0; i < v1.size(); i++) {
			if (v1[i].name == ident_name) {
				return v1[i].type;
			}
		}
		for (int i = 0; i < v2.size(); i++) {
			if (v2[i].name == ident_name) {
				return v2[i].type;
			}
		}
		for (int i = 0; i < v3.size(); i++) {
			if (v3[i].name == ident_name) {
				return v3[i].type;
			}
		}
	}
	else {
		GTABLE temp2 = func_table[0];
		vector<CONST> v4 = temp2.constTable;
		vector<VAR> v5 = temp2.varTable;
		vector<PARA> v6 = temp2.paraTable;
		for (int i = 0; i < v4.size(); i++) {
			if (v4[i].name == ident_name) {
				return v4[i].type;
			}
		}
		for (int i = 0; i < v5.size(); i++) {
			if (v5[i].name == ident_name) {
				return v5[i].type;
			}
		}
		for (int i = 0; i < v6.size(); i++) {
			if (v6[i].name == ident_name) {
				return v6[i].type;
			}
		}
	}
	return none_type;
}

void SymbolTable::add_S_reg_to_var(int index)
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	temp.varTable[index].reg = "s" + to_string(index);
}
void SymbolTable::add_A_reg_to_para(int index)
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	temp.paraTable[index-1].reg = "a" + to_string(index);
}

vector<string> SymbolTable::loop_var()
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	return temp.loop_var;
}

void SymbolTable::add_loop_var(string var)
{
	GTABLE temp;
	int index = -1;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
			index = i;
		}
	}
	vector<string> loop_list = temp.loop_var;
	int flag = 0;
	for (int i = 0; i < loop_list.size(); i++) {
		if (loop_list[i] == var) {
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		loop_list.push_back(var);
	}
	temp.loop_var = loop_list;
	func_table[index] = temp;
}
void SymbolTable::add_max_index(int index)
{
	GTABLE temp = func_table.back();
	func_table.pop_back();
	temp.max_index = index;
	func_table.push_back(temp);
}
int SymbolTable::get_max_index()
{
	GTABLE temp;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
		}
	}
	return temp.max_index;
}
void SymbolTable::add_current_temp_size(int add_index)
{
	GTABLE temp;
	int index = -1;
	for (int i = 0; i < table_num; i++) {
		if (func_table[i].func_name == current_func_name) {
			temp = func_table[i];
			index = i;
		}
	}
	temp.temp_var_size += add_index * 4;//更新temp_var_size
	temp.max_index += add_index;		//更新标签
	func_table[index] = temp;
}