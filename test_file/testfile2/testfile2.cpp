#include <iostream>

const int const_1 = 102435;
const int const_2 = -4325355;
const char const_3 = 'g';
int var_1, var_2;
int var_3[100];
int aa(int a, int bb, int cc) {
	int c, d, e, f, g;
	char ppg;
	c = d + (1 + 3) + f * (3 + 4423) + (324) + (g + e) + ppg;
	return (c);
}

int main(){
	int a, b, c, d, e, f, g;
	a = 3;
	b = 34;
	c = 3254;
	d = -35;
	e = 33247;
	f = -654;
	g = 33422;
	a = b+c+d+e+(f*g+(a+g-e+f)*d*(f-a+(g)));
	var_2 = const_2 + const_1 + var_1;
	std::cout<<a<<std::endl;
	std::cout<<var_2<<std::endl;
}
