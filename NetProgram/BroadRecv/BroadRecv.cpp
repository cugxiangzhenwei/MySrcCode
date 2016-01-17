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

	//绑定套接字的地址
	SOCKADDR_IN sBind;
	sBind.sin_family=AF_INET;
	sBind.sin_addr.s_addr=0;
	sBind.sin_port=htons(6000);

	int fromlength=sizeof(SOCKADDR);

	//用UDP初始化套接字
	SOCKET s=socket(AF_INET,SOCK_DGRAM,0);

	//设置该套接字为广播类型
	bool optval =true;
	setsockopt(s,SOL_SOCKET,SO_BROADCAST,(char FAR *)&optval,sizeof(optval));


	bind(s,(sockaddr *)&sBind,sizeof(sockaddr_in));


	char bufRecv[256] = {0};
	while(1)
	{   //从广播地址接收消息，注意用来绑定的地址和接收消息的地址是不一样的
		SOCKADDR_IN addrFrom;
		printf("等待接收广播消息...\n");
		recvfrom(s,bufRecv,256,0,(sockaddr*)&addrFrom,(int FAR *)&fromlength);
		//Sleep(2000);
		printf("接到%s的广播:%s\n",inet_ntoa(addrFrom.sin_addr),bufRecv);
		ZeroMemory(bufRecv,256);
	}

}