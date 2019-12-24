#include "Globalvar.h"
#include "targetcode.h"
#include "Register.h"
//函数调用给被调函数开栈:para

//jal函数调用寄存器保存顺序para(调用函数)——(被调用函数)ra——s8——t10——函数局部(变量+数组)——#i临时变量 从上到下

//全局变量和数组存放在$gp向上的空间内，我们需要有一个ident_name到
//距离$gp的偏移的映射(数组记录数组名)

//函数开栈我们需要有一个从ident_name到距离函数起始偏移
//的映射(数组记录数组名)

static string reg1, reg2, reg;
static string op_num1, op_num2, result;

void Target::add_const_string()
{
	TargetCode new_target1;
	new_target1.op = ".data";
	target_code.push_back(new_target1);
	tarcode_index++;

	for (int i = 0;i < str_const_count;i++) {
		TargetCode new_target2;
		new_target2.op = "string" + to_string(i) + ":.asciiz";
		new_target2.result = "\"";
		for (int j = 0;j < str_const[i].size();j++) {
			if (str_const[i][j] != '\\') {
				new_target2.result = new_target2.result + str_const[i][j];
			}
			else {
				new_target2.result = new_target2.result + "\\\\";
			}
		}
		new_target2.result = new_target2.result + "\"";
		target_code.push_back(new_target2);
		tarcode_index++;
	}
}

void Target::add_text_label()
{
	TargetCode new_target;
	new_target.op = ".text";
	target_code.push_back(new_target);
	tarcode_index++;
}

void Target::add_target_code(string op,string result, string op_num1,string op_num2)
{
	TargetCode new_target;
	new_target.op = op;
	new_target.result = result;
	new_target.op_num1 = op_num1;
	new_target.op_num2= op_num2;
	target_code.push_back(new_target);
	tarcode_index++;
}

void Target::ignore_para_declare()
{
	while (midcode[current_midcode_index].op == Mid::para_declare) {
		current_midcode_index++;
	}
}

void Target::ignore_const_declare()
{
	while (midcode[current_midcode_index].op == Mid::con_declare) {
		current_midcode_index++;
	}
}

void Target::ignore_vararr_declare()
{
	while (midcode[current_midcode_index].op == Mid::var_declare
		|| midcode[current_midcode_index].op == Mid::array_declare) {
		current_midcode_index++;
	}
}

void Target::add_calculate()
{
	op_num1 = midcode[current_midcode_index].op_num1;
	op_num2 = midcode[current_midcode_index].op_num2;
	result = midcode[current_midcode_index].result;
	reg1 = Reg::get_ident_reg(op_num1);
	reg2 = Reg::get_ident_reg(op_num2);
	reg = Reg::use_T_assign1(result);
}
void Target::add_calculate_i()
{
	op_num1 = midcode[current_midcode_index].op_num1;
	op_num2 = midcode[current_midcode_index].op_num2;
	result = midcode[current_midcode_index].result;
	reg1 = Reg::get_ident_reg(op_num1);
	reg = Reg::use_T_assign1(result);
}

void Target::add_neg()
{
	op_num1 = midcode[current_midcode_index].op_num1;
	result = midcode[current_midcode_index].result;
	reg1 = Reg::get_ident_reg(op_num1);
	reg = Reg::use_T_assign1(result);
}
void Target::add_my_assign()
{
	op_num1 = midcode[current_midcode_index].op_num1;
	result = midcode[current_midcode_index].result;
	if (op_num1[0] == '+' || op_num1[0] == '-' || (op_num1[0] >= '0' && op_num1[0] <= '9')) {
		reg = Reg::use_T_assign1(result);
		add_target_code("li", reg, op_num1, "");
	}
	else {
		reg1 = Reg::get_ident_reg(op_num1);
		reg = Reg::use_T_assign1(result);
		add_target_code("move", reg, reg1, "");
	}
}
void Target::add_assign()
{
	//等号右侧可能已经被分配t寄存器
	op_num1 = midcode[current_midcode_index].op_num1;
	op_num2 = midcode[current_midcode_index].op_num2;
	reg2 = Reg::get_ident_reg(op_num2);
	if (SymbolTable::is_local(op_num1)) {
		if (Reg::in_T_reg(op_num1)) {
			reg1 = Reg::get_T_reg(op_num1);
		}
		else {
			reg1 = Reg::use_T_assign1(op_num1);
		}
	}
	else {
		if (Reg::in_T_reg(op_num1)) {
			reg1 = Reg::get_T_reg(op_num1);
		}
		else {
			reg1 = Reg::use_T_assign1(op_num1);
		}
	}
	add_target_code("move", reg1, reg2, "");
}

