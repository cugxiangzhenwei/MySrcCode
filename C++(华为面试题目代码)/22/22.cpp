#include<stdio.h>
int main()
{
int i;
signed char count = 0;
for(i=0; ++i < 10;)
{
count++;
}
printf("%d\n",count);
return 0;
}
