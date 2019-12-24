#include <iostream>

const int num1=10, num2=1, num3=+10, num4=-19;
const char char1='a', char2='b';
const int chAr2=-109;
const int hide = 16;
const char char_a = 'a', b = 'b';
const char test1 = 'j';
int num5, num6, array1[9];
char char3, char4, array2[10], testresult[10];
char test2;
int test3;
int int_array1[2];
char char_array1[2], char_array2[2];
int global[4];
int global_int;
int a;
int c;
char d;
int i,j,k;

void testchar(){
	const char test1 = 'a';
	const char test3 = 'c';
	char test4;
    std::cin>>test2>> test4;
    std::cout<<"test1:"<<test1<<std::endl;
    std::cout<<"test2:"<<test2<<std::endl;
    std::cout<<"test3:"<<test3<<std::endl;
    std::cout<<"test4:"<<test4<<std::endl;
    std::cout<<"test1 in ascii:"<<test1+0<<std::endl;
    std::cout<<"test2 in ascii:"<<test2+0<<std::endl;
    std::cout<<"test3 in ascii:"<<test3+0<<std::endl;
    std::cout<<"test4 in ascii:"<<test4+0<<std::endl;
}

void testarray1() {
	int int_array2[3];
	char char_array2[3];
	int i1, i2, i3, i4, i5;
	char c1, c2, c3, c4, c5;
	std::cin>>c1>> i1>> c2>> i2>> c3>> i3>> c4>> i4>> c5>> i5;
	int_array1[0] = i1;
	int_array1[1] = i2;
	int_array2[0] = i3;
	int_array2[1] = i4;
	int_array2[2] = i5;
	char_array1[0] = c1;
	char_array1[1] = c2;
	char_array2[0] = c3;
	char_array2[1] = c4;
	char_array2[2] = c5;
	std::cout<<"sum of int array1:"<<int_array1[0]+int_array1[1]<<std::endl;
	std::cout<<"sum of int array2:"<<int_array2[0]+int_array2[1]+int_array2[2]<<std::endl;
	std::cout<<"char1:"<<char_array1[0]<<std::endl;
	std::cout<<"char2:"<<char_array1[1]<<std::endl;
	std::cout<<"char3:"<<char_array2[0]<<std::endl;
	std::cout<<"char4:"<<char_array2[1]<<std::endl;
	std::cout<<"char5:"<<char_array2[2]<<std::endl;
}

void teststack() {
	int i0, i1, i2, i3, i4, i5, i6, i7, i8, i9;
	std::cin>>i0>> i1>> i2>> i3>> i4>> i5>> i6>> i7>> i8>> i9;
	std::cout<<"remain:"<<i2+i3+i4+i5+i6+i7+i8+i9<<std::endl;
	std::cout<<"not remain:"<<i0+i1<<std::endl;
}

void testbranch() {
	int in, num, n;
	int doublevar, triple, square;
	std::cin>>num>> in;
	while (in) {
		if (in < 2) {
			doublevar = num * 2;
		} else {
			doublevar = in * 2;

		}
		std::cout<<"doublevar:"<<doublevar<<std::endl;
		std::cout<<"while finish"<<std::endl;
		std::cin>>in;
	}
	std::cin>>in;
	for (n = 0;  in > 0; n = n + 1) {
		if (in == 1) {
			std::cout<<"numb double:"<<num * 2<<std::endl;
		} else if (in == 2) {
			triple = num * 3;
			std::cout<<"triple:"<<triple<<std::endl;
		} else {
			square = num * num;
			std::cout<<"square:"<<square<<std::endl;
		}
		std::cin>>in;
	}
	std::cout<<"n:"<<n<<std::endl;
	std::cout<<"doublevar:"<<doublevar<<std::endl;
	std::cout<<"triple:"<<triple<<std::endl;
	std::cout<<"square:"<<square<<std::endl;
	std::cout<<"sum:"<<doublevar + triple + square<<std::endl;
}

