#include <iostream>

const int a = 10, b = 20, c = -10 ,d = 0;
const char aa = 'a', bb = 'b';
const int io_a = 14, io_b = 28;
const char io_c = 'c', io_d = 'd';
int e;
char f;
int g[5];
char h[5];
int io_e,io_f;
char io_g,io_h;
int array_a[4];

void func1() {
	std::cout<<"def"<<std::endl;
}

void func2(int a) {
	int b, c;
	std::cin>>a>> b>> c;
	std::cout<<a + b + c<<std::endl;
}

int func3(int a) {
	std::cout<<"in"<<a<<std::endl; std::cout<<"def"<<std::endl;
	return(a);
}

void integral() {
	const int a = 1;
	int b[10], c;
	std::cout<<"abc"<<std::endl;
	std::cout<<a + 1<<std::endl;
	std::cout<<1 + a<<std::endl;
	std::cout<<a - 1<<std::endl;
	std::cout<<1 - a<<std::endl;
	std::cout<<a * 2<<std::endl;
	std::cout<<a / 2<<std::endl;

	b[0] = 2;
	std::cout<<b[0]<<std::endl;
	c = b[0];
	std::cout<<c<<std::endl;
	c = a;
	std::cout<<c<<std::endl;

	if (a > 1) {
		std::cout<<1<<std::endl;
	} else {
		std::cout<<2<<std::endl;
	}

	if (a >= 1) {
		std::cout<<1<<std::endl;
	} else {
		std::cout<<2<<std::endl;
	}

	if (a < 1) {
		std::cout<<1<<std::endl;
	} else {
		std::cout<<2<<std::endl;
	}

	if (a <= 1) {
		std::cout<<1<<std::endl;
	} else {
		std::cout<<2<<std::endl;
	}

	if (a == 1) {
		std::cout<<1<<std::endl;
	} else {
		std::cout<<2<<std::endl;
	}

	func1();
	func2(8);
	std::cout<<func3(9)<<std::endl;
}

int funcif() {
	const int a = 10;
	const int b = 5;
	char c;
	c = '5';
	if(a>b){
		std::cout<<"a > b"<<std::endl;
	}else{
		std::cout<<"a <= b"<<std::endl;
	}
	if(a<b){

	}else{
		std::cout<<"a not less than b"<<std::endl;
	}

	if(a>=a){
		std::cout<<"1self yes"<<std::endl;
	}else{
		std::cout<<"1self no"<<std::endl;
	}

	if(b<=a){
		std::cout<<"2222 yes"<<std::endl;
	}else{
		std::cout<<"2222 no"<<std::endl;
	}

	if(a-b<b-a){
		std::cout<<"-aaa"<<std::endl;
	}else{
		std::cout<<"-bbb"<<std::endl;
	}


	if(-a>b){
		std::cout<<"-a > b"<<std::endl;
	}else{
		std::cout<<"-a <= b"<<std::endl;
	}
	if(b==c-'0'){
		std::cout<<"line21 y"<<std::endl;
	}else{
		std::cout<<"line23 n"<<std::endl;
	}
	return(a);
}

void funcloop(){
	int i;
	i = 10;
	while(i>=0){
		i=i-2;
		std::cout<<i<<std::endl;
	}
	while(i>10){
		std::cout<<"w r o n g"<<std::endl;
	}
	for(i=0;i<20;i=i+1){
		std::cout<<">>>"<<i<<std::endl;
	}
	for(i=20;i>=0;i=i-1){
		std::cout<<"***"<<i<<std::endl;
	}
	i=1;
	do{
		std::cout<<")))"<<i<<std::endl;
		i = i+2;
	}while(i<=1);
	std::cout<<"end"<<std::endl;
}

void funcio(){
	int aa[3],i,sum,tmp;
	io_e = 20;
	io_f = 30;
	io_g = 'g';
	io_h = 'h';
	std::cout<<"test1"<<std::endl;
	std::cout<<"test2"<<'g'+'h'<<std::endl;
	std::cout<<io_g+io_h<<std::endl;
	std::cout<<"test3"<<io_a*(io_b-(((io_c))))<<std::endl;
	std::cout<<io_a*(io_b-io_c)<<std::endl;
	std::cout<<"t e s t 4"<<std::endl;

	sum = 0;
	for(i=0;i<3;i=i+1){
		std::cin>>tmp;
		aa[i] = tmp;
		sum = sum+aa[i];
	}
	std::cout<<sum<<std::endl;

	std::cout<<io_e+io_f<<std::endl;
	std::cin>>io_e>>io_f;
	std::cout<<io_e<<std::endl;
	std::cout<<io_f<<std::endl;
	std::cin>>io_e>>io_f>>io_g>>io_h;
	std::cout<<io_e<<std::endl;
	std::cout<<io_f<<std::endl;
	std::cout<<io_g<<std::endl;
	std::cout<<io_h<<std::endl;
}

void funcarray() {
    int b[4];
    
    array_a[0] = 1;
    b[array_a[0]-1] = 2;

    array_a[b[array_a[0]-1]-1] = 3;
    b[array_a[b[array_a[0]-1]-1]-2] = 4;
    
    array_a[b[array_a[b[array_a[0]-1]-1]-2]-2] = 5;
    b[array_a[b[array_a[b[array_a[0]-1]-1]-2]-2]-3] = 6;
    
    array_a[b[array_a[b[array_a[b[array_a[0]-1]-1]-2]-2]-3]-3] = 7;
    b[array_a[b[array_a[b[array_a[b[array_a[0]-1]-1]-2]-2]-3]-3]-4] = 8;
    std::cout<<"array_a[0] = "<<array_a[0]<<std::endl;
    std::cout<<"array_a[1] = "<<array_a[1]<<std::endl;
    std::cout<<"array_a[2] = "<<array_a[2]<<std::endl;
    std::cout<<"array_a[3] = "<<array_a[3]<<std::endl;
    std::cout<<"b[0] = "<<b[0]<<std::endl;
    std::cout<<"b[1] = "<<b[1]<<std::endl;
    std::cout<<"b[2] = "<<b[2]<<std::endl;
    std::cout<<"b[3] = "<<b[3]<<std::endl;
    std::cout<<"e"<<std::endl;
}

int main(){
	integral();
	funcif();
	funcloop();
	funcio();
	funcarray();
}
