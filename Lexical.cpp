#include "Globalvar.h"
#include "error.h"
#include "Lexical.h"

void Lexical::skip_blank()
{
	while (nextchar == '\n' || nextchar == '\t' || nextchar == ' ' || nextchar == '\r')
	{
		if (nextchar == '\n') {
			line_num++;
		}
		next_char();
	}
}

bool Lexical::is_letter()
{
	if  ((nextchar >= 'a' && nextchar <= 'z') || (nextchar >= 'A' && nextchar <= 'Z') || (nextchar == '_'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Lexical::is_digit()
{
	if (nextchar >= '0' && nextchar <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Lexical::is_op()
{
	if (nextchar == '+' || nextchar == '-' || nextchar == '*' || nextchar == '/') {
		return true;
	} 
	else {
		return false;
	}
}

bool Lexical::is_legal_string()
{
	if (nextchar >= 32 && nextchar <= 126 && nextchar != 34)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Lexical::is_reserve()
{
	int flag = -1;
	for (int i = 0; i < RESERVE_NUM; i++)
	{
		if (reserve_string_list[i] == ident_string)
		{
			flag = i;
			break;
		}
	}
	return flag;
}

void Lexical::next_char()
{
	while (code_all[char_index] == '\n')
	{
		char_index++;
		line_num++;
	}
	if (code_all[char_index] == '\0')
	{
		nextchar = '\0';
		return;
	}
	nextchar = code_all[char_index++];
}

void Lexical::next_word()
{
	last_num = line_num;
	skip_blank();
	if (is_letter()) {
		ident_string.clear();
		do {
			ident_string.push_back(nextchar);
			next_char();
		} while (is_letter() || is_digit());
		int is_key = is_reserve();
		if (is_key == -1) {
			word = IDENFR;
		}
		else {
			word = reserve_enum_list[is_key];
		}
		return;
	}
	if (is_digit()) {
		int sum = 0;
		if (nextchar == '0') {
			next_char();
			word = INTCON;
			digits = sum;
			return;
		}
		while (is_digit()) {
			sum = sum * 10 + (nextchar - '0');
			next_char();
		}
		word = INTCON;
		digits = sum;
		return;
	}
	if (nextchar == '\'') {
		next_char();
		ch = nextchar;
		if (is_digit() || is_letter() || is_op()) {
			next_char();
			if (nextchar == '\'') {
				next_char();
				word = CHARCON;
				return;
			}
			else {
				Error::error_print(Error::lexical_error);
				word = CHARCON;
				return;//error
			}
		}
		else {
			Error::error_print(Error::lexical_error);
			next_char();
			if (nextchar == '\'') {
				next_char();
				word = CHARCON;
				return;
			}
			else {
				word = CHARCON;
				return;
			}
		}
	}
	if (nextchar == '\"') {
		const_string.clear();
		next_char();
		do {
			if (nextchar == '\n') {
				line_num++;
				break;
			}
			if (nextchar == '\"') {
				break;
			}
			if (!is_legal_string()) {
				Error::error_print(Error::lexical_error);
			}
			const_string.push_back(nextchar);
			next_char();
		} while (nextchar != '\"' && nextchar != '\0');
		next_char();
		word = STRCON;
		str_const[str_const_count++] = const_string;
		return;
	}
	switch (nextchar)
	{
	case '<':
	{
		next_char();
		if (nextchar == '=') {
			next_char();
			word = LEQ;
			return;
		}
		else {
			word = LSS;
			return;
		}
	}
	case '>':
	{
		next_char();
		if (nextchar == '=') {
			next_char();
			word = GEQ;
			return;
		}
		else {
			word = GRE;
			return;
		}
	}
	case '=':
	{
		next_char();
		if (nextchar == '=') {
			next_char();
			word = EQL;
			return;
		}
		else {
			word = ASSIGN;
			return;
		}
	}
	case '!':
	{
		next_char();
		if (nextchar == '=') {
			next_char();
			word = NEQ;
			return;
		}
		else {
			Error::error_print(Error::lexical_error);
			word = NEQ;
			return;//error
		}
	}
	case '+':
		next_char();
		word = PLUS;
		return;
	case '-':
		next_char();
		word = MINU;
		return;
	case '*':
		next_char();
		word = MULT;
		return;
	case '/':
		next_char();
		word = DIV;
		return;
	case ';':
		next_char();
		word = SEMICN;
		return;
	case ',':
		next_char();
		word = COMMA;
		return;
	case '(':
		next_char();
		word = LPARENT;
		return;
	case ')':
		next_char();
		word = RPARENT;
		return;
	case '[':
		next_char();
		word = LBRACK;
		return;
	case ']':
		next_char();
		word = RBRACK;
		return;
	case '{':
		next_char();
		word = LBRACE;
		return;
	case '}':
		next_char();
		word = RBRACE;
		return;
	default:
		word = DEFAULT;
		Error::error_print(Error::lexical_error);
		next_char();
		next_word();
		return;
	}
}

void Lexical::test_next_word()
{
	while (nextchar != '\0')
	{
		next_word();
		switch (word)
		{
		case IDENFR:
			printf("IDENFR %s\n",ident_string.c_str());
			break;
		case INTCON:
			printf("INTCON %d\n", digits);
			break;
		case CHARCON:
			printf("CHARCON %c\n", ch);
			break;
		case STRCON:
			printf("STRCON %s\n", const_string.c_str());
			break;
		case CONSTTK:
			printf("CONSTTK %s\n","const");
			break;
		case INTTK:
			printf("INTTK %s\n","int");
			break;
		case CHARTK:
			printf("CHARTK %s\n", "char");
			break;
		case VOIDTK:
			printf("VOIDTK %s\n", "void");
			break;
		case MAINTK:
			printf("MAINTK %s\n", "main");
			break;
		case IFTK:
			printf("IFTK %s\n", "if");
			break;
		case ELSETK:
			printf("ELSETK %s\n", "else");
			break;
		case DOTK:
			printf("DOTK %s\n", "do");
			break;
		case WHILETK:
			printf("WHILETK %s\n", "while");
			break;
		case FORTK:
			printf("FORTK %s\n", "for");
			break;
		case SCANFTK:
			printf("SCANFTK %s\n", "scanf");
			break;
		case PRINTFTK:
			printf("PRINTFTK %s\n", "printf");
			break;
		case RETURNTK:
			printf("RETURNTK %s\n", "return");
			break;
		case PLUS:
			printf("PLUS %s\n", "+");
			break;
		case MINU:
			printf("MINU %s\n", "-");
			break;
		case MULT:
			printf("MULT %s\n", "*");
			break;
		case DIV:
			printf("DIV %s\n", "/");
			break;
		case LSS:
			printf("LSS %s\n", "<");
			break;
		case LEQ:
			printf("LEQ %s\n", "<=");
			break;
		case GRE:
			printf("GRE %s\n", ">");
			break;
		case GEQ:
			printf("GEQ %s\n", ">=");
			break;
		case EQL:
			printf("EQL %s\n", "==");
			break;
		case NEQ:
			printf("NEQ %s\n", "!=");
			break;
		case ASSIGN:
			printf("ASSIGN %s\n", "=");
			break;
		case SEMICN:
			printf("SEMICN %s\n", ";");
			break;
		case COMMA:
			printf("COMMA %s\n", ",");
			break;
		case LPARENT:
			printf("LPARENT %s\n", "(");
			break;
		case RPARENT:
			printf("RPARENT %s\n", ")");
			break;
		case LBRACK:
			printf("LBRACK %s\n", "[");
			break;
		case RBRACK:
			printf("RBRACK %s\n", "]");
			break;
		case LBRACE:
			printf("LBRACE %s\n", "{");
			break;
		case RBRACE:
			printf("RBRACE %s\n", "}");
			break;
		default:
			break;
		}
	}
}

void Lexical::print_token(WORD token)
{
	switch (token)
	{
	case IDENFR:
		printf("IDENFR %s\n", ident_string.c_str());
		break;
	case INTCON:
		printf("INTCON %d\n", digits);
		break;
	case CHARCON:
		printf("CHARCON %c\n", ch);
		break;
	case STRCON:
		printf("STRCON %s\n", const_string.c_str());
		break;
	case CONSTTK:
		printf("CONSTTK %s\n", "const");
		break;
	case INTTK:
		printf("INTTK %s\n", "int");
		break;
	case CHARTK:
		printf("CHARTK %s\n", "char");
		break;
	case VOIDTK:
		printf("VOIDTK %s\n", "void");
		break;
	case MAINTK:
		printf("MAINTK %s\n", "main");
		break;
	case IFTK:
		printf("IFTK %s\n", "if");
		break;
	case ELSETK:
		printf("ELSETK %s\n", "else");
		break;
	case DOTK:
		printf("DOTK %s\n", "do");
		break;
	case WHILETK:
		printf("WHILETK %s\n", "while");
		break;
	case FORTK:
		printf("FORTK %s\n", "for");
		break;
	case SCANFTK:
		printf("SCANFTK %s\n", "scanf");
		break;
	case PRINTFTK:
		printf("PRINTFTK %s\n", "printf");
		break;
	case RETURNTK:
		printf("RETURNTK %s\n", "return");
		break;
	case PLUS:
		printf("PLUS %s\n", "+");
		break;
	case MINU:
		printf("MINU %s\n", "-");
		break;
	case MULT:
		printf("MULT %s\n", "*");
		break;
	case DIV:
		printf("DIV %s\n", "/");
		break;
	case LSS:
		printf("LSS %s\n", "<");
		break;
	case LEQ:
		printf("LEQ %s\n", "<=");
		break;
	case GRE:
		printf("GRE %s\n", ">");
		break;
	case GEQ:
		printf("GEQ %s\n", ">=");
		break;
	case EQL:
		printf("EQL %s\n", "==");
		break;
	case NEQ:
		printf("NEQ %s\n", "!=");
		break;
	case ASSIGN:
		printf("ASSIGN %s\n", "=");
		break;
	case SEMICN:
		printf("SEMICN %s\n", ";");
		break;
	case COMMA:
		printf("COMMA %s\n", ",");
		break;
	case LPARENT:
		printf("LPARENT %s\n", "(");
		break;
	case RPARENT:
		printf("RPARENT %s\n", ")");
		break;
	case LBRACK:
		printf("LBRACK %s\n", "[");
		break;
	case RBRACK:
		printf("RBRACK %s\n", "]");
		break;
	case LBRACE:
		printf("LBRACE %s\n", "{");
		break;
	case RBRACE:
		printf("RBRACE %s\n", "}");
		break;
	default:
		break;
	}
}