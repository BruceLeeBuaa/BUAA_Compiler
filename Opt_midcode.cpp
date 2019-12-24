#include "Opt_midcode.h"
#include "Globalvar.h"
#include "get_midcode.h"
#include "output_midcode.h"

void opt_mid::scan_inline_1()
{
	current_midcode_index = 0;
	while (current_midcode_index < midcode_index) {
		switch (midcode[current_midcode_index].op)
		{
		case Mid::func_declare: {
			current_func_name = midcode[current_midcode_index].op_num1;
			if (current_func_name == "main") {
				break;
			}
			In_Line::add_func();
			break;
		}
		default:
			break;
		}
		current_midcode_index++;
	}
}

void opt_mid::scan_inline_2()
{
	string op_num1, op_num2, result;
	current_midcode_index = 0;
	while (current_midcode_index < midcode_index) {
		switch (midcode[current_midcode_index].op)
		{
		case Mid::func_declare: {
			current_func_name = midcode[current_midcode_index].op_num1;
			break;
		}
		case Mid::push: {
			In_Line::push_case();
			break;
		}
		case Mid::call: {
			In_Line::call_case();
			break;
		}
		default:
			break;
		}
		current_midcode_index++;
	}
}

//循环变量优先分配S寄存器
void opt_mid::scan_zero()
{
	string op_num1, op_num2, result;
	for (int i = 0; i < midcode_index; i++) {
		switch (midcode[i].op)
		{
		case Mid::func_declare: {
			current_func_name = midcode[i].op_num1;
			break;
		}
		case Mid::_addi: {
			result = midcode[i].result;
			SymbolTable::add_loop_var(result);
			break;
		}
		case Mid::_subi: {
			result = midcode[i].result;
			SymbolTable::add_loop_var(result);
			break;
		}
		default:
			break;
		}
	}
}

