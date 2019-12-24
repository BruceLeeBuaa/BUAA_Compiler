#pragma once
#include "Lexical.h"

namespace Reg {
	//s0-7 —> 只存局部变量，选取每个函数引用次数最多的变量(跨基本快，循环体里面)
	//t0-7 —> 存的是哪个中间变量#i,或局部变量，或全局变量
	//从#i —> 是否在t寄存器中(-1, 0-7)
	//全局变量 —> 是否在t寄存器中(-1, 0-7)
	//局部变量 —> 先看是否在S中，再看是否在寄存器中(-1, 0-7)
	//FIFO 寄存器替换
	
	bool in_S_reg(string name);
	bool in_A_reg(string name);
	string get_S_reg(string name);
	string get_A_reg(string name);
	//string get_new_S_reg(string name);
	void reset_S_reg();
	void reset_A_reg();
	bool in_T_reg(string name);
	string get_T_reg(string name);
	void reset_T_reg();
	string get_new_T_reg(string name);
	bool is_T_reg_ok(int num);//num代表查询的寄存器下标
	string use_T_assign(string name);
	void scanf_T(string name);
	//string use_S_assign(string name);

	//string temp_to_reg(string temp_name, int use_flag);
	//把reg寄存器的值存到name的内存,临时或局部或全局
	void save_reg(string name, string reg);
	void load_reg(string name, string reg);
	string get_ident_reg(string name);//找到当前存着这个ident值的reg
	void save_ra_reg();
	void save_ra_opt();
	void write_back_T();
	//void write_back_S();
	void load_ra_reg();
	void load_ra_opt();
	//目标代码优化
	void save_S_reg();
	void load_S_reg();
	void save_A_reg();
	void load_A_reg();
	void save_T_reg();
	void load_T_reg();
	bool need_T_save(string name);
	string no_T_left(string name,int need_lw);//need_lw为1代表需要有值
	void opt_save(string name, string reg);//把寄存器值保存到name地址
	void opt_load(string name, string reg);//从name地址取值存到寄存器
	string get_new_T_reg1(string name);
	string use_T_assign1(string name);
	void write_back_T1();
	void reset_temp_reg();
	string clear_relation(string name);
}