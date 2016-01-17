/* Copyright (C) 2011 阙荣文
 *
 * 这是一个开源免费软件,您可以自由的修改和发布.
 * 禁止用作商业用途.
 *
 * 联系原作者: querw@sina.com 
*/

#pragma once
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 外部调用需要的定义
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 消息ID, HttpServer 会调用 SendMessage 函数发送消息通知调用者.
// 尽量不要阻塞消息响应函数,否则引起未知的结果.
#define WM_HTTP_SERVER (WM_USER + 1009) 

// 返回值定义(错误类型定义)
enum HTTP_SERVER_ERROR_TYPE
{
	SE_SUCCESS = 0,
	
	SE_RUNING = 1, // 正在运行
	SE_STOPPED = 2, // 已经停止

	SE_CREATESOCK_FAILED = 100, // 套接字创建失败
	SE_BIND_FAILED = 101, // 绑定端口失败
	SE_LISTEN_FAILED = 102, // listen() 函数调用失败.
	SE_CREATETIMER_FAILED = 103, // 无法创建定时器
	SE_CREATE_IOCP_FAILED = 104,


	SE_UNKNOWN = 1000
};

// 服务器移除套接字的原因
enum REMOVE_REASON
{
	RR_CLIENTCLOSED = 0, // 客户端关闭了连接
	RR_SENDCOMPLETE, // 发送完成
	RR_RECV_FAILED, // 接收失败.
	RR_SEND_FAILED, // 发送失败.

	RR_DEAD,
	RR_SESSION_TIMEOUT,

	RR_UNKNOWN // 未知.	
};


// 消息WM_HTTP_SERVER参数中
enum HTTP_MESSAGE_TYPE
{
	MT_CONNECTION_NEW = 100,
	MT_CONNECTION_CLOSED, // 连接关闭
	MT_REQUEST_URL,	// 客户请求的URL
	MT_CONNECTION_REFUSED,

	MT_UNKNOWN
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 内部实现使用 /////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define MAX_SOCKET 64
#define MAX_SOCKBUFF 8192 // 最大接收缓冲区.
#define MAX_METHODSIZE 100 // 用于保存HTTP方法字符串的长度,不宜超过 200
#define MAX_REQUESTHEADERSIZE 1024 * 10
#define MAX_RESPONSEHEADERSIZE 1024 * 10
#define MAX_IP_LENGTH 50 // IP地址的最大长度
#define MIN_SIZE_ONSPEEDLIMITED 512 // 达到速度限制时,发送的最小包字节数.
#define MAX_WAITTIME_ONSPEEDLIMITED 2000 // 达到速度限制时,最多等待多少毫秒发下一个包.这个值如果设置得过长,有可能导致客户端任务服务器没响应
#define DCTIMEOUT_DELAY 50 // 如果设置了速度限制定时器,那么死连接定时器要相应延后,因为并不是客户端没响应,而是服务主动延时发送,所以不能算死连接.

#define UNIT_BUFFER_SIZE 1024 * 10 // HTTP Content 的内存增长速度
#define MAX_BUFFER_SIZE 1024 * 1024 * 5 // HTTP Content 最大内存 5M

#define ETAG_BYTE_SIZE 5 // 对于内存数据,创建ETag时抽取的字节数.

#define G_BYTES (1024 * 1024 * 1024) // 1GB
#define M_BYTES (1024 * 1024)		 // 1MB
#define K_BYTES 1024				 // 1KB

// 服务器响应码
enum SERVER_CODE
{
	SC_AUTO = 0,
	SC_OK = 200,
	SC_NOCONTENT = 204,
	SC_PARTIAL = 206,
	SC_BADREQUEST = 400,
	SC_FORBIDDEN = 403,
	SC_NOTFOUND = 404,
	SC_BADMETHOD = 405,
	SC_SERVERERROR = 500,

	SC_UNKNOWN = 600
};


// 重叠操作的类型
enum opptype		
{
	opp_none = 0,		// 未定义
	opp_recv = 0x01,			// 接收
	opp_send = 0x02,			// 发送
	opp_accept = 0x04,			// 接收新连接
	opp_close = 0x08,			// 关闭
	opp_dead = 0x10,			// 死连接
	opp_session_timeout = 0x20,	// 会话超时

	opp_all = 0xFF
};

// HTTP 方法
enum HTTP_METHOD
{
	METHOD_UNDEFINE = 0,
	METHOD_GET = 1,
	METHOD_POST,
	METHOD_PUT,
	METHOD_HEAD, // 只返回响应头
	METHOD_DELETE, // 删除
	METHOD_TRACE,
	METHOD_CONNECT,