// 临时变量 ： 局部变量
// 临时变量 ： 临时变量(这个可能出现很多次，最多一层嵌套)
void opt_mid::scan_one()
{
	int index1,index2,index3;
	string op_num1,op_num2,result;
	for (int i = 0; i < midcode_index; i++) {
		switch (midcode[i].op)
		{
		case Mid::func_declare: {
			remove_all_relation();
			current_func_name = midcode[i].op_num1;
			midcode_opt1.push_back(midcode[i]);
			break;
		}
		case Mid::my_assign: { // #result = op_num1
			op_num1 = midcode[i].op_num1;
			result = midcode[i].result;
			index1 = get_temp_from_var(op_num1);
			index2 = get_temp_index(result);
			if (index1 == -1) {
				if (is_number(op_num1)) {
					add_relation(index2, op_num1);
				}
				else {
					if (SymbolTable::is_local(op_num1) || SymbolTable::is_const(op_num1)) {
						add_relation(index2,op_num1);
					}
				}
				midcode_opt1.push_back(midcode[i]);
			}
			else {
				push_back_none();
				add_relation(index2, "#" + to_string(index1));
			}
			break;
		}
		case Mid::assign: { //op_num1 = #op_num2
			op_num1 = midcode[i].op_num1;
			op_num2 = midcode[i].op_num2;
			index2 = get_temp_index(op_num2);
			if (relations[index2].var_name != "" && relations[index2].var_name[0] == '#') {
				//如果有对应的临时变量可以替换，则进行替换
				op_num2 = relations[index2].var_name;
			}
			remove_relation(op_num1);
			index2 = get_temp_index(op_num2);
			/*
			if (is_number(relations[index2].var_name)) {
				op_num2 = relations[index2].var_name;
				Mid::MidCode new_mid = midcode[i];
				new_mid.op_num2 = op_num2;
				midcode_opt1.push_back(new_mid);
				break;
			}
			*/
			if (SymbolTable::is_local(op_num1) || SymbolTable::is_const(op_num1)) {
				add_relation(index2,op_num1);
			}
			Mid::MidCode new_mid = midcode[i];
			new_mid.op_num2 = op_num2;
			midcode_opt1.push_back(new_mid);
			break;
		}
		case Mid::add:
		case Mid::mul:
		case Mid::div:
		case Mid::sub: { //#result = #var1 op #var2 
			int sign1 = 0;int sign2 = 0;
			op_num1 = midcode[i].op_num1;
			op_num2 = midcode[i].op_num2;
			result = midcode[i].result;
			index1 = get_temp_index(op_num1);
			index2 = get_temp_index(op_num2);
			if (relations[index2].var_name != "") {
				if (relations[index2].var_name[0] == '#') {
					op_num2 = relations[index2].var_name;
					index3 = get_temp_index(op_num2);
					if (is_number(relations[index3].var_name)) {
						op_num2 = relations[index3].var_name;
						sign2 = 1;
					}
				}
				else {
					if (is_number(relations[index2].var_name)) {
						op_num2 = relations[index2].var_name;
						sign2 = 1;
					}
				}
			}
			if (relations[index1].var_name != "") {
				if (relations[index1].var_name[0] == '#') {
					op_num1 = relations[index1].var_name;
					index3 = get_temp_index(op_num1);
					if (is_number(relations[index3].var_name)) {
						op_num1 = relations[index3].var_name;
						sign1 = 1;						// op_num2是数字
					}
				}
				else {
					if (is_number(relations[index1].var_name)) {
						op_num1 = relations[index1].var_name;
						sign1 = 1;
					}
				}
			}
			Mid::MidCode new_mid = midcode[i];
			new_mid.op_num1 = op_num1;
			new_mid.op_num2 = op_num2;
			if (sign1 == 0 && sign2 == 1) {
				new_mid.op = new_op(midcode[i].op);
			}	
			else if (sign1 == 1 && sign2 == 0 && (midcode[i].op == Mid::add || midcode[i].op == Mid::mul)) {
				string temp = new_mid.op_num1;
				new_mid.op_num1 = new_mid.op_num2;
				new_mid.op_num2 = temp;
				new_mid.op = new_op(midcode[i].op);
			}
			else if (sign1 == 1 && sign2 == 1) {
				int flag = 0;
				switch (midcode[i].op)
				{
				case Mid::add:
					flag = atoi(op_num1.c_str()) + atoi(op_num2.c_str());
					break;
				case Mid::sub:
					flag = atoi(op_num1.c_str()) - atoi(op_num2.c_str());
					break;
				case Mid::mul:
					flag = atoi(op_num1.c_str()) * atoi(op_num2.c_str());
					break;
				case Mid::div: {
					if (atoi(op_num2.c_str()) == 0) flag = 0;
					else flag = atoi(op_num1.c_str()) / atoi(op_num2.c_str());
					break;
				}
				default:
					break;
				}
				new_mid.op = Mid::my_assign;
				new_mid.op_num2.clear();
				new_mid.op_num1 = to_string(flag);
				index3 = get_temp_index(result);
				add_relation(index3, to_string(flag));
			}
			midcode_opt1.push_back(new_mid);
			break;
		}
		case Mid::neg: {//#result = -#var1
			op_num1 = midcode[i].op_num1;
			result = midcode[i].op_num2;
			index1 = get_temp_index(op_num1);
			if (relations[index1].var_name != "" && relations[index1].var_name[0] == '#') {
				op_num1 = relations[index1].var_name;
			}
			index1 = get_temp_index(op_num1);
			/*
			if (is_number(relations[index1].var_name)) {
				op_num1 = relations[index1].var_name;
				op_num1 = "-" + op_num1;
				Mid::MidCode new_mid = midcode[i];
				new_mid.op_num1 = op_num1;
				new_mid.op = Mid::my_assign;
				index2 = get_temp_index(result);
				add_relation(index2, op_num1);
				midcode_opt1.push_back(new_mid);
				break;
			}
			*/
			Mid::MidCode new_mid = midcode[i];
			new_mid.op_num1 = op_num1;
			midcode_opt1.push_back(new_mid);
			break;
		}
		case Mid::push:
		case Mid::output:
		case Mid::output_c: {
			op_num1 = midcode[i].op_num1;
			index1 = get_temp_index(op_num1);
			if (relations[index1].var_name != "" && relations[index1].var_name[0] == '#') {
				op_num1 = relations[index1].var_name;
			}
			Mid::MidCode new_mid = midcode[i];
			new_mid.op_num1 = op_num1;
			midcode_opt1.push_back(new_mid);
			break;
		}
		case Mid::arr_assign:
		case Mid::arr_take: {
			//arr_assign: var1[#var2] = #result
			//arr_take: #result = var1[#var2]					
			result = midcode[i].result;
			op_num2 = midcode[i].op_num2;
			index1 = get_temp_index(result);
			index2 = get_temp_index(op_num2);
			if (relations[index1].var_name != "" && relations[index1].var_name[0] == '#') {
				result = relations[index1].var_name;
			}
			if (relations[index2].var_name != "" && relations[index2].var_name[0] == '#') {
				op_num2 = relations[index2].var_name;
			}
			Mid::MidCode new_mid = midcode[i];
			new_mid.result = result;
			new_mid.op_num2 = op_num2;
			midcode_opt1.push_back(new_mid);
			break;
		}
		case Mid::_beqz:
		case Mid::_bnez: { //bnez #1 labeli
			int sign = 0;
			op_num1 = midcode[i].op_num1;
			index1 = get_temp_index(op_num1);
			if (relations[index1].var_name != "") {
				if (relations[index1].var_name[0] == '#') {
					op_num1 = relations[index1].var_name;
					index2 = get_temp_index(op_num1);
					if (is_number(relations[index2].var_name)) {
						op_num1 = relations[index2].var_name;
						sign = 1;
					}
				}
				else {
					if (is_number(relations[index1].var_name)) {
						op_num1 = relations[index1].var_name;
						sign = 1;
					}
				}
			}
			Mid::MidCode new_mid = midcode[i];
			new_mid.op_num1 = op_num1;
			if (midcode[i].op == Mid::_beqz) {
				if (sign == 1) {
					if (atoi(op_num1.c_str()) == 0) {
						new_mid.op = Mid::_goto;
						new_mid.op_num1 = new_mid.result;
						new_mid.result.clear();
					}
					else {//不跳转
						new_mid.op = Mid::none;
					}
				}
			}
			else {//_bnez
				if (sign == 1) {
					if (atoi(op_num1.c_str()) != 0) {
						new_mid.op = Mid::_goto;
						new_mid.op_num1 = new_mid.result;
						new_mid.result.clear();
					}
					else {
						new_mid.op = Mid::none;
					}
				}
			}
			midcode_opt1.push_back(new_mid);
			break;
		}
		case Mid::_beq:
		case Mid::_bne:
		case Mid::_bge:
		case Mid::_bgt:
		case Mid::_ble:
		case Mid::_blt: {
			int sign1 = 0;int sign2 = 0;
			op_num1 = midcode[i].op_num1;
			op_num2 = midcode[i].op_num2;
			index1 = get_temp_index(op_num1);
			index2 = get_temp_index(op_num2);
			if (relations[index1].var_name != "") {
				if (relations[index1].var_name[0] == '#') {
					op_num1 = relations[index1].var_name;
					index3 = get_temp_index(op_num1);
					if (is_number(relations[index3].var_name)) {
						op_num1 = relations[index3].var_name;
						sign1 = 1;
					}
				}
				else {
					if (is_number(relations[index1].var_name)) {
						op_num1 = relations[index1].var_name;
						sign1 = 1;
					}
				}
			}
			if (relations[index2].var_name != "") {
				if (relations[index2].var_name[0] == '#') {
					op_num2 = relations[index2].var_name;
					index3 = get_temp_index(op_num2);
					if (is_number(relations[index3].var_name)) {
						op_num2 = relations[index3].var_name;
						sign2 = 1;
					}
				}
				else {
					if (is_number(relations[index2].var_name)) {
						op_num2 = relations[index2].var_name;
						sign2 = 1;
					}
				}
			}
			Mid::MidCode new_mid = midcode[i];
			new_mid.op_num1 = op_num1;
			new_mid.op_num2 = op_num2;
			switch(new_mid.op) 
			{
			case Mid::_beq: {
				if (sign1 == 1 && sign2 == 1) {
					if (atoi(op_num1.c_str()) == atoi(op_num2.c_str())) {
						new_mid.op = Mid::_goto;
						new_mid.op_num1 = new_mid.result;
						new_mid.result.clear();
					}
					else {
						new_mid.op = Mid::none;
					}
				}
				else if (sign1 == 1 && sign2 == 0) {
					string temp = new_mid.op_num1;
					new_mid.op_num1 = new_mid.op_num2;
					new_mid.op_num2 = temp;
				}
				break;
			}
			case Mid::_bne: {
				if (sign1 == 1 && sign2 == 1) {
					if (atoi(op_num1.c_str()) != atoi(op_num2.c_str())) {
						new_mid.op = Mid::_goto;
						new_mid.op_num1 = new_mid.result;
						new_mid.result.clear();
					}
					else {
						new_mid.op = Mid::none;
					}
				}
				else if (sign1 == 1 && sign2 == 0) {
					string temp = new_mid.op_num1;
					new_mid.op_num1 = new_mid.op_num2;
					new_mid.op_num2 = temp;
				}
				break;
			}
			case Mid::_bge: {
				if (sign1 == 1 && sign2 == 1) {
					if (atoi(op_num1.c_str()) >= atoi(op_num2.c_str())) {
						new_mid.op = Mid::_goto;
						new_mid.op_num1 = new_mid.result;
						new_mid.result.clear();
					}
					else {
						new_mid.op = Mid::none;
					}
				}
				else if (sign1 == 1 && sign2 == 0) {
					string temp = new_mid.op_num1;
					new_mid.op_num1 = new_mid.op_num2;
					new_mid.op_num2 = temp;
					new_mid.op = Mid::_ble;
				}
				break;
			}
			case Mid::_bgt: {
				if (sign1 == 1 && sign2 == 1) {
					if (atoi(op_num1.c_str()) > atoi(op_num2.c_str())) {
						new_mid.op = Mid::_goto;
						new_mid.op_num1 = new_mid.result;
						new_mid.result.clear();
					}
					else {
						new_mid.op = Mid::none;
					}
				}
				else if (sign1 == 1 && sign2 == 0) {
					string temp = new_mid.op_num1;
					new_mid.op_num1 = new_mid.op_num2;
					new_mid.op_num2 = temp;
					new_mid.op = Mid::_blt;
				}
				break;
			}
			case Mid::_ble: {
				if (sign1 == 1 && sign2 == 1) {
					if (atoi(op_num1.c_str()) <= atoi(op_num2.c_str())) {
						new_mid.op = Mid::_goto;
						new_mid.op_num1 = new_mid.result;
						new_mid.result.clear();
					}
					else {
						new_mid.op = Mid::none;
					}
				}
				else if (sign1 == 1 && sign2 == 0) {
					string temp = new_mid.op_num1;
					new_mid.op_num1 = new_mid.op_num2;
					new_mid.op_num2 = temp;
					new_mid.op = Mid::_bge;
				}
				break;
			}
			case Mid::_blt: {
				if (sign1 == 1 && sign2 == 1) {
					if (atoi(op_num1.c_str()) < atoi(op_num2.c_str())) {
						new_mid.op = Mid::_goto;
						new_mid.op_num1 = new_mid.result;
						new_mid.result.clear();
					}
					else {
						new_mid.op = Mid::none;
					}
				}
				else if (sign1 == 1 && sign2 == 0) {
					string temp = new_mid.op_num1;
					new_mid.op_num1 = new_mid.op_num2;
					new_mid.op_num2 = temp;
					new_mid.op = Mid::_bgt;
				}
				break;
			}
			default:
				break;
			}
			midcode_opt1.push_back(new_mid);
			remove_all_relation();
			break;
		}
		case Mid::ret: {
			midcode_opt1.push_back(midcode[i]);
			op_num1 = midcode[i].op_num1;
			if (op_num1 == "") break;
			else {
				index1 = get_temp_index(op_num1);
				if (relations[index1].var_name != "" && relations[index1].var_name[0] == '#') {
					midcode_opt1[i].op_num1 = relations[index1].var_name;
				}
			}
			remove_all_relation();
			break;
		}
		case Mid::_goto:
		case Mid::set_label: {
			remove_all_relation();
			midcode_opt1.push_back(midcode[i]);
			break;
		}
		default:
			midcode_opt1.push_back(midcode[i]);
			break;
		}
	}
	
	for (int k = 0; k < 10; k++) {
		for (int j = 0; j < midcode_index; j++) {
			Mid::op_type op = midcode_opt1[j].op;
			if (op == Mid::func_declare) {
				current_func_name = midcode_opt1[j].op_num1;
				begin_index = j;
			}
			else if (op == Mid::my_assign) {
				result = midcode_opt1[j].result;
				if (!need_use_later(result, j)) {
					midcode_opt1[j].op = Mid::none;
				}
			}
			else if (op == Mid::assign) {
				op_num1 = midcode_opt1[j].op_num1;
				if (!SymbolTable::is_local(op_num1)) continue;
				if (!need_use_later_local(op_num1,j)) {
					midcode_opt1[j].op = Mid::none;
				}
			}
			else if (op == Mid::arr_take || op == Mid::receive || op == Mid::neg
				|| op == Mid::add || op == Mid::sub || op == Mid::mul || op == Mid::div
				|| op == Mid::addi || op == Mid::subi || op == Mid::muli || op == Mid::divi){
				result = midcode_opt1[j].result;
				if (!need_use_later(result, j)) {
					midcode_opt1[j].op = Mid::none;
				}
			}
		}
	}
}

