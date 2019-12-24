#include "Lexical.h"
#include "Globalvar.h"
#include "error.h"
#include "Syntax.h"
#include "SymbolTable.h"
#include "get_midcode.h"

int main_flag = 0;
#define skip2rparent while(word != Lexical::RPARENT) Lexical::next_word()
bool Syntax::statement_begin()
{
	if (word == Lexical::IFTK || word == Lexical::DOTK || word == Lexical::WHILETK || word == Lexical::FORTK
		|| word == Lexical::LBRACE || word == Lexical::IDENFR || word == Lexical::SCANFTK || word == Lexical::PRINTFTK
		|| word == Lexical::RETURNTK || word == Lexical::SEMICN) {
		return true;
	} else {
		return false;
	}
}

//整数
void Syntax::number()
{
	int op = 1;
	if (word == Lexical::PLUS || word == Lexical::MINU)
	{
		//有符号整数
		if (word == Lexical::PLUS) {
			op = 1;
		} else {
			op = -1;
		}
		Lexical::next_word();
		if (word == Lexical::INTCON) {
			digits = digits * op;
		}
	} 
	else {
		//无符号整数(整数值已经存到了digits中了)
	}
	Lexical::next_word();
	if (print == 1) printf("<无符号整数>\n");
	if (print == 1) printf("<整数>\n");
	//退出时digits保存了该整数的值
}
//因子
int Syntax::factor()
{
	int ret_temp = -1;
	int index_temp = -1;
	if (word == Lexical::CHARCON) {
		//字符常量
		Lexical::next_word();
		Mid::add_mid_const((int)ch);
		ret_temp = num_index;
	}
	else if (word == Lexical::INTCON || word == Lexical::PLUS || word == Lexical::MINU) {
		//整数
		is_int = 1;
		number();
		Mid::add_mid_const(digits);
		ret_temp = num_index;
	}
	else if (word == Lexical::LPARENT) {
		//(表达式)
		Lexical::next_word();
		ret_temp = expression();
		if (word == Lexical::RPARENT) {
			Lexical::next_word();
		} else {
			Error::error_print(Error::no_rparent_error);
		}
		is_int = 1;
	} 
	//函数调用或变量或数组
	else if (word == Lexical::IDENFR) {
		bool has_return = SymbolTable::func_has_return(ident_string);
		string str = ident_string;
		Lexical::next_word();
		if (word == Lexical::LBRACK) {
			//error未定义标识符
			SymbolTable::is_ident_define(str);
			Lexical::next_word();
			index_temp = expression();
			//error数组下标不是整型
			if (is_int == 0) {
				Error::error_print(Error::array_error);
			}
			is_int = 0;
			Mid::add_mid_arr_take(str, index_temp);
			ret_temp = num_index;
			if (SymbolTable::ident_type(str) == SymbolTable::int_array) {
				is_int = 1;
			}
			if (word == Lexical::RBRACK) {
				Lexical::next_word();
			} else {
				Error::error_print(Error::no_rbrack_error);
			}
		} 
		else if (word == Lexical::LPARENT){
			//error未定义函数名
			if (SymbolTable::is_func_define(str) == -1) {
				skip2rparent;
				Lexical::next_word();
				return 0;
			}
			int temp = is_int;
			if (has_return) {
				func_statement();
			}
			else {
				void_func_statement();
			}
			is_int = temp;
			if (SymbolTable::func_type(str) == SymbolTable::int_func) {
				is_int = 1;
			}
			Mid::add_mid_receive();
			ret_temp = num_index;
		} 
		else {
			SymbolTable::is_ident_define(str);//error未定义标识符（看是否）
			if (SymbolTable::ident_type(str) == SymbolTable::int_var || 
				SymbolTable::ident_type(str) == SymbolTable::int_para || 
				SymbolTable::ident_type(str) == SymbolTable::int_const) {
				is_int = 1;
			}
			if (SymbolTable::ident_type(str) == SymbolTable::int_const ||
				SymbolTable::ident_type(str) == SymbolTable::char_const) {
				Mid::add_mid_const(SymbolTable::const_ident_to_value(str));
				ret_temp = num_index;
			}
			else {
				Mid::add_mid_ident(str);
				ret_temp = num_index;
			}
		}
	} 
	if (print == 1) printf("<因子>\n");
	return ret_temp;//将ret_temp的值返回
}

int Syntax::term()
{
	int ret_temp = -1;
	int temp1, temp2;
	Lexical::WORD term_op;
	temp1 = factor();
	if (word == Lexical::MULT || word == Lexical::DIV) {
		is_int = 1;
	}
	while (word == Lexical::MULT || word == Lexical::DIV) {
		term_op = word;
		Lexical::next_word();
		temp2 = factor();
		Mid::add_mid_cal(term_op,temp1,temp2);
		temp1 = num_index;
	}
	if (print == 1) printf("<项>\n");
	ret_temp = temp1;
	return ret_temp;
}

