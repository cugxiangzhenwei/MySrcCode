#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32.lib")

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
		// 接受客户端的连接请求，并返回该连接的套接字
		SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&iLen);

		// 向客户端发送数据
		char Sendbuf[100];
		sprintf( Sendbuf,"Welcome %s to server...",inet_ntoa(addrClient.sin_addr) );
		send(sockConn,Sendbuf,strlen(Sendbuf)+1,0);// 多发送一个字节，给接受端一个'\0'结尾

		// 从客户端接受数据
		char RecvBuf[100];
		recv(sockConn,RecvBuf,100,0);
		printf("%s\n",RecvBuf);

		closesocket(sockConn);
	}
}