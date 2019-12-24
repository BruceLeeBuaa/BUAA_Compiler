#include "output_midcode.h"
#include "Globalvar.h"

void output_midcode(int index)
{
	switch (midcode[index].op)
	{
	case Mid::con_declare: {
		SymbolTable::TYPE type = SymbolTable::ident_type(midcode[index].op_num1);
		if (type == SymbolTable::char_const) {
			printf("const char %s = \'%c\'\n", midcode[index].op_num1.c_str(),(char)stoi(midcode[index].op_num2));
		}
		else {
			printf("const int %s = %d\n", midcode[index].op_num1.c_str(),stoi(midcode[index].op_num2));
		}
		break;
	}
	case Mid::var_declare: {
		SymbolTable::TYPE type = SymbolTable::ident_type(midcode[index].op_num1);
		if (type == SymbolTable::int_var) {
			printf("var int %s\n", midcode[index].op_num1.c_str());
		}
		else {
			printf("var char %s\n", midcode[index].op_num1.c_str());
		}
		break;
	}
	case Mid::array_declare: {
		SymbolTable::TYPE type = SymbolTable::ident_type(midcode[index].op_num1);
		if (type == SymbolTable::int_array) {
			printf("array int %s[%s]\n", midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str());
		}
		else {
			printf("array char %s[%s]\n", midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str());
		}
		break;
	}
	case Mid::para_declare: {
		SymbolTable::TYPE type = SymbolTable::ident_type(midcode[index].op_num1);
		if (type == SymbolTable::int_para) {
			printf("para int %s\n", midcode[index].op_num1.c_str());
		}
		else {
			printf("para char %s\n", midcode[index].op_num1.c_str());
		}
		break;
	}
	case Mid::func_declare: {
		SymbolTable::TYPE type = SymbolTable::current_func_type();
		if (type == SymbolTable::int_func) {
			printf("int %s()\n", midcode[index].op_num1.c_str());
		}
		else if (type == SymbolTable::char_func) {
			printf("char %s()\n", midcode[index].op_num1.c_str());
		}
		else {
			printf("void %s()\n", midcode[index].op_num1.c_str());
		}
		break;
	}
	case Mid::add:
		printf("%s = %s + %s\n", midcode[index].result.c_str(),midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str());
		break;
	case Mid::sub:
		printf("%s = %s - %s\n", midcode[index].result.c_str(),midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str());
		break;
	case Mid::mul:
		printf("%s = %s * %s\n", midcode[index].result.c_str(),midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str());
		break;
	case Mid::div:
		printf("%s = %s / %s\n", midcode[index].result.c_str(),midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str());
		break;
	case Mid::neg:
		printf("%s = - %s\n", midcode[index].result.c_str(),midcode[index].op_num1.c_str());
		break;
	case Mid::my_assign:
		printf("%s = %s\n", midcode[index].result.c_str(),midcode[index].op_num1.c_str());
		break;
	case Mid::assign:
		printf("%s = %s\n", midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str());
		break;
	case Mid::arr_assign:
		printf("%s[%s] = %s\n", midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str(),midcode[index].result.c_str());
		break;
	case Mid::arr_take:
		printf("%s = %s[%s]\n", midcode[index].result.c_str(),midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str());
		break;
	case Mid::output_str:
		printf("output string %s\n", midcode[index].op_num1.c_str());
		break;
	case Mid::output:
		printf("output %s\n", midcode[index].op_num1.c_str());
		break;
	case Mid::output_c:
		printf("output char %s\n", midcode[index].op_num1.c_str());
		break;
	case Mid::input:
		printf("input %s\n", midcode[index].op_num1.c_str());
		break;
	case Mid::call:
		printf("call %s()\n", midcode[index].op_num1.c_str());
		break;
	case Mid::push:
		printf("push %s\n", midcode[index].op_num1.c_str());
		break;
	case Mid::ret:
		printf("#ret %s\n", midcode[index].op_num1.c_str());
		break;
	case Mid::receive:
		printf("%s = #ret\n", midcode[index].result.c_str());
		break;
	case Mid::set_label:
		printf("%s:\n", midcode[index].op_num1.c_str());
		break;
	case Mid::_goto:
		printf("goto %s\n", midcode[index].op_num1.c_str());
		break;
	case Mid::_beq:
		printf("beq %s,%s,%s\n", midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str(),midcode[index].result.c_str());
		break;
	case Mid::_bne:
		printf("bne %s,%s,%s\n", midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str(),midcode[index].result.c_str());
		break;
	case Mid::_bgt:
		printf("bgt %s,%s,%s\n", midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str(),midcode[index].result.c_str());
		break;
	case Mid::_bge:
		printf("bge %s,%s,%s\n", midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str(),midcode[index].result.c_str());
		break;
	case Mid::_blt:
		printf("blt %s,%s,%s\n", midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str(),midcode[index].result.c_str());
		break;
	case Mid::_ble:
		printf("ble %s,%s,%s\n", midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str(),midcode[index].result.c_str());
		break;
	case Mid::_beqz:
		printf("beqz %s,%s\n", midcode[index].op_num1.c_str(),midcode[index].result.c_str());
		break;
	case Mid::_bnez:
		printf("bnez %s,%s\n", midcode[index].op_num1.c_str(),midcode[index].result.c_str());
		break;
	case Mid::_addi:
		printf("addi %s,%s,%s\n", midcode[index].result.c_str(),midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str());
		break;
	case Mid::_subi:
		printf("subi %s,%s,%s\n", midcode[index].result.c_str(),midcode[index].op_num1.c_str(),midcode[index].op_num2.c_str());
		break;
	default:
		break;
	}
}
void output_opt1_midcode(int index)
{
	switch (midcode_opt1[index].op)
	{
	case Mid::con_declare: {
		SymbolTable::TYPE type = SymbolTable::mid_opt_ident_type(midcode_opt1[index].op_num1);
		if (type == SymbolTable::char_const) {
			printf("const char %s = \'%c\'\n", midcode_opt1[index].op_num1.c_str(), (char)stoi(midcode_opt1[index].op_num2));
		}
		else {
			printf("const int %s = %d\n", midcode_opt1[index].op_num1.c_str(), stoi(midcode_opt1[index].op_num2));
		}
		break;
	}
	case Mid::var_declare: {
		SymbolTable::TYPE type = SymbolTable::mid_opt_ident_type(midcode_opt1[index].op_num1);
		if (type == SymbolTable::int_var) {
			printf("var int %s\n", midcode_opt1[index].op_num1.c_str());
		}
		else {
			printf("var char %s\n", midcode_opt1[index].op_num1.c_str());
		}
		break;
	}
	case Mid::array_declare: {
		SymbolTable::TYPE type = SymbolTable::mid_opt_ident_type(midcode_opt1[index].op_num1);
		if (type == SymbolTable::int_array) {
			printf("array int %s[%s]\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		}
		else {
			printf("array char %s[%s]\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		}
		break;
	}
	case Mid::para_declare: {
		SymbolTable::TYPE type = SymbolTable::mid_opt_ident_type(midcode_opt1[index].op_num1);
		if (type == SymbolTable::int_para) {
			printf("para int %s\n", midcode_opt1[index].op_num1.c_str());
		}
		else {
			printf("para char %s\n", midcode_opt1[index].op_num1.c_str());
		}
		break;
	}
	case Mid::func_declare: {
		opt_mid_current_func = midcode_opt1[index].op_num1;
		SymbolTable::TYPE type = SymbolTable::mid_opt_func_type();
		if (type == SymbolTable::int_func) {
			printf("int %s()\n", midcode_opt1[index].op_num1.c_str());
		}
		else if (type == SymbolTable::char_func) {
			printf("char %s()\n", midcode_opt1[index].op_num1.c_str());
		}
		else {
			printf("void %s()\n", midcode_opt1[index].op_num1.c_str());
		}
		break;
	}
	case Mid::add:
		printf("%s = %s + %s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	case Mid::sub:
		printf("%s = %s - %s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	case Mid::mul:
		printf("%s = %s * %s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	case Mid::div:
		printf("%s = %s / %s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	case Mid::neg:
		printf("%s = - %s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str());
		break;
	case Mid::my_assign:
		printf("%s = %s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str());
		break;
	case Mid::assign:
		printf("%s = %s\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	case Mid::arr_assign:
		printf("%s[%s] = %s\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str(), midcode_opt1[index].result.c_str());
		break;
	case Mid::arr_take:
		printf("%s = %s[%s]\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	case Mid::output_str:
		printf("output string %s\n", midcode_opt1[index].op_num1.c_str());
		break;
	case Mid::output:
		printf("output %s\n", midcode_opt1[index].op_num1.c_str());
		break;
	case Mid::output_c:
		printf("output char %s\n", midcode_opt1[index].op_num1.c_str());
		break;
	case Mid::input:
		printf("input %s\n", midcode_opt1[index].op_num1.c_str());
		break;
	case Mid::call:
		printf("call %s()\n", midcode_opt1[index].op_num1.c_str());
		break;
	case Mid::push:
		printf("push %s\n", midcode_opt1[index].op_num1.c_str());
		break;
	case Mid::ret:
		printf("#ret %s\n", midcode_opt1[index].op_num1.c_str());
		break;
	case Mid::receive:
		printf("%s = #ret\n", midcode_opt1[index].result.c_str());
		break;
	case Mid::set_label:
		printf("%s:\n", midcode_opt1[index].op_num1.c_str());
		break;
	case Mid::_goto:
		printf("goto %s\n", midcode_opt1[index].op_num1.c_str());
		break;
	case Mid::_beq:
		printf("beq %s,%s,%s\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str(), midcode_opt1[index].result.c_str());
		break;
	case Mid::_bne:
		printf("bne %s,%s,%s\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str(), midcode_opt1[index].result.c_str());
		break;
	case Mid::_bgt:
		printf("bgt %s,%s,%s\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str(), midcode_opt1[index].result.c_str());
		break;
	case Mid::_bge:
		printf("bge %s,%s,%s\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str(), midcode_opt1[index].result.c_str());
		break;
	case Mid::_blt:
		printf("blt %s,%s,%s\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str(), midcode_opt1[index].result.c_str());
		break;
	case Mid::_ble:
		printf("ble %s,%s,%s\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str(), midcode_opt1[index].result.c_str());
		break;
	case Mid::_beqz:
		printf("beqz %s,%s\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].result.c_str());
		break;
	case Mid::_bnez:
		printf("bnez %s,%s\n", midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].result.c_str());
		break;
	case Mid::_addi:
		printf("addi %s,%s,%s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	case Mid::_subi:
		printf("subi %s,%s,%s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	case Mid::none:
		break;
	case Mid::addi:
		printf("%s = %s + %s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	case Mid::subi:
		printf("%s = %s - %s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	case Mid::muli:
		printf("%s = %s * %s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	case Mid::divi:
		printf("%s = %s / %s\n", midcode_opt1[index].result.c_str(), midcode_opt1[index].op_num1.c_str(), midcode_opt1[index].op_num2.c_str());
		break;
	default:
		break;
	}
}
void output_all_midcode()
{
	for (int i = 0; i < midcode_index; i++) {
		output_midcode(i);
	}
}
void output_all_opt1_midcode()
{
	for (int i = 0; i < midcode_index; i++) {
		output_opt1_midcode(i);
	}
}