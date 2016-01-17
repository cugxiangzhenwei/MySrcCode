#include<stdio.h>

#define G_NUM 32

#define ADD(a,b)  (((a>a))?(a):0)

typedef struct AAA
{
unsigned char a:1;
unsigned char b;
unsigned char c:2;
}aaa1;

typedef struct BBB
{
unsigned char a:1;
unsigned char b:2;
unsigned char c:6;
}bbb1;

int main()
{

int a = 3;
int b = 2;
printf("%d\n",ADD(--a,b--));
//printf("%d\n",ADD(9,7));
printf("%d, %d\n\n",a,b);

printf("%d\n",sizeof(aaa1));
printf("%d\n",sizeof(bbb1));

char sz[G_NUM -1];
int iSize = sizeof(sz);
printf("%d\n",iSize);
sz[sizeof(sz)-1]='\0';
for(int i=0;i< iSize;i++)
	printf("%c",sz[i]);

printf("\n");
}
