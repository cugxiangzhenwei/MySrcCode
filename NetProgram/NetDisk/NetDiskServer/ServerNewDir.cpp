#include <WinSock2.h>
#include <iostream>
#include <string>
#include "CommonFun.h"
#include "SqliteBase.h"
#include "LogTools.h"
using namespace std;

extern CSqliteBase * g_FileDataBase;

DWORD ServerNewDir(SOCKET sockConn)
{
	CDataNewDir DataNewDir;
	string strErrMsg;
	if (!RecvData(sockConn,(char*)&DataNewDir,sizeof(CDataNewDir),strErrMsg))
		return 0;


	CDataNewDirResult resData; 

	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strSQL = FormatString("select * from %s where ItemType ='Folder'and ItemDir='%s'and ItemName='%s';",DataNewDir.m_szUsrId,DataNewDir.m_szParentDir,DataNewDir.m_szDirName);
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	g_FileDataBase->FreeSelectResult(pszDbResult);
	if (nRows==1)
	{
		strErrMsg = FormatString("目录%s下已经存在同名文件夹%s",DataNewDir.m_szParentDir,DataNewDir.m_szDirName);
		LogMsg(strErrMsg.c_str());
		resData.m_bFlag = false;
		g_FileDataBase->FreeSelectResult(pszDbResult);
		if (!SendData(sockConn,(char*)&resData,sizeof(CDataNewDirResult),strErrMsg))
			return 0;

		closesocket(sockConn);
		return 0;
	}

	strSQL = FormatString("insert into %s(FileId,ItemName,ItemType,ItemDir,FileSize,FileTime,FileMd5) values('%s','%s','%s','%s',%lld,%lld,'');",
		DataNewDir.m_szUsrId,"",DataNewDir.m_szDirName,"Folder",DataNewDir.m_szParentDir,0,0);

	if (!g_FileDataBase->ExcuteSqlCmd(strSQL.c_str()))
	{
		resData.m_bFlag = false;
		strcpy_s(resData.m_szErrMsg,"创建目录失败,数据库操作出现错误！");
	}
	else
		resData.m_bFlag = true;

	if (!SendData(sockConn,(char*)&resData,sizeof(CDataNewDirResult),strErrMsg))
		return 0;

	closesocket(sockConn);
	return 0;
}