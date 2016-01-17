#include <WinSock2.h>
#include <iostream>
#include <string>
#include "CommonFun.h"
#include "UsrDefine.h"
#include "LogTools.h"
#include "DataJsDefine.h"
using namespace std;
bool DelFileFromNetDisk(SOCKET sockConn,const char * pszUserId,vector<CDataDeleteFile> & vFileList)
{
	Client_Op op = deleteItem;
	string strErr;
	if (!SendData(sockConn,(char*)(&op),sizeof(Client_Op),strErr))
		return false;

	CJsonDataDelteFile JsonDataDel;
	JsonDataDel.m_strUserId = pszUserId;
	JsonDataDel.m_vDelFileList.assign(vFileList.begin(),vFileList.end());
	string strJson = JsonDataDel.GetResultJson();
	int iLen = strJson.length();
	if (iLen < 1)
		return false;

	iLen +=1;
	if (!SendIntData(sockConn,iLen,strErr))
		return false;

	if (!SendData(sockConn,(char*)strJson.c_str(),iLen,strErr))
		return false;

	CDataDelFileResult DataResult;
	if (!RecvData(sockConn,&DataResult,sizeof(CDataDelFileResult),strErr))
		return false;

	return true;
}