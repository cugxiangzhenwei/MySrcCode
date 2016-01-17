#include <WinSock2.h>
#include <iostream>
#include <io.h>
#include <time.h>
#include <string>
#include <ImageHlp.h>
#include "CommonFun.h"
#include "LogTools.h"
using namespace std;

#pragma comment(lib,"imagehlp.lib")

bool DownToClient(SOCKET sockConn,const char * pszFileId,const char *pszDestFile)
{
	LogMsg("发送下载文件【FileId=%s】请求...\n",pszFileId);
	Client_Op op = download;
	string strErrMsg;
	if (!SendData(sockConn,(char*)&op,sizeof(Client_Op),strErrMsg))
		return false;

	if (!SendData(sockConn,(char*)pszFileId,64,strErrMsg))
		return false;

	FileInfo info;
	if (!RecvData(sockConn,&info,sizeof(FileInfo),strErrMsg))
		return false;

	string strSumLen = FormatString("%lld",info.iFileLength);

	clock_t t_start = clock();
	FILE *pFile = NULL;
	fopen_s(&pFile,pszDestFile,"wb");
	if (pFile==NULL)
	{
		LogMsg("创建目标文件失败!\n");
		closesocket(sockConn);
		return false;
	}

	__int64 iCountBytes = 0;
	FileTransfer DstTransferArry[2];

	int iTimerCount = 0;
	__int64 iLastDataLen = 0;
	double dLastTimeGo = 0;
	bool bFailed = false;
	while(1)
	{
		FileTransfer TmpTransfer;
		memset(TmpTransfer.szData,0,sizeof(char)*SEND_RECV_LEN);
		int iLen = recv(sockConn,(char*)&TmpTransfer,sizeof(FileTransfer),0);
		if(iLen ==-1)
		{
			bFailed = true;
			LogMsg("\n传输错误\n");
			break;
		}
		else if(iLen==0)
		{
			bFailed = true;
			LogMsg("\n传输中断,服务器终止了连接\n");
			break;
		}

		int OterLength = CopyData(DstTransferArry,&TmpTransfer,iLen);// 利用队列的方式拼包，返回一个完整包之外还剩有的包大小
		if (OterLength == NOT_FULL_DATA_BAG)  
			continue; //还不到一个完整包，继续拼包

		if (DstTransferArry[0].iDataLen ==0)
			break;

		iCountBytes +=DstTransferArry[0].iDataLen;
		fwrite(DstTransferArry[0].szData,sizeof(char),DstTransferArry[0].iDataLen,pFile);
		ClearReveiveQueen(DstTransferArry,OterLength);

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
		LogMsg("\r已接收%s,总大小%s,耗时%f秒,平均速度%s",GetFileLenStr(iCountBytes).c_str(),strSumLen.c_str()
			,dTimeEplisedCur/1000,strSpeedMsg.c_str());
	}
	fclose(pFile);

	clock_t t_End = clock();
	double dTimeEplised = t_End - t_start;
	// 通讯完成后，关闭套接字，释放套接字所占资源
	closesocket(sockConn);
	LogMsg("\n");
	if (bFailed)
	{
		LogMsg("文件传输失败!\n");
		return false;
	}
	else
	{
		LogMsg("接收完成,耗时%f秒!\n",dTimeEplised/1000);
		return true;
	}
}