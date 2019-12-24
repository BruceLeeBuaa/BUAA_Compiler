#pragma once
#include "Lexical.h"

namespace Reg {
	//s0-7 ��> ֻ��ֲ�������ѡȡÿ���������ô������ı���(������죬ѭ��������)
	//t0-7 ��> ������ĸ��м����#i,��ֲ���������ȫ�ֱ���
	//��#i ��> �Ƿ���t�Ĵ�����(-1, 0-7)
	//ȫ�ֱ��� ��> �Ƿ���t�Ĵ�����(-1, 0-7)
	//�ֲ����� ��> �ȿ��Ƿ���S�У��ٿ��Ƿ��ڼĴ�����(-1, 0-7)
	//FIFO �Ĵ����滻
	
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
	bool is_T_reg_ok(int num);//num�����ѯ�ļĴ����±�
	string use_T_assign(string name);
	void scanf_T(string name);
	//string use_S_assign(string name);

	//string temp_to_reg(string temp_name, int use_flag);
	//��reg�Ĵ�����ֵ�浽name���ڴ�,��ʱ��ֲ���ȫ��
	void save_reg(string name, string reg);
	void load_reg(string name, string reg);
	string get_ident_reg(string name);//�ҵ���ǰ�������identֵ��reg
	void save_ra_reg();
	void save_ra_opt();
	void write_back_T();
	//void write_back_S();
	void load_ra_reg();
	void load_ra_opt();
	//Ŀ������Ż�
	void save_S_reg();
	void load_S_reg();
	void save_A_reg();
	void load_A_reg();
	void save_T_reg();
	void load_T_reg();
	bool need_T_save(string name);
	string no_T_left(string name,int need_lw);//need_lwΪ1������Ҫ��ֵ
	void opt_save(string name, string reg);//�ѼĴ���ֵ���浽name��ַ
	void opt_load(string name, string reg);//��name��ַȡֵ�浽�Ĵ���
	string get_new_T_reg1(string name);
	string use_T_assign1(string name);
	void write_back_T1();
	void reset_temp_reg();
	string clear_relation(string name);
}