#include <tchar.h>
#include <iostream>
#include <string>
using namespace std;

#if _DEBUG
#import "../../../bin/debug/AtToolCom.dll"
#else
#import "../../../bin/AtToolCom.dll"
#endif
using namespace AtToolComLib;

#define  ConstCharPtrToBstr(CharPtr) _com_util::ConvertStringToBSTR(CharPtr);

IAtImageToolComPtr RegistAndInitComObject()
{
	//初始化COM
	 HRESULT hr = ::CoInitialize(NULL);
	 if (hr!=S_OK)
	 {
		 printf("初始化com组件失败!\n");
		 return NULL;
	 }

	//定义对象变量
	IAtImageToolComPtr  lmObj;

	//生成实例
	lmObj.CreateInstance(__uuidof(AtImageToolCom));
	return lmObj;
}
int main()
{
	IAtImageToolComPtr  lmObj = RegistAndInitComObject();
	if (lmObj==NULL)
	{
		printf("获取com对象失败!\n");
		return -1;
	}

	string strInFile = "E:\\SJ9\\SJ9_SCENE\\SJ9A_PAN_20130427013203_000000\\SJ9A_PAN_20130427013203_000000_38\\SJ9A_PAN_20130427013203_000000_38.tif";
	string strOutFile = "E:\\SJ9\\SJ9_SCENE\\SJ9A_PAN_20130427013203_000000\\SJ9A_PAN_20130427013203_000000_38\\RaciancyComTest.tif";

	BSTR Input  = ConstCharPtrToBstr(strInFile.c_str());
	BSTR OutPut = ConstCharPtrToBstr(strOutFile.c_str());
	BSTR format = ConstCharPtrToBstr("GTiff");
	int iRev = lmObj->ATC_DoSJ9PanRadiancy(Input,OutPut,true,format);
	return iRev;
}