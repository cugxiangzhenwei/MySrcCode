#include<stdio.h>
int main(int argc,char *argv[])
{
  printf("Hello world,输入参数个数为：%d，参数依次为：\n",argc);
  for(int i=0;i< argc;i++)
	printf("%s ",argv[i]);

  printf("\n");
  return 0;
}
