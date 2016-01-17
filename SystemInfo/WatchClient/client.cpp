#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include "../commonheader/SysInfoClass.h"
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

typedef CClientInf ServerInfo ;

string ConvertWStringToString(const wstring &ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";  
	setlocale(LC_ALL, "chs");  
	//从wstring到wchar *  
	const wchar_t* _Source = ws.c_str();  

	//string对应的长度  
	size_t _Dsize = 2 * ws.size() + 1;  
	char *_Dest = new char[_Dsize];  
	//初始化内存  
	memset(_Dest,0,_Dsize);  

	//从unicode码转成ANSI码  
	wcstombs(_Dest, _Source, _Dsize);  

	//从char *转为string  
	string result = _Dest;  

	delete []_Dest;  
	return result;  
}

bool GetServerInfo(const char * pszServerIP,ServerInfo &svrInfo)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return  false;
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
			return  false; 
	}

	// 创建套接字
	SOCKET sockClent = socket(AF_INET,SOCK_STREAM,0);

	// 客户端不需要bind，直接连接
	SOCKADDR_IN addrSrv; //服务器端地址
	addrSrv.sin_addr.S_un.S_addr = inet_addr(pszServerIP); // 本地回路地址，对应服务器地址（多机时对应服务器端地址）
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000); // 必须与服务器端等待的端口一致，且为网络字节序
	int iResult = connect(sockClent,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	if (iResult == SOCKET_ERROR)
	{	printf("与服务器%s连接失败!\n",pszServerIP);
	closesocket(sockClent);
	WSACleanup(); // 终止对套接字库的使用
	return false;
	}

	int  iLen = recv(sockClent,(char*)&svrInfo,sizeof(ServerInfo),0);
	return true;
}
void main()
{
	cout << "请输入被监控服务器IP地址，例如 192.168.1.1\n";

	char szServerIP[20]={"172.172.4.158"};
	cin >> szServerIP;

	ServerInfo svrInfo;
	while(1)
	{
		if (!GetServerInfo(szServerIP,svrInfo))
		{
			printf("获取服务器运行状态失败！\n");
			break;
		}
		string  strSystemVersion =  ConvertWStringToString(svrInfo.wsSystemInf);

		printf("服务器主机名: %s \n",svrInfo.hostName);
		printf("服务器系统版本: %s \n",strSystemVersion.c_str());
		printf("服务器IP地址: %s \n",szServerIP);
		printf("服务器内存空间: %.3f GB \n",svrInfo.nMemorySize *1.0 / 1000); 
		printf("服务器磁盘空间: %u GB \n",svrInfo.nDiskSize);
		printf("服务器CPU频率 : %.3f GHz \n",svrInfo.nCPUFrequency*1.0/1000);

		printf("服务器物理内存使用: %.3f GB \n",svrInfo.nUsedMemSize *1.0 / 1000); 
		printf("服务器物理内存剩余: %.3f GB \n",svrInfo.nFreeMemSize *1.0 / 1000); 
		printf("服务器虚拟内存空间: %.3f GB \n",svrInfo.nTotalVirtualMemSize *1.0 / 1000); 
		printf("服务器虚可用拟内存: %.3f GB \n",svrInfo.nVirtualFreeMemSize *1.0 / 1000); 

		Sleep(1000);
	}
}
