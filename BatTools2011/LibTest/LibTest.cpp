// LibTest.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include "..\\BatLib\\BatFileSystem.h"
int _tmain(int argc, _TCHAR* argv[])
{
	path dir("c:\\xiang");
	vector<path>v;
	FindExtenFile(dir,v,".txt");
	int num=v.size();
	BatReName(v,"测试");
	return 0;
}
