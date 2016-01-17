#include<stdio.h>

typedef struct AAA
{
int a;
double b;
char c;
}AA;
void MyPrint(char sz[])
{
	printf("%d\n",sizeof(sz));
}

void MyPrint(int ar[])
{
	printf("%d\n",sizeof(ar));
}

void MyPrint(double ad[])
{
	printf("%d\n",sizeof(ad));
}

int main()
{
char sz[] = "Welcome to Huawei Test";
printf("%d\n",sizeof(sz));
MyPrint(sz);

int ia[] = {2,4,56,657,67,54,3,3};
printf("%d\n",sizeof(ia));
MyPrint(ia);

double da[] = {2.0,4,56,657,67,54,3,3};
printf("%d\n",sizeof(da));
MyPrint(da);

printf("\n\n\n%d,  %d,  %d,  %d,  %d, %d\n",sizeof(int *),sizeof(float*),sizeof(char*),sizeof(long*),sizeof(double*),sizeof(AAA*));
return 0;
}
