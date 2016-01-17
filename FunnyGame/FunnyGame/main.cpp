#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
FILE *fp;
int vfpf(char *fmt, ...)

{

	va_list argptr;

	int cnt;

	va_start(argptr, fmt);

	cnt = vfprintf(fp, fmt, argptr);

	va_end(argptr);

	return(cnt);

}

int Fun(int iTotal,int iNum)
{
	if (iTotal<iNum)
		return 0;
	else
	{
		int iCount = iTotal / iNum; //计算直接换取的个数
		int iOther = iTotal % iNum; //计算换取时剩余的个数
		return iCount + Fun(iOther + iCount,iNum); //进行下一次换桃
	}
}
/**
*@brief 计算桃囫换桃，最多可吃桃子的个数
*@param iTotal
*@param iTotal
*@return 返回最终吃桃子的总个数
*/
int CalcResult(int iTotal ,int iNum)
{
	int iCount = Fun(iTotal,iNum); //计算换取的总个数
	return iTotal +iCount; //最终吃桃子的总个数
}
int main(int argc,char *argv[])
{
	int iResult = CalcResult (10,3);
	printf("总共可吃得桃子%d个..\n",iResult);
	return 0;

	/*int inumber = 30;

	float fnumber = 90.0;

	char string[4] = "abc";

	fp = tmpfile();

	if (fp == NULL)

	{

		perror("tmpfile() call");

		exit(1);

	}

	vfpf("%d %f %s", inumber, fnumber, string);

	rewind(fp);

	fscanf(fp,"%d %f %s", &inumber, &fnumber, string);

	printf("%d %f %s\n", inumber, fnumber, string);

	fclose(fp);*/

}