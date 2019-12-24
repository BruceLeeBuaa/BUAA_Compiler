#include "Lexical.h"
#include "Globalvar.h"
#include "error.h"
#include "Syntax.h"
#include "SymbolTable.h"

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

//数字
void Syntax::number()
{
	int op = 1;
	if (word == Lexical::PLUS || word == Lexical::MINU)
	{
		if (print == 1) Lexical::print_token(word);
		if (word == Lexical::PLUS) {
			op = 1;
		} else {
			op = -1;
		}
		Lexical::next_word();
		if (print == 1) Lexical::print_token(word);
		if (word == Lexical::INTCON) {
			digits = digits * op;
		} else {
			//error类型错误
		}
	} 
	else {
		if (word != Lexical::INTCON) {
			//error类型错误
		}
		if (print == 1) Lexical::print_token(word);
	}
	Lexical::next_word();
	if (print == 1) printf("<无符号整数>\n");
	if (print == 1) printf("<整数>\n");
}
//因子
void Syntax::factor()
{
	if (word == Lexical::CHARCON) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
	}
	else if (word == Lexical::INTCON || word == Lexical::PLUS || word == Lexical::MINU) {
		is_int = 1;
		number();
	}
	else if (word == Lexical::LPARENT) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		expression();
		if (word == Lexical::RPARENT) {
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
		} else {
			//error缺少右小括号
			Error::error_print(Error::no_rparent_error);
		}
		is_int = 1;
	} 
	//函数调用或变量
	else if (word == Lexical::IDENFR) {
		if (print == 1) Lexical::print_token(word);
		bool has_return = SymbolTable::func_has_return(ident_string);
		string str = ident_string;
		Lexical::next_word();
		if (word == Lexical::LBRACK) {
			if (print == 1) Lexical::print_token(word);
			//error未定义标识符
			SymbolTable::is_ident_define(str);
			Lexical::next_word();
			expression();
			if (is_int == 0) {
				Error::error_print(Error::array_error);
			}
			is_int = 0;
			if (SymbolTable::ident_type(str) == SymbolTable::int_array) {
				is_int = 1;
			}
			if (word == Lexical::RBRACK) {
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
			} else {
				//error缺少右中括号
				Error::error_print(Error::no_rbrack_error);
			}
		} 
		else if (word == Lexical::LPARENT){
			if (print == 1) Lexical::print_token(word);
			//error未定义函数名
			if (SymbolTable::is_func_define(str) == -1) {
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
			if (SymbolTable::func_type(str) == SymbolTable::int_func) {
				is_int = 1;
			}
		} 
		else {
			SymbolTable::is_ident_define(str);//error未定义标识符（看是否）
			if (SymbolTable::ident_type(str) == SymbolTable::int_var || 
				SymbolTable::ident_type(str) == SymbolTable::int_para || 
				SymbolTable::ident_type(str) == SymbolTable::int_const) {
				is_int = 1;
			}
		}
	} 
	else {
		//error缺少因子
	}
	if (print == 1) printf("<因子>\n");
}

void Syntax::term()
{
	factor();
	if (word == Lexical::MULT || word == Lexical::DIV) {
		is_int = 1;
	}
	while (word == Lexical::MULT || word == Lexical::DIV) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		factor();
	}
	if (print == 1) printf("<项>\n");
}

void Syntax::expression()
{
	is_int = 0;//初始化为char
	if (word == Lexical::PLUS || word == Lexical::MINU) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
	}
	term();
	if (word == Lexical::PLUS || word == Lexical::MINU) {
		is_int = 1;
	}
	while (word == Lexical::PLUS || word == Lexical::MINU) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		term();
	}
	if (print == 1) printf("<表达式>\n");
}

