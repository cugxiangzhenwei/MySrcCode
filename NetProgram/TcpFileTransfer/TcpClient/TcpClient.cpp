#include <WinSock2.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

#define NOT_FULL_DATA_BAG -1
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
#define KB   1024
#define MB   (1024*KB)
#define GB   (1024*MB)
string GetFileLenStr(__int64 iLen)
{
	char strFileSize[50] ={0};
	if (iLen > GB )
	{
		sprintf(strFileSize,"%fGB",iLen*1.0/GB);
	}
	else if (iLen > MB)
	{
		sprintf(strFileSize,"%fMB",iLen*1.0/MB);
	}
	else if (iLen > KB)
	{
		sprintf(strFileSize,"%fKB",iLen*1.0/KB);
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

static int CopyData( FileTransfer * Dst,FileTransfer * SrcTmp,int iSrcLen)
{
	const static int ONE_DATA_LEN = sizeof(FileTransfer);
	static int iRecvLength = 0;
	memcpy((char*)Dst+iRecvLength,SrcTmp,iSrcLen); //将接收到的不完整数据拼到缓存数据包数组中
	iRecvLength += iSrcLen;
	if(iRecvLength >= ONE_DATA_LEN )
	{
		iRecvLength = iRecvLength - ONE_DATA_LEN; //已经有一个完整包了
		return iRecvLength; //返回完整包之外还剩余的数据大小
	}
	else
		return NOT_FULL_DATA_BAG;
}
void ClearReveiveQueen(FileTransfer *Dst,int iLen)
{
	const static int ONE_DATA_LEN = sizeof(FileTransfer);
	if (iLen == 0)
		return;

	char * pData = new char[iLen];
	memcpy(pData,(char*)Dst + ONE_DATA_LEN,iLen);
	memcpy(Dst,pData,iLen);
	delete pData;
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

	//设置套接字发送缓冲区
	int nBuf = SOCKET_BUFF;
	int nBufLen = sizeof(nBuf);
	int nRe = setsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, nBufLen);
	if(SOCKET_ERROR == nRe)
		printf("setsockopt error!");

	//检查缓冲区是否设置成功
	nRe = getsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, &nBufLen);
	if(SOCKET_BUFF != nBuf)
		printf("检查设置不成功!");


	printf("请输入服务器IP地址:\n");
	char szSrvI[30] ={"192.168.1.101"};
	gets(szSrvI);
	// 客户端不需要bind，直接连接
	SOCKADDR_IN addrSrv; //服务器端地址
	addrSrv.sin_addr.S_un.S_addr = inet_addr(szSrvI); // 本地回路地址，对应服务器地址（多机时对应服务器端地址）
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000); // 必须与服务器端等待的端口一致，且为网络字节序
	printf("正在连接服务器%s...\n",szSrvI);
	int iResult = connect(sockClent,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	if (iResult == SOCKET_ERROR)
	{	printf("与服务器%s连接失败!\n",szSrvI);
		closesocket(sockClent);
		WSACleanup(); // 终止对套接字库的使用
		return ;
	}

	printf("连接服务器%s成功\n",szSrvI);
	printf("等待接收文件...\n");
	FileInfo info;
	recv(sockClent,(char*)&info,sizeof(FileInfo),0);
	string strSumLen = GetFileLenStr(info.iFileLength);
	printf("传来文件 : %s,文件大小%s\n",info.szFileName,strSumLen.c_str());
	printf("请输入存储目录..\n");

	char szDstFolderName[256] ={0};
	gets(szDstFolderName);
	GetRightPath(szDstFolderName); //去除两端的引号

	char szDstFinal[500] ={0};
	sprintf(szDstFinal,"%s\\%s",szDstFolderName,info.szFileName);
	printf("目标文件为:%s\n",szDstFinal);

	send(sockClent,"start",strlen("start")+1,0); //请求开始传输
	clock_t t_start = clock();

	FILE *pFile = fopen(szDstFinal,"wb");

	__int64 iCountBytes = 0;
	FileTransfer DstTransferArry[2];

	int iTimerCount = 0;
	__int64 iLastDataLen = 0;
	double dLastTimeGo = 0;
	bool bFailed = false;
	while(1)
	{
		FileTransfer TmpTransfer;
		strset(TmpTransfer.szData,0);
		int iLen = recv(sockClent,(char*)&TmpTransfer,sizeof(FileTransfer),0);
		if(iLen ==-1)
		{
			bFailed = true;
			printf("\n传输中断,与服务器的连接已断开\n");
			break;
		}

		int OterLength = CopyData(DstTransferArry,&TmpTransfer,iLen);// 利用队列的方式拼包，返回一个完整包之外还剩有的包大小
		if (OterLength == NOT_FULL_DATA_BAG)  
			continue; //还不到一个完整包，继续拼包

		if (DstTransferArry[0].iDataLen ==0)
			break;

		iCountBytes +=DstTransferArry[0].iDataLen;
		fwrite(DstTransferArry[0].szData,sizeof(char),DstTransferArry[0].iDataLen,pFile);
		ClearReveiveQueen(DstTransferArry,OterLength);

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
		printf("\r已接收%s,总大小%s,耗时%f秒,平均速度%s",GetFileLenStr(iCountBytes).c_str(),strSumLen.c_str()
			,dTimeEplisedCur/1000,strSpeedMsg.c_str());
	}
	fclose(pFile);

	clock_t t_End = clock();
	double dTimeEplised = t_End - t_start;
	printf("\n");
	if (bFailed)
		printf("文件传输失败!\n");
	else
		printf("接收完成,耗时%f秒!\n",dTimeEplised/1000);


	// 通讯完成后，关闭套接字，释放套接字所占资源
	closesocket(sockClent);
	WSACleanup(); // 终止对套接字库的使用
	::system("pause");
}

