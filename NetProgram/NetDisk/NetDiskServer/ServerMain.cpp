#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <io.h>
#include <time.h>
#include "SqliteBase.h"
#include "UsrDefine.h"
#include "LogTools.h"
#include "CrashHandler.h"
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

extern DWORD WINAPI ProThreadServer(void * param);
extern CSqliteBase * g_FileDataBase;

void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	CCrashHandler ch;
	ch.SetProcessExceptionHandlers();
	ch.SetThreadExceptionHandlers();
	HANDLE hMutex = CreateMutex(NULL,TRUE,"NetDiskServer");
	if (hMutex)
	{
		if(ERROR_ALREADY_EXISTS==GetLastError())
		{
			CloseHandle(hMutex);
			return;
		}
	}

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

	char szFileConfig[MAX_PATH] = {0};
	GetModuleFileName(NULL,szFileConfig,MAX_PATH);
	string strConfigFile = szFileConfig;
	strConfigFile = strConfigFile.substr(0,strConfigFile.length()-3) + "ini";
	char szDataDir[MAX_PATH] = {0};
	GetPrivateProfileString("Setting","FieDataDir","",szDataDir,MAX_PATH,strConfigFile.c_str());
	if (strcmp(szDataDir,"")==0)
	{
		GetWindowsDirectory(szDataDir,MAX_PATH);
		szDataDir[3]=0;
		string strDir = string(szDataDir) + "NetDiskServerFileData";
		strcpy_s(szDataDir,MAX_PATH,strDir.c_str());
		WritePrivateProfileString("Setting","FieDataDir",strDir.c_str(),strConfigFile.c_str());
	}

	g_strServerLocalDir = _strdup(szDataDir);

	CHAR szExeFile[MAX_PATH] = {0};
	GetModuleFileName(NULL,szExeFile,MAX_PATH);
	string strDbFile = szExeFile;
	strDbFile = strDbFile.substr(0,strDbFile.length()- 4) + ".db";

	g_FileDataBase = CSqliteBase::OpenDataBase(strDbFile.c_str());
	if (!g_FileDataBase->IsTableExist("FILE"))
	{
		g_FileDataBase->ExcuteSqlCmd("create table FILE(Md5 varchar,FileName varchar,FileLength varchar,FileTime varchar,FileId varchar);");
		g_FileDataBase->ExcuteSqlCmd("create index File_Md5_Index on FILE(Md5);"); //创建查询索引
		g_FileDataBase->ExcuteSqlCmd("create table USER(UserId varchar,UserName varchar,PassWord varchar,FileTable varchar,AllSpace varchar,UsedSpace varchar);");
		g_FileDataBase->ExcuteSqlCmd("create index User_Id_Index on USER(UserId);"); //创建查询索引
	}

	// 创建流式套接字(用于监听客户的连接请求)
	SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // 网络字节序
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(SERVER_PORT); //端口号需转换为网络字节序

	// 将本地地址和套接字进行关联
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	//将套接字设置为监听模式
	listen(sockSrv,5/*SOMAXCONN*/); //设置等待队列中个数最大为5

	// 不断等待客户端连接请求
	SOCKADDR_IN  addrClient; //存储客户端的地址信息
	int iLen = sizeof(SOCKADDR);
	int timeout = 3000; //3s

	while(1)
	{
		LogMsg("等待客户端连接...\n");
		// 接受客户端的连接请求，并返回该连接的套接字
		SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&iLen);
		LogMsg("已经与客户%s建立了连接...\n",inet_ntoa(addrClient.sin_addr));

		//设置套接字发送缓冲区
		int nBuf = SOCKET_BUFF;
		int nBufLen = sizeof(nBuf);
		int nRe = setsockopt(sockConn, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, nBufLen);
		if(SOCKET_ERROR == nRe)
			LogMsg("setsockopt error!\n");

		//检查缓冲区是否设置成功
		nRe = getsockopt(sockConn, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, &nBufLen);
		if(SOCKET_BUFF != nBuf)
			LogMsg("检查设置不成功!\n");

	/*	nRe = setsockopt(sockConn,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
		if(0 != nRe)
			LogMsg("设置发送消息超时失败!\n");

		nRe = setsockopt(sockConn,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
		if(0 != nRe)
			LogMsg("设置接收消息超时失败!\n");*/

		void *pSendThread = CreateThread(NULL,0,ProThreadServer,(LPVOID)sockConn,0,NULL);
		WaitForSingleObject(pSendThread,5);
	}

	CloseHandle(hMutex);
	hMutex = NULL;
	free((void*)g_strServerLocalDir);
	g_strServerLocalDir = NULL;
}