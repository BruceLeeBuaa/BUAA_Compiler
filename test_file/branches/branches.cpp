#include <iostream>

int distance(int x, int y)
{
	int res;
	res = x - y;
	if (res < 0)
	{
		res = - res;
	}
	return (res);
}

int ads(int x)
{
	if (x < 0)
		return (-x);
	else
		return (x);
}

void compare(int a, int b, int c)
{
	int edage1, edage2, edage3;
	
	edage1 = distance(a, b);
	edage2 = distance(b, c);
	edage3 = distance(a, c);
	
	if (edage1 + edage3 >= edage2)
	{
		std::cout<<"assumption1 confirmed 1"<<std::endl;
		if (edage1 + edage3 > edage2){
			std::cout<<"situation 1"<<std::endl;
		}
		else
		{
			std::cout<<"no situation 1"<<std::endl;
		} 
		if (edage1 + edage3 == edage2)
		{
			std::cout<<"situation 2"<<std::endl;
		}
		else
		{
			std::cout<<"no situation 2"<<std::endl;
		}
	}
	
	if (edage1 <= edage2 + edage3)
	{
		std::cout<<"assumption1 confirmed 2"<<std::endl;
		if (edage1 != edage2 + edage3){
			std::cout<<"situation 1"<<std::endl;
		}
		else
		{
			std::cout<<"no situation 1"<<std::endl;
		}
		
		if (edage2 + edage3 == edage1){
			std::cout<<"situation 2"<<std::endl;
		}
		else
		{
			std::cout<<"no situation 2"<<std::endl;
		}
	}
	
	if (edage1 - edage2 <= edage3)
	{
		std::cout<<"assumption2 confirmed 1"<<std::endl;
		if (edage3 > edage1 - edage2){
			std::cout<<"situation 1"<<std::endl;
		}
		else
		{
			std::cout<<"no situation 1"<<std::endl;
		}
		
		if (edage1 - edage2 == edage1){
			std::cout<<"situation 2"<<std::endl;
		}
		else
		{
			std::cout<<"no situation 2"<<std::endl;
		}
	}
	
	if (edage1 >= edage3 - edage2)
	{
		std::cout<<"assumption2 confirmed 2"<<std::endl;
		if (edage2 + edage3 < edage1){
			std::cout<<"situation 1"<<std::endl;
		}
		else
		{
			std::cout<<"no situation 1"<<std::endl;
		}
		
		if (edage2 - edage3 == edage1){
			std::cout<<"situation 2"<<std::endl;
		}
		else
		{
			std::cout<<"no situation 2"<<std::endl;
		}
	}
}

void compare_to_num(int a, int b, int c)
{
	if (a < 10)
	{
		std::cout<<"a < 10"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
	
	if (85 < b)
	{
		std::cout<<"85 < b"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
	
	if (b <= 5)
	{
		std::cout<<"b <= 5"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
	
	if (1 <= b)
	{
		std::cout<<"1 <= b"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
	
	if (8 == c)
	{
		std::cout<<"8 == c"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
	
	if (a == 7)
	{
		std::cout<<"a == 7"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
	
	if(9 != b)
	{
		std::cout<<"9 != b"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
	
	if (a + c != 15)
	{
		std::cout<<"a + c != 15"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
	
	if (-1 > a - c)
	{
		std::cout<<"-1 > a - c"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
	
	if (c - a > -8)
	{
		std::cout<<"c - a > -8"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
	
	if (56 >= a + c + b)
	{
		std::cout<<"56 >= a + c + b"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
	
	if (a + b - c >= 5)
	{
		std::cout<<"a + b - c >= 5"<<std::endl;
	}
	else
	{
		std::cout<<" "<<std::endl;
	}
}

int main(){
	int a, b, c;
	while (1) {
		std::cin>>a;
		std::cin>>b;
		std::cin>>c;
		if (a == 0) {
			if (b == 0) {
				if (c == 0) {
					return 0;
				}
			}
		}
		compare(a, b, c);
		compare_to_num(a, b, c);
	}
	return 0;
}