void Syntax::compound_statement()
{
	const_declare();
	var_declare();
	statement_block();
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
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		if_else_statement();
		if (print == 1) printf("<语句>\n");
		break;
	}
	case Lexical::DOTK: {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		do_while_statement();
		if (print == 1) printf("<语句>\n");
		break;
	}
	case Lexical::WHILETK: {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		while_statement();
		if (print == 1) printf("<语句>\n");
		break;
	}
	case Lexical::FORTK: {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		for_statement();
		if (print == 1) printf("<语句>\n");
		break;
	}
	case Lexical::SCANFTK: {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		scanf_statement();
		if (word == Lexical::SEMICN) {
			if (print == 1) Lexical::print_token(word);
			if (print == 1) printf("<语句>\n");
			Lexical::next_word();
		}
		else {
			//error缺少分号
			Error::error_print(Error::no_semi_error);
		}
		break;
	}
	case Lexical::PRINTFTK: {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		printf_statement();
		if (word == Lexical::SEMICN) {
			if (print == 1) Lexical::print_token(word);
			if (print == 1) printf("<语句>\n");
			Lexical::next_word();
		}
		else {
			//error缺少分号
			Error::error_print(Error::no_semi_error);
		}
		break;
	}
	case Lexical::RETURNTK: {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		return_statement();
		if (word == Lexical::SEMICN) {
			if (print == 1) Lexical::print_token(word);
			if (print == 1) printf("<语句>\n");
			Lexical::next_word();
		}
		else {
			//error缺少分号
			Error::error_print(Error::no_semi_error);
		}
		break;
	}
	case Lexical::SEMICN:
		if (print == 1) Lexical::print_token(word);
		if (print == 1) printf("<语句>\n");
		Lexical::next_word();
		break;
	case Lexical::LBRACE: {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		statement_block();
		if (word == Lexical::RBRACE) {
			if (print == 1) Lexical::print_token(word);
			if (print == 1) printf("<语句>\n");
			Lexical::next_word();
		}
		else {
			//error缺少右大括号
		}
		break;
	}
	case Lexical::IDENFR: {
		if (print == 1) Lexical::print_token(word);
		bool has_return = SymbolTable::func_has_return(ident_string);
		Lexical::next_word();
		if (word == Lexical::LPARENT) {
			if (print == 1) Lexical::print_token(word);
			//error未定义函数名
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
				if (print == 1) Lexical::print_token(word);
				if (print == 1) printf("<语句>\n");
				Lexical::next_word();
			}
			else {
				//error缺分号
				Error::error_print(Error::no_semi_error);
			}
		}
		else {
			SymbolTable::is_ident_define(ident_string);//error未定义标识符
			assign_statement();
			if (word == Lexical::SEMICN) {
				if (print == 1) Lexical::print_token(word);
				if (print == 1) printf("<语句>\n");
				Lexical::next_word();
			}
			else {
				//error缺少分号
				Error::error_print(Error::no_semi_error);
			}
		}
		break;
	}
	default:
		break;
	}
}

void Syntax::condition()
{
	expression();
	if (is_int == 0) {
		Error::error_print(Error::condition_error);
	}
	if (word == Lexical::LSS || word == Lexical::LEQ || word == Lexical::GRE
		|| word == Lexical::GEQ || word == Lexical::EQL || word == Lexical::NEQ) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		expression();
		if (is_int == 0) {
			Error::error_print(Error::condition_error);
		}
	}
	if (print == 1) printf("<条件>\n");
}

void Syntax::if_else_statement()
{
	if (word == Lexical::LPARENT) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		condition();
		if (word == Lexical::RPARENT) {
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
			statement();
			if (word == Lexical::ELSETK) {
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
				statement();
			}
		} else {
			//error缺少右小括号
			Error::error_print(Error::no_rparent_error);
		}
	}
	else {
		//error缺少左括号
	}
	if (print == 1) printf("<条件语句>\n");
}

