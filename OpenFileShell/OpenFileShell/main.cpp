#include<Windows.h>
#include <shellapi.h>
#include <stdio.h>

void main()
{
	// 打开目录或文件并选中
	const char * pszPath = " /select ,K:\\program\\Programing Tools\\许可\\License.Manager.Center\\msvbvm60.dll";
	printf("打开目录或文件并选中%s\n",pszPath);
	ShellExecuteA(0,"open","explorer.exe",pszPath,NULL,SW_SHOWNORMAL);

	//运行指定文件或打开指定目录
	const char * pszPath2 = " /select ,K:\\program\\Programing Tools\\许可\\License.Manager.Center\\msvbvm60.dll";
	printf("运行指定文件或打开指定目录%s\n",pszPath2);
	ShellExecuteA(0,"open","explorer.exe",pszPath2,NULL,SW_SHOWNORMAL);
}