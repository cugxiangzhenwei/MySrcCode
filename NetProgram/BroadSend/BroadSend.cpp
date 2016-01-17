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

	SOCKET s = WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN addrTo;
	addrTo.sin_family = AF_INET;
	addrTo.sin_addr.s_addr = INADDR_BROADCAST;
	addrTo.sin_port = htons(6000);

	//设置该套接字为广播类型
	bool opt = true;
	setsockopt(s,SOL_SOCKET,SO_BROADCAST,(char FAR *)&opt,sizeof(opt));

	int nlen=sizeof(addrTo);
	while(1)
	{
		printf("请输入要广播的消息:\n");
		char szMsg[100] ={0};
		gets(szMsg);
		//从广播地址发送消息
		int ret=sendto(s,szMsg,strlen(szMsg)+1,0,(sockaddr*)&addrTo,nlen);

		if(ret==SOCKET_ERROR)
			printf("%s\n",WSAGetLastError());
		else
		{		
			printf("广播消息发送成功!\n");	
		}
	}
}