void Syntax::do_while_statement()
{
	statement();
	if (word == Lexical::WHILETK) {
		//int line = line_num;
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		if (word == Lexical::LPARENT) {
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
			condition();
			if (word == Lexical::RPARENT) {
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
			} 
			else {
				//缺少右小括号
				Error::error_print(Error::no_rparent_error);
			}
		}
		else {
			//error缺少左小括号
		}
	} 
	else {
		//error缺少while
		Error::error_print(Error::no_while_error);
		skip2rparent;
		Lexical::next_word();
	}
	if (print == 1) printf("<循环语句>\n");
}

void Syntax::while_statement()
{
	if (word == Lexical::LPARENT) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		condition();
		if (word == Lexical::RPARENT) {
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
			statement();
		}
		else {
			//error缺少右小括号
			Error::error_print(Error::no_rparent_error);
		}
	}
	else {
		//error缺少左小括号
	}
	if (print == 1) printf("<循环语句>\n");
}

void Syntax::for_statement()
{
	if (word == Lexical::LPARENT) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		if (word == Lexical::IDENFR) {
			if (print == 1) Lexical::print_token(word);
			SymbolTable::is_ident_define(ident_string);//error未定义标识符
			Lexical::next_word();
			if (word == Lexical::ASSIGN) {
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
				expression();
				if (word == Lexical::SEMICN) {
					if (print == 1) Lexical::print_token(word);
					Lexical::next_word();
					condition();
					if(word == Lexical::SEMICN){
						if (print == 1) Lexical::print_token(word);
						Lexical::next_word();
						if (word == Lexical::IDENFR) {
							if (print == 1) Lexical::print_token(word);
							SymbolTable::is_ident_define(ident_string);//error未定义标识符
							Lexical::next_word();
							if (word == Lexical::ASSIGN) {
								if (print == 1) Lexical::print_token(word);
								Lexical::next_word();
								if (word == Lexical::IDENFR) {
									if (print == 1) Lexical::print_token(word);
									SymbolTable::is_ident_define(ident_string);//error未定义标识符
									Lexical::next_word();
									switch(word)
									{
									case Lexical::PLUS:
										break;
									case Lexical::MINU:
										break;
									default:
										//error缺少加法运算符
										break;
									}
									if (print == 1) Lexical::print_token(word);
									Lexical::next_word();
									step();
									if (word == Lexical::RPARENT) {
										if (print == 1) Lexical::print_token(word);
										Lexical::next_word();
										statement();
									}
									else {
										//error缺少右小括号
										Error::error_print(Error::no_rparent_error);
									}
								}
								else {
									//error缺少标识符
								}
							}
							else {
								//error缺少赋值符号
							}
						}
						else {
							//error缺少标识符
						}
					}
					else {
						//error缺少分号
						Error::error_print(Error::no_semi_error);
					}
				}
				else {
					//error缺少分号
					Error::error_print(Error::no_semi_error);
				}
			}
			else {
				//error缺少赋值符号
			}
		}
		else {
			//error缺少标识符
		}
	}
	else {
		//error缺少左小括号
	}
	if (print == 1) printf("<循环语句>\n");
}

void Syntax::step()
{
	if (word == Lexical::INTCON) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
	} 
	else {
		//error类型错误
	}
	if (print == 1) printf("<无符号整数>\n");
	if (print == 1) printf("<步长>\n");
}

void Syntax::assign_statement()
{
	if (word == Lexical::LBRACK) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		expression();
		if (is_int == 0) {
			Error::error_print(Error::array_error);
		}
		if (word == Lexical::RBRACK) {
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
			if (word == Lexical::ASSIGN) {
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
				expression();
			}
			else {
				//error缺少赋值符号
			}
		}
		else {
			//error缺少右中括号
			Error::error_print(Error::no_rbrack_error);
			if (word == Lexical::ASSIGN) {
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
				expression();
			}
		}
	}
	else if (word == Lexical::ASSIGN) {
		if (print == 1) Lexical::print_token(word);
		if (SymbolTable::is_const(ident_string)) {
			Error::error_print(Error::const_assign_error);
		}
		Lexical::next_word();
		expression();
	}
	else {
		//error缺少等号或左中括号
	}
	if (print == 1) printf("<赋值语句>\n");
}

