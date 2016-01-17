#include<stdio.h>
#define N 2
#define Y(n) (N+(N+1)*n)
int main()
{
int z =2*(N+2*Y(5));
printf("%d\n",z);
return 0;
}
