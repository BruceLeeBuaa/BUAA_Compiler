#include <iostream>

int mean(int n) {
	const int MAX_N = 10000;
	const int ERR = -1;
	int i, tmp, result;
	int num[10000];

	if (n > MAX_N) {
		std::cout<<"Too much input "<<n<<std::endl;
		return(ERR);
	}

	for (i = 0; i < n; i = i + 1) {
		std::cin>>tmp;
		num[i] = tmp;
	}

	result = 0;
	for (i = 0; i < n; i = i + 1) {
		result = result + i;
	}
	result = result / n;
	return(result);
}

void testmean() {
	int n;
	while (1) {
		std::cin>>n;
		if (n == 0) {
			return;
		}
		std::cout<<"please input numbers to calc mean"<<std::endl;
		n = mean(n);
		std::cout<<"mean value is "<<n<<std::endl;
	}
	return;
}

int isOdd(int num) {
	const int TWO = 2;
	int half;
	half = num / TWO;
	return (num - half * 2);
}

int isEven(int num) {
	const int TWO = 2;
	int half;
	half = num / TWO;
	return (1 - (num - half * 2));
}

int pick(int num, int odd) {
	if (num == 1) {
		return (1);
	}
	if (num <= 1) {
		std::cout<<"unexpected error happened"<<std::endl;
		return (-1);
	}
	if (odd) {
		return (pick(num / 2, isEven(num)) * 2);
	} else {
		return (pick((num + 1) / 2, isOdd(num)) * 2 - 1);
	}
}

int pick2(int num) {
	int i, j;
	i = 1;
	for (j = i * 2; j <= num; j = j + 0) {
		i = j;
		j = i * 2;
	}
	return ((num - i) * 2 + 1);
}

void testpick() {
	int base, exp, tmp;
	while (2) {
		std::cin>>base;
		std::cin>>exp;
		if (base == 0) {
			if (exp == 0) {
				return;
			}
		}
		for (tmp = 0; exp > 0; exp = exp - 1) {
			base = base * 10;
		}
		std::cout<<pick(base, 0)<<std::endl;
		std::cout<<pick2(base)<<std::endl;
	}
	return;
}

void test1() {
    int i,j,k;
    std::cout<<""<<std::endl;
    for(i=1;i<5;i=i+1) {
        for(j=1;j<5;j=j+1) {
            for (k=1;k<5;k=k+1) {
                if (i!=k) { 
					if (i!=j) {
						if (j!=k) {
							std::cout<<i<<std::endl;
							std::cout<<j<<std::endl;
							std::cout<<k<<std::endl;
						}
					}
                }
            }
        }
    }
}

int mod(int a, int b) {
	return (a - a / b * b);
}

void test2()
{
    int n, reversedInteger, remainder, originalInteger;
	while (1) {
		reversedInteger = 0;
 
    	std::cout<<"input an integer please:"<<std::endl;
		std::cin>>n;
		if (n == 0) {
			return;
		}
 
    	originalInteger = n;
 
    	while( n!=0 ) {
    	    remainder = mod(n, 10);
    	    reversedInteger = reversedInteger*10 + remainder;
    	    n = n / 10;
    	}
 
    	if (originalInteger == reversedInteger)
    	    std::cout<<"is palindrome"<<originalInteger<<std::endl;
    	else
    	    std::cout<<"not palindrome"<<originalInteger<<std::endl;
	}
}

void test3() {
    int n, i, flag;
	while (1) {
	    flag = 0;
 
        std::cout<<"input a positive integer please:"<<std::endl;
	    std::cin>>n;
		if (n == 0) {
			return;
		}
 
        for(i=2; i<=n/2; i = i + 1) {
            if(mod(n, i)==0) {
                flag=1;
                i = n;
            }
        }
 
        if (flag==0)
            std::cout<<"is prime"<<n<<std::endl;
        else
            std::cout<<"not prime"<<n<<std::endl;
	}
	return;
}

int main(){
	testmean();
	testpick();
	test1();
	test2();
	test3();
}
