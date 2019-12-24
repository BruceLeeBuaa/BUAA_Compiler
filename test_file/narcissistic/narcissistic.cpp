#include <iostream>

const int MAX_NUM = 128 ;

int factorial(int n){
   if(n<=1) return (1);

   return(n*factorial(n-1)) ;
}

int mod(int x, int y){
   x=x-x/y*y;

   return (x) ;
}

void swap(int x, int y){
    int temp;

    std::cout<<"x = "<<x<<std::endl;
    std::cout<<"y = "<<y<<std::endl;
    temp = x;
    x=y;
    y=temp;
    std::cout<<"SWAP x = "<<x<<std::endl;
    std::cout<<"SWAP y = "<<y<<std::endl;
}

int full_num(int n, int j, int a){
  return (n*100+j*10+a) ;
}

int flower_num(int n, int j, int a){
  return (n*n*n+j*j*j+a*a*a) ;
}

void complete_flower_num()
{
  int k[128];
  int i,j,n,s,x1,y;
  int m,k2,h,leap,x2;
  int a,b,c ;


  for(j=2;j< MAX_NUM ;j=j+1)
  {
    n = -1;
    s = j;
    for(i=1; i<j; i=i+1)
    {
      x1 = (j/i) * i ;
      if( mod(j,i) == 0 )
      {
        n = n + 1;
        s = s - i;
        if (n >= 128)
           std::cout<<"OVERFLOW!"<<std::endl;
        else
           k[n] = i;
      }
    }

    if(s==0)
    {
      std::cout<<"complete number: "<<j<<std::endl;
      for(i=0;i<=n;i=i+1)
        std::cout<<"  "<<k[i]<<std::endl;
      std::cout<<" "<<std::endl;
    }
  }

  std::cout<<"---------------------------------------------------------------"<<std::endl;
  std::cout<<"'water flower'number is:"<<std::endl;
  y = 0 ;
  for(i=100;i<100+MAX_NUM;i=i+1){
      n=i/100;
	  std::cout<<n<<std::endl;
      j=mod(i/10,10);
	  std::cout<<j<<std::endl;
      a=mod(i,10);
	  std::cout<<a<<std::endl;
	  std::cout<<"full_num = "<<full_num(n,j,a)<<std::endl;
	  std::cout<<"flower_num = "<<flower_num(n,j,a)<<std::endl;
      if(full_num(n,j,a)==flower_num(n, j, a)){
        k[y] = i ;
        y = y + 1 ;
		std::cout<<y<<std::endl;
      }
  }
  std::cout<<y<<std::endl;
  for(i = 0 ; i<y ; i=i+1){
    std::cout<<"  "<<k[i]<<std::endl;
  }
  std::cout<<" "<<std::endl;



  std::cout<<"---------------------------------------------------------------"<<std::endl;
  h = 0 ;
  leap = 1 ;
  for(m = 2 ; m <= MAX_NUM ; m=m+1)
  {
    k2 = m / 2;
    for(i=2; i<=k2; i=i+1){
      x2 = (m/i)*i ;
	  std::cout<<"mod = "<<mod(m, i)<<std::endl;
      if( mod(m,i) == 0)
      {
        leap=0;
      }
     }
    if(leap == 1)
    {
      std::cout<<" "<<m<<std::endl;
      h = h +1;

      x2 = (h/10)*10 ;
      if( x2 == h)
        std::cout<<" "<<std::endl;
     }
     leap=1;
  }

  std::cout<<"The total is "<<h<<std::endl;

}


int main(){
   int n ;

   n = factorial(5) ;
   std::cout<<"5 != "<<n<<std::endl;

   swap(5, 10 ) ;

   complete_flower_num() ;
}
