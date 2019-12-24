#include "Opt_target.h"
#include "Globalvar.h"
#include "Register.h"
#include "Opt_midcode.h"

static string reg1, reg2, reg;
static string op_num1, op_num2, result;

void Opt_Target::get_opt_target()
{
	current_midcode_index = 0;
	opt_const_string();
	opt_enter();
	opt_const_declare();
	opt_global_var();
	opt_text_label();
	opt_j_main();
	while (current_midcode_index < midcode_index) {
		switch (midcode_opt1[current_midcode_index].op)
		{
		case Mid::func_declare: {
			current_func_name = midcode_opt1[current_midcode_index].op_num1;
			reset_relation_TSA();
			memset(need_save_A, 0, sizeof(need_save_A));
			memset(need_save_S, 0, sizeof(need_save_S));
			add_opt_target(current_func_name, ":", "", "");
			int addr = SymbolTable::sp_down_size();
			add_opt_target("subi", "$sp", "$sp", to_string(addr));
			add_opt_target("move", "$fp", "$sp", "");
			Reg::save_ra_opt();
			current_midcode_index++;
			opt_para_declare();
			opt_const_declare();
			opt_vararr_declare();
			current_midcode_index--;
			break;
		}
		case Mid::add: {
			opt_calculate();
			add_opt_target("add", reg, reg1, reg2);
			opt_save_t89();
			break;
		}
		case Mid::sub: {
			opt_calculate();
			add_opt_target("sub", reg, reg1, reg2);
			opt_save_t89();
			break;
		}
		case Mid::mul: {
			opt_calculate();
			add_opt_target("mult", reg1, reg2, "");
			add_opt_target("mflo", reg, "", "");
			opt_save_t89();
			break;
		}
		case Mid::div: {
			opt_calculate();
			add_opt_target("div", reg1, reg2, "");
			add_opt_target("mflo", reg, "", "");
			opt_save_t89();
			break;
		}
		case Mid::addi: {
			opt_calculate_i();
			add_opt_target("addi", reg, reg1, op_num2);
			opt_save_t89();
			break;
		}
		case Mid::subi: {
			opt_calculate_i();
			add_opt_target("subi", reg, reg1, op_num2);
			opt_save_t89();
			break;
		}
		case Mid::muli: {
			opt_calculate_i();
			add_opt_target("mul",reg, reg1, op_num2);
			//add_opt_target("mflo", reg, "", "");
			opt_save_t89();
			break;
		}
		case Mid::divi: {
			opt_calculate_i();
			add_opt_target("div", reg, reg1, op_num2);
			//add_opt_target("mflo", reg, "", "");
			opt_save_t89();
			break;
		}
		case Mid::neg: {
			opt_neg();
			break;
		}
		case Mid::my_assign: {
			opt_my_assign();
			break;
		}
		case Mid::assign: {
			opt_assign();
			break;
		}
		case Mid::arr_assign: {
			opt_arr_assign();
			break;
		}
		case Mid::arr_take: {
			opt_arr_take();
			break;
		}
		case Mid::output_str: {
			op_num2 = midcode_opt1[current_midcode_index].op_num2;
			add_opt_target("la", "$a0", "string" + op_num2, "");
			add_opt_target("li", "$v0", "4", "");
			add_opt_target("syscall", "", "", "");
			if (midcode_opt1[current_midcode_index].result == "enter") {
				add_opt_target("la", "$a0", "enter", "");
				add_opt_target("li", "$v0", "4", "");
				add_opt_target("syscall", "", "", "");
			}
			break;
		}
		case Mid::output: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			reg1 = Reg::get_new_T_reg(op_num1);
			add_opt_target("move", "$a0", reg1, "");
			add_opt_target("li", "$v0", "1", "");
			add_opt_target("syscall", "", "", "");
			add_opt_target("la", "$a0", "enter", "");
			add_opt_target("li", "$v0", "4", "");
			add_opt_target("syscall", "", "", "");
			break;
		}
		case Mid::output_c: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			reg1 = Reg::get_new_T_reg(op_num1);
			add_opt_target("move", "$a0", reg1, "");
			add_opt_target("li", "$v0", "11", "");
			add_opt_target("syscall", "", "", "");
			add_opt_target("la", "$a0", "enter", "");
			add_opt_target("li", "$v0", "4", "");
			add_opt_target("syscall", "", "", "");
			break;
		}
		case Mid::input: {
			opt_input();
			break;
		}
		case Mid::call: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			//Reg::write_back_T();
			Reg::save_S_reg();
			Reg::save_A_reg();
			Reg::save_T_reg();
			add_opt_target("jal", op_num1, "", "");
			Reg::load_T_reg();
			Reg::load_A_reg();
			Reg::load_S_reg();
			break;
		}
		case Mid::push: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			reg1 = Reg::get_new_T_reg(op_num1);
			add_opt_target("addi", "$sp", "$sp", "-4");
			add_opt_target("sw", reg1, "0($sp)", "");
			break;
		}
		case Mid::ret: {
			if (midcode_opt1[current_midcode_index - 1].op == Mid::ret) {
				break;
			}
			if (current_func_name != "main") {
				op_num1 = midcode_opt1[current_midcode_index].op_num1;
				if (op_num1 != "") {
					reg1 = Reg::get_new_T_reg(op_num1);
					add_opt_target("move", "$v0", reg1, "");
				}
				//Reg::write_back_T();
				Reg::load_ra_opt();
				int addr1 = SymbolTable::sp_down_size();
				int addr2 = SymbolTable::func_para_size();
				add_opt_target("addi", "$sp", "$sp", to_string(addr1 + addr2));//升栈时直接升到Push之前
				add_opt_target("move", "$fp", "$sp", "");
				add_opt_target("jr", "$ra", "", "");
			}
			else {
				add_opt_target("li", "$v0", "10", "");
				add_opt_target("syscall", "", "", "");
			}
			break;
		}
		case Mid::receive: {
			result = midcode_opt1[current_midcode_index].result;
			reg = Reg::use_T_assign(result);
			add_opt_target("move", reg, "$v0", "");
			opt_save_t89();
			break;
		}
		case Mid::set_label: {
			//Reg::write_back_T();
			add_opt_target(midcode_opt1[current_midcode_index].op_num1 + ":", "", "", "");
			break;
		}
		case Mid::_goto: {
			//Reg::write_back_T();
			add_opt_target("j", midcode_opt1[current_midcode_index].op_num1, "", "");
			break;
		}
		case Mid::_beq: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			op_num2 = midcode_opt1[current_midcode_index].op_num2;
			result = midcode_opt1[current_midcode_index].result;
			reg1 = Reg::get_new_T_reg(op_num1);
			if (opt_mid::is_number(op_num2)) {
				//Reg::write_back_T();
				add_opt_target("beq", reg1, op_num2, result);
			}
			else {
				reg2 = Reg::get_new_T_reg(op_num2);
				//Reg::write_back_T();
				add_opt_target("beq", reg1, reg2, result);
			}
			break;
		}
		case Mid::_bne: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			op_num2 = midcode_opt1[current_midcode_index].op_num2;
			result = midcode_opt1[current_midcode_index].result;
			reg1 = Reg::get_new_T_reg(op_num1);
			if (opt_mid::is_number(op_num2)) {
				//Reg::write_back_T();
				add_opt_target("bne", reg1, op_num2, result);
			}
			else {
				reg2 = Reg::get_new_T_reg(op_num2);
				//Reg::write_back_T();
				add_opt_target("bne", reg1, reg2, result);
			}
			break;
		}
		case Mid::_bgt: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			op_num2 = midcode_opt1[current_midcode_index].op_num2;
			result = midcode_opt1[current_midcode_index].result;
			reg1 = Reg::get_new_T_reg(op_num1);
			if (opt_mid::is_number(op_num2)) {
				//Reg::write_back_T();
				add_opt_target("bgt", reg1, op_num2, result);
			}
			else {
				reg2 = Reg::get_new_T_reg(op_num2);
				//Reg::write_back_T();
				add_opt_target("bgt", reg1, reg2, result);
			}
			break;
		}
		case Mid::_bge: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			op_num2 = midcode_opt1[current_midcode_index].op_num2;
			result = midcode_opt1[current_midcode_index].result;
			reg1 = Reg::get_new_T_reg(op_num1);
			if (opt_mid::is_number(op_num2)) {
				//Reg::write_back_T();
				add_opt_target("bge", reg1, op_num2, result);
			}
			else {
				reg2 = Reg::get_new_T_reg(op_num2);
				//Reg::write_back_T();
				add_opt_target("bge", reg1, reg2, result);
			}
			break;
		}
		case Mid::_blt: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			op_num2 = midcode_opt1[current_midcode_index].op_num2;
			result = midcode_opt1[current_midcode_index].result;
			reg1 = Reg::get_new_T_reg(op_num1);
			if (opt_mid::is_number(op_num2)) {
				//Reg::write_back_T();
				add_opt_target("blt", reg1, op_num2, result);
			}
			else {
				reg2 = Reg::get_new_T_reg(op_num2);
				//Reg::write_back_T();
				add_opt_target("blt", reg1, reg2, result);
			}
			break;
		}
		case Mid::_ble: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			op_num2 = midcode_opt1[current_midcode_index].op_num2;
			result = midcode_opt1[current_midcode_index].result;
			reg1 = Reg::get_new_T_reg(op_num1);
			if (opt_mid::is_number(op_num2)) {
				//Reg::write_back_T();
				add_opt_target("ble", reg1, op_num2, result);
			}
			else {
				reg2 = Reg::get_new_T_reg(op_num2);
				//Reg::write_back_T();
				add_opt_target("ble", reg1, reg2, result);
			}
			break;
		}
		case Mid::_beqz: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			result = midcode_opt1[current_midcode_index].result;
			reg1 = Reg::get_new_T_reg(op_num1);
			//Reg::write_back_T();
			add_opt_target("beqz", reg1, result, "");
			break;
		}
		case Mid::_bnez: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			result = midcode_opt1[current_midcode_index].result;
			reg1 = Reg::get_new_T_reg(op_num1);
			//Reg::write_back_T();
			add_opt_target("bnez", reg1, result, "");
			break;
		}
		case Mid::_addi: {//默认循环变量一定自递增且不超过8个
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			op_num2 = midcode_opt1[current_midcode_index].op_num2;
			result = midcode_opt1[current_midcode_index].result;
			if (SymbolTable::is_para(result)) {
				reg = Reg::get_A_reg(result);
			}
			else {
				reg = Reg::get_S_reg(result);
			}
			add_opt_target("addi", reg, reg, op_num2);
			break;
		}
		case Mid::_subi: {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			op_num2 = midcode_opt1[current_midcode_index].op_num2;
			result = midcode_opt1[current_midcode_index].result;
			if (SymbolTable::is_para(result)) {
				reg = Reg::get_A_reg(result);
			}
			else {
				reg = Reg::get_S_reg(result);
			}
			add_opt_target("subi", reg, reg, op_num2);
			break;
		}
		case Mid::none:
			break;
		default:
			break;
		}
		current_midcode_index++;
	}
}
void Opt_Target::output_opt_target()
{
	for (int i = 0; i < opt_index; i++) {
		printf("%s %s %s %s \n", target_opt[i].op.c_str(), target_opt[i].result.c_str(), target_opt[i].op_num1.c_str(), target_opt[i].op_num2.c_str());
	}
}
void Opt_Target::opt_const_string()
{
	OptTarget new_target1;
	new_target1.op = ".data";
	target_opt.push_back(new_target1);
	opt_index++;

	for (int i = 0; i < str_const_count; i++) {
		OptTarget new_target2;
		new_target2.op = "string" + to_string(i) + ":.asciiz";
		new_target2.result = "\"";
		for (int j = 0; j < str_const[i].size(); j++) {
			if (str_const[i][j] != '\\') {
				new_target2.result = new_target2.result + str_const[i][j];
			}
			else {
				new_target2.result = new_target2.result + "\\\\";
			}
		}
		new_target2.result = new_target2.result + "\"";
		target_opt.push_back(new_target2);
		opt_index++;
	}
}
void Opt_Target::opt_text_label()
{
	OptTarget new_target;
	new_target.op = ".text";
	target_opt.push_back(new_target);
	opt_index++;
}
void Opt_Target::opt_j_main()
{
	add_opt_target("j", "main", "", "");
}
void Opt_Target::add_opt_target(string op, string result, string op_num1, string op_num2)
{
	OptTarget new_target;
	new_target.op = op;
	new_target.result = result;
	new_target.op_num1 = op_num1;
	new_target.op_num2 = op_num2;
	target_opt.push_back(new_target);
	opt_index++;
}
void Opt_Target::opt_para_declare()
{
	int para_num = 0;
	while (midcode_opt1[current_midcode_index].op == Mid::para_declare) {
		op_num1 = midcode_opt1[current_midcode_index].op_num1;
		if (para_num < 3) {
			para_num++;
			A_reg[para_num] = op_num1;
			//SymbolTable::add_A_reg_to_para(para_num);
			Reg::opt_load(op_num1,"$a"+to_string(para_num));
			need_save_A[para_num] = 1;
		}
		current_midcode_index++;
	}
}
void Opt_Target::opt_const_declare()
{
	while (midcode_opt1[current_midcode_index].op == Mid::con_declare) {
		current_midcode_index++;
	}
}
void Opt_Target::opt_global_var()
{
	while (midcode_opt1[current_midcode_index].op == Mid::var_declare
		|| midcode_opt1[current_midcode_index].op == Mid::array_declare) {
		current_midcode_index++;
	}
}
void Opt_Target::opt_vararr_declare()
{
	int var_num = 0;
	vector<string> temp = SymbolTable::loop_var();
	for (int i = 0; i < temp.size(); i++) {
		S_reg[i] = temp[i];
	}
	var_num = temp.size();
	while (midcode_opt1[current_midcode_index].op == Mid::var_declare
		|| midcode_opt1[current_midcode_index].op == Mid::array_declare) {
		if (midcode_opt1[current_midcode_index].op == Mid::var_declare) {
			op_num1 = midcode_opt1[current_midcode_index].op_num1;
			if (var_num <= 7) {
				if (!Reg::in_S_reg(op_num1)) {
					S_reg[var_num] = op_num1;
					//SymbolTable::add_S_reg_to_var(var_num);
					var_num++;
				}
			}
		}
		current_midcode_index++;
	}
}
void Opt_Target::opt_enter()
{
	add_opt_target("enter:.asciiz", "\"\\n\"", "", "");
}
bool Opt_Target::is_block(Mid::op_type op)
{
	if (op == Mid::_goto || op == Mid::set_label || op == Mid::ret
		|| op == Mid::_beq || op == Mid::_bne || op == Mid::_bgt
		|| op == Mid::_bge || op == Mid::_blt || op == Mid::_ble
		|| op == Mid::_beqz || op == Mid::_bnez) {
		return true;
	}
	else {
		return false;
	}
}
int Opt_Target::get_temp_index(string temp_name)
{
	return (atoi(temp_name.substr(1, temp_name.size() - 1).c_str()));
}
int Opt_Target::get_reg_index(string reg_name)
{
	return (atoi(reg_name.substr(2, reg_name.size() - 2).c_str()));
}
void Opt_Target::opt_save_t89()
{
	if (Reg::need_T_save(result) && (reg == "$t8" || reg == "$t9")) {
		Reg::opt_save(result, reg);
	}
}
void Opt_Target::opt_calculate()
{
	op_num1 = midcode_opt1[current_midcode_index].op_num1;
	op_num2 = midcode_opt1[current_midcode_index].op_num2;
	result = midcode_opt1[current_midcode_index].result;
	reg1 = Reg::get_new_T_reg(op_num1);
	reg2 = Reg::get_new_T_reg(op_num2);
	reg = Reg::use_T_assign(result);
}
void Opt_Target::opt_calculate_i()
{
	op_num1 = midcode_opt1[current_midcode_index].op_num1;
	op_num2 = midcode_opt1[current_midcode_index].op_num2;
	result = midcode_opt1[current_midcode_index].result;
	reg1 = Reg::get_new_T_reg(op_num1);
	reg = Reg::use_T_assign(result);
}
void Opt_Target::opt_neg()
{
	op_num1 = midcode_opt1[current_midcode_index].op_num1;
	result = midcode_opt1[current_midcode_index].result;
	reg1 = Reg::get_new_T_reg(op_num1);
	reg = Reg::use_T_assign(result);
	add_opt_target("mul", reg, reg1, "-1");
	opt_save_t89();
}
void Opt_Target::opt_my_assign() //#result = op_num1
{
	op_num1 = midcode_opt1[current_midcode_index].op_num1;
	result = midcode_opt1[current_midcode_index].result;
	if (op_num1[0] == '+' || op_num1[0] == '-' || (op_num1[0] >= '0' && op_num1[0] <= '9')) {
		reg = Reg::use_T_assign(result);
		add_opt_target("li", reg, op_num1, "");
	}
	else {
		if (SymbolTable::is_para(op_num1)) {
			if (Reg::in_A_reg(op_num1)) {
				reg = Reg::get_A_reg(op_num1);
				int index = get_reg_index(reg);
				temp_reg[get_temp_index(result)] = reg;
			}
			else {
				reg = Reg::use_T_assign(result);
				int addr2 = SymbolTable::para_addr(op_num1);
				add_opt_target("lw",reg,to_string(addr2)+"($fp)","");
			}
		}
		else {
			if (SymbolTable::is_local(op_num1)) {
				if (Reg::in_S_reg(op_num1)) {
					reg = Reg::get_S_reg(op_num1);
					int index = get_reg_index(reg);
					//if (need_save_S[index] == 0) {//为0代表里面还没有存值，但由于局部变量先赋值再使用，所以这块代码其实没用
					//	need_save_S[index] = 1;
					//	int addr3 = SymbolTable::local_addr(op_num1);
					//	add_opt_target("lw", reg, to_string(addr3) + "($fp)", "");
					//}
					temp_reg[get_temp_index(result)] = reg;
				}
				else {
					reg = Reg::use_T_assign(result);
					int addr4 = SymbolTable::local_addr(op_num1);
					add_opt_target("lw", reg, to_string(addr4) + "($fp)", "");
				}
			}
			else {
				//全局变量
				reg = Reg::use_T_assign(result);
				int addr5 = SymbolTable::global_var_addr(op_num1);
				add_opt_target("lw", reg, to_string(addr5) + "($gp)", "");
			}
		}
	}
	opt_save_t89();
}
void Opt_Target::opt_assign() //op_num1 = #op_num2
{
	op_num1 = midcode_opt1[current_midcode_index].op_num1;
	op_num2 = midcode_opt1[current_midcode_index].op_num2;
	reg2 = Reg::get_new_T_reg(op_num2);
	if (SymbolTable::is_para(op_num1)) {
		if (Reg::in_A_reg(op_num1)) {
			reg = Reg::get_A_reg(op_num1);
			int index = get_reg_index(reg);
			need_save_A[index] = 1;
			string temp = "";
			while ((temp = Reg::clear_relation(reg)) != "") {
				if (Reg::need_T_save(temp)) {
					Reg::opt_save(temp, reg);
				}
			}
			//temp_reg[get_temp_index(op_num2)] = reg;
			add_opt_target("move", reg, reg2, "");
		}
		else {
			Reg::opt_save(op_num1, reg2);
		}
	}
	else {
		if (SymbolTable::is_local(op_num1)) {
			if (Reg::in_S_reg(op_num1)) {
				reg = Reg::get_S_reg(op_num1);
				int index = get_reg_index(reg);
				need_save_S[index] = 1;
				string temp = "";
				while ((temp = Reg::clear_relation(reg)) != "") {
					if (Reg::need_T_save(temp)) {
						Reg::opt_save(temp, reg);
					}
				}
				//temp_reg[get_temp_index(op_num2)] = reg;
				add_opt_target("move", reg, reg2, "");
			}
			else {
				Reg::opt_save(op_num1, reg2);
			}
		}
		else {
			//全局变量
			Reg::opt_save(op_num1, reg2);
		}
	}
}
void Opt_Target::opt_arr_assign()
{
	op_num1 = midcode_opt1[current_midcode_index].op_num1;//数组名
	op_num2 = midcode_opt1[current_midcode_index].op_num2;//数组下标
	result = midcode_opt1[current_midcode_index].result;//所赋值
	reg2 = Reg::get_new_T_reg(op_num2);
	if (reg2 == "$t9") reg2 = Reg::get_new_T_reg(op_num2);
	add_opt_target("sll", "$t9", reg2, "2");
	if (SymbolTable::is_local(op_num1)) {
		int addr1 = SymbolTable::local_addr(op_num1);
		add_opt_target("addi", "$t9", "$t9", to_string(addr1));
		add_opt_target("add", "$t9", "$t9", "$fp");
	}
	else {
		int addr2 = SymbolTable::global_var_addr(op_num1);
		add_opt_target("addi", "$t9", "$t9", to_string(addr2));
		add_opt_target("add", "$t9", "$t9", "$gp");
	}
	reg = Reg::get_new_T_reg(result);
	add_opt_target("sw", reg, "0($t9)", "");
}
void Opt_Target::opt_arr_take()
{
	op_num1 = midcode_opt1[current_midcode_index].op_num1;//数组名
	op_num2 = midcode_opt1[current_midcode_index].op_num2;//数组下标
	result = midcode_opt1[current_midcode_index].result;//赋给的值
	reg2 = Reg::get_new_T_reg(op_num2);
	if (reg2 == "$t9") reg2 = Reg::get_new_T_reg(op_num2);
	add_opt_target("sll", "$t9", reg2, "2");
	if (SymbolTable::is_local(op_num1)) {
		int addr1 = SymbolTable::local_addr(op_num1);
		add_opt_target("addi", "$t9", "$t9", to_string(addr1));
		add_opt_target("add", "$t9", "$t9", "$fp");
	}
	else {
		int addr2 = SymbolTable::global_var_addr(op_num1);
		add_opt_target("addi", "$t9", "$t9", to_string(addr2));
		add_opt_target("add", "$t9", "$t9", "$gp");
	}
	reg = Reg::use_T_assign(result);
	add_opt_target("lw", reg, "0($t9)", "");
	opt_save_t89();
}
void Opt_Target::opt_input()
{
	op_num1 = midcode_opt1[current_midcode_index].op_num1;
	if (SymbolTable::is_int(op_num1)) {
		add_opt_target("li", "$v0", "5", "");
	}
	else {
		add_opt_target("li", "$v0", "12", "");
	}
	add_opt_target("syscall", "", "", "");
	if (SymbolTable::is_para(op_num1)) {
		if (Reg::in_A_reg(op_num1)) {
			reg1 = Reg::get_A_reg(op_num1);
			int index = get_reg_index(reg1);
			need_save_A[index] = 1;
			string temp = "";
			while ((temp = Reg::clear_relation(reg)) != "") {
				if (Reg::need_T_save(temp)) {
					Reg::opt_save(temp, reg);
				}
			}
			add_opt_target("move", reg1, "$v0", "");
		}
		else {
			int addr1 = SymbolTable::local_addr(op_num1);
			add_opt_target("sw", "$v0", to_string(addr1) + "($fp)", "");
		}
	}
	else {
		if (SymbolTable::is_local(op_num1)) {
			if (Reg::in_S_reg(op_num1)) {
				reg1 = Reg::get_S_reg(op_num1);
				int index = get_reg_index(reg1);
				need_save_S[index] = 1;
				string temp = "";
				while ((temp = Reg::clear_relation(reg)) != "") {
					if (Reg::need_T_save(temp)) {
						Reg::opt_save(temp, reg);
					}
				}
				add_opt_target("move", reg1, "$v0", "");
			}
			else {
				int addr2 = SymbolTable::local_addr(op_num1);
				add_opt_target("sw", "$v0", to_string(addr2) + "($fp)", "");
			}
		}
		else {
			//全局变量
			int addr3 = SymbolTable::global_var_addr(op_num1);
			add_opt_target("sw", "$v0", to_string(addr3) + "($gp)", "");
		}
	}
}
void Opt_Target::reset_relation_TSA()
{
	Reg::reset_A_reg();
	Reg::reset_S_reg();
	Reg::reset_T_reg();
	Reg::reset_temp_reg();
}