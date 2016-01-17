#include<stdio.h>
int main()
{
int x = 10,y=10,i;
for(i=0;x>8;y=++i)
	printf("%d, %d ",x--,y);

printf("\n");
printf("x=%d,y=%d,i=%d\n",x,y,i);

return 0;
}
