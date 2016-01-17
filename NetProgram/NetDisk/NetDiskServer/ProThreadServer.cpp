#include <WinSock2.h>
#include <stdio.h>
#include "CommonFun.h"
#include "LogTools.h"

extern DWORD SendToClient(SOCKET sockConn);
extern DWORD ReceiveToServer(SOCKET sockConn,FileInfo *pFileInfo);
extern DWORD SendFileListToClient(SOCKET sockConn);
extern DWORD RegisterUser(SOCKET sockConn);
extern DWORD Login(SOCKET sockConn);
extern DWORD ServerNewDir(SOCKET sockConn);
extern DWORD ServerCopyFile(SOCKET sockConn);
extern DWORD ServerDeleteFile(SOCKET sockConn);
extern DWORD ServerMoveFile(SOCKET sockConn);
extern DWORD ServerReNameFile(SOCKET sockConn);


DWORD WINAPI ProThreadServer(void * param)
{
	SOCKET sockConn  = (SOCKET)param;
	// 获取用户请求类型
	Client_Op opType;
	string strErrMsg;
	if (!RecvData(sockConn,&opType,sizeof(Client_Op),strErrMsg))
		return 0;

	DWORD dwRev = 0;
	if (opType == upload)
	{
		//处理用户上传请求
		LogMsg("收到上传文件请求...\n");
		FileInfo *pinfo = new FileInfo;
		if (!RecvData(sockConn,pinfo,sizeof(FileInfo),strErrMsg))
		{
			delete pinfo;
			return 0;
		}

		dwRev = ReceiveToServer(sockConn,pinfo);
		delete pinfo;
	}
	else if (opType == download)
	{
		//处理用户下载请求
		LogMsg("收到下载文件请求...\n");
		dwRev = SendToClient(sockConn);
	}
	else if (opType == getlist)
	{
		//处理用户获取文件列表请求
		LogMsg("收到获取文件列表请求...\n");
		dwRev = SendFileListToClient(sockConn);
	}
	else if (opType == getuserInfo)
	{
		//处理用户获取用户信息请求，例如网盘使用情况，账户级别等
	}
	else if (opType == registerUser)
	{
		//处理注册新账户请求
		LogMsg("收到注册用户请求...\n");
		dwRev = RegisterUser(sockConn);
	}
	else if (opType == userLogin)
	{
		//用户登录请求
		LogMsg("收到用户登录请求...\n");
		dwRev = Login(sockConn);
	}
	else if (opType == addDir)
	{
		//增加目录请求
		LogMsg("收到用户新建目录请求...\n");
		dwRev = ServerNewDir(sockConn);
	}
	else if (opType == moveFile)
	{
		//移动文件请求
		LogMsg("收到移动文件请求...\n");
		dwRev = ServerMoveFile(sockConn);
	}
	else if (opType == copyFile)
	{
		//拷贝文件请求
		LogMsg("收到拷贝文件请求...\n");
		dwRev = ServerCopyFile(sockConn);
	}
	else if (opType == reName)
	{
		//重命名请求
		LogMsg("收到用户重命名请求...\n");
		dwRev = ServerReNameFile(sockConn);
	}
	else if (opType == deleteItem)
	{
		//删除文件请求
		LogMsg("收到删除请求...\n");
		dwRev = ServerDeleteFile(sockConn);
	}
	else
	{
		// 其他请求类型不处理
		LogMsg("错误的请求：%d\n",opType);
		closesocket(sockConn);
	}

	return dwRev;
}