bool opt_mid::need_use_later_local(string var_name,int k) {
	for (int i = begin_index+1; i < midcode_index; i++)
	{
		if (midcode_opt1[i].op == Mid::none) continue;
		if (i == k) continue;
		if (midcode_opt1[i].op == Mid::var_declare || midcode_opt1[i].op == Mid::para_declare) continue;
		if ((var_name == midcode_opt1[i].op_num1 && midcode_opt1[i].op != Mid::assign) || var_name == midcode_opt1[i].op_num2) {
			return true;
		}
		if (midcode_opt1[i].op == Mid::func_declare) {
			return false;
		}
	}
	return false;//代表后面不会用到var_name这个变量:删除
}

void opt_mid::add_relation(int index, string var_name)
{
	relations[index].var_name = var_name;
	relations[index].temp_name = "#" + to_string(index);
}

void opt_mid::remove_relation(string var_name)
{
	for (int i = 0;i < 3000;i++) {
		if (relations[i].var_name == var_name) {
			relations[i].var_name.clear();
			remove_relation("#" + to_string(i));//只会有一层递归
		}
	}
}

void opt_mid::remove_all_relation()
{
	for (int i = 1;i < 3000;i++) {
		relations[i].var_name.clear();
	}
}

int opt_mid::get_temp_from_var(string var_name)
{
	for (int i = 1;i < 3000;i++) {
		if (relations[i].var_name == var_name) {
			return i;
		}
	}
	return -1;//没有临时变量与此ident对应
}
//只有加减
Mid::op_type opt_mid::new_op(Mid::op_type op)
{
	Mid::op_type new_op = op;
	switch (op)
	{
	case Mid::add:
		new_op = Mid::addi;
		break;
	case Mid::sub:
		new_op = Mid::subi;
		break;
	case Mid::mul:
		new_op = Mid::muli;
		break;
	case Mid::div:
		new_op = Mid::divi;
		break;
	default:
		break;
	}
	return new_op;
}

