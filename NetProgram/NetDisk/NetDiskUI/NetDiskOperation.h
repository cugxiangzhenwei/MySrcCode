#pragma once

#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <io.h>
#include <time.h>
using namespace std;
#include "CommonFun.h"
#include"DataJsDefine.h"

#pragma comment(lib,"Ws2_32.lib")


class CFileInfo
{
public:
	CFileInfo()
	{
		m_iImageID = -1;
		m_strName.Empty();
		m_strFileSize.Empty();
		m_strModifyTime.Empty();
		m_strType.Empty();
		m_strFileId.Empty();
		m_strProgressText.Empty();
		m_strMd5.Empty();
		m_strLocalPath.Empty();
		m_strNetDiskPath.Empty();
	}
	CFileInfo(const CFileInfo & other)
	{
		m_iImageID = other.m_iImageID;
		m_strName  = other.m_strName;
		m_strFileSize = other.m_strFileSize;
		m_strModifyTime = other.m_strModifyTime;
		m_strType = other.m_strType;
		m_strFileId = other.m_strFileId;
		m_strProgressText = other.m_strProgressText;
		m_strMd5 = other.m_strMd5;
		m_strLocalPath = other.m_strLocalPath;
		m_strNetDiskPath = other.m_strNetDiskPath;
	}
	int m_iImageID;
	CString m_strName;
	CString m_strFileSize;
	CString m_strModifyTime;
	CString m_strType;
	CString m_strFileId;
	CString m_strProgressText;
	CString m_strMd5;
	CString m_strLocalPath;
	CString m_strNetDiskPath;
};

extern string UploadToServer(SOCKET sockConn,const char * pszLocalFile,const char * pszNetDiskDir,const char *pszUserId);
extern bool DownToClient(SOCKET sockConn,const char * pszFileId,const char *pszDestFile);
extern bool GetNetDiskFileList(SOCKET sockConn,const char * pszUserName,const char * pszNetDiskPath,bool bRecursion,vector<FileListItem *> &pFileList);
extern bool RegisterUser(SOCKET sockConn,const char * pszUserId,const char *pszUserName,const char *pszPasswd);
extern bool Login(SOCKET socket,const char * pszUsrId,const char * pszPassWd,CDataLoginResult & LoginRes);
extern bool CreateNetDiskDir(SOCKET sockConn,const char *pszUsrId,const char * pszNewDirName,const char * pszNetDiskDir,string &strErrMsg);
extern bool DelFileFromNetDisk(SOCKET sockConn,const char * pszUserId,vector<CDataDeleteFile> & vFileList);
extern bool ReNameFileFromNetDisk(SOCKET sockConn,const char * pszUserId,
	const char * pszItemName,const char *pszItemDir,const char * pszNewName,string &strErr);

SOCKET GetConnect(const char *pszServerIp);
CString TransferFileSize(ULONGLONG fSize);
CString TransferFileTime(ULONGLONG iFileTime);
BOOL GetFileInfo(const CString & strFile,CFileInfo & fInfo,BOOL bLocalFile);