void testbubble() {
	int local[5];
	int tmp, i, j;
	std::cout<<"input globals"<<std::endl;
	for (i = 0; i < 4; i = i + 1) {
		std::cin>>tmp;
		global[i] = tmp;
	}
	std::cout<<"input locals"<<std::endl;
	for (i = 0; i < 5; i = i + 1) {
		std::cin>>tmp;
		local[i] = tmp;
	}
	for (i = 0; i < 4; i = i + 1) {
		for (j = 0; j < 4 - i - 1; j = j + 1) {
			if (global[j] > global[j+1]) {
				tmp = global[j];
				global[j] = global[j+1];
				global[j+1] = tmp;
			}
		}
	}
	for (i = 0; i < 5; i = i + 1) {
		for (j = 0; j < 5 - i - 1; j = j + 1) {
			if (local[j] > local[j+1]) {
				tmp = local[j];
				local[j] = local[j+1];
				local[j+1] = tmp;
			}
		}
	}
	std::cout<<"global"<<std::endl;
	for (i = 0; i < 4; i = i + 1) {
		std::cout<<global[i]<<std::endl;
	}
	std::cout<<"local"<<std::endl;
	for (i = 0; i < 5; i = i + 1) {
		std::cout<<local[i]<<std::endl;
	}
}

void increase() {
	global_int = global_int + 1;
}

int doublevar(int n) {
	int tmp;
	tmp = n * 2;
	return (tmp);
}

char fa() {
	return ('a');
}

void testsimplefunc() {
	int tmp;
	global_int = 0;
	increase();
	std::cout<<"new int:"<<global_int<<std::endl;
	tmp = 1;
	std::cout<<"doublevar:"<<doublevar(tmp)<<std::endl;
	std::cout<<"char:"<<fa()<<std::endl;
	std::cout<<"tmp:"<<tmp<<std::endl;
}

int sum(int a, int b) {
	return (a+b);
}

void testembed() {
	int a, b, c;
	std::cin>>a>>b>>c;
	std::cout<<"sum:"<<sum(sum(a,b),c)<<std::endl;
	std::cout<<"sum:"<<sum(a,sum(b,c))<<std::endl;
}

int product(int n) {
	if (n <= 1) {
		return (n);
	} 
	return (n * product(n - 1));
}

int fibo(int n) {
	if (n <= 2) {
		return (1);
	} else {
		return (fibo(n - 1) + fibo(n - 2));
	}
}

void testrec() {
	int n;
	std::cin>>n;
	do {
		std::cout<<"pro:"<<product(n)<<std::endl;
		std::cout<<"fibo:"<<fibo(n)<<std::endl;
		std::cin>>n;
	} while(n);
}

int test(int a, int b){
    std::cout<<"******"<<std::endl;
    std::cout<<i<<std::endl;
    std::cout<<j<<std::endl;
    std::cout<<k<<std::endl;
    if(i>=j){
    	    for(i=0;i<10;i=i+1){
    	        if(i!=3){
    	        std::cout<<i<<std::endl;
    	        }else{
    	            for(k=0;k<3;k=k+1){
    	            std::cout<<"333333"<<std::endl;
    	            }
    	        }
    	    }
    	}else{
    	    for(i=9;i>=0;i=i-1){
    	        std::cout<<i<<std::endl;
    	    }
    	}
    return(-(a+b)-b);
}

void testcontrol(){
	i = 10;
	j = 20;
	k = 30;
	if(i<j){
	    for(i=0;i<10;i=i+1){
	        if(i!=3){
	        std::cout<<i<<std::endl;
	        }else{
	            for(k=0;k<3;k=k+1){
	            std::cout<<"333333"<<std::endl;
	            }
	        }
	    }
	}else{
	    for(i=9;i>=0;i=i-1){
	        std::cout<<i<<std::endl;
	    }
	}
	std::cout<<test(1,2)<<std::endl;
}