void Target::add_arr_assign()
{
	op_num1 = midcode[current_midcode_index].op_num1;//数组名
	op_num2 = midcode[current_midcode_index].op_num2;//数组下标
	result = midcode[current_midcode_index].result;//所赋值
	reg2 = Reg::get_ident_reg(op_num2);
	reg = Reg::get_ident_reg(result);
	add_target_code("sll","$t9",reg2,"2");
	if (SymbolTable::is_local(op_num1)) {
		int addr1 = SymbolTable::local_addr(op_num1);
		add_target_code("addi", "$t9", "$t9", to_string(addr1));
		add_target_code("add", "$t9", "$t9", "$fp");
	}
	else {
		int addr2 = SymbolTable::global_var_addr(op_num1);
		add_target_code("addi", "$t9", "$t9", to_string(addr2));
		add_target_code("add", "$t9", "$t9", "$gp");
	}
	add_target_code("sw",reg,"0($t9)","");
}
void Target::add_arr_take()
{
	op_num1 = midcode[current_midcode_index].op_num1;//数组名
	op_num2 = midcode[current_midcode_index].op_num2;//数组下标
	result = midcode[current_midcode_index].result;//赋给的值
	reg2 = Reg::get_ident_reg(op_num2);
	reg = Reg::use_T_assign1(result);//此函数专门针对#i
	add_target_code("sll", "$t9", reg2, "2");
	if (SymbolTable::is_local(op_num1)) {
		int addr1 = SymbolTable::local_addr(op_num1);
		add_target_code("addi", "$t9", "$t9", to_string(addr1));
		add_target_code("add", "$t9", "$t9", "$fp");
	}
	else {
		int addr2 = SymbolTable::global_var_addr(op_num1);
		add_target_code("addi", "$t9", "$t9", to_string(addr2));
		add_target_code("add", "$t9", "$t9", "$gp");
	}
	add_target_code("lw", reg, "0($t9)", "");
}
void Target::add_input()
{
	op_num1 = midcode[current_midcode_index].op_num1;
	Reg::scanf_T(op_num1);
	if (SymbolTable::is_int(op_num1)) {
		add_target_code("li","$v0","5","");
	}
	else {
		add_target_code("li","$v0","12","");
	}
	add_target_code("syscall", "", "", "");
	if (SymbolTable::is_local(op_num1)) {
		int addr1 = SymbolTable::local_addr(op_num1);
		add_target_code("sw","$v0",to_string(addr1)+"($fp)","");
	}
	else {
		int addr2 = SymbolTable::global_var_addr(op_num1);
		add_target_code("sw", "$v0",to_string(addr2)+"($gp)", "");
	}
}
void Target::add_j_main()
{
	add_target_code("j", "main", "", "");
}
void Target::add_enter()
{
	add_target_code("enter:.asciiz","\"\\n\"","","");
}
void Target::get_target_code()
{
	current_midcode_index = 0;
	add_const_string();
	add_enter();
	ignore_const_declare();
	ignore_vararr_declare();
	add_text_label();
	add_j_main();
	while (current_midcode_index < midcode_index) {
		switch (midcode[current_midcode_index].op)
		{
		case Mid::func_declare: {
			current_func_name = midcode[current_midcode_index].op_num1;
			add_target_code(current_func_name, ":", "", "");
			int addr = SymbolTable::sp_down_size();
			add_target_code("subi", "$sp", "$sp", to_string(addr));
			add_target_code("move", "$fp", "$sp","");
			Reg::save_ra_reg();
			current_T_index = 0;

			current_midcode_index++;
			ignore_para_declare();
			ignore_const_declare();
			ignore_vararr_declare();
			current_midcode_index--;
			break;
		}
		case Mid::add: {
			add_calculate();
			add_target_code("add", reg, reg1, reg2);
			break;
		}
		case Mid::sub: {
			add_calculate();
			add_target_code("sub", reg, reg1, reg2);
			break;
		}
		case Mid::mul: {
			add_calculate();
			add_target_code("mult", reg1, reg2, "");
			add_target_code("mflo", reg, "", "");
			break;
		}
		case Mid::div: {
			add_calculate();
			add_target_code("div", reg1, reg2, "");
			add_target_code("mflo", reg, "", "");
			break;
		}
		case Mid::addi: {
			add_calculate_i();
			add_target_code("addi", reg, reg1, op_num2);
			break;
		}
		case Mid::subi: {
			add_calculate_i();
			add_target_code("subi", reg, reg1, op_num2);
			break;
		}
		case Mid::muli: {
			add_calculate_i();
			add_target_code("mul", reg, reg1, op_num2);
			break;
		}
		case Mid::divi: {
			add_calculate_i();
			add_target_code("div", reg, reg1, op_num2);
			break;
		}
		case Mid::neg: {
			add_neg();
			add_target_code("li", "$t9", "-1", "");
			add_target_code("mult", "$t9", reg1, "");
			add_target_code("mflo", reg, "", "");
			break;
		}
		case Mid::my_assign: {
			add_my_assign();
			break;
		}
		case Mid::assign: {
			add_assign();
			break;
		}
		case Mid::arr_assign: {
			add_arr_assign();
			break;
		}
		case Mid::arr_take: {
			add_arr_take();
			break;
		}
		case Mid::output_str: {
			op_num2 = midcode[current_midcode_index].op_num2;
			add_target_code("la","$a0","string" + op_num2,"");
			add_target_code("li", "$v0", "4","");
			add_target_code("syscall", "", "","");
			if (midcode[current_midcode_index].result == "enter") {
				add_target_code("la", "$a0", "enter", "");
				add_target_code("li", "$v0", "4", "");
				add_target_code("syscall", "", "", "");
			}
			break;
		}
		case Mid::output: {
			op_num1 = midcode[current_midcode_index].op_num1;
			reg1 = Reg::get_ident_reg(op_num1);
			add_target_code("move","$a0",reg1,"");
			add_target_code("li", "$v0", "1", "");
			add_target_code("syscall", "", "", "");
			add_target_code("la", "$a0", "enter", "");
			add_target_code("li", "$v0", "4", "");
			add_target_code("syscall", "", "", "");
			break;
		}
		case Mid::output_c: {
			op_num1 = midcode[current_midcode_index].op_num1;
			reg1 = Reg::get_ident_reg(op_num1);
			add_target_code("move", "$a0", reg1, "");
			add_target_code("li", "$v0", "11", "");
			add_target_code("syscall", "", "", "");
			add_target_code("la", "$a0", "enter", "");
			add_target_code("li", "$v0", "4", "");
			add_target_code("syscall", "", "", "");
			break;
		}
		case Mid::input: {
			add_input();
			break;
		}
		case Mid::call: {
			op_num1 = midcode[current_midcode_index].op_num1;
			Reg::write_back_T1();
			add_target_code("jal",op_num1,"","");
			break;
		}
		case Mid::push: {
			op_num1 = midcode[current_midcode_index].op_num1;
			reg1 = Reg::get_ident_reg(op_num1);
			add_target_code("addi", "$sp", "$sp", "-4");
			add_target_code("sw",reg1,"0($sp)","");
			break;
		}
		case Mid::ret: {
			if (midcode[current_midcode_index - 1].op == Mid::ret) {
				break;
			}
			if (current_func_name != "main") {
				op_num1 = midcode[current_midcode_index].op_num1;
				if (op_num1 != "") {
					reg1 = Reg::get_ident_reg(op_num1);
					add_target_code("move", "$v0", reg1, "");
				}
				Reg::write_back_T1();
				Reg::load_ra_reg();
				int addr1 = SymbolTable::sp_down_size();
				int addr2 = SymbolTable::func_para_size();
				add_target_code("addi", "$sp", "$sp", to_string(addr1+addr2));//升栈时直接升到Push之前
				add_target_code("move", "$fp", "$sp", "");
				add_target_code("jr", "$ra", "", "");
			}
			else {
				add_target_code("li","$v0","10","");
				add_target_code("syscall","","","");
			}
			break;
		}
		case Mid::receive: {
			result = midcode[current_midcode_index].result;
			reg = Reg::use_T_assign1(result);
			add_target_code("move",reg,"$v0","");
			break;
		}
		case Mid::set_label: {
			Reg::write_back_T1();
			add_target_code(midcode[current_midcode_index].op_num1+":","","","");
			break;
		}
		case Mid::_goto: {
			Reg::write_back_T1();
			add_target_code("j", midcode[current_midcode_index].op_num1,"","");
			break;
		}
		case Mid::_beq: {
			op_num1 = midcode[current_midcode_index].op_num1;
			op_num2 = midcode[current_midcode_index].op_num2;
			result = midcode[current_midcode_index].result;
			reg1 = Reg::get_ident_reg(op_num1);
			reg2 = Reg::get_ident_reg(op_num2);
			Reg::write_back_T1();
			add_target_code("beq",reg1,reg2,result);
			break;
		}
		case Mid::_bne: {
			op_num1 = midcode[current_midcode_index].op_num1;
			op_num2 = midcode[current_midcode_index].op_num2;
			result = midcode[current_midcode_index].result;
			reg1 = Reg::get_ident_reg(op_num1);
			reg2 = Reg::get_ident_reg(op_num2);
			Reg::write_back_T1();
			add_target_code("bne", reg1, reg2, result);
			break;
		}
		case Mid::_bgt: {
			op_num1 = midcode[current_midcode_index].op_num1;
			op_num2 = midcode[current_midcode_index].op_num2;
			result = midcode[current_midcode_index].result;
			reg1 = Reg::get_ident_reg(op_num1);
			reg2 = Reg::get_ident_reg(op_num2);
			Reg::write_back_T1();
			add_target_code("bgt", reg1, reg2, result);
			break;
		}
		case Mid::_bge: {
			op_num1 = midcode[current_midcode_index].op_num1;
			op_num2 = midcode[current_midcode_index].op_num2;
			result = midcode[current_midcode_index].result;
			reg1 = Reg::get_ident_reg(op_num1);
			reg2 = Reg::get_ident_reg(op_num2);
			Reg::write_back_T1();
			add_target_code("bge", reg1, reg2, result);
			break;
		}
		case Mid::_blt: {
			op_num1 = midcode[current_midcode_index].op_num1;
			op_num2 = midcode[current_midcode_index].op_num2;
			result = midcode[current_midcode_index].result;
			reg1 = Reg::get_ident_reg(op_num1);
			reg2 = Reg::get_ident_reg(op_num2);
			Reg::write_back_T1();
			add_target_code("blt", reg1, reg2, result);
			break;
		}
		case Mid::_ble: {
			op_num1 = midcode[current_midcode_index].op_num1;
			op_num2 = midcode[current_midcode_index].op_num2;
			result = midcode[current_midcode_index].result;
			reg1 = Reg::get_ident_reg(op_num1);
			reg2 = Reg::get_ident_reg(op_num2);
			Reg::write_back_T1();
			add_target_code("ble", reg1, reg2, result);
			break;
		}
		case Mid::_beqz: {
			op_num1 = midcode[current_midcode_index].op_num1;
			result = midcode[current_midcode_index].result;
			reg1 = Reg::get_ident_reg(op_num1);
			Reg::write_back_T1();
			add_target_code("beqz", reg1, result, "");
			break;
		}
		case Mid::_bnez: {
			op_num1 = midcode[current_midcode_index].op_num1;
			result = midcode[current_midcode_index].result;
			reg1 = Reg::get_ident_reg(op_num1);
			Reg::write_back_T1();
			add_target_code("bnez", reg1, result, "");
			break;
		}
		case Mid::_addi: {
			op_num1 = midcode[current_midcode_index].op_num1;
			op_num2 = midcode[current_midcode_index].op_num2;
			result = midcode[current_midcode_index].result;
			reg = Reg::get_ident_reg(result);
			reg1 = Reg::get_ident_reg(op_num1);
			add_target_code("addi",reg,reg1,op_num2);
			break;
		}
		case Mid::_subi: {
			op_num1 = midcode[current_midcode_index].op_num1;
			op_num2 = midcode[current_midcode_index].op_num2;
			result = midcode[current_midcode_index].result;
			reg = Reg::get_ident_reg(result);
			reg1 = Reg::get_ident_reg(op_num1);
			add_target_code("subi", reg, reg1, op_num2);
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

void Target::output_target()
{
	for (int i = 0; i < tarcode_index; i++) {
		printf("%s %s %s %s \n", target_code[i].op.c_str(), target_code[i].result.c_str(), target_code[i].op_num1.c_str(), target_code[i].op_num2.c_str());
	}
}