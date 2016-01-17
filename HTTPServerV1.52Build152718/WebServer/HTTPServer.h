/* Copyright (C) 2011 阙荣文
 *
 * 这是一个开源免费软件,您可以自由的修改和发布.
 * 禁止用作商业用途.
 *
 * 联系原作者: querw@sina.com 
*/

#if !defined(AFX_HTTPFILESERVER1_H__2E98529A_F7A8_4D40_AD95_457ADF4FD0B8__INCLUDED_)
#define AFX_HTTPFILESERVER1_H__2E98529A_F7A8_4D40_AD95_457ADF4FD0B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
*/

/*
HTTP Server的驱动/控制类
目的: 代表一个HTTP Server在内存中的存在,根据需要创建线程,驱动网络使服务器运行起来.
1. 创建并管理服务线程,为完成端口服务,当有网络事件发生时,调用OnAcceptEx(), OnSend(), OnRecv(),OnClose()回调函数以驱动程序运行.
2. 创建并维持定时器队列,当有超时事件发生时,调用OnDelaySend(),OnDeadConnectionDetected(),OnSessionTimeout()回调函数.

by 阙荣文 - Que's C++ Studio
2011.7.7
*/
#pragma warning(disable : 4786)
#include "HTTPDef.h"

class CHTTPServer
{
protected:
	std::wstring m_strWorkDir;			// 工作路径
	BOOL m_bRuning;						// 是否正在运行
	BOOL m_bNavDir;						// 是否允许浏览目录
	string_vector m_vecDeftFileNames;	// 默认文件名

	int m_nListenPort;					// 侦听端口
	SOCKET m_sockListen;				// 侦听套接字.
	SOCKET m_sockNewClient;				// AccpetEx()用的新客户套接字.
	char *m_pAcceptBuf;					// AccpetEx()用缓冲
	int m_nAcceptBufLen;				// ..
	OVERLAPPED m_AcceptOL;				// ..

	SVRINF* m_pServiceInf;				// 完成端口信息.
	int m_nMaxConnections;				// 服务接受的最大连接数

	SOCKINFMAP m_SockInfMap;			// 客户信息列表,每个连接(客户)对应一个记录(PCLIENTINF)指针.
	CRITICAL_SECTION m_cs;				// 同步对象

	int m_nMaxClientConn;				// 一个客户端(IP)的最大连接数.0表示不限制.
	__int64 m_llMaxSpeed;				// 每个连接最大速度限制,单位 B/s. 0表示不限制.
	string_int_map m_ClientIPMap;		// 客户端IP地址表(每IP对应一个记录,用来限制每客户的最大连接数)

	DWORD m_dwDeadConnectionTimeout;	// 毫秒,死连接超时,如果一个连接在指定毫秒内没有发送或者接收到任何数据,则被认为是死连接,将被移除
	DWORD m_dwSessionTimeout;			// 毫秒,会话超时.一个连接最多只维持指定时间长.
	HANDLE m_hDCTimerQueue;				// 死连接定时器队列.
	HANDLE m_hSessionTimerQueue;		// 会话超时定时器队列
	HANDLE m_hSpeedTimerQueue;			// 速度限制定时器队列.
	
	IHTTPServerStatusHandler *m_pStatusHandler; // 状态回调接口,实现这个借口可以获得网络状态.
public:

	CHTTPServer();
	virtual ~CHTTPServer();

	// 启动或者清除 WS2 库, 用 AfxSocketInit() 也可以.
	static BOOL InitWinsock(WORD nMainVer, WORD nSubVer);
	static BOOL CleanWinsock();

	// 启动或者停止服务器
	HTTP_SERVER_ERROR_TYPE Run(PHTTPSTARTDESC pStartDesc);
	HTTP_SERVER_ERROR_TYPE Stop();
	BOOL IsRuning() { return m_bRuning; }
	
protected:
	CHTTPServer(const CHTTPServer& other);
	const CHTTPServer& operator = (const CHTTPServer& other);

	PCLIENTINF newClientDesc(SOCKET hSocket, const std::wstring &strIP, unsigned int nPort); // 初始化一个客户描述符.
	void deleteClientDesc(PCLIENTINF pClientInf, BOOL bSkeepTimer = FALSE);	// 回收客户描述符.
	HTTP_SERVER_ERROR_TYPE prepareListenSocket(int nPort, SOCKET& hListenSock);
	void doStop();	// 回收服务器资源,在Run()调用失败的情况下调用.
	BOOL doAccept(); // 发起一次 AcceptEx()调用
	BOOL doRecv(PCLIENTINF pClientInf);	// WSARecv()包装函数,方便调用
	BOOL doSend(PCLIENTINF pClientInf); // WSASend()包装函数,方便调用
	BOOL mapServerFile(const std::wstring &strUrl, std::wstring &strFilePath); //把URL映射为服务器上的完整文件名.
	void lock();
	void unlock();

protected:
	static UINT __cdecl ServiceProc(LPVOID pParam); // 服务线程, 异步处理套接字.
	static VOID CALLBACK DeadConnectionTimeoutCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
	static VOID CALLBACK SessionTimeoutCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
	static VOID CALLBACK SpeedTimeoutCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

	// 回调函数 ////////////////////////////////////////////////////////////////////////
	BOOL OnAcceptEx(BOOL bSkeep); // 处理accept事件
	void OnRequest(PCLIENTINF pSockInf);					// 处理一个请求头
	int OnClose(PCLIENTINF pClientInf, REMOVE_REASON reason); // 检测到有连接被关闭或者数据发送完毕需要关闭
	int OnRecv(PCLIENTINF pClientInf, DWORD dwTransfered); // 接收数据的网络操作完成即WSARecv()结束
	int OnSend(PCLIENTINF pClientInf, DWORD dwTransfered); // 发送数据的网络操作完成即WSASend()结束
	int OnDelaySend(PCLIENTINF pClient); // 速度控制,延时发送
	int OnDeadConnectionDetected(PCLIENTINF pClientInf); // 检测到死连接
	int OnSessionTimeout(PCLIENTINF pClientInf); // 检测到会话超时
};

#endif // !defined(AFX_HTTPFILESERVER1_H__2E98529A_F7A8_4D40_AD95_457ADF4FD0B8__INCLUDED_)