int return1() {
    return (1);
}

int factorial(int n) {
    if(n==1)return (n);
    else return(n*factorial(n-1));
}

int testfor(int a, char b, int c, int d, int f, char e) {
    int i;
    a=9;
    c=0;
    for(i=100;i<10;i=i-1)a=a+1;
    for(i=0;i<10;i=i+1)c=c+1;
    if(a==10){
        if(c==10){
            return (1);
		}
    }
    return (0);   
}

int testwhile() {
    int num1, num2, num3, num4;
    num1=0;
    num2=10;
    num3=-1;
    num4=0;
    while(num1!=10){
        num2=num2-1;
        num1=num1+1;
    }
    while(num3>0){
        num4=num4+10;
        num3=num3-10;
    }
    if(num2==0){
        if(num4==0)
            return (1);
		else
			return (-1);
    }
    else return (0);
}

int testexpression() {
    int a,c;
    char b;
    a=10;
    b='a';
    array1[0]=100;
    array1[1]=99;
    c=(a-b+'a')*return1()-array1[0]/10+1;
    ;;;
    if(array1[c]==99)return (1);
    return (0);
}

int testarray() {
    int a,b,c;
    a=1;
    b=2;
    c=3;
    array1[0]=1;
    array1[4]=array1[0];
    array1[c+a*(b+return1())-array1[0]*2]=10;
    if(array1[4]==10)return (1);
    return (0);
}

void testprintf(int flag) {
    if(flag<=0)std::cout<<"this is a string, !#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}~|"<<std::endl;
    else if(flag>=2){
            std::cout<<'a'<<std::endl;
            std::cout<<'2'<<std::endl;
            std::cout<<'*'<<std::endl;
         }
         else std::cout<<"this is a string "<<num1-5<<std::endl;
}

char testfactorial() {
    if(factorial(1)==1)
        if(factorial(3)==6)
            return ('T');
    return ('F');
}

void testfunc1() {
    int i, flag;
    num5=9;
    num6=8;
    char3='a';
    char4='b';
    std::cin>>flag;
    for(i=0;i<10;i=i+1)testresult[i]='T';
    for(num5=9;num5>=0;num5=num5-1)array2[num5]=testresult[num5];
    for(num6=0;num6!=8;num6=num6+1)array1[num6]=0;
    if(testfor(num5, char1, num6, chAr2, 10, char4)==1)testresult[0]='T';
    else testresult[0]='F';
    if(testwhile())testresult[1]='T';
    else testresult[1]='F';
    if((testfactorial())==('T'))testresult[2]='T';
    else testresult[2]='F';
    if(testexpression())testresult[3]='T';
    else testresult[3]='F';
    if(testarray())testresult[4]='T';
    else testresult[4]='F';
    testprintf(flag);
    testprintf(-1);
    testprintf(0);
    testprintf(1);
    testprintf(2);
    testprintf(3);
    for(i=0;i<5;i=i+1)std::cout<<testresult[i]<<std::endl;
}

void v0(int char_a, char b){
	std::cout<<char_a<<std::endl;
	std::cout<<b<<std::endl;
}

int i0(int char_a, char b){
	return(char_a*(b-'0'));
}

char c0(char char_a, char b, char c){
	std::cout<<char_a+b-2*'0'<<std::endl;
	return(c);
}

void testfunc2() {
	const char char_a = '4';
	const char b = 'z';
	int c;
	char d;
	v0(-64, b);
	c = i0(4, char_a);
	std::cout<<c<<std::endl;
	d = c0('4','5','v');
	std::cout<<d<<std::endl;
	return;
}

int main(){
	testchar();
	testarray1();
	teststack();
	testbranch();
	testbubble();
	testsimplefunc();
	testembed();
	testrec();
	testcontrol();
	testfunc1();
	testfunc2();
}
