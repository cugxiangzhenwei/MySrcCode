// BatLibTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "..\BatFileSystem.h"
#pragma comment(lib,"..\\Debug\\BatLib.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	path dir("c:\\xiang");
	vector<path>v;
	FindExtenFile(dir,v,".txt",true);
	printf("共找到%d个txt文件\n",v.size());
	return 0;
}