int Syntax::expression()
{
	int ret_temp = -1;
	int positive = 1;
	int temp1, temp2;
	Lexical::WORD expression_op;
	is_int = 0;//初始化为char
	if (word == Lexical::PLUS || word == Lexical::MINU) {
		if (word == Lexical::MINU) {
			positive = -1;
		}
		Lexical::next_word();
	}
	temp1 = term();
	if (positive == -1) {
		Mid::add_mid_neg(temp1);
		temp1 = num_index;
	}
	if (word == Lexical::PLUS || word == Lexical::MINU) {
		is_int = 1;
	}
	while (word == Lexical::PLUS || word == Lexical::MINU) {
		expression_op = word;
		Lexical::next_word();
		temp2 = term();
		Mid::add_mid_cal(expression_op, temp1, temp2);
		temp1 = num_index;
	}
	if (print == 1) printf("<表达式>\n");
	ret_temp = temp1;
	max_num_index = (ret_temp > max_num_index) ? (ret_temp) : (max_num_index);
	return ret_temp;
}

void Syntax::compound_statement()
{
	max_num_index = 20;
	const_declare();
	var_declare();
	statement_block();
	SymbolTable::current_func_temp_size();
	SymbolTable::add_max_index(max_num_index);//保存当前函数临时变量使用的最大编号
	if (max_num_index > total_max_num_index) {
		total_max_num_index = max_num_index;
	}
	num_index = 0;
	if (!SymbolTable::func_return()) {
		Mid::add_mid_return(-1);
	}
	if (print == 1) printf("<复合语句>\n");
}

void Syntax::statement_block()
{
	while (statement_begin()) {
		statement();
	}
	if (print == 1) printf("<语句列>\n");
}


void Syntax::statement()
{
	switch (word) 
	{
	case Lexical::IFTK: {
		Lexical::next_word();
		if_else_statement();
		if (print == 1) printf("<语句>\n");
		break;
	}
	case Lexical::DOTK: {
		Lexical::next_word();
		do_while_statement();
		if (print == 1) printf("<语句>\n");
		break;
	}
	case Lexical::WHILETK: {
		Lexical::next_word();
		while_statement();
		if (print == 1) printf("<语句>\n");
		break;
	}
	case Lexical::FORTK: {
		Lexical::next_word();
		for_statement();
		if (print == 1) printf("<语句>\n");
		break;
	}
	case Lexical::SCANFTK: {
		Lexical::next_word();
		scanf_statement();
		if (word == Lexical::SEMICN) {
			if (print == 1) printf("<语句>\n");
			Lexical::next_word();
		}
		else {
			Error::error_print(Error::no_semi_error);
		}
		break;
	}
	case Lexical::PRINTFTK: {
		Lexical::next_word();
		printf_statement();
		if (word == Lexical::SEMICN) {
			if (print == 1) printf("<语句>\n");
			Lexical::next_word();
		}
		else {
			Error::error_print(Error::no_semi_error);
		}
		break;
	}
	case Lexical::RETURNTK: {
		Lexical::next_word();
		return_statement();
		if (word == Lexical::SEMICN) {
			if (print == 1) printf("<语句>\n");
			Lexical::next_word();
		}
		else {
			Error::error_print(Error::no_semi_error);
		}
		break;
	}
	case Lexical::SEMICN:
		if (print == 1) printf("<语句>\n");
		Lexical::next_word();
		break;
	case Lexical::LBRACE: {
		Lexical::next_word();
		statement_block();
		if (word == Lexical::RBRACE) {
			if (print == 1) printf("<语句>\n");
			Lexical::next_word();
		}
		break;
	}
	case Lexical::IDENFR: {
		bool has_return = SymbolTable::func_has_return(ident_string);
		Lexical::next_word();
		if (word == Lexical::LPARENT) {
			//函数调用——error未定义函数名
			if (SymbolTable::is_func_define(ident_string) == -1) {
				skip2rparent;
				Lexical::next_word();
				return;
			}
			if (has_return) {
				func_statement();
			}
			else {
				void_func_statement();
			}
			if (word == Lexical::SEMICN) {
				if (print == 1) printf("<语句>\n");
				Lexical::next_word();
			}
			else {
				Error::error_print(Error::no_semi_error);
			}
		}
		else {
			//赋值语句——error未定义标识符
			SymbolTable::is_ident_define(ident_string);
			assign_statement();
			if (word == Lexical::SEMICN) {
				if (print == 1) printf("<语句>\n");
				Lexical::next_word();
			}
			else {
				Error::error_print(Error::no_semi_error);
			}
		}
		break;
	}
	default:
		break;
	}
}
/*
void Syntax::condition()
{
	int temp1, temp2;
	Lexical::WORD condition_op = Lexical::DEFAULT;
	temp1 = expression();
	if (is_int == 0) {
		Error::error_print(Error::condition_error);
	}
	if (word == Lexical::LSS || word == Lexical::LEQ || word == Lexical::GRE
		|| word == Lexical::GEQ || word == Lexical::EQL || word == Lexical::NEQ) {
		condition_op = word;
		Lexical::next_word();
		temp2 = expression();
		if (is_int == 0) {
			Error::error_print(Error::condition_error);
		}
	}
	if (print == 1) printf("<条件>\n");
}
*/
void Syntax::if_else_statement()
{
	int temp1, temp2;
	int label1 = label_index++;
	int label2 = label_index++;
	Lexical::WORD condition_op = Lexical::DEFAULT;
	if (word == Lexical::LPARENT) {
		Lexical::next_word();
		temp1 = expression();
		if (is_int == 0) {
			Error::error_print(Error::condition_error);
		}
		if (word == Lexical::LSS || word == Lexical::LEQ || word == Lexical::GRE
			|| word == Lexical::GEQ || word == Lexical::EQL || word == Lexical::NEQ) {
			//2个表达式
			condition_op = word;
			Lexical::next_word();
			temp2 = expression();
			if (is_int == 0) {
				Error::error_print(Error::condition_error);
			}
			int flag1 = 0;
			if (word == Lexical::RPARENT) {
				Lexical::next_word();
				Mid::add_mid_condition_whileforif(condition_op,temp1,temp2,label1);
				statement();
				if (word == Lexical::ELSETK) {
					Mid::add_mid_goto(label2);
					Mid::add_mid_label(label1);
					Lexical::next_word();
					statement();
					flag1 = 1;
				}
				if (flag1 == 1) {
					Mid::add_mid_label(label2);
				}
				else {
					Mid::add_mid_label(label1);
				}
			} 
			else {
				Error::error_print(Error::no_rparent_error);
			}
		}
		else {
			//1个表达式
			int flag2 = 0;
			if (word == Lexical::RPARENT) {
				Lexical::next_word();
				Mid::add_mid_condition_whileforif(condition_op,temp1,0,label1);
				statement();
				if (word == Lexical::ELSETK) {
					Mid::add_mid_goto(label2);
					Mid::add_mid_label(label1);
					Lexical::next_word();
					statement();
					flag2 = 1;
				}
				if (flag2 == 1) {
					Mid::add_mid_label(label2);
				}
				else {
					Mid::add_mid_label(label1);
				}
			} 
			else {
				Error::error_print(Error::no_rparent_error);
			}
		}
	}
	if (print == 1) printf("<条件语句>\n");
}

