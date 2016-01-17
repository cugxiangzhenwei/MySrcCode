#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <io.h>
#include <time.h>
using namespace std;
#include "CommonFun.h"
#include "LogTools.h"

#pragma comment(lib,"Ws2_32.lib")

extern DWORD WINAPI ProThreadClient(void * param);

extern string UploadToServer(SOCKET sockConn,const char * pszLocalFile,const char * pszNetDiskDir,const char *pszUserId);
extern bool DownToClient(SOCKET sockConn,const char * pszFileId,const char *pszDestFile);
extern bool GetNetDiskFileList(SOCKET sockConn,const char * pszUserName,const char * pszNetDiskPath,bool bRecursion,vector<FileListItem *> &pFileList);
extern bool RegisterUser(SOCKET sockConn,const char * pszUserId,const char *pszUserName,const char *pszPasswd);

SOCKET GetConnect(const char *pszServerIp)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return NULL;
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
			return NULL; 
	}

	// 创建套接字
	SOCKET sockClent = socket(AF_INET,SOCK_STREAM,0);

	//设置套接字发送缓冲区
	int nBuf = SOCKET_BUFF;
	int nBufLen = sizeof(nBuf);
	int nRe = setsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, nBufLen);
	if(SOCKET_ERROR == nRe)
		LogMsg("setsockopt error!");

	//检查缓冲区是否设置成功
	nRe = getsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, &nBufLen);
	if(SOCKET_BUFF != nBuf)
		LogMsg("检查设置不成功!");

	const char  * pszHostIp =  pszServerIp;
	// 客户端不需要bind，直接连接
	SOCKADDR_IN addrSrv; //服务器端地址
	addrSrv.sin_addr.S_un.S_addr = inet_addr(pszHostIp); // 本地回路地址，对应服务器地址（多机时对应服务器端地址）
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000); // 必须与服务器端等待的端口一致，且为网络字节序
	LogMsg("正在连接服务器%s...\n",pszHostIp);

	int iResult = connect(sockClent,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	if (iResult == SOCKET_ERROR)
	{
		LogMsg("与服务器%s连接失败!\n",pszHostIp);
		closesocket(sockClent);
		WSACleanup(); // 终止对套接字库的使用
		return NULL;
	}

	int timeout = 3000; //3s

	//nRe = setsockopt(sockClent,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
	//if(0 != nRe)
	//	LogMsg("设置发送消息超时失败!\n");

	//nRe = setsockopt(sockClent,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
	//if(0 != nRe)
	//	LogMsg("设置接收消息超时失败!\n");

	return sockClent;
}
void main()
{
	SOCKET sockClent = GetConnect("127.0.0.1");
	RegisterUser(sockClent,"cugxiangzhenwei","博朗科技","acfgkp1017");
	
	sockClent = GetConnect("127.0.0.1");
	string strFile = "L:\\文档入库\\2010Excel培训教程完整版.pdf";
	UploadToServer(sockClent,strFile.c_str(),"/","cugxiangzhenwei");

	vector<FileListItem *> vFileList;
	sockClent = GetConnect("127.0.0.1");
	bool bok = GetNetDiskFileList(sockClent,"cugxiangzhenwei","/",false,vFileList);

	sockClent = GetConnect("127.0.0.1");
	string strDestFile = "L:\\Boost程序库完全开发指南_down.tif";
	DownToClient(sockClent,"1246D8A4-B585-4383-AB71-437AC8152E06",strDestFile.c_str());
	::system("pause");
}