#include "inline.h"
#include "Globalvar.h"
#include "assert.h"
bool in_vector(vector<string> v, string element)
{
	vector<string> temp = v;
	for (int i = 0; i < temp.size(); i++) {
		if (temp[i] == element) {
			return true;
		}
	}
	return false;
}
void In_Line::add_func()
{
	InLineFunc new_func;
	new_func.func_name = current_func_name;

	vector<Mid::MidCode> temp_mid;
	vector<Mid::MidCode> func_mid;
	vector<string> paralist;
	temp_mid.push_back(midcode[current_midcode_index]);
	current_midcode_index++;
	while (midcode[current_midcode_index].op != Mid::func_declare) {
		temp_mid.push_back(midcode[current_midcode_index]);
		current_midcode_index++;
	}
	int n = temp_mid.size();
	if (temp_mid[n - 2].op == Mid::ret) {//除掉两个ret的情况
		temp_mid.pop_back();
	}
	new_func.code = temp_mid;//代表原始中间代码
	temp_mid.erase(temp_mid.begin());//删除函数名那条中间代码
	bool has_ret;
	if (temp_mid.back().op_num1 == "") {
		has_ret = false;
	}
	else {
		has_ret = true;
	}

	bool is_inline = true;
	for (int i = 0; i < temp_mid.size() - 1; i++) {
		if (temp_mid[i].op == Mid::para_declare) {
			paralist.push_back(temp_mid[i].op_num1);
		}
		else if (temp_mid[i].op == Mid::var_declare || temp_mid[i].op == Mid::array_declare) {
			is_inline = false;
			break;
		}
		else if (temp_mid[i].op == Mid::call) {
			is_inline = false;
			break;
		}
		else if (temp_mid[i].op == Mid::set_label || temp_mid[i].op == Mid::_goto || temp_mid[i].op == Mid::_beq
			|| temp_mid[i].op == Mid::_bne || temp_mid[i].op == Mid::_bgt || temp_mid[i].op == Mid::_bge
			|| temp_mid[i].op == Mid::_blt || temp_mid[i].op == Mid::_ble || temp_mid[i].op == Mid::_beqz || temp_mid[i].op == Mid::_bnez) {
			is_inline = false;
			break;
		}
		else if (temp_mid[i].op == Mid::ret) {
			is_inline = false;
			break;
		}
		else {
			func_mid.push_back(temp_mid[i]);
		}
	}
	func_mid.push_back(temp_mid.back());//把return语句加入

	for (int j = 0; j < paralist.size(); j++) {
		string prev = "";
		vector<string> huan;
		for (int i = 0; i < func_mid.size(); i++) {
			Mid::MidCode one_mid = func_mid[i];
			if (in_vector(huan, one_mid.op_num1)) {
				one_mid.op_num1 = prev;
			}
			if (in_vector(huan, one_mid.op_num2)) {
				one_mid.op_num2 = prev;
			}
			if (in_vector(huan, one_mid.result)) {
				one_mid.result = prev;
			}
			if (one_mid.op == Mid::my_assign) {
				if (one_mid.op_num1 == paralist[j]) {
					if (prev == "") {
						prev = one_mid.result;
					}
					else {
						one_mid.op = Mid::none;
						huan.push_back(one_mid.result);
					}
				}
			}
			else if (one_mid.op == Mid::assign) {
				if (one_mid.op_num1 == paralist[j]) {
					prev = "";
					for (int k = 0; k < huan.size(); k++) {
						huan[k].clear();
					}
				}
			}
			func_mid[i] = one_mid;
		}
	}

	if (is_inline) {
		new_func.inline_code = func_mid;//代表如果要进行内联的话的中间代码
		new_func.has_ret = has_ret;
		new_func.is_inline = true;
		new_func.max_index = SymbolTable::get_max_index();
		new_func.para_list = paralist;
		inline_func.push_back(new_func);
	}
	current_midcode_index--;
}