void Syntax::scanf_statement()
{
	if (word == Lexical::LPARENT) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		if (word == Lexical::IDENFR) {
			while (word == Lexical::IDENFR) {
				if (print == 1) Lexical::print_token(word);
				SymbolTable::is_ident_define(ident_string);//error未定义标识符
				Lexical::next_word();
				if (word == Lexical::COMMA) {
					if (print == 1) Lexical::print_token(word);
					Lexical::next_word();
					continue;
				}
				else if (word == Lexical::RPARENT) {
					if (print == 1) Lexical::print_token(word);
					Lexical::next_word();
					break;
				}
				else {
					//缺少右小括号
					Error::error_print(Error::no_rparent_error);
				}
			}
		}
		else {
			//error缺少标识符
		}
	}
	else {
		//error缺少左小括号
	}
	if (print == 1) printf("<读语句>\n");
}

void Syntax::printf_statement()
{
	if (word == Lexical::LPARENT) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		if (word == Lexical::STRCON) {
			if (print == 1) Lexical::print_token(word);
			if (print == 1) printf("<字符串>\n");		//别忘了存字符串
			Lexical::next_word();
			if (word == Lexical::COMMA) {
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
				printf_expression();
			}
			else if (word == Lexical::RPARENT) {
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
			}
			else {
				//error缺少右小括号
				Error::error_print(Error::no_rparent_error);
			}
		}
		else {
			printf_expression();
		}
	}
	else {
		//error缺少左小括号
	}
	if (print == 1) printf("<写语句>\n");
}

void Syntax::printf_expression()
{
	expression();
	if (word == Lexical::RPARENT) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
	}
	else {
		//error缺少右小括号
		Error::error_print(Error::no_rparent_error);
	}
}

void Syntax::return_statement()
{
	if (word == Lexical::LPARENT) {
		if (!SymbolTable::func_return()) {
			Error::error_print(Error::void_return_error);
		}
		has_return = 1;
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		expression();
		if ((is_int == 1 && SymbolTable::current_func_type() == SymbolTable::char_func)
			|| (is_int == 0 && SymbolTable::current_func_type() == SymbolTable::int_func)) {
			Error::error_print(Error::unvoid_unreturn_error);
		}
		if (word == Lexical::RPARENT) {
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
		}
		else {
			//error缺少右小括号
			Error::error_print(Error::no_rparent_error);
		}
	}
	else {
		//无返回值
		if (SymbolTable::func_return()) {
			Error::error_print(Error::unvoid_unreturn_error);
		}
	}
	if (print == 1) printf("<返回语句>\n");
}

void Syntax::func_statement()
{
	//进入时word为左括号
	string str = ident_string;//函数名
	int num = SymbolTable::func_para_num();
	Lexical::next_word();
	if (num != 0 && word == Lexical::RPARENT) {
		//error参数个数错误,少
		Error::error_print(Error::paranum_error);
		Lexical::next_word();
		return;
	}
	for (int i = 1;i < num;i++) {
		expression();
		SymbolTable::para_type_inspect(i-1, is_int,str);
		if (word == Lexical::COMMA) {
			if (print == 1) Lexical::print_token(word);
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
		expression();
		SymbolTable::para_type_inspect(num-1, is_int, str);
	}
	if (word == Lexical::RPARENT) {
		if (print == 1) printf("<值参数表>\n");
		if (print == 1) Lexical::print_token(word);
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
		//error缺少右小括号
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
			Error::error_print(Error::no_rparent_error);
		}
	}
	if (print == 1) printf("<有返回值函数调用语句>\n");
}

