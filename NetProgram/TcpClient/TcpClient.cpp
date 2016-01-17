#include <WinSock2.h>
#include <stdio.h>
#include <time.h>

#pragma comment(lib,"Ws2_32.lib")

char * GetRanUser()
{
	static char szUserArry[4][20] ={"高松峰","向振伟","关峰","刘鹏飞"};

	srand(time(0));
	int iIndex = rand()%4;
	return szUserArry[iIndex];
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

	// 创建套接字
	SOCKET sockClent = socket(AF_INET,SOCK_STREAM,0);

	// 客户端不需要bind，直接连接
	SOCKADDR_IN addrSrv; //服务器端地址
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // 本地回路地址，对应服务器地址（多机时对应服务器端地址）
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000); // 必须与服务器端等待的端口一致，且为网络字节序
	connect(sockClent,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));


	char recvBuf[100];
	recv(sockClent,recvBuf,100,0);
	printf("%s\n",recvBuf);

	char * user = GetRanUser();
	char  SendMsg[100];
	sprintf(SendMsg,"This is %s",user);
	send(sockClent,SendMsg,strlen(SendMsg)+1,0);

	// 通讯完成后，关闭套接字，释放套接字所占资源
	closesocket(sockClent);
	WSACleanup(); // 终止对套接字库的使用
}

