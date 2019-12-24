#include "Lexical.h"
#include "Globalvar.h"
#include "Register.h"
#include "Opt_target.h"

bool Reg::in_A_reg(string name)
{//只使用a1-a3
	for (int i = 1; i < 4; i++) {
		if (A_reg[i] == name) {
			return true;
		}
	}
	return false;
}
string Reg::get_A_reg(string name)
{
	for (int i = 1; i < 4; i++) {
		if (A_reg[i] == name) {
			return "$a" + to_string(i);
		}
	}
}
void Reg::reset_A_reg()
{
	for (int i = 1; i < 4; i++) {
		A_reg[i].clear();
	}
}
bool Reg::in_S_reg(string name)
{
	for (int i = 0; i < 8; i++) {
		if (S_reg[i] == name) {
			return true;
		}
	}
	return false;
}
string Reg::get_S_reg(string name)
{
	for (int i = 0; i < 8; i++) {
		if (S_reg[i] == name) {
			return "$s" + to_string(i);
		}
	}
}
void Reg::reset_S_reg()
{
	for (int i = 0; i < 8; i++) {
		S_reg[i].clear();
	}
}
bool Reg::in_T_reg(string name)
{
	for (int i = 0; i < 8; i++) {
		if (T_reg[i] == name) {
			return true;
		}
	}
	return false;
}
string Reg::get_T_reg(string name)
{
	for (int i = 0; i < 8; i++) {
		if (T_reg[i] == name) {
			return "$t" + to_string(i);
		}
	}
}
void Reg::scanf_T(string name)
{
	for (int i = 0; i < 8; i++) {
		if (T_reg[i] == name) {
			T_reg[i] = "";
		}
	}
}
void Reg::reset_T_reg()
{
	for (int i = 0; i < 8; i++) {
		T_reg[i].clear();
	}
}
/*
string Reg::use_S_assign(string name)
{
	if (current_S_index == 8) {
		return "no_S_reg";
	}
	S_reg[current_S_index] = name;
	current_S_index++;
	return "$s" + to_string(current_S_index-1);
}
string Reg::get_new_S_reg(string name)
{
	if (current_S_index == 8) {
		return "no_S_reg";
	}
	S_reg[current_S_index] = name;
	load_reg(name, "$s" + to_string(current_S_index));
	current_S_index++;
	return "$s" + to_string(current_S_index-1);
}
*/
void Reg::reset_temp_reg()
{
	for (int i = 0; i < 3000; i++) {
		temp_reg[i].clear();
	}
}
string Reg::no_T_left(string name, int need_lw)
{
	t8t9++;
	if (t8t9 % 2 == 0) {
		t8t9 = 8;
	}
	else {
		t8t9 = 9;
	}
	if (need_lw == 1) {
		if ((!(midcode_opt1[current_midcode_index].op == Mid::arr_assign && t8t9 == 9)) &&
			(!(midcode_opt1[current_midcode_index].op == Mid::arr_take && t8t9 == 9))) {
			opt_load(name, "$t" + to_string(t8t9));
		}
	}
	return "$t" + to_string(t8t9);
}
string Reg::use_T_assign(string name)//里面不用有值 
{
	int index = Opt_Target::get_temp_index(name);
	if (temp_reg[index] != "") {
		return temp_reg[index];
	}
	else {
		int flag = -1;
		for (int i = 0; i < 8; i++) {
			if (!need_T_save(T_reg[i])) {
				if (T_reg[i] != "") {
					temp_reg[Opt_Target::get_temp_index(T_reg[i])].clear();
				}
				T_reg[i] = name;
				temp_reg[index] = "$t" + to_string(i);
				flag = i;
				break;
			}
		}
		if (flag == -1) {
			return no_T_left(name,0);
		}
		else {
			return "$t" + to_string(flag);
		}
	}
}
string Reg::get_new_T_reg(string name)//里面需要有值
{
	int index = Opt_Target::get_temp_index(name);
	if (temp_reg[index] != "") {
		return temp_reg[index];
	}
	else {
		int flag = -1;
		for (int i = 0; i < 8; i++) {
			if (!need_T_save(T_reg[i])) {
				if (T_reg[i] != "") {
					temp_reg[Opt_Target::get_temp_index(T_reg[i])].clear();
				}
				T_reg[i] = name;
				temp_reg[index] = "$t" + to_string(i);
				flag = i;
				break;
			}
		}
		if (flag == -1) {
			return no_T_left(name, 1);
		}
		else {
			opt_load(name,"$t"+to_string(flag));
			return "$t" + to_string(flag);
		}
	}
}
bool Reg::is_T_reg_ok(int num)
{
	if (T_reg[num] == "") {
		return true;
	}
	else {
		return false;
	}
}

