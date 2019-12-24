#pragma once
#include "Lexical.h"
#include "SymbolTable.h"
#include "get_midcode.h"
#include "targetcode.h"
#include "Opt_midcode.h"
#include "Opt_target.h"
#include "inline.h"
#define RESERVE_NUM 13
#define ALLLENGTH_MAX 300000

extern string reserve_string_list[RESERVE_NUM];
extern Lexical::WORD reserve_enum_list[RESERVE_NUM];
extern Lexical::WORD word;
extern char code_all[ALLLENGTH_MAX];
extern int code_all_len;								   					   
extern char nextchar;									   									   
extern string ident_string;			//标识符							   							
extern string const_string;			//字符串常量
extern int digits;					//数字常量
extern int ch;						//字符常量
extern int char_index;								   
extern int line_num;
extern int last_num;

extern int str_const_count;
extern string str_const[];

extern int table_num;
extern vector<SymbolTable::GTABLE> func_table;
extern vector<string> func_name_list;

extern int print;
extern int is_int;//0:char, 1:int
extern int has_return;

extern int print_midcode;
extern int num_index;
extern int label_index;
extern int midcode_index;
extern vector<Mid::MidCode> midcode;
extern vector<Mid::MidCode> midcode_opt1;

extern int current_midcode_index;
extern int tarcode_index;
extern int opt_index;

extern vector<Target::TargetCode> target_code;
extern vector<Opt_Target::OptTarget> target_opt;
extern string current_func_name;
extern string opt_mid_current_func;

extern int ret_temp;
extern int max_num_index;
extern int total_max_num_index;

extern int current_S_index;//当前使用了几个，%8可得当前应该使用第几个
extern int current_T_index;

extern vector<string> S_reg; //索引是寄存器编号，string代表"局部变量"名
extern vector<string> T_reg; //索引是寄存器编号，string代表"中间变量#i,局部变量，全局变量"名
extern vector<string> A_reg;
extern vector<string> temp_reg;

extern vector<opt_mid::REPLACE> relations;
extern int need_save_A[10];//0代表无需保存
extern int need_save_S[10];//1代表需要保存
extern int t8t9;

extern vector<In_Line::InLineFunc> inline_func;
extern int begin_index;
extern vector<Mid::MidCode> opt_for;