#include<stdio.h>
#define M(a,b,c) a*b+c
int main()
{
int a = 1;
int b = 2;
int c = 3;
printf("%d\n",M(a+b,b+c,c+a));
return 0;
}