void Reg::load_reg(string name, string reg)
{
	if (name[0] == '#') {
		//临时变量存储
		int addr1 = (atoi(name.substr(1, name.size() - 1).c_str()) - 1) * 4;
		Target::add_target_code("lw", reg, to_string(addr1) + "($fp)", "");
	}
	else {
		if (SymbolTable::is_local(name)) {
			//局部变量
			int addr2 = SymbolTable::local_addr(name);
			Target::add_target_code("lw", reg, to_string(addr2) + "($fp)", "");
		}
		else {
			//全局变量
			int addr3 = SymbolTable::global_var_addr(name);
			Target::add_target_code("lw", reg, to_string(addr3) + "($gp)", "");
		}
	}
}

void Reg::save_reg(string name, string reg)
{
	if (name[0] == '#') {
		//临时变量存储
		int addr1 = (atoi(name.substr(1, name.size() - 1).c_str()) - 1) * 4;
		Target::add_target_code("sw", reg, to_string(addr1) + "($fp)", "");
	}
	else {
		if (SymbolTable::is_local(name)) {
			//局部变量
			int addr2 = SymbolTable::local_addr(name);
			Target::add_target_code("sw", reg, to_string(addr2) + "($fp)", "");
		}
		else {
			//全局变量
			int addr3 = SymbolTable::global_var_addr(name);
			Target::add_target_code("sw", reg, to_string(addr3) + "($gp)", "");
		}
	}
}

string Reg::get_ident_reg(string name)
{
	string get_reg;//代表用哪个寄存器存name的值
	if (in_T_reg(name)) {
		get_reg = get_T_reg(name);
	}
	else {
		get_reg = get_new_T_reg1(name);
	}
	return get_reg;
}
//call的时候
void Reg::write_back_T()
{
	for (int i = 0; i < 8; i++) {
		if (T_reg[i] != "") {
			string name = T_reg[i];
			if (need_T_save(name)) {
				opt_save(name, "$t" + to_string(i));
			}
			T_reg[i] = "";
		}
	}
}
void Reg::write_back_T1()
{
	for (int i = 0; i < 8; i++) {
		if (T_reg[i] != "") {
			string name = T_reg[i];
			save_reg(name, "$t" + to_string(i));
			T_reg[i] = "";
		}
	}
}
//return出现在函数中间的一个if里面
/*
void Reg::write_back_S()
{
	for (int i = 0; i < 8; i++) {
		if (S_reg[i] != "") {
			string name = S_reg[i];
			save_reg(name, "$s" + to_string(i));
			S_reg[i] = "";
		}
	}
}
*/
//进函数时
void Reg::save_ra_reg()
{
	int addr3 = SymbolTable::ra_addr();
	Target::add_target_code("sw", "$ra", to_string(addr3) + "($fp)", "");
}
void Reg::save_ra_opt()
{
	int addr3 = SymbolTable::ra_addr();
	Opt_Target::add_opt_target("sw", "$ra", to_string(addr3) + "($fp)", "");
}
//出函数时
void Reg::load_ra_reg()
{
	int addr3 = SymbolTable::ra_addr();
	Target::add_target_code("lw", "$ra", to_string(addr3) + "($fp)", "");
}
void Reg::load_ra_opt()
{
	int addr3 = SymbolTable::ra_addr();
	Opt_Target::add_opt_target("lw", "$ra", to_string(addr3) + "($fp)", "");
}
void Reg::save_S_reg()
{
	for (int i = 0; i < 8; i++) {
		if (need_save_S[i] == 1) {
			int addr = SymbolTable::S_reg_addr(i);
			Opt_Target::add_opt_target("sw","$s" + to_string(i),to_string(addr) + "($fp)","");
		}
	}
}
void Reg::load_S_reg()
{
	for (int i = 0; i < 8; i++) {
		if (need_save_S[i] == 1) {
			int addr = SymbolTable::S_reg_addr(i);
			Opt_Target::add_opt_target("lw", "$s" + to_string(i), to_string(addr) + "($fp)", "");
		}
	}
}
void Reg::save_A_reg()
{
	for (int i = 1; i < 4; i++) {
		if (need_save_A[i] == 1) {
			int addr = SymbolTable::A_reg_addr(i);
			Opt_Target::add_opt_target("sw", "$a" + to_string(i), to_string(addr) + "($fp)", "");
		}
	}
}
void Reg::load_A_reg()
{
	for (int i = 1; i < 4; i++) {
		if (need_save_A[i] == 1) {
			int addr = SymbolTable::A_reg_addr(i);
			Opt_Target::add_opt_target("lw", "$a" + to_string(i), to_string(addr) + "($fp)", "");
		}
	}
}
void Reg::save_T_reg()
{
	for (int i = 0; i < 8; i++) {
		if (need_T_save(T_reg[i])) {
			int addr = SymbolTable::T_reg_addr(i);
			Opt_Target::add_opt_target("sw", "$t" + to_string(i), to_string(addr) + "($fp)","");
		}
	}
}
void Reg::load_T_reg()
{
	for (int i = 0; i < 8; i++) {
		if (need_T_save(T_reg[i])) {
			int addr = SymbolTable::T_reg_addr(i);
			Opt_Target::add_opt_target("lw", "$t" + to_string(i), to_string(addr) + "($fp)", "");
		}
	}
}
bool Reg::need_T_save(string name)
{
	if (name == "") return false;
	Mid::op_type op;
	for (int i = current_midcode_index + 1; i < midcode_index; i++) {
		op = midcode_opt1[i].op;
		if (midcode_opt1[i].op_num1 == name || midcode_opt1[i].op_num2 == name || midcode_opt1[i].result == name) {
			return true;
		}
		if (Opt_Target::is_block(op)) {
			break;
		}
	}
	return false;
}

