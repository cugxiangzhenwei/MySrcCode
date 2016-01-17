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

	SOCKET sockClient = socket(AF_INET,SOCK_DGRAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr =inet_addr("192.168.1.101");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);


	int iLen = sizeof(SOCKADDR);
	while(1)
	{
		printf("请输入要发送的消息：\n");
		char szSend[100]={0};
		gets(szSend);
		sendto(sockClient,szSend,strlen(szSend)+1,0,(sockaddr*)&addrSrv,sizeof(SOCKADDR));//等待接受发送到服务器的消息
		printf("发送完成!\n");

		char szRecv[100]={0};
		recvfrom(sockClient,szRecv,100,0,(sockaddr*)&addrSrv,&iLen);
		if (szRecv[0]=='q')
		{
			sendto(sockClient,"q",strlen("q")+1,0,(sockaddr*)&addrSrv,iLen);
			printf("\a与[%s]聊天结束!\n",inet_ntoa(addrSrv.sin_addr));
		}
		printf("\a[%s]说:\"%s\"\n",inet_ntoa(addrSrv.sin_addr),szRecv);
	}

	closesocket(sockClient);
	WSACleanup();
}
