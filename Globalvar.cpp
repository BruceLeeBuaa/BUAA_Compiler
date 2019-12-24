#include "Lexical.h"
#include "Globalvar.h"
#include "SymbolTable.h"
#include "Opt_midcode.h"
#include "Opt_target.h"
#define MAX_STR_CONST 1000

string reserve_string_list[RESERVE_NUM] = {"const","int","char","void","main","if","else","do","while",
"for","scanf","printf","return"};
Lexical::WORD reserve_enum_list[RESERVE_NUM] = { Lexical::CONSTTK,Lexical::INTTK,Lexical::CHARTK,
Lexical::VOIDTK,Lexical::MAINTK,Lexical::IFTK,Lexical::ELSETK,Lexical::DOTK,Lexical::WHILETK,
Lexical::FORTK,Lexical::SCANFTK,Lexical::PRINTFTK,Lexical::RETURNTK};
Lexical::WORD word = Lexical::DEFAULT;
char code_all[ALLLENGTH_MAX];
int code_all_len = 0;
char nextchar = ' ';
string ident_string = "";
string const_string = "";
int digits = 0;						
int ch = ' ';
int char_index = 0;
int line_num = 1;
int last_num = 1;

int str_const_count = 0;
string str_const[MAX_STR_CONST];

int table_num = 0;
vector<SymbolTable::GTABLE> func_table;
vector<string> func_name_list;

int print = 0;
int is_int = 0;//默认char
int has_return = 0;//只针对有返回值函数定义

int print_midcode = 0;
int num_index = 0;//#1开始代表寄存器，中间变量
int label_index = 0;//代表标签的编号
int midcode_index = 0;//代表一共多少条中间代码
vector<Mid::MidCode> midcode;
vector<Mid::MidCode> midcode_opt1;

int current_midcode_index = 0;
int tarcode_index = 0;
int opt_index = 0;

vector<Target::TargetCode> target_code;
vector<Opt_Target::OptTarget> target_opt;
string current_func_name;
string opt_mid_current_func;

int ret_temp = 0;
int max_num_index = 20;
int total_max_num_index = 20;

vector<string> S_reg(8, ""); //索引是寄存器编号，string代表"局部变量"名
vector<string> T_reg(8, ""); //索引是寄存器编号，string代表"中间变量#i,局部变量，全局变量"名
vector<string> A_reg(4, "");
vector<string> temp_reg(3000, "");

int current_S_index = 0;//当前使用了几个，%8可得当前应该使用第几个
int current_T_index = 0;

vector<opt_mid::REPLACE> relations(10000);
int need_save_A[10] = { 0 };
int need_save_S[10] = { 0 };
int t8t9 = 0;

vector<In_Line::InLineFunc> inline_func;
int begin_index = 0;
vector<Mid::MidCode> opt_for;