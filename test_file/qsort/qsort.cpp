#include <iostream>

int sort[10];
int judge1(int a, int b, int c, int d)
{
    int ret;
    ret = 0;
    if(a < b)
        if(c >= d)
            ret = 1;
    return (ret);
}
int judge2(int a, int b, int c, int d)
{
    int ret;
    ret = 0;
    if(a < b)
        if(c <= d)
            ret = 1;
    return (ret);
}
void Qsort(int low, int high)
{
    int first, last, key;
    first = low;
    last = high;
    key = sort[first];
    if(low >= high)
        return;
    while(first < last)
    {
        while(judge1(first, last, sort[last], key) == 1)
            last = last - 1;
 
        sort[first] = sort[last];
 
        while(judge2(first, last, sort[first], key) == 1)
            first = first + 1;
         
        sort[last] = sort[first];    
    }
    sort[first] = key;
    Qsort(low, first-1);
    Qsort(first+1, high);
    return;
}
void testRecursion()
{
    int i;
    sort[0] = 5;
    sort[1] = 8;
    sort[2] = 3;
    sort[3] = 6;
    sort[4] = 4;
    sort[5] = 1;
    sort[6] = 2;
    sort[7] = 7;
    Qsort(0,7);
    i = 0;
    while(i < 8)
    {
        std::cout<<sort[i]<<std::endl;
        i = i + 1;
    }
}
int main(){
	testRecursion();
	return 0;
}