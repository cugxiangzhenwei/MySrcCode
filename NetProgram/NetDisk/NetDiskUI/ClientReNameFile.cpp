#include <WinSock2.h>
#include <iostream>
#include <string>
#include "CommonFun.h"
#include "UsrDefine.h"
#include "LogTools.h"
#include "DataJsDefine.h"
using namespace std;
bool ReNameFileFromNetDisk(SOCKET sockConn,const char * pszUserId,
	const char * pszItemName,const char *pszItemDir,const char * pszNewName,string &strErr)
{
	Client_Op op = reName;
	strErr.clear();
	if (!SendData(sockConn,(char*)(&op),sizeof(Client_Op),strErr))
		return false;

	CDataReNameItem DataReName;
	strcpy_s(DataReName.m_szUsrId,pszUserId);
	strcpy_s(DataReName.szItemName,pszItemName);
	strcpy_s(DataReName.szItemDir,pszItemDir);
	strcpy_s(DataReName.szNewName,pszNewName);

	if (!SendData(sockConn,(char*)&DataReName,sizeof(CDataReNameItem),strErr))
		return false;

	CDataReNameResult DataResult;
	if (!RecvData(sockConn,&DataResult,sizeof(CDataReNameResult),strErr))
		return false;

	if (DataResult.m_bFlag == false)
	{
		strErr = DataResult.m_szErrMsg;
		return false;
	}
	return true;
}