int get_index(vector<string> v, string element)
{
	vector<string> temp = v;
	int index = -1;
	for (int i = 0; i < temp.size(); i++) {
		if (temp[i] == element) {
			index = i;
			break;
		}
	}
	return index;
}

void In_Line::push_case()
{
	vector<string> push_list;
	int temp_index = current_midcode_index;
	while (midcode[current_midcode_index].op == Mid::push) {
		push_list.push_back(midcode[current_midcode_index].op_num1);
		current_midcode_index++;
		//midcode.erase(midcode.begin() + current_midcode_index);
		//midcode_index--;
	}

	int index = -1;
	for (int i = 0; i < inline_func.size(); i++) {
		if (inline_func[i].func_name == midcode[current_midcode_index].op_num1) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		return;
	}
	midcode.erase(midcode.begin() + temp_index, midcode.begin() + current_midcode_index);
	midcode_index -= push_list.size();
	current_midcode_index = temp_index;

	InLineFunc func = inline_func[index];
	vector<Mid::MidCode> mid_1 = func.inline_code;
	int inline_index = func.max_index;//所内联的函数的最大标签
	int current_index = SymbolTable::get_max_index();//调用者函数的最大标签

	vector<string> para_list = func.para_list;
	//assert(para_list.size() == push_list.size());
	string ret;
	for (int i = 0; i < mid_1.size(); i++) {
		Mid::MidCode one_mid = mid_1[i];
		if (one_mid.op_num1[0] == '#') {
			int num1 = Opt_Target::get_temp_index(one_mid.op_num1);
			one_mid.op_num1 = '#' + to_string(num1 + current_index);
		}
		if (one_mid.op_num2[0] == '#') {
			int num2 = Opt_Target::get_temp_index(one_mid.op_num2);
			one_mid.op_num2 = '#' + to_string(num2 + current_index);
		}
		if (one_mid.result[0] == '#') {
			int num3 = Opt_Target::get_temp_index(one_mid.result);
			one_mid.result = '#' + to_string(num3 + current_index);
		}
		mid_1[i] = one_mid;
	}
	vector<string> replace(100);//被push_list替换的中间变量
	for (int i = 0; i < mid_1.size(); i++) {
		Mid::MidCode one_mid = mid_1[i];
		if (one_mid.op == Mid::my_assign) {
			if (in_vector(para_list, one_mid.op_num1)) {
				replace[get_index(para_list, one_mid.op_num1)] = one_mid.result;
				one_mid.op = Mid::none;
			}
		}
		else if (one_mid.op == Mid::assign) {
			if (in_vector(para_list, one_mid.op_num1)) {
				push_list[get_index(para_list, one_mid.op_num1)] = one_mid.op_num2;
				one_mid.op = Mid::none;
			}
		}
		else {
			if (one_mid.op_num1 != "" && in_vector(replace, one_mid.op_num1)) {
				one_mid.op_num1 = push_list[get_index(replace, one_mid.op_num1)];
			}
			if (one_mid.op_num2 != "" && in_vector(replace, one_mid.op_num2)) {
				one_mid.op_num2 = push_list[get_index(replace, one_mid.op_num2)];
			}
			if (one_mid.result != "" && in_vector(replace, one_mid.result)) {
				one_mid.result = push_list[get_index(replace, one_mid.result)];
			}
		}
		mid_1[i] = one_mid;
	}
	ret = mid_1.back().op_num1;
	string replacetemp = midcode[current_midcode_index + 1].result;
	if (func.has_ret) {
		midcode.erase(midcode.begin() + current_midcode_index, midcode.begin() + current_midcode_index + 2);//删除call和返回值接收
		midcode_index -= 2;
		int index = current_midcode_index;
		while (midcode[current_midcode_index].op != Mid::ret) {
			if (midcode[current_midcode_index].op_num1 == replacetemp) {
				midcode[current_midcode_index].op_num1 = ret;
			}
			if (midcode[current_midcode_index].op_num2 == replacetemp) {
				midcode[current_midcode_index].op_num2 = ret;
			}
			if (midcode[current_midcode_index].result == replacetemp) {
				midcode[current_midcode_index].result = ret;
			}
			current_midcode_index++;
		}
		current_midcode_index = index;
		mid_1.pop_back();
		SymbolTable::add_current_temp_size(inline_index);
		midcode.insert(midcode.begin() + current_midcode_index, mid_1.begin(), mid_1.end());
		midcode_index += mid_1.size();
	}
	else {
		midcode.erase(midcode.begin() + current_midcode_index);//删除call
		midcode_index -= 1;
		mid_1.pop_back();//除去return语句
		SymbolTable::add_current_temp_size(inline_index);
		midcode.insert(midcode.begin() + current_midcode_index, mid_1.begin(), mid_1.end());//内联
		midcode_index += mid_1.size();
		current_midcode_index--;//防止空函数
	}
}

