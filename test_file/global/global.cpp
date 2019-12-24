#include <iostream>

const int const_int_1 = 12345679, const_int_2 = 0;
const int const_int_3 = -0, const_int_4 = -12345679;
const char const_char_1 = '9', const_char_2 = '_', const_char_3 = '+', const_char_4 = '*';
int global_int_1,global_int_2;
char global_char_1,global_char_2;
int global_array_1[100], global_array_2[100];
char global_array_3[100], global_array_4[100];
int num5;

void assignGlobal(int a, char b, int c, char d) {
	global_int_1 = a;
    global_char_1 = b;
    global_int_2 = c;
    global_char_2 = d;
	for (num5 = 0; num5 < 100; num5 = num5 + 1) {
		global_array_1[num5] = num5;
	}
	for (num5 = 100; 0 <= num5; num5 = num5 - 1) {
		global_array_2[num5] = 10000 - num5;
	}
	for (num5 = 100; num5 < 100 + 100; num5 = num5 + 1) {
		global_array_3[num5 - 100] = const_char_1;
	}
	for (num5 = 0; num5 < 100; num5 = num5 + 1) {
		global_array_4[num5] = const_char_2;
	}
    return;
}

void printGlobal() {
	std::cout<<"global_int_1 = "<<global_int_1<<std::endl;
	std::cout<<"global_int_2 = "<<global_int_2<<std::endl;
	std::cout<<"global_char_1 = "<<global_char_1<<std::endl;
	std::cout<<"global_char_2 = "<<global_char_2<<std::endl;

	std::cout<<"num5 = "<<num5<<std::endl;
	num5=9;
	for(num5=9;num5>=0;num5=num5-1) std::cout<<num5<<std::endl;

	std::cout<<"const_int_1 = "<<const_int_1<<std::endl;
	std::cout<<"const_int_2 = "<<const_int_2<<std::endl;
	std::cout<<"const_int_3 = "<<const_int_3<<std::endl;
	std::cout<<"const_int_4 = "<<const_int_4<<std::endl;
	std::cout<<"const_char_1 = "<<const_char_1<<std::endl;
	std::cout<<"const_char_2 = "<<const_char_2<<std::endl;
	std::cout<<"const_char_3 = "<<const_char_3<<std::endl;
	std::cout<<"const_char_4 = "<<const_char_4<<std::endl;

	for (num5 = 0; num5 < 100; num5 = num5 + 1) {
		std::cout<<global_array_1[num5]<<std::endl;
		std::cout<<global_array_2[num5]<<std::endl;
		std::cout<<global_array_3[num5]<<std::endl;
		std::cout<<global_array_4[num5]<<std::endl;
	}
}

int globalPara(int a, char b, int c, char d, int e, char f) {
	std::cout<<"a = "<<a<<std::endl;
	std::cout<<"b = "<<b<<std::endl;
	std::cout<<"c = "<<c<<std::endl;
	std::cout<<"d = "<<d<<std::endl;
	std::cout<<"e = "<<e<<std::endl;
	std::cout<<"f = "<<f<<std::endl;
	a = 1;
	b = 'c';
	c = 1;
	d = 'c';
	e = 1;
	f = 'c';
	std::cout<<"a = "<<a<<std::endl;
	std::cout<<"b = "<<b<<std::endl;
	std::cout<<"c = "<<c<<std::endl;
	std::cout<<"d = "<<d<<std::endl;
	std::cout<<"e = "<<e<<std::endl;
	std::cout<<"f = "<<f<<std::endl;
	return(num5);
}

void testglobal() {
	assignGlobal(const_int_1, const_char_1, const_int_4, const_char_4);
	printGlobal();
	globalPara(const_int_1, const_char_1, global_int_1, global_char_1, global_array_1[30], global_array_3[50]);
	printGlobal();
}

int main(){
	testglobal();
	return 0;
}
