#include <iostream>

int array[10],symbol[10];
int n;
void permutation(int index)
{
	int i;
	if(index >= n)
	{
		for(i = 0;i < n;i = i + 1)
		{
			std::cout<<" "<<array[i]<<std::endl;
		}
	}
	else
	{
		for(i = n - 1;i >= 0;i = i - 1)
		{
			if(symbol[i]==0)
			{
				array[index] = i + 1;
				symbol[i] = 1;
				permutation(index + 1);
				symbol[i] = 0;
			}
			
		}
	}
}
int main(){
	std::cin>>n;
	permutation(0);
	permutation(6);
}
