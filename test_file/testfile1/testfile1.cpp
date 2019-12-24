#include <iostream>

const int a = 10, b = 100;
const char c = 'd';
int var_a ,var_b ,var_c;
int A(int k, int a, char dg) {
	char arr_char[300];
	arr_char[33] = 'd';
	k = a + dg;
	dg = c;
	return (42+k);
}
int main(){
	int tmp_a, tmp_b, tmp_d;
	char tmp_c;
	int arr_a[300];
	tmp_c = 'd';
	std::cin>>var_a>>tmp_b>>tmp_c;
	tmp_d = var_a;
	var_a = a + 20 + c;
	var_b = var_a+ var_a - 2;
	var_c = b - var_b + var_a;
	var_c = 10 - 32 - 4543 + 343;
	var_b = 34325;
	std::cout<<var_a<<std::endl;
	std::cout<<var_b<<std::endl;
	std::cout<<"This is answer "<<var_c+var_b+tmp_d<<std::endl;
	std::cout<<tmp_c<<std::endl;
	std::cout<<c<<std::endl;
	std::cout<<a<<std::endl;
	arr_a[3] = 3;
	arr_a[8] = tmp_a + tmp_b;
	tmp_a=234;
	arr_a[tmp_a] = 543;
	std::cout<<arr_a[tmp_a]+3<<std::endl;
	tmp_a=324;
	tmp_b=-123;
	arr_a[tmp_a+tmp_b - 3] = tmp_a;
	std::cout<<arr_a[tmp_a+tmp_b-3]+3<<std::endl;
	return 0;
}
