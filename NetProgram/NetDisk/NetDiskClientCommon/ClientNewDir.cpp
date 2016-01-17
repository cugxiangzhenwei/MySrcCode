#include <WinSock2.h>
#include <iostream>
#include <string>
#include "CommonFun.h"
#include "UsrDefine.h"
#include "LogTools.h"
using namespace std;
bool CreateNetDiskDir(SOCKET sockConn,const char *pszUsrId,const char * pszNewDirName,const char * pszNetDiskDir,string &strErrMsg)
{
	Client_Op op = addDir;
	if (!SendData(sockConn,(char*)&op,sizeof(Client_Op),strErrMsg))
		return false;

	CDataNewDir DataNewDir;
	strcpy_s(DataNewDir.m_szDirName,pszNewDirName);
	strcpy_s(DataNewDir.m_szParentDir,pszNetDiskDir);
	strcpy_s(DataNewDir.m_szUsrId,pszUsrId);

	if (!SendData(sockConn,(char*)&DataNewDir,sizeof(CDataNewDir),strErrMsg))
		return false;

	CDataNewDirResult ResData;
	if (!RecvData(sockConn,(char*)&ResData,sizeof(CDataNewDirResult),strErrMsg))
		return false;

	return true;
}