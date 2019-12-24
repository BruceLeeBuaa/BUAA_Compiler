#pragma once

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
using namespace std;

namespace Lexical {
	enum WORD {
		IDENFR, INTCON, CHARCON, STRCON, CONSTTK, INTTK, CHARTK, VOIDTK, MAINTK, IFTK, ELSETK, DOTK, WHILETK,
		FORTK, SCANFTK, PRINTFTK, RETURNTK, PLUS, MINU, MULT, DIV, LSS, LEQ, GRE, GEQ, EQL, NEQ, ASSIGN,
		SEMICN, COMMA, LPARENT, RPARENT, LBRACK, RBRACK, LBRACE, RBRACE, DEFAULT
	};

	void skip_blank();
	bool is_letter();
	bool is_digit();
	bool is_op();
	bool is_legal_string();
	int is_reserve();
	void next_char();
	void next_word();
	void test_next_word();
	void print_token(WORD token);
}