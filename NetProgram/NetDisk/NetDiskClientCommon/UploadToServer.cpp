#include <WinSock2.h>
#include <iostream>
#include <io.h>
#include <time.h>
#include <string>
#include <ImageHlp.h>
#include "CommonFun.h"
#include "UsrDefine.h"
#include "LogTools.h"
using namespace std;

#pragma comment(lib,"imagehlp.lib")

string UploadToServer(SOCKET sockConn,const char * pszLocalFile,const char * pszNetDiskDir,const char *pszUserId)
{
	FILE *pFile = NULL;
	fopen_s(&pFile,pszLocalFile,"rb");
	if(!pFile)
	{
		LogMsg("无法打开传输文件[%s]\n",pszLocalFile);
		return "";
	}
	string strFile = pszLocalFile;
	string strFileName = strFile.substr(strFile.rfind("\\")+1);

	FileInfo info;
	strcpy_s(info.szFileName,strFileName.c_str());
	info.iFileLength =  _filelengthi64(_fileno(pFile));
	info.iLastModifyTime = FileTimeLastModify(strFile.c_str());
	string strMd5 = GetMD5(strFile.c_str());
	strcpy_s(info.szMd5,strMd5.c_str());
	strcpy_s(info.szDir,pszNetDiskDir);
	strcpy_s(info.szUserId,pszUserId);

	LogMsg("发送上传文件【%s】请求...\n",strFile.c_str());
	Client_Op op = upload;
	string strErrMsg;
	if (!SendData(sockConn,(char*)&op,sizeof(Client_Op),strErrMsg))
		return "";

	if (!SendData(sockConn,(char*)&info,sizeof(FileInfo),strErrMsg))
		return "";

	UploadStartInfo StartInfo;
	if (!RecvData(sockConn,&StartInfo,sizeof(StartInfo),strErrMsg))
		return "";

	if (StartInfo.m_iFinishedSize == info.iFileLength)
	{
		LogMsg("文件秒传成功,FileId=%s\n",StartInfo.m_szCmdMsg);
		closesocket(sockConn);
		return StartInfo.m_szCmdMsg;
	}
	else if (StartInfo.m_iFinishedSize < 0 || StartInfo.m_iFinishedSize > info.iFileLength)
	{
		LogMsg("文件上传失败,%s\n",StartInfo.m_szCmdMsg);
		closesocket(sockConn);
		return "";
	}

	__int64 iCountBytes = 0;
	FileTransfer trans;
	int iReadCount = sizeof(trans.szData)/sizeof(char);

	clock_t t_start = clock(); //开始计时

	string strSumLen = GetFileLenStr(info.iFileLength);

	int iLen = 0;
	int iTimerCount = 0;
	__int64 iLastDataLen = 0;
	double dLastTimeGo = 0;
	while(trans.iDataLen = fread(trans.szData,sizeof(char),iReadCount,pFile))
	{
		if(trans.iDataLen < 0)
		{
			LogMsg("\n读取数据出现错误!\n");
			break;
		}
		int iTryCount = 0;
RE_TRY:
		iLen = send(sockConn,(char*)&trans,sizeof(FileTransfer),0);
		if(iLen ==-1)
		{
			LogMsg("\n传输错误\n");
			break;
		}
		else if(iLen ==0)
		{
			LogMsg("\n传输中断,服务器终止了连接\n");
			break;
		}
		if (iLen != sizeof(FileTransfer))
		{
			if (iTryCount > 3)
			{
				LogMsg("数据无法成功发送!\n");
				break;
			}

			iTryCount++;
			LogMsg("\n数据发送不完整,正在尝试第%d次重新发送\n",iTryCount);
			Sleep(1);
			goto RE_TRY;
		}
		iCountBytes += __int64(trans.iDataLen);
		double dTimeEplisedCur = clock() - t_start;
		iTimerCount++;
		string strSpeedMsg;
		if (iTimerCount%50==0) //每50次统计一下平均速度
		{
			dLastTimeGo = dTimeEplisedCur - dLastTimeGo; // 求取两次累计耗时时间差
			int iSpeed = int((iCountBytes - iLastDataLen)*1000/dLastTimeGo); //数据量差值和时间差的比值为速度
			strSpeedMsg = GetFileLenStr(iSpeed);
			strSpeedMsg +="/s";
			iLastDataLen  = iCountBytes;
			dLastTimeGo  = dTimeEplisedCur; // 存储当前耗时，用于计算速度
			iTimerCount = 0;
		}
		LogMsg("\r已传输%s,总大小%s,耗时%f秒,平均速度%s",GetFileLenStr(iCountBytes).c_str(),
			strSumLen.c_str(),dTimeEplisedCur/1000,strSpeedMsg.c_str());
	}
	fclose(pFile);

	clock_t t_End = clock(); //结束计时
	double dTimeEplised = t_End - t_start;

	LogMsg("\n");
	trans.iDataLen = 0;
	if (!SendData(sockConn,(char*)&trans,sizeof(FileTransfer),strErrMsg))
		return "";

	if (iCountBytes == info.iFileLength)
		LogMsg("传输完毕，耗时%f秒!\n",dTimeEplised/1000);

	if (!RecvData(sockConn,&StartInfo,sizeof(StartInfo),strErrMsg))
		return "";

	closesocket(sockConn);
	if (StartInfo.m_iFinishedSize == info.iFileLength)
	{
		LogMsg("上传成功,FileId=%s\n",StartInfo.m_szCmdMsg);
		return StartInfo.m_szCmdMsg;
	}
	else
		LogMsg("上传失败，%s",StartInfo.m_szCmdMsg);

		return "";
}