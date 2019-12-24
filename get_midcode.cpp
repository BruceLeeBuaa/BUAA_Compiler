#include "get_midcode.h"
#include "output_midcode.h"
#include "Globalvar.h"

void Mid::add_mid_const(int value)//常量
{
	num_index++;//#1

	MidCode new_mid;
	new_mid.op = my_assign;
	new_mid.op_num1 = to_string(value);
	new_mid.result = "#" + to_string(num_index);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);//打印当前中间代码
}

void Mid::add_mid_arr_take(string ident_name, int index_temp)//数组取值
{
	num_index++;

	MidCode new_mid;
	new_mid.op = arr_take;
	new_mid.op_num1 = ident_name;
	new_mid.op_num2 = "#" + to_string(index_temp);
	new_mid.result = "#" + to_string(num_index);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_receive()//接受函数调用返回值
{
	num_index++;

	MidCode new_mid;
	new_mid.op = receive;
	new_mid.result = "#" + to_string(num_index);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_ident(string ident_name)//变量标识符(非数组)
{
	num_index++;

	MidCode new_mid;
	new_mid.op = my_assign;
	new_mid.op_num1 = ident_name;
	new_mid.result = "#" + to_string(num_index);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_cal(Lexical::WORD op, int temp1, int temp2)//表达式计算(项或因子)
{
	num_index++;

	MidCode new_mid;
	switch (op)
	{
	case Lexical::PLUS:
		new_mid.op = add;
		break;
	case Lexical::MINU:
		new_mid.op = sub;
		break;
	case Lexical::MULT:
		new_mid.op = mul;
		break;
	case Lexical::DIV:
		new_mid.op = div;
		break;
	default:
		break;
	}
	new_mid.op_num1 = "#" + to_string(temp1); 
	new_mid.op_num2 = "#" + to_string(temp2);
	new_mid.result = "#" + to_string(num_index);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_neg(int temp)
{
	num_index++;

	MidCode new_mid;
	new_mid.op = neg;
	new_mid.op_num1 = "#" + to_string(temp);
	new_mid.result = "#" + to_string(num_index);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_label(int label_num)
{
	MidCode new_mid;
	new_mid.op = set_label;
	new_mid.op_num1 = "Label" + to_string(label_num);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_goto(int label_num)
{
	MidCode new_mid;
	new_mid.op =_goto;
	new_mid.op_num1 = "Label" + to_string(label_num);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_condition_whileforif(Lexical::WORD op, int temp1, int temp2, int label_num)//不满足条件跳转
{
	MidCode new_mid;
	switch (op)
	{
	case Lexical::LSS:
		new_mid.op = _bge;
		break;
	case Lexical::LEQ:
		new_mid.op = _bgt;
		break;
	case Lexical::GRE:
		new_mid.op = _ble;
		break;
	case Lexical::GEQ:
		new_mid.op = _blt;
		break;
	case Lexical::EQL:
		new_mid.op = _bne;
		break;
	case Lexical::NEQ:
		new_mid.op = _beq;
		break;
	case Lexical::DEFAULT:
		new_mid.op = _beqz;
		break;
	default:
		break;
	}
	if (op == Lexical::DEFAULT) {
		new_mid.op_num1 = "#" + to_string(temp1);
		new_mid.result = "Label" + to_string(label_num);
	}
	else {
		new_mid.op_num1 = "#" + to_string(temp1);
		new_mid.op_num2 = "#" + to_string(temp2);
		new_mid.result = "Label" + to_string(label_num);
	}

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_condition_dowhile(Lexical::WORD op, int temp1, int temp2, int label_num)	//满足条件跳转
{
	MidCode new_mid;
	switch (op)
	{
	case Lexical::LSS:
		new_mid.op = _blt;
		break;
	case Lexical::LEQ:
		new_mid.op = _ble;
		break;
	case Lexical::GRE:
		new_mid.op = _bgt;
		break;
	case Lexical::GEQ:
		new_mid.op = _bge;
		break;
	case Lexical::EQL:
		new_mid.op = _beq;
		break;
	case Lexical::NEQ:
		new_mid.op = _bne;
		break;
	case Lexical::DEFAULT:
		new_mid.op = _bnez;
		break;
	default:
		break;
	}
	if (op == Lexical::DEFAULT) {
		new_mid.op_num1 = "#" + to_string(temp1);
		new_mid.result = "Label" + to_string(label_num);
	}
	else {
		new_mid.op_num1 = "#" + to_string(temp1);
		new_mid.op_num2 = "#" + to_string(temp2);
		new_mid.result = "Label" + to_string(label_num);
	}

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_assign(string ident_name, int temp1)
{
	MidCode new_mid;
	new_mid.op = assign;
	new_mid.op_num1 = ident_name;
	new_mid.op_num2 = "#" + to_string(temp1);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_arr_assign(string ident_name, int temp1, int temp2)
{
	//temp1:数组下标，temp2:所赋值
	MidCode new_mid;
	new_mid.op = arr_assign;
	new_mid.op_num1 = ident_name;
	new_mid.op_num2 = "#" + to_string(temp1);
	new_mid.result = "#" + to_string(temp2);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_scanf(string ident_name)
{
	MidCode new_mid;
	new_mid.op = input;
	new_mid.op_num1 = ident_name;

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_print_str(int index, int have_n)
{
	MidCode new_mid;
	new_mid.op = output_str;
	new_mid.op_num1 = str_const[index];
	new_mid.op_num2 = to_string(index);
	if (have_n == 1) {
		new_mid.result = "enter";
	}
	else {
		new_mid.result = "";
	}

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_print(int index)
{
	MidCode new_mid;
	new_mid.op = output;
	new_mid.op_num1 = "#" + to_string(index);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_print_c(int index)
{
	MidCode new_mid;
	new_mid.op = output_c;
	new_mid.op_num1 = "#" + to_string(index);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_return(int index)
{
	MidCode new_mid;
	new_mid.op = ret;
	if (index != -1) {
		new_mid.op_num1 = "#" + to_string(index);
	}

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_push(int temp)
{
	MidCode new_mid;
	new_mid.op = push;
	new_mid.op_num1 = "#" + to_string(temp);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_call(string func_name)
{
	MidCode new_mid;
	new_mid.op = call;
	new_mid.op_num1 = func_name;

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_con_declare(string const_ident_name, int value)
{
	MidCode new_mid;
	new_mid.op = con_declare;
	new_mid.op_num1 = const_ident_name;
	new_mid.op_num2 = to_string(value);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_var_declare(string var_ident_name)
{
	MidCode new_mid;
	new_mid.op = var_declare;
	new_mid.op_num1 = var_ident_name;

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_arr_declare(string arr_ident_name, int arr_size)
{
	MidCode new_mid;
	new_mid.op = array_declare;
	new_mid.op_num1 = arr_ident_name;
	new_mid.op_num2 = to_string(arr_size);

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_para_declare(string para_ident_name)
{
	MidCode new_mid;
	new_mid.op = para_declare;
	new_mid.op_num1 = para_ident_name;

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_func_declare(string func_ident_name)
{
	MidCode new_mid;
	new_mid.op = func_declare;
	new_mid.op_num1 = func_ident_name;

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}

void Mid::add_mid_for_step(string ident_name1,string ident_name2, Lexical::WORD op, int step)
{
	MidCode new_mid;
	if (op == Lexical::PLUS) {
		new_mid.op = _addi;
	}
	else {
		new_mid.op = _subi;
	}
	new_mid.op_num1 = ident_name2;
	new_mid.op_num2 = to_string(step);
	new_mid.result = ident_name1;

	midcode_index++;
	midcode.push_back(new_mid);
	if (print_midcode) output_midcode(midcode_index - 1);
}