void In_Line::call_case()
{
	int index = -1;
	for (int i = 0; i < inline_func.size(); i++) {
		if (inline_func[i].func_name == midcode[current_midcode_index].op_num1) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		return;
	}
	
	InLineFunc func = inline_func[index];
	vector<Mid::MidCode> mid_1 = func.inline_code;
	int inline_index = func.max_index;//所内联的函数的最大标签
	int current_index = SymbolTable::get_max_index();//调用者函数的最大标签
	string ret;
	for (int i = 0; i < mid_1.size(); i++) {
		Mid::MidCode one_mid = mid_1[i];
		if (one_mid.op_num1[0] == '#') {
			int num1 = Opt_Target::get_temp_index(one_mid.op_num1);
			one_mid.op_num1 = '#' + to_string(num1 + current_index);
		}
		if (one_mid.op_num2[0] == '#') {
			int num2 = Opt_Target::get_temp_index(one_mid.op_num2);
			one_mid.op_num2 = '#' + to_string(num2 + current_index);
		}
		if (one_mid.result[0] == '#') {
			int num3 = Opt_Target::get_temp_index(one_mid.result);
			one_mid.result = '#' + to_string(num3 + current_index);
		}
		if (one_mid.op == Mid::ret) {
			one_mid.op = Mid::none;
			ret = one_mid.op_num1;
		}
		mid_1[i] = one_mid;
	}
	string replacetemp = midcode[current_midcode_index + 1].result;
	if (func.has_ret) {
		midcode.erase(midcode.begin() + current_midcode_index, midcode.begin() + current_midcode_index + 2);//删除call和返回值接收
		midcode_index -= 2;
		int index = current_midcode_index;
		while (midcode[current_midcode_index].op != Mid::ret) {
			if (midcode[current_midcode_index].op_num1 == replacetemp) {
				midcode[current_midcode_index].op_num1 = ret;
			}
			if (midcode[current_midcode_index].op_num2 == replacetemp) {
				midcode[current_midcode_index].op_num2 = ret;
			}
			if (midcode[current_midcode_index].result == replacetemp) {
				midcode[current_midcode_index].result = ret;
			}
			current_midcode_index++;
		}
		current_midcode_index = index;
		mid_1.pop_back();
		SymbolTable::add_current_temp_size(inline_index);
		midcode.insert(midcode.begin() + current_midcode_index, mid_1.begin(), mid_1.end());
		midcode_index += mid_1.size();
		current_midcode_index--;
	}
	else {
		midcode.erase(midcode.begin() + current_midcode_index);//删除call
		midcode_index -= 1;
		mid_1.pop_back();//除去return语句
		SymbolTable::add_current_temp_size(inline_index);
		midcode.insert(midcode.begin()+current_midcode_index,mid_1.begin(),mid_1.end());//内联
		midcode_index += mid_1.size();
		current_midcode_index--;//防止空函数
	}
}