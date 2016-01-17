#include <WinSock2.h>
#include <iostream>
#include <io.h>
#include <time.h>
#include <string>
#include <ImageHlp.h>
#include "CommonFun.h"
#include "SqliteBase.h"
#include "LogTools.h"
#include <shlwapi.h>
using namespace std;

#pragma comment(lib,"imagehlp.lib")

CSqliteBase * g_FileDataBase = NULL;
extern void UpdateUserSpace(const char *pszUsrId,LONGLONG iSpace,bool bAdd);

bool AddFileToListTable(const char * pszFileId,FileInfo *pFileInfo)
{
	string strTable = pFileInfo->szUserId;

	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strSQL = FormatString("select * from %s where ItemType ='File'and ItemDir='%s'and ItemName='%s'and FileMd5='%s';",strTable.c_str(),pFileInfo->szDir,pFileInfo->szFileName,pFileInfo->szMd5);
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	if (nRows==1)
	{
		LogMsg("当前目录下已经存在同名文件\n");
		g_FileDataBase->FreeSelectResult(pszDbResult);
		return false;
	}
	if (pszDbResult)
		g_FileDataBase->FreeSelectResult(pszDbResult);

	strSQL = FormatString("insert into %s(FileId,ItemName,ItemType,ItemDir,FileSize,FileTime,FileMd5) values('%s','%s','%s','%s',%lld,%lld,'%s');",
		strTable.c_str(),pszFileId,pFileInfo->szFileName,"File",pFileInfo->szDir,pFileInfo->iFileLength,pFileInfo->iLastModifyTime,pFileInfo->szMd5);
	bool bOK = g_FileDataBase->ExcuteSqlCmd(strSQL.c_str());
	if (!bOK)
		return false;

	UpdateUserSpace(pFileInfo->szUserId,pFileInfo->iFileLength,true);
	return true;
}
DWORD ReceiveToServer(SOCKET sockConn,FileInfo *pFileInfo)
{
	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strErrMsg;
	string strSQL = FormatString("select * from FILE where Md5 ='%s';",pFileInfo->szMd5);
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	if (nRows==1)
	{
		string strFileId = pszDbResult[nColums + 4];
		g_FileDataBase->FreeSelectResult(pszDbResult);

		AddFileToListTable(strFileId.c_str(),pFileInfo);

		UploadStartInfo StartInfo;
		StartInfo.m_iFinishedSize = pFileInfo->iFileLength;
		strcpy_s(StartInfo.m_szCmdMsg,strFileId.c_str());
		if (!SendData(sockConn,(char*)&StartInfo,sizeof(UploadStartInfo),strErrMsg))  //发送妙传成功结果
			return 0;

		closesocket(sockConn);
		LogMsg("秒传成功，FileId=%s\n",strFileId.c_str());
		return 0;
	}

	if (pszDbResult)
		g_FileDataBase->FreeSelectResult(pszDbResult);

	string strSumLen = GetFileLenStr(pFileInfo->iFileLength);
	LogMsg("传来文件 : %s,文件大小%s\n",pFileInfo->szFileName,strSumLen.c_str());

	string strFileName = string(pFileInfo->szDir) + pFileInfo->szFileName;
	strFileName = RepalceAll(strFileName.c_str(),"/","\\");
	string strDestDir = string(g_strServerLocalDir) + strFileName;

	RepalceAll(strDestDir.c_str(),"/","\\");
	LogMsg("目标文件为:%s\n",strDestDir.c_str());
	MakeSureDirectoryPathExists(strDestDir.c_str());

	clock_t t_start = clock();
	FILE *pFile = NULL;
	fopen_s(&pFile,strDestDir.c_str(),"ab+");
	if (pFile==NULL)
	{
		LogMsg("创建目标文件失败!\n");
		closesocket(sockConn);
		return 0;
	}

	UploadStartInfo StartInfo;
	fseek(pFile,0,SEEK_END);
	StartInfo.m_iFinishedSize = _ftelli64(pFile);
	strcpy_s(StartInfo.m_szCmdMsg,"start");
	if (!SendData(sockConn,(char*)&StartInfo,sizeof(UploadStartInfo),strErrMsg))  //请求开始传输
		return 0;

	__int64 iCountBytes = StartInfo.m_iFinishedSize;
	FileTransfer trans;
	bool bFailed = false;
	while(1)
	{
		memset(trans.szData,0,sizeof(char)*SEND_RECV_LEN);
		if (!RecvData(sockConn,(char*)&trans,sizeof(FileTransfer),strErrMsg))
		{
			bFailed = true;
			LogMsg(strErrMsg.c_str());
			break;
		}
		if (trans.iDataLen ==0)
			break; // 发送数据方最后会发送一个iDataLen=0的FileTransfer结构体,用于标识文件传输完毕

		iCountBytes += trans.iDataLen;
		fwrite(trans.szData,sizeof(char),trans.iDataLen,pFile);
	}
	fclose(pFile);

	if (iCountBytes!= pFileInfo->iFileLength)
		bFailed = true;

	clock_t t_End = clock();
	
	if (bFailed)
	{
		LogMsg("文件传输失败!\n");
	}
	else
	{
		double dTimeEplised = t_End - t_start;
		LogMsg("接收完成,耗时%f秒!\n",dTimeEplised/1000);

		string strFileId = GetNewGUID();
		strSQL = FormatString("insert into FILE(Md5,FileName,FileLength,FileTime,FileId) values('%s','%s',%lld,%lld,'%s');",
			pFileInfo->szMd5,strFileName.c_str(),pFileInfo->iFileLength,pFileInfo->iLastModifyTime,strFileId.c_str());

		bool bOk = g_FileDataBase->ExcuteSqlCmd(strSQL.c_str());
		if (!bOk)
		{
			LogMsg("插入文件数据库失败！SQL =【%s】",strSQL.c_str());
			closesocket(sockConn);
			return 0;
		}
		bOk = AddFileToListTable(strFileId.c_str(),pFileInfo);
		if (bOk)
		{
			UploadStartInfo StartInfo;
			StartInfo.m_iFinishedSize = pFileInfo->iFileLength;
			strcpy_s(StartInfo.m_szCmdMsg,strFileId.c_str());
			if (!SendData(sockConn,(char*)&StartInfo,sizeof(UploadStartInfo),strErrMsg))  //发送妙传成功结果
				return 0;

			LogMsg("上传成功，FileId=%s\n",strFileId.c_str());
		}
		else
			LogMsg("更新数据库失败!");

		
	}
	
	// 通讯完成后，关闭套接字，释放套接字所占资源
	closesocket(sockConn);
	return 0;
}