void Reg::opt_save(string name, string reg)
{
	if (name[0] == '#') {
		//临时变量存储
		int addr1 = (atoi(name.substr(1, name.size() - 1).c_str()) - 1) * 4;
		Opt_Target::add_opt_target("sw", reg, to_string(addr1) + "($fp)", "");
	}
	else {
		if (SymbolTable::is_local(name)) {
			//局部变量
			int addr2 = SymbolTable::local_addr(name);
			Opt_Target::add_opt_target("sw", reg, to_string(addr2) + "($fp)", "");
		}
		else {
			//全局变量
			int addr3 = SymbolTable::global_var_addr(name);
			Opt_Target::add_opt_target("sw", reg, to_string(addr3) + "($gp)", "");
		}
	}
}
void Reg::opt_load(string name, string reg)
{
	if (name[0] == '#') {
		//临时变量存储
		int addr1 = (atoi(name.substr(1, name.size() - 1).c_str()) - 1) * 4;
		Opt_Target::add_opt_target("lw", reg, to_string(addr1) + "($fp)", "");
	}
	else {
		if (SymbolTable::is_local(name)) {
			//局部变量
			int addr2 = SymbolTable::local_addr(name);
			Opt_Target::add_opt_target("lw", reg, to_string(addr2) + "($fp)", "");
		}
		else {
			//全局变量
			int addr3 = SymbolTable::global_var_addr(name);
			Opt_Target::add_opt_target("lw", reg, to_string(addr3) + "($gp)", "");
		}
	}
}
string Reg::use_T_assign1(string name) {
	int index = current_T_index % 8;
	if (is_T_reg_ok(index)) {
		T_reg[index] = name;
	}
	else {
		string last_ident = T_reg[index];
		save_reg(last_ident, "$t" + to_string(index));
		T_reg[index] = name;
	}
	current_T_index++;
	return "$t" + to_string(index);
}
string Reg::get_new_T_reg1(string name)
{
	int index = current_T_index % 8;
	if (is_T_reg_ok(index)) {
		T_reg[index] = name;//赋值
		load_reg(name, "$t" + to_string(index));
	}
	else {
		string last_ident = T_reg[index];
		save_reg(last_ident, "$t" + to_string(index));
		T_reg[index] = name;//修改
		load_reg(name, "$t" + to_string(index));
	}
	current_T_index++;
	return "$t" + to_string(index);
}
string Reg::clear_relation(string name)
{
	int index = -1;
	for (int i = 0; i < 3000; i++) {
		if (temp_reg[i] == name) {
			temp_reg[i] = "";
			index = i;
			break;
		}
	}
	if (index == -1) {
		return "";
	}
	return "#" + to_string(index);
}