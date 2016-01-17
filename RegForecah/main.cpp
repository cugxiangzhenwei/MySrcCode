#include <stdio.h>
#include "RegRead.h"
void Test()
{
	vector<SNewFileTypeReg> vRegNew;
	if (!GetAllNewFileTypeReg(vRegNew))
	{
		printf("获取文件文件类型失败!\n");
		return;
	}

	for(size_t i=0;i< vRegNew.size();i++)
	{
		printf("新建文件类型：%s\n",vRegNew[i].m_strExt.c_str());
		printf("注册表键值：%s\n",vRegNew[i].m_strKeyPath.c_str());
		printf("...................................\n\n");
	}

	printf("共获得%d种新建文件类型：\n",vRegNew.size());
}

void Test2()
{
	vector<NewType> vStrList;
	GetAllDesktopRightMenuNewItems(vStrList);
	for (int i=0;i<vStrList.size();i++)
		if (!CreateNewItem(vStrList[i].m_strExt,"E:\\TestNewItem"))
			printf("创建类型【%s】文件失败!\n",vStrList[i].m_strDesrciption.c_str());
}
void Test3()
{
	string strOut =  GetWindowVesion();
	printf("系统版本为：%s\n",strOut.c_str());
}
void main()
{
	Test2();
	::system("pause");
}