void Syntax::void_func_statement()
{
	string str = ident_string;//函数名
	int num = SymbolTable::func_para_num();
	Lexical::next_word();
	if (num != 0 && word == Lexical::RPARENT) {
		//error参数个数错误,少
		Error::error_print(Error::paranum_error);
		Lexical::next_word();
		return;
	}
	for (int i = 1; i < num; i++) {
		expression();
		SymbolTable::para_type_inspect(i-1, is_int, str);
		if (word == Lexical::COMMA) {
			if (print == 1) Lexical::print_token(word);
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
		expression();
		SymbolTable::para_type_inspect(num-1, is_int, str);
	}
	if (word == Lexical::RPARENT) {
		if (print == 1) printf("<值参数表>\n");
		if (print == 1) Lexical::print_token(word);
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
		//error缺少右小括号
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
			Error::error_print(Error::no_rparent_error);
		}
	}
	if (print == 1) printf("<无返回值函数调用语句>\n");
}


void Syntax::const_define()
{
	Lexical::WORD type = word;
	if (word == Lexical::INTTK || word == Lexical::CHARTK) {
		if (print == 1) Lexical::print_token(word);
		do {
			Lexical::next_word();
			if (print == 1) Lexical::print_token(word);
			if (word != Lexical::IDENFR) {
				//error 缺少标识符
			}
			string str = ident_string;
			Lexical::next_word();
			if (print == 1) Lexical::print_token(word);
			if (word != Lexical::ASSIGN) {
				//error缺少赋值符号
			}
			Lexical::next_word();
			if (print == 1) Lexical::print_token(word);
			int op = 1;
			if (word == Lexical::CHARCON) {
				SymbolTable::add_const(type, str, (int)ch);
				if (type == Lexical::INTTK) {
					//error类型不匹配
				}
			}
			else if (word == Lexical::PLUS || word == Lexical::MINU) {
				if (word == Lexical::PLUS) {
					op = 1;
				}
				else {
					op = -1;
				}
				Lexical::next_word();
				if (print == 1) Lexical::print_token(word);
				if (word != Lexical::INTCON) {
					//error常量定义错误
				}
				if (print == 1) printf("<无符号整数>\n");
				digits = digits * op;
				if (print == 1) printf("<整数>\n");
				SymbolTable::add_const(type, str, digits);
				if (type == Lexical::CHARTK) {
					//error类型不匹配
				}
			}
			else if (word == Lexical::INTCON) {
				if (print == 1) printf("<无符号整数>\n");
				if (print == 1) printf("<整数>\n");
				SymbolTable::add_const(type, str, digits);
				if (type == Lexical::CHARTK) {
					//error类型不匹配
				}
			}
			else {
				Error::error_print(Error::const_type_error);
			}
			Lexical::next_word();
			if (word == Lexical::COMMA) {
				if (print == 1) Lexical::print_token(word);
				continue;
			}
			else if (word == Lexical::SEMICN) {
				if (print == 1) printf("<常量定义>\n");
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
				break;
			}
			else {
				//缺少分号
				Error::error_print(Error::no_semi_error);
			}
		} while (word == Lexical::COMMA);
	}
	else {
		//error 标识符类型错误
	}
}

void Syntax::const_declare()
{
	if (word != Lexical::CONSTTK) {
		return;
	}
	while (word == Lexical::CONSTTK) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		const_define();
	}
	if (print == 1) printf("<常量说明>\n");
}