void Syntax::do_while_statement()
{
	int temp1, temp2;
	int label1 = label_index++;
	Lexical::WORD condition_op = Lexical::DEFAULT;
	Mid::add_mid_label(label1);
	statement();
	if (word == Lexical::WHILETK) {
		Lexical::next_word();
		if (word == Lexical::LPARENT) {
			Lexical::next_word();
			//condition begin
			temp1 = expression();
			if (is_int == 0) {
				Error::error_print(Error::condition_error);
			}
			//1 or 2 expr
			if (word == Lexical::LSS || word == Lexical::LEQ || word == Lexical::GRE
				|| word == Lexical::GEQ || word == Lexical::EQL || word == Lexical::NEQ) {
				condition_op = word;
				Lexical::next_word();
				temp2 = expression();
				if (is_int == 0) {
					Error::error_print(Error::condition_error);
				}
				if (word == Lexical::RPARENT) {
					Mid::add_mid_condition_dowhile(condition_op,temp1,temp2,label1);
					Lexical::next_word();
				} 
				else {
					Error::error_print(Error::no_rparent_error);
				}
			}
			else {
				if (word == Lexical::RPARENT) {
					Mid::add_mid_condition_dowhile(condition_op,temp1,0,label1);
					Lexical::next_word();
				} 
				else {
					Error::error_print(Error::no_rparent_error);
				}
			}
		}
	} 
	else {
		Error::error_print(Error::no_while_error);
		skip2rparent;
		Lexical::next_word();
	}
	if (print == 1) printf("<循环语句>\n");
}

Mid::op_type get_new_branch_op(Mid::op_type op)
{
	Mid::op_type new_op;
	switch (op)
	{
	case Mid::_beq:
		new_op = Mid::_bne;
		break;
	case Mid::_bne:
		new_op = Mid::_beq;
		break;
	case Mid::_bgt:
		new_op = Mid::_ble;
		break;
	case Mid::_bge:
		new_op = Mid::_blt;
		break;
	case Mid::_ble:
		new_op = Mid::_bgt;
		break;
	case Mid::_blt:
		new_op = Mid::_bge;
		break;
	case Mid::_beqz:
		new_op = Mid::_bnez;
		break;
	case Mid::_bnez:
		new_op = Mid::_beqz;
		break;
	default:
		break;
	}
	return new_op;
}

