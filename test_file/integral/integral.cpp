#include <iostream>

const int PI=3;
const char PLUS='+';

int queue[5];
int counter;

int subPi(int x1,int x2){
	return(x1+x2-PI);
}

char shiftPlus(int s1,int s2){
	return(PLUS);
}

void printShift(int key,int s1,int s2){
	if(key >= 1) std::cout<<"def"<<shiftPlus(s1,s2)<<std::endl;
	else{std::cout<<"ghi"<<shiftPlus(s2,s1)<<std::endl;}
}

void printDefault(){
	char c;
	printShift(1,2,0);
	c = 'd';
	std::cout<<"abc"<<c<<std::endl;
	return;
}

int main(){
	int a;
	char c;
	if (a > a) {
		return 0;
		a = 1;
		a = 2;
		c = 'd';
	}
	printDefault();
	for(a=0;a>-5;a=a-1)
		queue[-a]=1;
	for(a=0;a<5;a=a+1)
		queue[a]=1;
	std::cin>>c;
}
