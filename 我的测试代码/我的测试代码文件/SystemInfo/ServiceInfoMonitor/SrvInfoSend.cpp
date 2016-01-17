#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <direct.h>
#include "../commonheader/SysInfoClass.h"
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

extern bool CollectHostInf(CClientInf *pCClientInf);

void main()
{

	char szPath[256] ={0};
	getcwd(szPath,256);

	char LOG_FIlE[300] ={0};
	sprintf(LOG_FIlE,"%s\\\%s",szPath,"Monitor_log.txt");

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
	listen(sockSrv,/*5*/SOMAXCONN); //设置等待队列中个数最大为5

	// 不断等待客户端连接请求
	SOCKADDR_IN  addrClient; //存储客户端的地址信息
	int iLen = sizeof(SOCKADDR);

	FILE * pFileLog  = NULL;
	pFileLog = fopen(LOG_FIlE,"a+");

	while(1)
	{
		printf("等待客户端连接...");
		// 接受客户端的连接请求，并返回该连接的套接字
		SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&iLen);
		if(pFileLog)
		{
			fprintf(pFileLog,"\n客户机: %s请求本机运行状态信息...\n",inet_ntoa(addrClient.sin_addr));
			fflush(pFileLog);
		}

		printf("\n已经与客户%s建立了连接...\n",inet_ntoa(addrClient.sin_addr));
		// 向客户端发送数据
		printf("正在获取系统运行状态...\n");
		CClientInf ServiceSysInf;
		if (!CollectHostInf(&ServiceSysInf))
		{
			printf("获取系统运行状态失败!\n");
			if(pFileLog)
			{
				fprintf(pFileLog,"获取系统运行状态失败\n");
				fclose(pFileLog);
			}
			return; // 获取主机运行状态信息失败
		}

		printf("正在向客户端发送监控信息...\n");
		send(sockConn,(char*)&ServiceSysInf,sizeof(CClientInf) + 1,0); 		// 多发送一个字节，给接受端一个'\0'结尾

		closesocket(sockConn);
	}
}