void Syntax::while_statement()
{
	/*
	int temp1, temp2;
	int label1 = label_index++;
	int label2 = label_index++;
	Lexical::WORD condition_op = Lexical::DEFAULT;
	if (word == Lexical::LPARENT) {
		Lexical::next_word();
		Mid::add_mid_label(label1);
		//condition begin
		temp1 = expression();
		if (is_int == 0) {
			Error::error_print(Error::condition_error);
		}
		if (word == Lexical::LSS || word == Lexical::LEQ || word == Lexical::GRE
			|| word == Lexical::GEQ || word == Lexical::EQL || word == Lexical::NEQ) {
			condition_op = word;
			Lexical::next_word();
			temp2 = expression();
			if (is_int == 0) {
				Error::error_print(Error::condition_error);
			}
			if (word == Lexical::RPARENT) {
				Mid::add_mid_condition_whileforif(condition_op,temp1,temp2,label2);
				Lexical::next_word();
				statement();
				Mid::add_mid_goto(label1);
				Mid::add_mid_label(label2);
			}
			else {
				Error::error_print(Error::no_rparent_error);
			}
		}
		else {
			if (word == Lexical::RPARENT) {
				Mid::add_mid_condition_whileforif(condition_op,temp1,0,label2);
				Lexical::next_word();
				statement();
				Mid::add_mid_goto(label1);
				Mid::add_mid_label(label2);
			}
			else {
				Error::error_print(Error::no_rparent_error);
			}
		}
	}
	if (print == 1) printf("<循环语句>\n");
	*/
	////////////////////////////////////////////////////////////////
	
	int temp1, temp2;
	int label1 = label_index++;
	int label2 = label_index++;
	Lexical::WORD condition_op = Lexical::DEFAULT;
	if (word == Lexical::LPARENT) {
		Lexical::next_word();
		//condition begin
		int index1 = midcode_index;
		temp1 = expression();
		if (is_int == 0) {
			Error::error_print(Error::condition_error);
		}
		int index2;
		if (word == Lexical::LSS || word == Lexical::LEQ || word == Lexical::GRE
			|| word == Lexical::GEQ || word == Lexical::EQL || word == Lexical::NEQ) {
			condition_op = word;
			Lexical::next_word();
			temp2 = expression();
			if (is_int == 0) {
				Error::error_print(Error::condition_error);
			}
			if (word == Lexical::RPARENT) {
				Mid::add_mid_condition_whileforif(condition_op,temp1,temp2,label2);
				index2 = midcode_index;
				Mid::add_mid_label(label1);
				Lexical::next_word();
				statement();
				vector<Mid::MidCode> temp;
				for (int i = index1; i < index2; i++) {
					if (i == index2 - 1) {
						Mid::MidCode mid = midcode[i];
						mid.op = get_new_branch_op(mid.op);
						mid.result = "Label" + to_string(label1);
						temp.push_back(mid);
					}
					else {
						temp.push_back(midcode[i]);
					}
				}
				for (int i = 0; i < index2 - index1; i++) {
					midcode.push_back(temp[i]);
					midcode_index++;
				}
				Mid::add_mid_label(label2);
			}
			else {
				Error::error_print(Error::no_rparent_error);
			}
		}
		else {
			if (word == Lexical::RPARENT) {
				Mid::add_mid_condition_whileforif(condition_op,temp1,0,label2);
				index2 = midcode_index;
				Mid::add_mid_label(label1);
				Lexical::next_word();
				statement();
				vector<Mid::MidCode> temp;
				for (int i = index1; i < index2; i++) {
					if (i == index2 - 1) {
						Mid::MidCode mid = midcode[i];
						mid.op = get_new_branch_op(mid.op);
						mid.result = "Label" + to_string(label1);
						temp.push_back(mid);
					}
					else {
						temp.push_back(midcode[i]);
					}
				}
				for (int i = 0; i < index2 - index1; i++) {
					midcode.push_back(temp[i]);
					midcode_index++;
				}
				Mid::add_mid_label(label2);
			}
			else {
				Error::error_print(Error::no_rparent_error);
			}
		}
	}
	
}
/*
 * ident1 = expr
 * condition
 * ident2 = ident3 (+-) no_sign_num
 */
void Syntax::for_statement()
{
	/*
	int temp,temp1,temp2 = 0;
	Lexical::WORD step_type;
	int label1 = label_index++;
	int label2 = label_index++;
	Lexical::WORD condition_op = Lexical::DEFAULT;
	//进入时word是左括号
	Lexical::next_word();//word为ident1
	string str1 = ident_string;
	SymbolTable::is_ident_define(ident_string);
	Lexical::next_word();//word为赋值符号
	Lexical::next_word();
	temp = expression();
	Mid::add_mid_assign(str1,temp);
	if (word != Lexical::SEMICN) {
		Error::error_print(Error::no_semi_error);
	}
	Mid::add_mid_label(label1);

	Lexical::next_word();
	//condition begin
	temp1 = expression();
	if (is_int == 0) {
		Error::error_print(Error::condition_error);
	}
	if (word == Lexical::LSS || word == Lexical::LEQ || word == Lexical::GRE
		|| word == Lexical::GEQ || word == Lexical::EQL || word == Lexical::NEQ) {
		condition_op = word;
		Lexical::next_word();
		temp2 = expression();
		if (is_int == 0) {
			Error::error_print(Error::condition_error);
		}
	}
	Mid::add_mid_condition_whileforif(condition_op,temp1,temp2,label2);
	if (word != Lexical::SEMICN) {
		Error::error_print(Error::no_semi_error);
	}

	Lexical::next_word();//word为ident2
	string str2 = ident_string;
	SymbolTable::is_ident_define(ident_string);
	Lexical::next_word();//word为赋值符号
	Lexical::next_word();//word为ident3
	string str3 = ident_string;
	SymbolTable::is_ident_define(ident_string);
	Lexical::next_word();//+或-
	step_type = word;
	Lexical::next_word();//步长(无符号整数)
	int num = digits;
	Lexical::next_word();
	if (word != Lexical::RPARENT) {
		Error::error_print(Error::no_rparent_error);
	}
	Lexical::next_word();
	statement();
	Mid::add_mid_for_step(str2,str3,step_type,num);
	Mid::add_mid_goto(label1);
	Mid::add_mid_label(label2);
	if (print == 1) printf("<循环语句>\n");
	*/
	////////////////////////////////////////////////////////
	
	int temp, temp1, temp2 = 0;
	Lexical::WORD step_type;
	int label1 = label_index++;
	int label2 = label_index++;
	Lexical::WORD condition_op = Lexical::DEFAULT;
	//进入时word是左括号
	Lexical::next_word();//word为ident1
	string str1 = ident_string;
	SymbolTable::is_ident_define(ident_string);
	Lexical::next_word();//word为赋值符号
	Lexical::next_word();
	temp = expression();
	Mid::add_mid_assign(str1, temp);
	if (word != Lexical::SEMICN) {
		Error::error_print(Error::no_semi_error);
	}

	Lexical::next_word();
	//condition begin
	int index1 = midcode_index;
	temp1 = expression();
	if (is_int == 0) {
		Error::error_print(Error::condition_error);
	}
	if (word == Lexical::LSS || word == Lexical::LEQ || word == Lexical::GRE
		|| word == Lexical::GEQ || word == Lexical::EQL || word == Lexical::NEQ) {
		condition_op = word;
		Lexical::next_word();
		temp2 = expression();
		if (is_int == 0) {
			Error::error_print(Error::condition_error);
		}
	}
	Mid::add_mid_condition_whileforif(condition_op, temp1, temp2, label2);
	int index2 = midcode_index;
	Mid::add_mid_label(label1);
	if (word != Lexical::SEMICN) {
		Error::error_print(Error::no_semi_error);
	}

	Lexical::next_word();//word为ident2
	string str2 = ident_string;
	SymbolTable::is_ident_define(ident_string);
	Lexical::next_word();//word为赋值符号
	Lexical::next_word();//word为ident3
	string str3 = ident_string;
	SymbolTable::is_ident_define(ident_string);
	Lexical::next_word();//+或-
	step_type = word;
	Lexical::next_word();//步长(无符号整数)
	int num = digits;
	Lexical::next_word();
	if (word != Lexical::RPARENT) {
		Error::error_print(Error::no_rparent_error);
	}
	Lexical::next_word();
	statement();
	Mid::add_mid_for_step(str2, str3, step_type, num);
	vector<Mid::MidCode> _temp;
	for (int i = index1; i < index2; i++) {
		if (i == index2 - 1) {
			Mid::MidCode mid = midcode[i];
			mid.op = get_new_branch_op(mid.op);
			mid.result = "Label" + to_string(label1);
			_temp.push_back(mid);
		}
		else {
			_temp.push_back(midcode[i]);
		}
	}
	for (int i = 0; i < index2 - index1; i++) {
		midcode.push_back(_temp[i]);
		midcode_index++;
	}
	Mid::add_mid_label(label2);
	
}

