
#include <stdlib.h>

#if _DEBUG
#import "../ComTest/Debug/ComTest.dll"
#else
#import "../ComTest/Release/ComTest.dll"
#endif

using namespace ComTestLib;
IComBasePtr RegisterAndGetComObject()
{

	//初始化COM
	::CoInitialize(NULL);

	//定义对象变量
	IComBasePtr lmObj;

	//生成实例
	lmObj.CreateInstance(__uuidof(ComBase));
	if(lmObj == NULL)
		return NULL;

	return lmObj;

	//BSTR pkName = _com_util::ConvertStringToBSTR(strName.c_str());
	//BSTR pkVersion = _com_util::ConvertStringToBSTR(strVersion.c_str());
	//BSTR plName = _com_util::ConvertStringToBSTR(strPlatName.c_str());

	//string strResult = lmObj->getLicense(&pkName,&pkVersion,&plName);

	//析构COM
	::CoUninitialize();
}

void main()
{
	IComBasePtr lmObj = RegisterAndGetComObject();
	if (lmObj==NULL)
	{
		printf("获取com对象失败!");
		return;
	}
	HRESULT hr1 =  lmObj->PrintMsg("Hello World！");
	HRESULT hr2 =  lmObj->Sum(1,50);
}