void Syntax::var_define(Lexical::WORD type)
{
	Lexical::WORD var_type = type;
	do {
		if (word != Lexical::IDENFR) {
			//error缺少标识符
		}
		if (print == 1) Lexical::print_token(word);
		string str = ident_string;
		Lexical::next_word();
		if (word == Lexical::LBRACK) {
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();//数组大小
			if (print == 1) Lexical::print_token(word);
			if (print == 1) printf("<无符号整数>\n");
			Lexical::next_word();
			if (word == Lexical::RBRACK) {
				if (print == 1) Lexical::print_token(word);
				SymbolTable::add_var(var_type, str, digits);
				Lexical::next_word();
			}
			else {
				//error缺少右中括号
				Error::error_print(Error::no_rbrack_error);
				SymbolTable::add_var(var_type, str, digits);
			}
		}
		else {
			if (var_type == Lexical::CHARTK) {
				SymbolTable::add_var(var_type, str, 0);
			}
			else {
				SymbolTable::add_var(var_type, str, 0);
			}
		}
		if (word == Lexical::COMMA) {
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
			continue;
		}
		else if (word == Lexical::SEMICN) {
			if (print == 1) printf("<变量定义>\n");
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
			break;
		}
		else {
			//error缺少分号
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
		if (print == 1) Lexical::print_token(word);
		var_type = word;
		Lexical::next_word();
		var_define(var_type);
	}
	if (print == 1) printf("<变量说明>\n");
}

void Syntax::para_define()
{
	if (word == Lexical::RPARENT) {
		if (print == 1) printf("<参数表>\n");				//要在右小括号输出前输出参数表
		if (print == 1) Lexical::print_token(word);
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
		if (!(word == Lexical::CHARTK || word == Lexical::INTTK)) {
			//error参数类型错误
		}
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		if (word != Lexical::IDENFR) {
			//error标识符错误
		}
		if (print == 1) Lexical::print_token(word);
		string str = ident_string;
		SymbolTable::add_para(str,para_type);
		Lexical::next_word();
		if (word == Lexical::COMMA) {
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
			if (!(word == Lexical::CHARTK || word == Lexical::INTTK)) {
				//error参数类型错误
			}
			continue;
		}
		else if (word == Lexical::RPARENT) {
			if (print == 1) printf("<参数表>\n");
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
			return;
		}
		else {
			Error::error_print(Error::no_rparent_error);
		}
	} while (word == Lexical::CHARTK || word == Lexical::INTTK);
}

void Syntax::print1(Lexical::WORD temp, string temp_str)
{
	if (temp == Lexical::MAINTK) {
		if (print == 1) printf("MAINTK %s\n", "main");
	}
	else {
		if (print == 1) printf("IDENFR %s\n", temp_str.c_str());
	}
}

void Syntax::complex_define()
{
	int func_flag = 0;
	Lexical::WORD complex_type;
	Lexical::WORD temp_type;
	int var_flag = 0;
	int once_enter;
	while ((func_flag == 0) && (word == Lexical::INTTK || word == Lexical::CHARTK || word == Lexical::VOIDTK)) {
		once_enter = 1;
		complex_type = word;
		Lexical::next_word();
		do {
			if (word != Lexical::IDENFR && word != Lexical::MAINTK) {
				//error缺少标识符
			}
			string str = ident_string;
			temp_type = word;
			Lexical::next_word();
			//数组变量定义
			if (word == Lexical::LBRACK) {
				var_flag = 1;
				if (once_enter == 1) {
					if (print == 1) Lexical::print_token(complex_type);
					once_enter = 0;
				}
				if (print == 1) printf("IDENFR %s\n", str.c_str());
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();//数组大小
				if (print == 1) Lexical::print_token(word);
				if (print == 1) printf("<无符号整数>\n");
				Lexical::next_word();
				if (word == Lexical::RBRACK) {
					if (print == 1) Lexical::print_token(word);
					SymbolTable::add_var(complex_type, str, digits);
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
				if (print == 1) Lexical::print_token(complex_type);
				print1(temp_type, str);
				func_flag = 1;
				if (complex_type == Lexical::CHARTK || complex_type == Lexical::INTTK) {
					if (print == 1) printf("<声明头部>\n");
				}
				if (print == 1) Lexical::print_token(word);
				if (complex_type == Lexical::CHARTK) {
					SymbolTable::add_func_name(str);
					SymbolTable::add_func(str, complex_type);
				}
				else if (complex_type == Lexical::INTTK) {
					SymbolTable::add_func_name(str);
					SymbolTable::add_func(str, complex_type);
				}
				else if (complex_type == Lexical::VOIDTK) {
					SymbolTable::add_func_name(str);
					SymbolTable::add_func(str, complex_type);
				}
				else {
					//error类型错误
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
				if (once_enter == 1) {
					if (print == 1) Lexical::print_token(complex_type);
					once_enter = 0;
				}
				if (print == 1) printf("IDENFR %s\n", str.c_str());
				if (complex_type == Lexical::CHARTK) {
					SymbolTable::add_var(complex_type, str, 0);
				}
				else if (complex_type == Lexical::INTTK) {
					SymbolTable::add_var(complex_type, str, 0);
				}
				else {
					//error变量定义类型错误
				}
			}
			else {
				//error格式错误
			}
			if (func_flag == 1) {
				break;
			}
			if (word == Lexical::COMMA) {
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
				continue;
			}
			else if (word == Lexical::SEMICN) {
				if (print == 1) printf("<变量定义>\n");
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
				break;
			}
			else {
				//缺少分号
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
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
		}
		else {
			//error缺少右小括号
			Error::error_print(Error::no_rparent_error);
		}
	}
	if (word == Lexical::LBRACE) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		compound_statement();
		if (word == Lexical::RBRACE) {
			if (print == 1) Lexical::print_token(word);
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
		else {
			//error缺少右大括号
		}
	}
	else {
		//error缺少左大括号
	}
}

void Syntax::func_body()
{
	Lexical::next_word();
	para_define();
	if (word == Lexical::LBRACE) {
		if (print == 1) Lexical::print_token(word);
		Lexical::next_word();
		compound_statement();
		if (word == Lexical::RBRACE) {
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
		}
		else {
			//error缺少右大括号
		}
	}
	else {
		//error缺少左大括号
	}
}

void Syntax::func_define()
{
	Lexical::WORD func_type;
	if (main_flag == 1) {
		return;
	}
	while (word == Lexical::INTTK || word == Lexical::CHARTK || word == Lexical::VOIDTK) {
		if (print == 1) Lexical::print_token(word);
		func_type = word;
		Lexical::next_word();
		if (word == Lexical::MAINTK) {
			if (print == 1) Lexical::print_token(word);
			main_flag = 1;
			SymbolTable::add_func_name(ident_string);
			SymbolTable::add_func(ident_string, func_type);
			if (func_type != Lexical::VOIDTK) {
				//main函数类型错误
			}
			Lexical::next_word();
			if (word != Lexical::LPARENT) {
				//error缺少左小括号
			}
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
			if (word == Lexical::RPARENT) {
				if (print == 1) Lexical::print_token(word);
				Lexical::next_word();
			}
			else {
				//error缺少右小括号
				Error::error_print(Error::no_rparent_error);
			}
			if (word != Lexical::LBRACE) {
				//error缺少左大括号
			}
			if (print == 1) Lexical::print_token(word);
			Lexical::next_word();
			compound_statement();
			if (word != Lexical::RBRACE) {
				//error缺少右大括号
			}
			if (print == 1) Lexical::print_token(word);
			if (print == 1) printf("<主函数>\n");
			return;
		}
		else if (word == Lexical::IDENFR) {
			has_return = 0;
			if (print == 1) Lexical::print_token(word);
			if (func_type != Lexical::VOIDTK) {
				if (print == 1) printf("<声明头部>\n");
			}
			SymbolTable::add_func_name(ident_string);
			SymbolTable::add_func(ident_string, func_type);
			Lexical::next_word();
			if (word != Lexical::LPARENT) {
				//error缺少左小括号
			}
			if (print == 1) Lexical::print_token(word);
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
		else {
			//缺少函数名
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
	if (main_flag == 0) {
		//error缺少主函数
	}
	if (print == 1) printf("<程序>\n");
}