void Syntax::step()
{
	if (word == Lexical::INTCON) {
		Lexical::next_word();
	} 
	if (print == 1) printf("<无符号整数>\n");
	if (print == 1) printf("<步长>\n");
}

void Syntax::assign_statement()
{
	int temp1,temp2;
	string str = ident_string;
	if (word == Lexical::LBRACK) {
		Lexical::next_word();
		temp1 = expression();
		if (is_int == 0) {
			Error::error_print(Error::array_error);
		}
		if (word == Lexical::RBRACK) {
			Lexical::next_word();
			if (word == Lexical::ASSIGN) {
				Lexical::next_word();
				temp2 = expression();
				Mid::add_mid_arr_assign(str,temp1,temp2);
			}
		}
		else {
			Error::error_print(Error::no_rbrack_error);
			if (word == Lexical::ASSIGN) {
				Lexical::next_word();
				expression();
			}
		}
	}
	else if (word == Lexical::ASSIGN) {
		if (SymbolTable::is_ident_const(ident_string)) {
			Error::error_print(Error::const_assign_error);
		}
		Lexical::next_word();
		temp1 = expression();
		Mid::add_mid_assign(str,temp1);
	}
	if (print == 1) printf("<赋值语句>\n");
}

void Syntax::scanf_statement()
{
	if (word == Lexical::LPARENT) {
		Lexical::next_word();
		if (word == Lexical::IDENFR) {
			while (word == Lexical::IDENFR) {
				SymbolTable::is_ident_define(ident_string);
				Mid::add_mid_scanf(ident_string);
				Lexical::next_word();
				if (word == Lexical::COMMA) {
					Lexical::next_word();
					continue;
				}
				else if (word == Lexical::RPARENT) {
					Lexical::next_word();
					break;
				}
				else {
					Error::error_print(Error::no_rparent_error);
				}
			}
		}
	}
	if (print == 1) printf("<读语句>\n");
}

void Syntax::printf_statement()
{
	if (word == Lexical::LPARENT) {
		Lexical::next_word();
		if (word == Lexical::STRCON) {
			if (print == 1) printf("<字符串>\n");		//别忘了存字符串
			Lexical::next_word();
			if (word == Lexical::COMMA) {
				Mid::add_mid_print_str(str_const_count - 1,0);
				Lexical::next_word();
				printf_expression();
			}
			else if (word == Lexical::RPARENT) {
				Mid::add_mid_print_str(str_const_count - 1,1);//1代表有回车
				Lexical::next_word();
			}
			else {
				Error::error_print(Error::no_rparent_error);
			}
		}
		else {
			printf_expression();
		}
	}
	if (print == 1) printf("<写语句>\n");
}

void Syntax::printf_expression()
{
	int temp;
	temp = expression();
	if (is_int == 1) {
		Mid::add_mid_print(temp);
	}
	else {
		Mid::add_mid_print_c(temp);
	}
	if (word == Lexical::RPARENT) {
		Lexical::next_word();
	}
	else {
		Error::error_print(Error::no_rparent_error);
	}
}

