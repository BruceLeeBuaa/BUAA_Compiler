#include <iostream>

const int const_1 = 102435;
const int const_2 = -4325355;
const char const_3 = 'g';
int a, b, c, d;
int e[100];
char aa(int a, int bb, int cc) {
	int c, d, e, f, g;
	c = d + (a + b) + f * (3 + 4423) + (324) + (g + e);
	return ('c');
}

int main(){
	char ch_a;
	std::cin>>ch_a;
	std::cout<<"___this_is_b_string___"<<ch_a<<std::endl;
	std::cout<<"___this_is_c_string___"<<(ch_a)<<std::endl;
	std::cout<<"___this_is_d_string___"<<std::endl;
	std::cout<<1+2+3+aa(4,5,6)<<std::endl;
	std::cout<<'a'<<std::endl;
	std::cout<<'a'+1<<std::endl;
	std::cout<<('a')<<std::endl;
}
