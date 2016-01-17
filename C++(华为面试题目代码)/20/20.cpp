#include<stdio.h>
int main(int argc,char *argv[])
{
  char *p;
  char a[10] ={1,2,3,4,5,6,7,8,9,10};
  p = a;
  printf(" %d, %d\n",*(p+2),*p+3);
  return 0;
}
