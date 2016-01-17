#include "stdafx.h"
#include "BaiduPicture.h"
#include "reader.h"
#include"writer.h"
CString GetJsFromFile(const CString & strFile)
{
	FILE *pFile = NULL;
	fopen_s(&pFile,strFile,"r");
	if (pFile==NULL)
		return "";

	_fseeki64(pFile, 0L, SEEK_END);
	long  len = ftell(pFile);
	_fseeki64(pFile,0L,SEEK_SET);
	char * pBuf = new char[len];
	fread_s(pBuf,len,1,len,pFile);
	CString strFileContent = pBuf;
	fclose(pFile);
	return strFileContent;
}
void GetAllImgUrl(vector<CString> &VstrUrl,CString strJson)
{
	Json::Reader read;
	Json::Value Obj;
	if (!read.parse((LPCTSTR)strJson,Obj,false))
		return;

	Json::Value Arry = Obj["imgs"];
	if (!Arry.isArray())
		return;

	size_t iCount = Arry.size();
	for (size_t i=0;i< iCount;i++)
	{
		CString strUrl = Arry[i]["downloadUrl"].isString() ? Arry[i]["downloadUrl"].asCString():"";
		if (strUrl.IsEmpty())
			continue;

		VstrUrl.push_back(strUrl);
	}
}