	METHOD_UNKNOWN
};

// 结构定义
// 一个客户套接字的描述对象(客户上下文) WSAOVERLAPPED 必须是第一个成员.
class CHTTPRequest;
class CHTTPResponse;

typedef struct tagClientInf			
{
	WSAOVERLAPPED	Overlapped;				// 重叠结构
	WSABUF			WSABuf;					// socket数据缓冲

	SOCKET			hSocket;				// 套接字句柄
	int				nOprationType;			// 重叠操作类型 enum opptype

	CHTTPRequest*	pRequest;				// HTTP请求描述符
	CHTTPResponse*  pResponse;				// HTTP响应描述符
	DWORD			dwLastSent;				// 最近一次发送成功时发送的字节数(延时发送时用这个字段)
	__int64			nSended;				// 一共发送的字符数
	DWORD			dwRecved;				// 一共接收到的字符数
	DWORD			dwStartTime;			// 连接开始时的时间 GetTickcount()返回值/不用考虑GetTickcount 溢出,只是显示错误而已.
	DWORD			dwEndTime;				// 连接结束时的时间
	wchar_t			pszIP[MAX_IP_LENGTH + 1]; // 客户端连接IP地址,本来可以有hSocket取得,但是需要用到的地方太多了,另外保存一份提高效率.
	unsigned int	nPort;					// 客户端连接端口

	DWORD			dwLastActiveTime;		// 记录最后一次发送或者接收数据的时间,用来作为判断是否超时过期的依据.(要考虑GetTickcount()循环的情况).
	HANDLE			hDeadConnectionTimeout; // 死连接超时触发器
	HANDLE			hSessionTimeout;		// 会话超时触发器
	HANDLE			hSpeedtimeout;			// 限速定时器
	void*			pInstant;				// 用来记录 CHTTPServer 实例指针
}CLIENTINF, *PCLIENTINF;

// 一个服务端口的描述对象(服务上下文)
typedef struct tagServiceInf		
{
	HANDLE hCompletionPort;				// 完成端口句柄
	CWinThread* *pThread;				// 线程数组
	int	nThreadCount;					// 每个端口对应的线程数,(CPU数量 + 1)
	int nSockCount;						// 当前套接字的数量
}SVRINF;


typedef std::map<SOCKET, CLIENTINF*> SOCKINFMAP;
typedef SOCKINFMAP::iterator sockinf_iter;
typedef SOCKINFMAP::const_iterator sockinf_citer;
typedef std::map<std::wstring, int> string_int_map;
typedef std::vector<std::wstring> string_vector;

// 外部定义的字符串
extern const char* g_HTTP_Content_NotFound;
extern const char* g_HTTP_Bad_Request;
extern const char* g_HTTP_Bad_Method;
extern const char* g_HTTP_Server_Error;
extern const char* g_HTTP_Forbidden;

// 把一个时间格式化为 HTTP 要求的时间格式(GMT).
std::string FormatHTTPDate(__int64* ltime);
std::string ToHex(const unsigned char* pData, int nSize);

// OnAccept() 返回值
#define ON_ACCEPT_SUCCESS 0
#define ON_ACCEPT_BUSY 1
#define ON_ACCEPT_CLOSED 2

// 服务器状态信息接接口
// HTTP服务器在运行期间会调用这个接口的方法以使该接口的实现可以获取时时的HTTP服务器
class IHTTPServerStatusHandler
{
public:
	// 有新连接时调用,由参数 bRefused 标识是否被服务器拒绝.
	virtual void OnNewConnection(const wchar_t *pszIP, unsigned int nPort, BOOL bRefused, BOOL bKicked) = 0;

	// 连接端口时调用
	virtual void OnConnectionClosed(const wchar_t *pszIP, unsigned int nPort, REMOVE_REASON rr, __int64 nByteSent, unsigned int nTimeUsed) = 0;

	// 数据发送完成时调用
	virtual void OnDataSent(const wchar_t *pszIP, unsigned int nPort, unsigned int nBytesSent) = 0;

	// 接收到数据时调用
	virtual void OnDataReceived(const wchar_t *pszIP, unsigned int nPort, unsigned int nBytesReceived) = 0;

	// 处理完客户端请求,在发送回应到客户端前调用.
	virtual void OnRequested(const wchar_t *pszIP, unsigned int nPort, const wchar_t *pszUrl, HTTP_METHOD hm, SERVER_CODE sc) = 0;
};

// HTTP 启动参数
typedef struct tagHTTPServerStartDesc
{
	TCHAR szRootDir[MAX_PATH + 1]; //
	TCHAR szDefaultFileName[MAX_PATH + 1]; // 默认文件名
	BOOL bNavDir; // = TRUE, 
	int nPort;// = 80, 
	int nMaxConnection;// = 2000, 
	int nMaxClientConn;  // = 0,
	__int64 llMaxSpeed;	// 每个连接最大速度 B/s
	DWORD dwSessionTimeout; // = 600000, 
	DWORD dwDeadConnectionTimeout; // = 30000,
	IHTTPServerStatusHandler *pStatusHandler; // = NULL
}HTTPSTARTDESC, *PHTTPSTARTDESC;