void Syntax::return_statement()
{
	int temp = -1;
	if (word == Lexical::LPARENT) {
		if (!SymbolTable::func_return()) {
			Error::error_print(Error::void_return_error);
		}
		has_return = 1;
		Lexical::next_word();
		temp = expression();
		if ((is_int == 1 && SymbolTable::current_func_type() == SymbolTable::char_func)
			|| (is_int == 0 && SymbolTable::current_func_type() == SymbolTable::int_func)) {
			Error::error_print(Error::unvoid_unreturn_error);
		}
		Mid::add_mid_return(temp);
		if (word == Lexical::RPARENT) {
			Lexical::next_word();
		}
		else {
			Error::error_print(Error::no_rparent_error);
		}
	}
	else {
		//无返回值
		Mid::add_mid_return(temp);
		if (SymbolTable::func_return()) {
			Error::error_print(Error::unvoid_unreturn_error);
		}
	}
	if (print == 1) printf("<返回语句>\n");
}

void Syntax::func_statement()
{
	int temp;
	//进入时word为左括号,str为函数名
	vector<int> reverse_push;
	string str = ident_string;
	int num = SymbolTable::func_para_num();
	Lexical::next_word();
	if (num != 0 && word == Lexical::RPARENT) {
		//error参数个数错误,少
		Error::error_print(Error::paranum_error);
		Lexical::next_word();
		return;
	}
	for (int i = 1;i < num;i++) {
		temp = expression();
		//Mid::add_mid_push(temp);
		reverse_push.push_back(temp);
		SymbolTable::para_type_inspect(i-1, is_int,str);
		if (word == Lexical::COMMA) {
			Lexical::next_word();
		}
		else {
			//error参数个数错误,少
			Error::error_print(Error::paranum_error);
			num = 0;
			break;
		}
	}
	if (num != 0) {
		temp = expression();
		//Mid::add_mid_push(temp);
		reverse_push.push_back(temp);
		SymbolTable::para_type_inspect(num-1, is_int, str);
	}
	for (int i = 0; i < reverse_push.size(); i++) {
		Mid::add_mid_push(reverse_push[i]);
	}
	if (word == Lexical::RPARENT) {
		if (print == 1) printf("<值参数表>\n");
		Lexical::next_word();
	}
	else if (word == Lexical::COMMA) {
		//error参数个数错误,多
		Error::error_print(Error::paranum_error);
		while (word == Lexical::COMMA) {
			Lexical::next_word();
			expression();
		}
		if (word == Lexical::RPARENT) {
			Lexical::next_word();
		}
	}
	else {
		if (word == Lexical::IDENFR){
			//参数个数多
			Error::error_print(Error::paranum_error);
			Lexical::next_word();
			while (word != Lexical::RPARENT) {
				Lexical::next_word();
			}
			Lexical::next_word();
		}
		else {
			//error缺少右小括号
			Error::error_print(Error::no_rparent_error);
		}
	}
	Mid::add_mid_call(str);
	if (print == 1) printf("<有返回值函数调用语句>\n");
}

void Syntax::void_func_statement()
{
	int temp;
	vector<int> reverse_push;
	//进入时word为左括号,str为函数名
	string str = ident_string;
	int num = SymbolTable::func_para_num();
	Lexical::next_word();
	if (num != 0 && word == Lexical::RPARENT) {
		//error参数个数错误,少
		Error::error_print(Error::paranum_error);
		Lexical::next_word();
		return;
	}
	for (int i = 1; i < num; i++) {
		temp = expression();
		//Mid::add_mid_push(temp);
		reverse_push.push_back(temp);
		SymbolTable::para_type_inspect(i-1, is_int, str);
		if (word == Lexical::COMMA) {
			Lexical::next_word();
		}
		else {
			//error参数个数错误,少
			Error::error_print(Error::paranum_error);
			num = 0;
			break;
		}
	}
	if (num != 0) {
		temp = expression();
		//Mid::add_mid_push(temp);
		reverse_push.push_back(temp);
		SymbolTable::para_type_inspect(num-1, is_int, str);
	}
	for (int i = 0; i < reverse_push.size(); i++) {
		Mid::add_mid_push(reverse_push[i]);
	}
	if (word == Lexical::RPARENT) {
		if (print == 1) printf("<值参数表>\n");
		Lexical::next_word();
	}
	else if (word == Lexical::COMMA) {
		//error参数个数错误,多
		Error::error_print(Error::paranum_error);
		while (word == Lexical::COMMA) {
			Lexical::next_word();
			expression();
		}
		if (word == Lexical::RPARENT) {
			Lexical::next_word();
		}
	}
	else {
		if (word == Lexical::IDENFR) {
			//参数个数多
			Error::error_print(Error::paranum_error);
			Lexical::next_word();
			while (word != Lexical::RPARENT) {
				Lexical::next_word();
			}
			Lexical::next_word();
		}
		else {
			//error缺少右小括号
			Error::error_print(Error::no_rparent_error);
		}
	}
	Mid::add_mid_call(str);
	if (print == 1) printf("<无返回值函数调用语句>\n");
}


