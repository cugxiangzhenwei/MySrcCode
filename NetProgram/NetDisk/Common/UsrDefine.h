#pragma once

#define NOT_FULL_DATA_BAG -1
#define SOCKET_BUFF 204800    //套接字缓冲区大小
#define SERVER_PORT  6000     //服务器端口
#define MSG_CMD_LEN  500      // 命令字符串最大长度
#define SEND_RECV_LEN  131072 //一次发送接收的文件大小

#define KB   1024
#define MB   ULONGLONG(1024*KB)
#define GB   ULONGLONG(1024*MB)
#define TB   ULONGLONG(1024*GB)

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

typedef enum{
	upload   = 0,
	download,
	getlist, 
	getuserInfo,
	registerUser,
	userLogin,
	addDir,       // 新建目录
	reName,      // 重命名
	deleteItem ,  //删除操作
	moveFile,    // 移动
	copyFile,    //拷贝

}Client_Op;

struct FileTransfer
{
	char szData[SEND_RECV_LEN];
	int  iDataLen;
};
class FileInfo
{
public:
	__int64 iFileLength;       // 文件大小
	__int64 iLastModifyTime;  // 最后更新时间
	char szFileName[512];      // 文件名称
	char szMd5[128];         // md5值
	char szUserId[30];
	char szDir[512];
	FileInfo();
	FileInfo * GetCopied();
};

class FileListItem
{
public:
	char szFileId[128];        // 如果为文件,该字段内容为文件的id,用于下载
	char szItemName[512];      // 文件或者文件夹的名称
	char szItemType[10];       // 区分文件和文件夹
	char szMd5[128];          // md5值;
	__int64 iFileLength;       // 文件大小
	__int64 iLastModifyTime;   // 最后更新时间
	FileListItem();
	FileListItem * GetCopied();
};
class UploadStartInfo
{
public:
	UploadStartInfo();
	__int64 m_iFinishedSize; // 已上传的文件大小，上传错误时，应该置为小于0的值
	char m_szCmdMsg[MSG_CMD_LEN];// 上传成功时，存储文件的FileId,失败时存储错误信息
};

class CDataLogin
{
public:
	CDataLogin();
	char m_szUsrId[100];
	char m_szPassWd[50];
	void Code();
	void Decode();
};
class CDataLoginResult
{
public:
	CDataLoginResult();
	void Clear();
	char m_szUsrId[50];
	char m_szUserName[50];
	char m_szErrMsg[50];
	long long m_iUsedSpace;
	long long m_iAllSpace;
};

class CDataNewDir
{
public:
	char m_szDirName[MAX_PATH*2];
	char m_szParentDir[MAX_PATH*2]; 
	char m_szUsrId[50];

	CDataNewDir();
	void Clear();
};

class CDataNewDirResult
{
public:
	CDataNewDirResult();
	bool m_bFlag;
	char m_szErrMsg[500];
};

class CDataReNameItem
{
public:
	char m_szUsrId[50];         // 用户ID
	char szItemName[512];       // 文件或者文件夹的名称
	char szItemDir[512];        // 网盘目录
	char szNewName[512];        // 新的文件或者文件夹的名称
	CDataReNameItem();
};
typedef CDataNewDirResult CDataReNameResult; // 重命名结果结构体

extern const char * g_strServerLocalDir; // 服务器本地文件目录