int opt_mid::get_temp_index(string temp_name)
{
	int index = atoi(temp_name.substr(1, temp_name.size() - 1).c_str());
	return index;
}

bool opt_mid::is_number(string var_name)
{
	return (var_name[0] == '+' || var_name[0] == '-' || (var_name[0] >= '0' && var_name[0] <= '9'));
}
bool opt_mid::is_alu_op(Mid::op_type op)
{
	return (op == Mid::add || op == Mid::sub || op == Mid::div || op == Mid::mul);
}
bool opt_mid::is_branch_op(Mid::op_type op)
{
	return (op == Mid::_beq || op == Mid::_bge || op == Mid::_bgt || op == Mid::_ble || op == Mid::_blt || op == Mid::_bne
		|| op == Mid::_beqz || op == Mid::_bnez);
}
bool opt_mid::is_baseblock_op(Mid::op_type op)
{
	return (op == Mid::ret || op == Mid::_goto || op == Mid::set_label);
}
bool opt_mid::need_use_later(string var_name, int begin)
{
	for (int i = begin + 1; i < midcode_index; i++)
	{
		if (midcode_opt1[i].op == Mid::none) continue;
		if (var_name == midcode_opt1[i].result || var_name == midcode_opt1[i].op_num1 || var_name == midcode_opt1[i].op_num2) {
			return true;
		}
		if (midcode_opt1[i].op == Mid::func_declare) {
			return false;
		}
	}
	return false;//代表后面不会用到var_name这个变量
}

void opt_mid::push_back_none()
{
	Mid::MidCode new_mid;
	new_mid.op = Mid::none;
	midcode_opt1.push_back(new_mid);
}

void opt_mid::opt_midcode()
{
	scan_inline_1();
	scan_inline_2();
	scan_zero();
	scan_one();
}