#include <iostream>

const int f = 45;
int a,b;
int rec1, rec2;

void inline1() {
	int a;
	std::cin>>a;
	if (a) {
		std::cout<<"inline1 "<<a<<std::endl;
	} else {
		std::cout<<"no input"<<std::endl;
	}
	do {
		std::cout<<"inline1 iter "<<a<<std::endl;
		a = a + 1;
	} while (a < 10);
}

void recur1() {
	rec1 = rec1 + 1;
	if (rec1 > 10) {
		std::cout<<"recur1 exit"<<std::endl;
		return;
		rec1 = rec1 + 10;
		std::cout<<"unreachable"<<std::endl;
		return;
		recur1();
		std::cin>>rec1;
	}
	std::cout<<"recur1 iter "<<rec1<<std::endl;
	recur1();
	std::cout<<"recur1 return"<<std::endl;
}

void recur2() {
	rec2 = rec2 - 1;
	if (rec2 < -20) {
		std::cout<<"recur2 exit"<<std::endl;
		return;
	}
	recur2();
	inline1();
	return;
	std::cout<<"unreachable"<<std::endl;
	recur2();
	inline1();
}

int inline2(int a, char b, int c) {
	a = 3;
	b = 'f';
	std::cout<<a<<std::endl;
	std::cout<<b<<std::endl;
	std::cout<<c<<std::endl;
	return(a);
}

void testinline() {
	rec1 = 5;
	rec2 = -11;
	inline1();
	recur1();
	recur2();
	std::cout<<inline2(rec1, 'd', rec2)<<std::endl;
}

int f0(int a){
    std::cout<<a+1<<std::endl;
    return (a+1);
}

void f1(int a){
    std::cout<<f0(f0(a))<<std::endl;
}

char cbase(char b){
    std::cout<<b<<std::endl;
    return ('z');
}

int ibase(int a){
    std::cout<<a<<std::endl;
    return(a+1);
}

void cv(int a, char b){
    std::cout<<ibase(ibase(a))<<std::endl;
    std::cout<<cbase(b)<<std::endl;
}

void testcall(){
    int a;
    std::cin>>a;
    std::cout<<f0(a)<<std::endl;
    f1(1);
    cv(6, 'd');
    cv(5, 'c');
}

void fun1(int p1,int p2, int p3, int p4, char p5, int p6,int p7, int p8, int p9, int p10, int p11, int p12, int p13)
{
	const int y = 987;
	const char x = 'I';
	char c[10];
	int s1;
	int s2;
	int s3;
	int s4;
	int s5;
	int s6;
	int s7;
	int s8;
	int s9;
	int s10;
	c[0] = 'Q';
	c[2] = 'F';
	c[9] = '-';
	s1 = 12;
	s2 = 123;
	s3 = 1234;
	s4 = 12345;
	s5 = 123456;
	s6 = 1234567;
	s7 = 1234567;
	s8 = 12345467;
	s9 = 12343567;
	s10 = 12234567;
	std::cout<<"s1: "<<s1<<std::endl;
	std::cout<<"s2: "<<s2<<std::endl;
	std::cout<<"s3: "<<s3<<std::endl;
	std::cout<<"s4: "<<s4<<std::endl;
	std::cout<<"s5: "<<s5<<std::endl;
	std::cout<<"s6: "<<s6<<std::endl;
	std::cout<<"s7: "<<s7<<std::endl;
	std::cout<<"s8: "<<s8<<std::endl;
	std::cout<<"s9: "<<s9<<std::endl;
	std::cout<<"s10: "<<s10<<std::endl;
	std::cout<<"s6: "<<s6<<std::endl;
	std::cout<<"c0: "<<c[0]<<std::endl;
	std::cout<<"c2: "<<c[2]<<std::endl;
	std::cout<<"c9: "<<c[9]<<std::endl;
	std::cout<<p1<<std::endl;
	std::cout<<p2<<std::endl;
	std::cout<<p3<<std::endl;
	std::cout<<p4<<std::endl;
	std::cout<<p5<<std::endl;
	std::cout<<p6<<std::endl;
	std::cout<<p7<<std::endl;
	std::cout<<p8<<std::endl;
	std::cout<<p9<<std::endl;
	std::cout<<p10<<std::endl;
	std::cout<<p11<<std::endl;
	std::cout<<p12<<std::endl;
	std::cout<<p13<<std::endl;
	std::cout<<y<<std::endl;
	std::cout<<x<<std::endl;
	std::cout<<"I am fun1 with a parameter"<<std::endl;
}

void testpara()
{
	char ch[10];
	char c;
	char c1;
	int array[20];
	c = 'e';
	ch[9] = c;
	c1 = ch[9];
	a = 2;
	b = 4;
	array[2] = a + 78;
	fun1(5,7,1,3,'9',1234,321,345,1231,5345,1321,array[2],123);
	fun1(5,7,1,3,'8',1234,321,345,1231,5345,1321,array[2],123);
	std::cout<<array[2]<<std::endl;
	std::cout<<c1<<std::endl;
	std::cout<<"hello world"<<std::endl;
}


int main(){
	testinline();
	testcall();
	testpara();
}
