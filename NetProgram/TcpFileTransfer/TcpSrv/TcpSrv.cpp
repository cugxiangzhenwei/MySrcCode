#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <io.h>
#include <time.h>
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

#define SOCKET_BUFF 204800    //套接字缓冲区大小

struct FileTransfer
{
	char szData[153600];
	int  iDataLen;
};
struct FileInfo
{
	__int64 iFileLength;
	char szFileName[256];
};
string GetFileName(const char * pszFilePath)
{
	char *pEnd = (char*)pszFilePath + strlen(pszFilePath);
	while(*pEnd !='\\' && *pEnd !='/')
		pEnd--;
	return pEnd+1;
}
#define KB   1024
#define MB  ( 1024*KB)
#define GB  ( 1024*MB)
string GetFileLenStr(__int64 iLen)
{
	char strFileSize[50] ={0};
	if (iLen > GB )
	{
		sprintf(strFileSize,"%.2fGB",iLen*1.0/GB);
	}
	else if (iLen > MB)
	{
		sprintf(strFileSize,"%.2fMB",iLen*1.0/MB);
	}
	else if (iLen > KB)
	{
		sprintf(strFileSize,"%.2fKB",iLen*1.0/KB);
	}
	else
		sprintf(strFileSize,"%dByte",iLen);

	return strFileSize;
}
void GetRightPath(char *path)
{
	if (NULL == path)
		return;

	int iLen = strlen(path);
	if (iLen < 3)
		return;

	if (path[0] == '"' && path[iLen - 1] == '"')
	{
		char NewPath[2000] = {0};
		strcpy(NewPath,path+1);
		iLen = strlen(NewPath);
		NewPath[iLen-1] = 0;
		strset(path,0);
		strcpy(path,NewPath);
	}
}
//设置控制台输出颜色  
BOOL SetConsoleColor(WORD wAttributes)  
{  
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  
	if (hConsole == INVALID_HANDLE_VALUE)  
		return FALSE;     
	return SetConsoleTextAttribute(hConsole, wAttributes);  
}

struct ThreadParam
{
	SOCKET sockConn;
	string  strFile;
};
DWORD WINAPI ThreadSendData(void * param)
{
	SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // 恢复为白色字
	ThreadParam *Myparam = (ThreadParam*)(param);
	SOCKET sockConn = Myparam->sockConn;
	string strFile  = Myparam->strFile;
	
	FILE *pFile = fopen(strFile.c_str(),"rb");
	if(!pFile)
	{
		printf("无法打开传输文件[%s]\n",strFile.c_str());
		return 0;
	}

	FileInfo info;
	strcpy(info.szFileName,GetFileName(strFile.c_str()).c_str());
	info.iFileLength =  _filelengthi64(_fileno(pFile));

	send(sockConn,(char*)&info,sizeof(FileInfo),0);

	char szCommandMsg[50] ={0};
	bool bStart = true;
	while(1)
	{
		SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // 恢复为白色字
		printf("等待客户端发送传输数据请求...");
		int iLen = recv(sockConn,szCommandMsg,50,0);
		if (iLen == -1)
		{
			SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // 恢复为白色字
			printf("客户端中断数据请求!\n");
			closesocket(sockConn);
			bStart = false;
			break;
		}
		if (strcmp("start",szCommandMsg)==0)
			break;
		else
		{
			printf("请求消息不正确!\n");
		}
	}
	if (!bStart)
		return 0;


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
			printf("\n读取数据出现错误!\n");
			break;
		}
		int iTryCount = 0;
RE_TRY:
		iLen = send(sockConn,(char*)&trans,sizeof(FileTransfer),0);
		if(iLen ==-1)
		{
			SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // 恢复为白色字
			printf("\n传输中断,与客户端的连接已断开\n");
			break;
		}
		if (iLen != sizeof(FileTransfer))
		{
			if (iTryCount > 3)
			{
				printf("数据无法成功发送!\n");
				break;
			}

			iTryCount++;
			SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // 恢复为白色字
			printf("\n数据发送不完整,正在尝试第%d次重新发送\n",iTryCount);
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
			int iSpeed = (iCountBytes - iLastDataLen)*1000/dLastTimeGo; //数据量差值和时间差的比值为速度
			strSpeedMsg = GetFileLenStr(iSpeed);
			strSpeedMsg +="/s";
			iLastDataLen  = iCountBytes;
			dLastTimeGo  = dTimeEplisedCur; // 存储当前耗时，用于计算速度
			iTimerCount = 0;
		}
		SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // 恢复为白色字
		printf("\r已传输%s,总大小%s,耗时%f秒,平均速度%s",GetFileLenStr(iCountBytes).c_str(),
			strSumLen.c_str(),dTimeEplisedCur/1000,strSpeedMsg.c_str());
	}
	fclose(pFile);

	clock_t t_End = clock(); //结束计时
	double dTimeEplised = t_End - t_start;

	printf("\n");
	trans.iDataLen = 0;
	send(sockConn,(char*)&trans,sizeof(FileTransfer),0); //发送结束标示
	if (iCountBytes == info.iFileLength)
		printf("传输完毕，耗时%f秒!\n",dTimeEplised/1000);
	closesocket(sockConn);
	return 0;
}
void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return ;
	}

	/* Confirm that the WinSock DLL supports 1.1.*/
	/* Note that if the DLL supports versions greater    */
	/* than 1.1 in addition to 1.1, it will still return */
	/* 1.1 in wVersion since that is the version we      */
	/* requested.                                        */

	if ( LOBYTE( wsaData.wVersion ) != 1||
		HIBYTE( wsaData.wVersion ) != 1 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			return ; 
	}

	// 创建流式套接字(用于监听客户的连接请求)
	SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // 网络字节序
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000); //端口号需转换为网络字节序

	// 将本地地址和套接字进行关联
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	//将套接字设置为监听模式
	listen(sockSrv,5/*SOMAXCONN*/); //设置等待队列中个数最大为5

	// 不断等待客户端连接请求
	SOCKADDR_IN  addrClient; //存储客户端的地址信息
	int iLen = sizeof(SOCKADDR);

	while(1)
	{
		SetConsoleColor(FOREGROUND_GREEN);
		printf("\n等待客户端连接...");
		// 接受客户端的连接请求，并返回该连接的套接字
		SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&iLen);
		SetConsoleColor(FOREGROUND_GREEN);
		printf("\n已经与客户%s建立了连接...\n",inet_ntoa(addrClient.sin_addr));

		//设置套接字发送缓冲区
		int nBuf = SOCKET_BUFF;
		int nBufLen = sizeof(nBuf);
		int nRe = setsockopt(sockConn, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, nBufLen);
		if(SOCKET_ERROR == nRe)
			printf("setsockopt error!");

		//检查缓冲区是否设置成功
		nRe = getsockopt(sockConn, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, &nBufLen);
		if(SOCKET_BUFF != nBuf)
			printf("检查设置不成功!");

		//const char *pszFileName = "E:\\[C++ STL开发技术引导].pdf";
		SetConsoleColor(FOREGROUND_GREEN);
		printf("请拖入要传送的文件...\n");
		char pszFileName[500] ={0};
		gets(pszFileName);
		GetRightPath(pszFileName);
		ThreadParam param;
		param.sockConn = sockConn;
		param.strFile = pszFileName;
		void *pSendThread = CreateThread(NULL,0,ThreadSendData,&param,0,NULL);
		WaitForSingleObject(pSendThread,5);
	}
}