void Syntax::const_define()
{
	Lexical::WORD type = word;
	if (word == Lexical::INTTK || word == Lexical::CHARTK) {
		do {
			Lexical::next_word();//word为标识符
			string str = ident_string;//str为常量标识符
			Lexical::next_word();//word为赋值符
			Lexical::next_word();//word为值
			int op = 1;
			if (word == Lexical::CHARCON) {
				SymbolTable::add_const(type, str, (int)ch);
				Mid::add_mid_con_declare(str, (int)ch);
			}
			else if (word == Lexical::PLUS || word == Lexical::MINU) {
				if (word == Lexical::PLUS) {
					op = 1;
				}
				else {
					op = -1;
				}
				Lexical::next_word();
				if (print == 1) printf("<无符号整数>\n");
				digits = digits * op;
				if (print == 1) printf("<整数>\n");
				SymbolTable::add_const(type, str, digits);
				Mid::add_mid_con_declare(str, digits);
			}
			else if (word == Lexical::INTCON) {
				if (print == 1) printf("<无符号整数>\n");
				if (print == 1) printf("<整数>\n");
				SymbolTable::add_const(type, str, digits);
				Mid::add_mid_con_declare(str, digits);
			}
			else {
				Error::error_print(Error::const_type_error);
			}
			Lexical::next_word();
			if (word == Lexical::COMMA) {
				continue;
			}
			else if (word == Lexical::SEMICN) {
				if (print == 1) printf("<常量定义>\n");
				Lexical::next_word();
				break;
			}
			else {
				Error::error_print(Error::no_semi_error);
			}
		} while (word == Lexical::COMMA);
	}
}

void Syntax::const_declare()
{
	if (word != Lexical::CONSTTK) {
		return;
	}
	while (word == Lexical::CONSTTK) {
		Lexical::next_word();
		const_define();
	}
	if (print == 1) printf("<常量说明>\n");
}

void Syntax::var_define(Lexical::WORD type)
{
	Lexical::WORD var_type = type;
	do {
		string str = ident_string;//变量定义标识符
		Lexical::next_word();
		if (word == Lexical::LBRACK) {
			Lexical::next_word();//数组大小
			if (print == 1) printf("<无符号整数>\n");
			Lexical::next_word();
			if (word == Lexical::RBRACK) {
				SymbolTable::add_var(var_type, str, digits);
				Mid::add_mid_arr_declare(str, digits);
				Lexical::next_word();
			}
			else {
				Error::error_print(Error::no_rbrack_error);
				SymbolTable::add_var(var_type, str, digits);
			}
		}
		else {
			//普通变量定义
			if (var_type == Lexical::CHARTK) {
				SymbolTable::add_var(var_type, str, 0);
				Mid::add_mid_var_declare(str);
			}
			else {
				SymbolTable::add_var(var_type, str, 0);
				Mid::add_mid_var_declare(str);
			}
		}
		if (word == Lexical::COMMA) {
			Lexical::next_word();
			continue;
		}
		else if (word == Lexical::SEMICN) {
			if (print == 1) printf("<变量定义>\n");
			Lexical::next_word();
			break;
		}
		else {
			Error::error_print(Error::no_semi_error);
		}
	}while(word == Lexical::IDENFR);
}

void Syntax::var_declare()
{
	Lexical::WORD var_type;
	if (word != Lexical::INTTK && word != Lexical::CHARTK) {
		return;
	}
	while (word == Lexical::INTTK || word == Lexical::CHARTK) {
		var_type = word;
		Lexical::next_word();
		var_define(var_type);
	}
	if (print == 1) printf("<变量说明>\n");
}

void Syntax::para_define()
{
	if (word == Lexical::RPARENT) {
		if (print == 1) printf("<参数表>\n");
		Lexical::next_word();
		return;
	}
	//无参数且缺少右小括号
	if (word == Lexical::LBRACE) {
		Error::error_print(Error::no_rparent_error);
		return;
	}
	Lexical::WORD para_type;
	do {
		para_type = word;
		Lexical::next_word();//word为标识符
		string str = ident_string;//参数定义标识符
		SymbolTable::add_para(str,para_type);
		Mid::add_mid_para_declare(str);
		Lexical::next_word();
		if (word == Lexical::COMMA) {
			Lexical::next_word();
			continue;
		}
		else if (word == Lexical::RPARENT) {
			if (print == 1) printf("<参数表>\n");
			Lexical::next_word();
			return;
		}
		else {
			Error::error_print(Error::no_rparent_error);
		}
	} while (word == Lexical::CHARTK || word == Lexical::INTTK);
}
/*
void Syntax::print1(Lexical::WORD temp, string temp_str)
{
	if (temp == Lexical::MAINTK) {
		if (print == 1) printf("MAINTK %s\n", "main");
	}
	else {
		if (print == 1) printf("IDENFR %s\n", temp_str.c_str());
	}
}
*/
/*
 * 该函数在main函数外常量定义后执行
 * 发现出现了第一个函数定义后立即跳出循环(可能是一般函数定义，也可能是main函数)
 * 函数退出时变量定义已结束，继续函数定义解析
 */
void Syntax::complex_define()
{
	int func_flag = 0;
	Lexical::WORD complex_type;
	Lexical::WORD temp_type;
	int var_flag = 0;
	while ((func_flag == 0) && (word == Lexical::INTTK || word == Lexical::CHARTK || word == Lexical::VOIDTK)) {
		complex_type = word;//标识符类型
		Lexical::next_word();
		do {
			string str = ident_string;//数组或变量或函数ident
			temp_type = word;
			Lexical::next_word();
			//数组变量定义
			if (word == Lexical::LBRACK) {
				var_flag = 1;
				Lexical::next_word();//word为数组大小
				if (print == 1) printf("<无符号整数>\n");
				Lexical::next_word();
				if (word == Lexical::RBRACK) {
					SymbolTable::add_var(complex_type, str, digits);
					Mid::add_mid_arr_declare(str, digits);
					Lexical::next_word();
				}
				else {
					//error缺少右中括号
					Error::error_print(Error::no_rbrack_error);
					SymbolTable::add_var(complex_type, str, digits);
				}
			}
			//函数定义
			else if (word == Lexical::LPARENT) {
				has_return = 0;
				if (var_flag == 1) {
					if (print == 1) printf("<变量说明>\n");
				}
				func_flag = 1;
				if (complex_type == Lexical::CHARTK || complex_type == Lexical::INTTK) {
					if (print == 1) printf("<声明头部>\n");
				}
				if (complex_type == Lexical::CHARTK) {
					SymbolTable::add_func_name(str);
					SymbolTable::add_func(str, complex_type);
					Mid::add_mid_func_declare(str);
				}
				else if (complex_type == Lexical::INTTK) {
					SymbolTable::add_func_name(str);
					SymbolTable::add_func(str, complex_type);
					Mid::add_mid_func_declare(str);
				}
				else if (complex_type == Lexical::VOIDTK) {
					SymbolTable::add_func_name(str);
					SymbolTable::add_func(str, complex_type);
					Mid::add_mid_func_declare(str);
				}
				complex_func_body(complex_type);
				if (complex_type != Lexical::VOIDTK && has_return != 1) {
					Error::error_print(Error::unvoid_unreturn_error);
				}
				if (main_flag == 1) {
					return;
				}
			}
			//普通变量定义
			else if (word == Lexical::SEMICN || word == Lexical::COMMA) {
				var_flag = 1;
				if (complex_type == Lexical::CHARTK) {
					SymbolTable::add_var(complex_type, str, 0);
					Mid::add_mid_var_declare(str);
				}
				else if (complex_type == Lexical::INTTK) {
					SymbolTable::add_var(complex_type, str, 0);
					Mid::add_mid_var_declare(str);
				}
			}
			if (func_flag == 1) {
				break;
			}
			if (word == Lexical::COMMA) {
				Lexical::next_word();
				continue;
			}
			else if (word == Lexical::SEMICN) {
				if (print == 1) printf("<变量定义>\n");
				Lexical::next_word();
				break;
			}
			else {
				Error::error_print(Error::no_semi_error);
			}
		} while (word == Lexical::IDENFR);
	} 
}

void Syntax::complex_func_body(Lexical::WORD type)
{
	if (ident_string != "main") {
		Lexical::next_word();
		para_define();
	}
	else {
		main_flag = 1;
		Lexical::next_word();
		if (word == Lexical::RPARENT) {
			Lexical::next_word();
		}
		else {
			Error::error_print(Error::no_rparent_error);
		}
	}
	if (word == Lexical::LBRACE) {
		Lexical::next_word();
		compound_statement();
		if (word == Lexical::RBRACE) {
			if (main_flag == 0) {
				if (type == Lexical::VOIDTK) {
					if (print == 1) printf("<无返回值函数定义>\n");
				}
				else {
					if (print == 1) printf("<有返回值函数定义>\n");
				}
				Lexical::next_word();
			}
			else {
				if (print == 1) printf("<主函数>\n");
				return;
			}
		}
	}
}

void Syntax::func_body()
{
	Lexical::next_word();
	para_define();
	if (word == Lexical::LBRACE) {
		Lexical::next_word();
		compound_statement();
		if (word == Lexical::RBRACE) {
			Lexical::next_word();
		}
	}
}

void Syntax::func_define()
{
	Lexical::WORD func_type;
	if (main_flag == 1) {
		return;
	}
	while (word == Lexical::INTTK || word == Lexical::CHARTK || word == Lexical::VOIDTK) {
		func_type = word;
		Lexical::next_word();
		string str = ident_string;//函数名
		if (word == Lexical::MAINTK) {
			main_flag = 1;
			SymbolTable::add_func_name(ident_string);
			SymbolTable::add_func(ident_string, func_type);
			Mid::add_mid_func_declare(str);
			Lexical::next_word();//word为左小括号
			Lexical::next_word();//word为右小括号
			if (word == Lexical::RPARENT) {
				Lexical::next_word();//word为左大括号
			}
			else {
				Error::error_print(Error::no_rparent_error);
			}
			Lexical::next_word();
			compound_statement();
			if (print == 1) printf("<主函数>\n");//此时word为右大括号
			return;
		}
		else if (word == Lexical::IDENFR) {
			has_return = 0;
			if (func_type != Lexical::VOIDTK) {
				if (print == 1) printf("<声明头部>\n");
			}
			SymbolTable::add_func_name(ident_string);
			SymbolTable::add_func(ident_string, func_type);
			Mid::add_mid_func_declare(str);
			Lexical::next_word();//word为左小括号
			func_body();
			if (func_type == Lexical::VOIDTK) {
				if (print == 1) printf("<无返回值函数定义>\n");
			}
			else {
				if (print == 1) printf("<有返回值函数定义>\n");
			}
			if (has_return != 1 && func_type != Lexical::VOIDTK) {
				Error::error_print(Error::unvoid_unreturn_error);
			}
		}
	}
}

void Syntax::syntax()
{
	SymbolTable::add_func("global", Lexical::VOIDTK);
	Lexical::next_word();
	const_declare();
	complex_define();
	func_define();
	if (print == 1) printf("<程序>\n");
}
