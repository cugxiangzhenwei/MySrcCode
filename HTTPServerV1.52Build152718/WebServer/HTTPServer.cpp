/* Copyright (C) 2011 阙荣文
 *
 * 这是一个开源免费软件,您可以自由的修改和发布.
 * 禁止用作商业用途.
 *
 * 联系原作者: querw@sina.com 
*/

#include "stdafx.h"
#include "HttpServer.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "HTTPContent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define EXTHREAD 1		/*除CPU数目外额外的线程数*/

// HTTP1.1选项定义
#define OPT_KEEPALIVE 0x00000001
#define _OUTPUT_DETAIL

//////////////////////////////////////////////////////////////////////////
// 结构定义
typedef struct tagListenParam				// 侦听线程的参数
{
	CHTTPServer *pHttpserver;				// 类实例指针
	int nPort;
	SOCKET hSock;	// 侦听套接字
}PARAM_LISTENTHREAD;

typedef struct tagSvrParam					// 服务线程的参数
{
	CHTTPServer			*pHttpserver;
	int					nThreadIndex;				// 线程序号(每个服务至少有两个线程)
	HANDLE				hComplitionPort;			// 完成端口句柄
}PARAM_SVRTHREAD;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHTTPServer::CHTTPServer(/*HWND hNotifyWnd / * = NULL * /, UINT uMessageID / * = WM_HTTPFILESERVER * /*/)
{
	m_bRuning = FALSE;
	m_bNavDir = TRUE;
	
	m_nListenPort = 80;					// 侦听端口
	m_nAcceptBufLen = (sizeof(sockaddr_in) + 16) * 2;  // 额外的16个字节是 AcceptEx()的要求,见MSDN.
	m_pAcceptBuf = new char[m_nAcceptBufLen + 1];
	ASSERT(m_pAcceptBuf);
	
	m_pServiceInf = NULL;				// 服务数组
	m_nMaxConnections = 2000;			// 每个完成端口(服务)接受的最大连接数

	m_nMaxClientConn = 0;
	m_llMaxSpeed = 0;

	m_sockListen = INVALID_SOCKET;
	m_sockNewClient = INVALID_SOCKET;
	m_pStatusHandler = NULL;

	m_dwSessionTimeout = 0;		// 0表示不检查超时.
	m_dwDeadConnectionTimeout = 0;	// 0表示不检查超时.

	m_hDCTimerQueue = NULL;				// 死连接定时器队列.
	m_hSessionTimerQueue = NULL;		// 会话超时定时器队列
	m_hSpeedTimerQueue = NULL;			// 速度限制定时器队列.

	// 初始化同步对象
	InitializeCriticalSection(&m_cs);
}

CHTTPServer::~CHTTPServer()
{
	ASSERT(m_sockListen == INVALID_SOCKET);
	ASSERT(m_sockNewClient == INVALID_SOCKET);
	ASSERT(m_pServiceInf == NULL);

	delete []m_pAcceptBuf;
	DeleteCriticalSection(&m_cs);
}

BOOL CHTTPServer::InitWinsock(WORD nMainVer, WORD nSubVer)
{
	WORD wVer;
	WSADATA ws;
	wVer = MAKEWORD(nMainVer, nSubVer);
	return WSAStartup(wVer, &ws) == 0;
}

BOOL CHTTPServer::CleanWinsock()
{
	return WSACleanup() == 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

PCLIENTINF CHTTPServer::newClientDesc(SOCKET hSocket, const std::wstring &strIP, unsigned int nPort)
{
	PCLIENTINF pClientInf = new CLIENTINF;
	ASSERT(pClientInf);
	if( NULL == pClientInf ) return NULL;

	pClientInf->pInstant = (void *)this;
	pClientInf->nSended = 0;
	pClientInf->dwLastSent = 0;
	pClientInf->dwRecved = 0;
	pClientInf->hSocket = hSocket;
	pClientInf->nOprationType = opp_none;
	//pClientInf->pBuff = new char[MAX_SOCKBUFF + 1]; ASSERT(pClientInf->pBuff);
	//memset(pClientInf->pBuff, 0, MAX_SOCKBUFF + 1);
	pClientInf->WSABuf.buf = new char[MAX_SOCKBUFF + 1];
	memset(pClientInf->WSABuf.buf, 0, MAX_SOCKBUFF + 1);
	pClientInf->WSABuf.len = MAX_SOCKBUFF;
	memset(&pClientInf->Overlapped, 0, sizeof(WSAOVERLAPPED));
	
	pClientInf->pRequest = new CHTTPRequest(strIP.c_str(), nPort); // 创建HTTP Request 对象.
	pClientInf->pResponse = new CHTTPResponse(strIP.c_str(), nPort); // 创建HTTP Response 对象

	pClientInf->dwStartTime = GetTickCount(); // 记录时间.
	pClientInf->dwEndTime = pClientInf->dwStartTime;
	pClientInf->dwLastActiveTime = pClientInf->dwStartTime;

	ASSERT(strIP.size() <= MAX_IP_LENGTH);
	wcscpy(pClientInf->pszIP, strIP.c_str()); // 记录远程IP和端口.
	pClientInf->nPort = nPort;

	// 设置超时触发器
	pClientInf->hDeadConnectionTimeout = NULL;
	pClientInf->hSessionTimeout = NULL;
	pClientInf->hSpeedtimeout = NULL;

	return pClientInf;
}

void CHTTPServer::deleteClientDesc(PCLIENTINF pClientInf, BOOL bSkeepTimer)
{
	if(NULL == pClientInf) 
	{
		ASSERT(0);
		return;
	}

	// 回收资源.一定要先停止定时器,再回收资源.
	if(!bSkeepTimer)
	{
		if(pClientInf->hDeadConnectionTimeout != NULL)
		{
			// INVALID_HANDLE_VALUE 会等待OnTimeout()函数返回,如果此时OnTimeout()正在运行的话
			if(!DeleteTimerQueueTimer(m_hDCTimerQueue, pClientInf->hDeadConnectionTimeout, INVALID_HANDLE_VALUE))
			{
				LOGGER_CERROR(theLogger, _T("无法删除死连接定时器,错误码[%d].\r\n"), GetLastError());
			}
		}
		if(pClientInf->hSessionTimeout != NULL)
		{
			if(!DeleteTimerQueueTimer(m_hSessionTimerQueue, pClientInf->hSessionTimeout, INVALID_HANDLE_VALUE))
			{
				LOGGER_CERROR(theLogger, _T("无法删除会话定时器,错误码[%d].\r\n"), GetLastError());
			}
		}
		if(pClientInf->hSpeedtimeout != NULL)
		{
			if(!DeleteTimerQueueTimer(m_hSpeedTimerQueue, pClientInf->hSpeedtimeout, INVALID_HANDLE_VALUE))
			{
				LOGGER_CERROR(theLogger, _T("无法删除速度限制定时器,错误码[%d].\r\n"), GetLastError());
			}
		}
	}
	else
	{
		// doStop() 强制删除时运行到这里.
		// 所有的定时器已经先被删除了.
	}

	if(pClientInf->pRequest) delete pClientInf->pRequest;
	if(pClientInf->pResponse) delete pClientInf->pResponse;
	if(pClientInf->hSocket != INVALID_SOCKET && pClientInf->nOprationType != opp_dead && pClientInf->nOprationType != opp_session_timeout)
	{
		// 死连接或者会话超时的连接的套接字已经关闭
		shutdown(pClientInf->hSocket, SD_BOTH);
		closesocket(pClientInf->hSocket);
	}
	//if(pClientInf->pBuff) delete []pClientInf->pBuff;
	if(pClientInf->WSABuf.buf) delete []pClientInf->WSABuf.buf;
	delete pClientInf;
}

// 准备侦听套接字
HTTP_SERVER_ERROR_TYPE CHTTPServer::prepareListenSocket(int nPort, SOCKET& hListenSock)
{
	SOCKET hSock = INVALID_SOCKET;

	// 创建套接字
	if( (hSock = socket(PF_INET, SOCK_STREAM, /*IPPROTO_TCP*/0 )) == INVALID_SOCKET )
	{
		return SE_CREATESOCK_FAILED;
	}

	// 设置为非阻塞模式
	u_long nonblock = 1;
	ioctlsocket(hSock, FIONBIO, &nonblock);
	
	// 绑定端口
	sockaddr_in addr;
	addr.sin_family			= AF_INET;
	addr.sin_port			= htons(nPort);
	addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	if( 0 != bind(hSock, (sockaddr *)&addr, sizeof(sockaddr_in)) )
	{
		closesocket(hSock);
		return SE_BIND_FAILED;
	}

	// 侦听
	if( 0 != listen(hSock, 10))
	{
		closesocket(hSock);
		return SE_LISTEN_FAILED;
	}
	else
	{
		hListenSock = hSock;
		return SE_SUCCESS;
	}
}

HTTP_SERVER_ERROR_TYPE CHTTPServer::Run(PHTTPSTARTDESC pStartDesc)
{
	ASSERT(pStartDesc);
	if(m_bRuning) return SE_RUNING;

	// 创建超时检查的定时器
	BOOL bTimersReady = FALSE;
	do 
	{
		if(pStartDesc->dwSessionTimeout > 0)
		{
			if(NULL == (m_hSessionTimerQueue = CreateTimerQueue()))
			{
				ASSERT(0);
				break;
			}
		}
		if(pStartDesc->dwDeadConnectionTimeout > 0)
		{
			if( NULL == (m_hDCTimerQueue = CreateTimerQueue()) )
			{
				ASSERT(0);
				break;
			}
		}
		if(pStartDesc->llMaxSpeed > 0)
		{
			if(NULL == (m_hSpeedTimerQueue = CreateTimerQueue()))
			{
				ASSERT(0);
				break;
			}
		}
		bTimersReady = TRUE;
	} while (false);
	if(!bTimersReady)
	{
		doStop();
		return SE_CREATETIMER_FAILED;
	}

	// 创建侦听套接字
	HTTP_SERVER_ERROR_TYPE ret = prepareListenSocket(pStartDesc->nPort, m_sockListen);
	if(SE_SUCCESS != ret)
	{
		doStop();
		return ret;
	}

	// 获取CPU数量
	int i = 0, j = 0;
	SYSTEM_INFO sysInfo;
	DWORD dwThreadId = 0;
	GetSystemInfo(&sysInfo);
	int nThreadCount = sysInfo.dwNumberOfProcessors + EXTHREAD;
	if( nThreadCount > 64 ) nThreadCount = 64; // 一个完成端口最多允许有64个线程.

	// 记录状态
	m_bRuning = TRUE;
	m_strWorkDir = pStartDesc->szRootDir;
	m_nListenPort = pStartDesc->nPort;
	m_nMaxConnections = pStartDesc->nMaxConnection;
	m_nMaxClientConn = pStartDesc->nMaxClientConn;
	m_bNavDir = pStartDesc->bNavDir;
	m_pStatusHandler = pStartDesc->pStatusHandler; // 保存状态通知的接口
	m_dwDeadConnectionTimeout = pStartDesc->dwDeadConnectionTimeout;
	m_dwSessionTimeout = pStartDesc->dwSessionTimeout;
	m_llMaxSpeed = pStartDesc->llMaxSpeed;

	m_SockInfMap.clear();
	m_ClientIPMap.clear();
	m_vecDeftFileNames.clear();

	// 创建默认文件名列表
	std::wstring strDftFileNames(pStartDesc->szDefaultFileName);
	strDftFileNames.push_back(L','); // 额外一个逗号,方便循环处理.
	std::wstring::size_type st = 0;
	std::wstring::size_type stNext = 0;
	while( (stNext = strDftFileNames.find(L',', st)) != std::wstring::npos )
	{
		if(stNext > st)
		{
			std::wstring strDftFileName =  strDftFileNames.substr(st, stNext - st);
			m_vecDeftFileNames.push_back(strDftFileName);
		}

		// 尝试下一个文件
		st = stNext + 1;
	}

	// 分配服务上下文,并创建相应的服务线程
	m_pServiceInf = new SVRINF;
	ASSERT(m_pServiceInf);
	m_pServiceInf->nSockCount = 0;
	m_pServiceInf->nThreadCount = nThreadCount; // 记录下线程的个数.
	m_pServiceInf->hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, nThreadCount); // 创建完成端口.
	m_pServiceInf->pThread = new CWinThread* [m_pServiceInf->nThreadCount];
	for(int j = 0; j < m_pServiceInf->nThreadCount; ++j)
	{
		// 准备线程参数
		PARAM_SVRTHREAD *pThreadInf = new PARAM_SVRTHREAD;
		pThreadInf->hComplitionPort = m_pServiceInf->hCompletionPort;
		pThreadInf->nThreadIndex	= j;
		pThreadInf->pHttpserver		= this;

		// 创建线程.
		m_pServiceInf->pThread[j] = AfxBeginThread(ServiceProc, (LPVOID)pThreadInf, 0, 0, CREATE_SUSPENDED); //CreateThread(NULL, 0, ServiceProc, pThreadInf, 0, &dwThreadId);
		ASSERT(m_pServiceInf->pThread[j]);
		m_pServiceInf->pThread[j]->m_bAutoDelete = FALSE;
		m_pServiceInf->pThread[j]->ResumeThread();
	}

	// 关联侦听套接字到完成端口,并且执行第一次accept()
	if(m_pServiceInf->hCompletionPort != CreateIoCompletionPort((HANDLE)m_sockListen, m_pServiceInf->hCompletionPort, (DWORD)m_sockListen, 0))
	{
		LOGGER_CERROR(theLogger, _T("无法关联侦听套接字到完成端口,服务器无法工作,错误码[%d].\r\n"), GetLastError());
		doStop();
		return SE_CREATE_IOCP_FAILED;
	}
	else
	{
		doAccept();
	}

	// 日志
	//LOGGER_CTRACE(theLogger, _T("服务线程开始运行(%d个服务线程,CPU数量:%d)...\r\n"), nThreadCount, sysInfo.dwNumberOfProcessors);
	return SE_SUCCESS;
}

void CHTTPServer::doStop()
{
	int nThreadCount = 0;
	DWORD dwRet = 0;
	HANDLE *pThreadArr = NULL;

	// 发消息给每个服务线程,然后等待每一个服务线程停止
	// 服务线程停止后,所有的网络事件都不再处理.
	if(m_pServiceInf)
	{
		nThreadCount = m_pServiceInf->nThreadCount;
		pThreadArr = new HANDLE[nThreadCount];
		for( int ii = 0; ii < nThreadCount; ++ii)
		{
			pThreadArr[ii] = m_pServiceInf->pThread[ii]->m_hThread;
			PostQueuedCompletionStatus(m_pServiceInf->hCompletionPort, 0, 0, NULL);		// 3个参赛均为0时,表示退出. 每个线程处理一个包
		}

		// MAXIMUM_WAIT_OBJECTS = 64; 所以,每个服务最多可以有的线程数是 64个.
		dwRet = WaitForMultipleObjects(nThreadCount, pThreadArr, TRUE, INFINITE);     // 发送退出标志后,等待关联线程退出
		if(dwRet == WAIT_TIMEOUT)
		{
			// 超时,把所有还未退出的线程强制结束.
		}
		for( int ii = 0; ii < nThreadCount; ++ii)
		{
			if(m_pServiceInf->pThread[ii] != NULL)
			{
				delete m_pServiceInf->pThread[ii];
				m_pServiceInf->pThread[ii] = NULL;
			}
		}
		delete[] pThreadArr;
	}

	// 关闭定时器队列,取消所有的定时器,并确保回调函数不再被调用.
	// 然后在释放客户上下文就是安全的.
	if(m_hDCTimerQueue != NULL)
	{
		if( !DeleteTimerQueueEx(m_hDCTimerQueue, INVALID_HANDLE_VALUE) )
		{
			LOGGER_CERROR(theLogger, _T("无法删除死连接定时器队列,错误码[%d].\r\n"), GetLastError());
		}
		m_hDCTimerQueue = NULL;
	}
	if(m_hSessionTimerQueue != NULL)
	{
		if( !DeleteTimerQueueEx(m_hSessionTimerQueue, INVALID_HANDLE_VALUE) )
		{
			LOGGER_CERROR(theLogger, _T("无法删除会话定时器队列,错误码[%d].\r\n"), GetLastError());
		}
		m_hSessionTimerQueue = NULL;
	}
	if(m_hSpeedTimerQueue != NULL)
	{
		if( !DeleteTimerQueueEx(m_hSpeedTimerQueue, INVALID_HANDLE_VALUE) )
		{
			LOGGER_CERROR(theLogger, _T("无法删除速度限制定时器队列,错误码[%d].\r\n"), GetLastError());
		}
		m_hSpeedTimerQueue = NULL;
	}

	// 关闭侦听套接字和为接受新连接而准备的套接字
	if(INVALID_SOCKET != m_sockListen)
	{
		closesocket(m_sockListen);
		m_sockListen = INVALID_SOCKET;
	}
	if(INVALID_SOCKET != m_sockNewClient)
	{
		shutdown(m_sockNewClient, SD_BOTH);
		closesocket(m_sockNewClient);
		m_sockNewClient = INVALID_SOCKET;
	}

	// 清除服务线程上下文
	// 关闭完成端口,使所有I/O操作取消以便安全删除 LPWSAOVERLAPPED 指针.
	if(m_pServiceInf)
	{
		delete[] m_pServiceInf->pThread;
		m_pServiceInf->pThread = NULL;
		m_pServiceInf->nThreadCount = 0;
		CloseHandle(m_pServiceInf->hCompletionPort); 
		m_pServiceInf->hCompletionPort = NULL;
		delete m_pServiceInf;
		m_pServiceInf = NULL;
	}

	// 最后才能释放剩下的套接字和关联数据,清空客户IP表
	// 不需要加锁,运行到这里,所有定时器,线程都已经停止运行,不再会有资源竞争.
	if(m_SockInfMap.size() > 0)
	{
		LOGGER_CWARNING(theLogger, _T("退出时还有:[%d]个连接,将强制关闭.\r\n"), m_SockInfMap.size());
	}
	for(sockinf_iter iter = m_SockInfMap.begin(); iter != m_SockInfMap.end(); ++iter)
	{
		// 所有的Timer已经在 DeleteTimerQueueEx()中被删除,这里不用再删除Timer了
		deleteClientDesc(iter->second, TRUE);
	}
	m_SockInfMap.clear();
	m_ClientIPMap.clear();

	// 清除状态接口和默认文件名列表
	m_pStatusHandler = NULL;
	m_vecDeftFileNames.clear();
}

HTTP_SERVER_ERROR_TYPE CHTTPServer::Stop()
{
	if(!m_bRuning) return SE_STOPPED;

	doStop();

	// 清除标志
	m_bRuning = FALSE;
	return SE_SUCCESS;
}

/*
int CHTTPServer::OnAccept()
{
	// 调用 accept()
	sockaddr_in clientAddr;
	int nLen = sizeof(sockaddr_in);
	SOCKET hNewSocket = accept(m_sockListen, (sockaddr*)&clientAddr, &nLen);
	if( INVALID_SOCKET == hNewSocket )
	{
		// accept()失败,由于网络DOWN或者侦听套接字被关闭.
		LOGGER_CERROR(theLogger, _T("无法接收新的连接,可能是由于侦听套接字被关闭,或者网络子系统错误.错误码[%d]\r\n"), WSAGetLastError());
		return ON_ACCEPT_CLOSED;
	}

	// 获得远程IP和端口
	std::wstring strClientIP = AtoW(inet_ntoa(clientAddr.sin_addr)).c_str();
	unsigned int nClientPort = ntohs(clientAddr.sin_port);

	//int nRet = 4; // 初始状态: 0 表示连接被接受; 1表示关联完成端口失败; 2表示WSARecv失败; 3表示计时器创建失败; 4表示服务器忙;
	//DWORD dwRecvBytes = 0,dwFlags = 0, dwErr = 0; 
	PCLIENTINF pClientInf = NULL;
	BOOL bAccepted = FALSE;
	
	// 添加到列表中.
	lock();
	do 
	{
		if(m_pServiceInf->nSockCount < m_nMaxConnections)
		{
			// 关联到完成端口.
			HANDLE hRet = CreateIoCompletionPort((HANDLE)hNewSocket, m_pServiceInf->hCompletionPort, (DWORD)hNewSocket, 0);
			if(hRet != m_pServiceInf->hCompletionPort)
			{
				LOGGER_CERROR(theLogger, _T("关联到完成端口失败.[%s][%d],套接字将被关闭.\r\n"), strClientIP.c_str(), nClientPort);
				break;
			}
			else
			{
				// 关联成功, 准备客户连接上下文
				pClientInf = newClientDesc(hNewSocket, strClientIP, nClientPort);

				// 设置超时触发器
				if(m_dwDeadConnectionTimeout > 0)
				{
					ASSERT(m_hTimerQueue);
					if(!CreateTimerQueueTimer(&(pClientInf->hDeadConnectionTimeout), m_hTimerQueue, &(CHTTPServer::DeadConnectionTimeoutCallback), (PVOID)pClientInf, m_dwDeadConnectionTimeout, 0, 0))
					{
						LOGGER_CERROR(theLogger, _T("无法创建死连接超时触发器,错误码[%d].\r\n"), GetLastError());
						break;
					}
				}
				
				if(m_dwSessionTimeout > 0)
				{
					ASSERT(m_hTimerQueue);
					if(!CreateTimerQueueTimer(&(pClientInf->hSessionTimeout), m_hTimerQueue, &(CHTTPServer::SessionTimeoutCallback), (PVOID)pClientInf, m_dwSessionTimeout, 0, 0))
					{
						LOGGER_CERROR(theLogger, _T("无法创建会话超时触发器,错误码[%d].\r\n"), GetLastError());
						break;
					}
				}

				// 关联成功后, 执行一次接收动作.
				// 由于有 LPWSAOVERLAPPED 结构, WSARecv() 会立即返回,而不管套接字是 阻塞模式还是非阻塞模式.
				pClientInf->WSABuf.len = MAX_SOCKBUFF;
				if(!doRecv(pClientInf))
				{
					LOGGER_CERROR(theLogger, _T("无法接收客户端数据,返回值[%d], 套接字[%s][%d]将被关闭.\r\n"), WSAGetLastError(), strClientIP.c_str(), nClientPort);
					break;
				}
				
				// 第一个数据包接收操作成功后,正式加入列表.
				m_pServiceInf->nSockCount++;
				m_SockInfMap.insert(std::make_pair(hNewSocket, pClientInf));
				bAccepted = TRUE;
			}
		}
	} while(false);
	unlock();

	if(bAccepted)
	{
		// 连接被接受,分配的资源指针已经被保存了.
	}
	else
	{
		// 连接没有被接受,回收资源.
		if( pClientInf != NULL ) deleteClientDesc(pClientInf);
	}

	if(m_pStatusHandler)
	{
		m_pStatusHandler->OnNewConnection(strClientIP.c_str(), nClientPort, !bAccepted);
	}

	return bAccepted ? ON_ACCEPT_SUCCESS : ON_ACCEPT_BUSY;
}
*/

BOOL CHTTPServer::OnAcceptEx(BOOL bSkeep)
{
	// 如果是失败的 AcceptEx()调用返回,只重试而跳过其它处理
	if(bSkeep)
	{
		LOGGER_CWARNING(theLogger, _T("捕捉到一次失败的AcceptEx调用,将重试,错误码[%d].\r\n"), WSAGetLastError());
		shutdown(m_sockNewClient, SD_BOTH);
		closesocket(m_sockNewClient);
		m_sockNewClient = INVALID_SOCKET;
		doAccept();
		return TRUE;
	}

	// 获取客户IP等信息
	// 更新套接字信息,使 getsockname() 和 getpeername() 可用.
	if( 0 != setsockopt( m_sockNewClient, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&m_sockListen, sizeof(m_sockListen)) )
	{
		LOGGER_CERROR(theLogger, _T("无法更新套接字信息,错误码[%d].\r\n"), WSAGetLastError());
	}
	sockaddr_in clientAddr;
	int nAddrLen = sizeof(sockaddr_in);
	if( 0 != getpeername(m_sockNewClient, (sockaddr *)&clientAddr, &nAddrLen) )
	{
		LOGGER_CERROR(theLogger, _T("无法获取客户端地址和端口,错误码[%d].\r\n"), WSAGetLastError());
	}

	// 获得远程IP和端口
	std::wstring strClientIP = AtoW(inet_ntoa(clientAddr.sin_addr)).c_str();
	unsigned int nClientPort = ntohs(clientAddr.sin_port);

	// 添加到列表中.
	PCLIENTINF pClientInf = NULL;
	BOOL bAccepted = FALSE;
	SOCKET hNewSocket = m_sockNewClient;
	BOOL bKicked = FALSE;

	lock();
	do 
	{
		// 查看是否已经达到最大连接.
		if(m_pServiceInf->nSockCount < m_nMaxConnections)
		{
			// 查看该IP是否已经达到最大连接数
			string_int_map::iterator iterClientIP = m_ClientIPMap.end();
			if(m_nMaxClientConn > 0)
			{
				iterClientIP = m_ClientIPMap.find(strClientIP);
				if(iterClientIP != m_ClientIPMap.end() && iterClientIP->second >= m_nMaxClientConn)
				{
					bKicked = TRUE;
					break; // 超出最大限制.
				}
			}

			// 关联到完成端口.
			HANDLE hRet = CreateIoCompletionPort((HANDLE)hNewSocket, m_pServiceInf->hCompletionPort, (DWORD)hNewSocket, 0);
			if(hRet != m_pServiceInf->hCompletionPort)
			{
				LOGGER_CERROR(theLogger, _T("[%s:%d] - 关联到完成端口失败,套接字将被关闭.\r\n"), strClientIP.c_str(), nClientPort);
				break;
			}
			else
			{
				// 关联成功, 准备客户连接上下文
				pClientInf = newClientDesc(hNewSocket, strClientIP, nClientPort);

				// 设置超时触发器
				if(m_dwDeadConnectionTimeout > 0)
				{
					ASSERT(m_hDCTimerQueue);
					if(!CreateTimerQueueTimer(&(pClientInf->hDeadConnectionTimeout), m_hDCTimerQueue, &(CHTTPServer::DeadConnectionTimeoutCallback), (PVOID)pClientInf, m_dwDeadConnectionTimeout, 0, 0))
					{
						LOGGER_CERROR(theLogger, _T("无法创建死连接超时触发器,错误码[%d].\r\n"), GetLastError());
						break;
					}
				}

				if(m_dwSessionTimeout > 0)
				{
					ASSERT(m_hSessionTimerQueue);
					if(!CreateTimerQueueTimer(&(pClientInf->hSessionTimeout), m_hSessionTimerQueue, &(CHTTPServer::SessionTimeoutCallback), (PVOID)pClientInf, m_dwSessionTimeout, 0, 0))
					{
						LOGGER_CERROR(theLogger, _T("无法创建会话超时触发器,错误码[%d].\r\n"), GetLastError());
						break;
					}
				}

				// 关联成功后, 执行一次接收动作.
				pClientInf->WSABuf.len = MAX_SOCKBUFF; // 初始长度
				if(!doRecv(pClientInf))
				{
					LOGGER_CERROR(theLogger, _T("[%s:%d] - 无法接收客户端数据,错误码[%d],套接字将被关闭.\r\n"), strClientIP.c_str(), nClientPort, WSAGetLastError());
					break;
				}

				// 第一个数据包接收操作成功后,正式加入列表.
				m_pServiceInf->nSockCount++;
				m_SockInfMap.insert(std::make_pair(hNewSocket, pClientInf));
				if(m_nMaxClientConn > 0)
				{
					// 记录当前客户一共已经有多少个连接了.
					if(iterClientIP != m_ClientIPMap.end()) iterClientIP->second++;
					else m_ClientIPMap.insert(std::make_pair(strClientIP, 1));
				}
				bAccepted = TRUE;
			}
		}
	} while(false);
	unlock();

	// 查看结果,并回收资源,主要是如果新客户端没有被接受,那么应该关闭这个套接字.
	if(bAccepted)
	{
		// 连接被接受,分配的资源指针已经被保存了.
	}
	else
	{
		// 连接没有被接受,回收资源.
		if( pClientInf != NULL )
		{
			deleteClientDesc(pClientInf);
		}
		else
		{
			shutdown(hNewSocket, SD_BOTH);
			closesocket(hNewSocket);
		}
	}
	
	// 准备接收下一个连接
	doAccept();

	// 发送状态
	if(m_pStatusHandler)
	{
		m_pStatusHandler->OnNewConnection(strClientIP.c_str(), nClientPort, !bAccepted, bKicked);
	}
	return bAccepted;
}

int CHTTPServer::OnClose(PCLIENTINF pClientInf, REMOVE_REASON reason)
{
	PCLIENTINF pSockInf = pClientInf;

	// 记录必要的数据.
	__int64 nSended = pClientInf->nSended;
	DWORD dwStartTime = pClientInf->dwStartTime;
	DWORD dwEndTime = GetTickCount();
	std::wstring strClientIP(pClientInf->pszIP);
	unsigned int nClientPort = pClientInf->nPort;

	// 从队列中清除,并且清除IP表
	lock();
	m_SockInfMap.erase(pClientInf->hSocket);
	m_pServiceInf->nSockCount--;

	if(m_nMaxClientConn > 0)
	{
		string_int_map::iterator iter = m_ClientIPMap.find(strClientIP);
		if(iter != m_ClientIPMap.end())
		{
			if( --(iter->second) <= 0) m_ClientIPMap.erase(iter);
		}
	}
	unlock();

	// 回收资源
	deleteClientDesc(pClientInf);
	
	// 状态会送
	if(m_pStatusHandler)
	{
		m_pStatusHandler->OnConnectionClosed(strClientIP.c_str(), nClientPort, reason, nSended, (dwEndTime - dwStartTime));
	}

	return 0;
}

//HRESULT CHTTPServer::Notify(HTTP_MESSAGE_TYPE msgType, PHTTP_MSG pMsg)
//{
//	if(m_hNotifyWnd != NULL)
//	{
//		return SendMessage(m_hNotifyWnd, m_uMessageID, (WPARAM)msgType, (LPARAM)pMsg);
//
//		PHTTP_MSG pNewMsg = new HTTP_MSG;
//		pNewMsg->nClientPort = pMsg->nClientPort;
//		pNewMsg->nValue = pMsg->nValue;
//		pNewMsg->strClientIP = pMsg->strClientIP;
//		pNewMsg->strValue = pMsg->strValue;
//		return SendNotifyMessage(m_hNotifyWnd, m_uMessageID, (WPARAM)msgType, (LPARAM)pNewMsg);
//	}
//	return 0;
//}

//UINT __cdecl CHTTPServer::ListenProc(LPVOID pParam)
//{
//	// 复制参数
//	PARAM_LISTENTHREAD ListenInf;
//	memcpy(&ListenInf, pParam, sizeof(PARAM_LISTENTHREAD));
//	delete (PARAM_LISTENTHREAD *)pParam;
//
//	SOCKET sockArr[10];
//	WSAEVENT eventArr[10];
//	DWORD nIndex = 0;
//	BOOL bStop = FALSE;
//	WSANETWORKEVENTS netevent;
//	sockaddr_in addrNew;
//	int nSize;
//	int nSockRet = 0;
//	HTTP_ERROR err;
//
//	// 创建套接字, 绑定, 侦听
//	sockArr[0] = INVALID_SOCKET;
//	sockArr[0] = socket(PF_INET, SOCK_STREAM, /*IPPROTO_TCP*/0);
//	if(sockArr[0] == INVALID_SOCKET)
//	{
//		err.bSuccess = FALSE;
//		ListenInf.pHttpserver->Notify(SE_LISTENPROC_CREATESOCKET, &err);
//		return 1;
//	}
//	else
//	{
//		err.bSuccess = TRUE;
//		ListenInf.pHttpserver->Notify(SE_LISTENPROC_CREATESOCKET, &err);
//	}
//
//	sockaddr_in addr;
//	addr.sin_family			= AF_INET;
//	addr.sin_port			= htons(ListenInf.nPort);
//	addr.sin_addr.s_addr	= htonl(INADDR_ANY);
//	nSockRet = bind(sockArr[0], (sockaddr *)&addr, sizeof(sockaddr_in));
//	if(nSockRet)
//	{
//		err.bSuccess = FALSE;
//		err.nValue = nSockRet;
//		ListenInf.pHttpserver->Notify(SE_LISTENPROC_BIND, &err);
//		return 1;
//	}
//	else
//	{
//		err.bSuccess = TRUE;
//		ListenInf.pHttpserver->Notify(SE_LISTENPROC_BIND, &err);
//	}
//
//	nSockRet = listen(sockArr[0], 10);
//	if(nSockRet)
//	{
//		err.bSuccess = FALSE;
//		err.nValue = nSockRet;
//		ListenInf.pHttpserver->Notify(SE_LISTENPROC_LISTEN, &err);
//		return 1;
//	}
//	else
//	{
//		err.bSuccess = TRUE;
//		ListenInf.pHttpserver->Notify(SE_LISTENPROC_LISTEN, &err);
//	}
//
//	// 用 WAS EventIO模型, 对于侦听套接字, 用select() 也完全可以. 
//	eventArr[0] = WSACreateEvent();
//	if( WSA_INVALID_EVENT == eventArr[0] )
//	{
//		err.bSuccess = FALSE;
//		err.nValue = WSAGetLastError();
//		ListenInf.pHttpserver->Notify(SE_LISTENPROC_CREATEEVENT, &err);
//		return 1;
//	}
//	else
//	{
//		err.bSuccess = TRUE;
//		ListenInf.pHttpserver->Notify(SE_LISTENPROC_CREATEEVENT, &err);
//	}
//	
//	if( SOCKET_ERROR == WSAEventSelect(sockArr[0], eventArr[0], FD_ACCEPT) )
//	{
//		err.bSuccess = FALSE;
//		err.nValue = WSAGetLastError();
//		ListenInf.pHttpserver->Notify(SE_LISTENPROC_SELECT, &err);
//		return 1;
//	}
//	else
//	{
//		err.bSuccess = TRUE;
//		ListenInf.pHttpserver->Notify(SE_LISTENPROC_SELECT, &err);
//	}
//
//	while(1)
//	{
//		nIndex = WSAWaitForMultipleEvents(1, eventArr, TRUE, ListenInf.dwTimeOut, TRUE);
//
//		// 退出标志
//		if ( ListenInf.pHttpserver->IsStopping() )
//		{
//			closesocket(sockArr[0]);
//			WSACloseEvent(eventArr[0]);
//			return 0;
//		}
//
//		// 正常处理
//		else if(nIndex == WSA_WAIT_EVENT_0)
//		{
//			if( SOCKET_ERROR == WSAEnumNetworkEvents(sockArr[0], eventArr[0], &netevent))
//			{
//				// 忽略错误,调用 WSAWaitForMultipleEvents() 也会返回同样的错误.
//				TRACE("WSAEnumNetworkEvents()返回失败[%d]\r\n", WSAGetLastError());
//			}
//			else
//			{
//				if(netevent.lNetworkEvents & FD_ACCEPT)
//				{
//					if(netevent.iErrorCode[FD_ACCEPT_BIT] != 0)
//					{
//					}
//					else
//					{
//						nSize = sizeof(sockaddr_in);
//						SOCKET sockNew = accept(sockArr[0], (sockaddr *)&addrNew, &nSize);
//						if(sockNew == INVALID_SOCKET)
//						{
//							err.bSuccess = FALSE;
//							err.bSuccess = WSAGetLastError();
//							ListenInf.pHttpserver->Notify(SE_LISTENPROC_ACCEPT, &err);
//						}
//						else
//						{
//							ListenInf.pHttpserver->OnAccept(sockNew);
//						}
//					}
//				}
//			}
//		}
//
//		// 函数调用失败
//		else if( WSA_WAIT_FAILED == nIndex)
//		{
//			err.bSuccess = FALSE;
//			err.nValue = WSAGetLastError();
//			if( WSAENETDOWN == err.nValue )
//			{
//				// 如果是断网的话, 5秒后重新检查.
//				ListenInf.pHttpserver->Notify(SE_LISTENPROC_WAIT, &err);
//				Sleep(5 * 1000);
//			}
//			else
//			{
//				// 如果是其他错误,退出.
//				ListenInf.pHttpserver->Notify(SE_LISTENPROC_WAIT, &err);
//				closesocket(sockArr[0]);
//				WSACloseEvent(eventArr[0]);
//				return 1;
//			}
//		}
//
//		// 等待超时.
//		else
//		{
//			// nIndex == WSA_WAIT_TIMEOUT;
//		}
//
//		// 重置事件
//		WSAResetEvent(eventArr[0]);
//	}
//
//	// 不应该从这里退出.
//	ASSERT(FALSE);
//	return 0;
//}

/*
UINT __cdecl CHTTPServer::ListenProc(LPVOID pParam)
{
	LOGGER_CTRACE(theLogger, _T("侦听线程开始运行.\r\n"));

	// 复制参数
	PARAM_LISTENTHREAD ListenInf;
	memcpy(&ListenInf, pParam, sizeof(PARAM_LISTENTHREAD));
	delete (PARAM_LISTENTHREAD *)pParam;

	fd_set readfds;
	
	//timeval timout;
	//timout.tv_sec = ListenInf.dwTimeOut / 1000;
	//timout.tv_usec = 0;
	//LOGGER_CTRACE(theLogger, _T("侦听线程开始运行...\r\n"));

	while(TRUE)
	{
		FD_ZERO(&readfds);
		FD_SET(ListenInf.hSock, &readfds);

		int nSockRet = select(0, &readfds, NULL, NULL, NULL);

		if(nSockRet == SOCKET_ERROR) // 关闭或者有什么问题发生.
		{
			LOGGER_CERROR(theLogger, _T("侦听线程非正常退出,错误码[%d].\r\n"), WSAGetLastError());
			return 1;
			//break;
		}
		else if(nSockRet == 0) // time out
		{
			ASSERT(0); // 不应该超时.
		}
		else // accpet() 函数可以调用了.或者是listen套接字被关闭/重置.
		{
			ASSERT(FD_ISSET(ListenInf.hSock, &readfds));
			if( ON_ACCEPT_CLOSED == ListenInf.pHttpserver->OnAccept())
			{
				LOGGER_CTRACE(theLogger, _T("侦听线程退出.\r\n"));
				return 0;
			}
		}
	}

	return 0;
}
*/
UINT __cdecl CHTTPServer::ServiceProc(LPVOID pParam)
{
	// 复制参数
	PARAM_SVRTHREAD ServiceInf;
	memcpy(&ServiceInf, pParam, sizeof(PARAM_SVRTHREAD));
	delete (PARAM_SVRTHREAD *)pParam;
	TRACE("线程[%d]开始运行...\r\n", ServiceInf.nThreadIndex);
		
	while(true)
	{
		DWORD dwBytesTransferred = 0;
		SOCKET hSocket = INVALID_SOCKET;
		PCLIENTINF pSockInf = NULL;
		if(!GetQueuedCompletionStatus(ServiceInf.hComplitionPort, &dwBytesTransferred, (DWORD *)&hSocket, (LPOVERLAPPED*)&pSockInf, INFINITE))
		{
			if(pSockInf == NULL)
			{
				// 发生了未知错误,可能需要重新启动才能解决
				LOGGER_CERROR(theLogger, _T("服务线程[%d]非正常停止,需要重启服务器才能修复,错误码[%d].\r\n"), ServiceInf.nThreadIndex, GetLastError());
				break;
			}
			else
			{
				if(hSocket == ServiceInf.pHttpserver->m_sockListen)
				{
					// 侦听套接字,IO失败,重试.
					ServiceInf.pHttpserver->OnAcceptEx(TRUE);
				}
				else
				{
					// 其它套接字 IO操作失败. 
					// 1.Server由于超时的原因,主动关闭了套接字导致当前正在执行的IO失败.
					// 2.客户端关闭了套接字.
					REMOVE_REASON rr = RR_UNKNOWN;
					if(pSockInf->nOprationType == opp_dead)
					{
						rr = RR_DEAD;
					}
					else if(pSockInf->nOprationType == opp_session_timeout)
					{
						rr = RR_SESSION_TIMEOUT;
					}
					else
					{
						rr = RR_CLIENTCLOSED;
					}
					ServiceInf.pHttpserver->OnClose(pSockInf, rr);
				}
			}
		}
		else
		{
			// 事先约定好的退出标志
			if(dwBytesTransferred == 0 && hSocket == 0 && pSockInf == NULL)
			{
				break;
			}

			if(hSocket == ServiceInf.pHttpserver->m_sockListen)
			{
				// 侦听套接字可读
				ServiceInf.pHttpserver->OnAcceptEx(FALSE);
			}
			else
			{
				// 其它套接字,正常处理
				// 对于一个套接字,任何时候只有一个网络操作在进行.程序的逻辑保证了这一点,所以,可以放心的使用pSockInf指针.
				ASSERT(pSockInf);
				if(dwBytesTransferred == 0)
				{
					// 一般情况没理由网络操作成功却没有数据.
					ASSERT(0);
					ServiceInf.pHttpserver->OnClose(pSockInf, RR_UNKNOWN);
				}
				else
				{
					ASSERT(pSockInf->hSocket == hSocket);
					if(pSockInf->nOprationType == opp_recv)
					{
						// 接收操作.
						ServiceInf.pHttpserver->OnRecv(pSockInf, dwBytesTransferred);
					}
					else if(pSockInf->nOprationType == opp_send)
					{
						// 发送操作完成
						ServiceInf.pHttpserver->OnSend(pSockInf, dwBytesTransferred);
					}
					else
					{
						// 如果发送或者接收操作已经执行了一部分,正好因为超时而导致套接字被服务器主动关闭
						REMOVE_REASON rr = RR_UNKNOWN;
						if(pSockInf->nOprationType == opp_dead)
						{
							rr = RR_DEAD;
						}
						else if(pSockInf->nOprationType == opp_session_timeout)
						{
							rr = RR_SESSION_TIMEOUT;
						}
						else
						{
							rr = RR_CLIENTCLOSED;
							ASSERT(0);
						}
						ServiceInf.pHttpserver->OnClose(pSockInf, rr);
					}
				} // dwBytesTransferred != 0
			} // 是否是侦听套接字
		} // GetQueuedCompletionStatus() return

		//TRACE("服务线程[%d]处理了一个I/O事件.\r\n", ServiceInf.nThreadIndex);
	} // while

	TRACE("服务[%d]线程退出.\r\n", ServiceInf.nThreadIndex);
	return 0;
}

VOID CALLBACK CHTTPServer::DeadConnectionTimeoutCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	// 只要定时器函数被调用,那么 pClientInf 指针一定是有效的.
	PCLIENTINF pClientInf = (PCLIENTINF)lpParameter;
	CHTTPServer *pServer = (CHTTPServer *)pClientInf->pInstant;
	pServer->OnDeadConnectionDetected(pClientInf);
}

VOID CALLBACK CHTTPServer::SessionTimeoutCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	// 只要定时器函数被调用,那么 pClientInf 指针一定是有效的.
	PCLIENTINF pClientInf = (PCLIENTINF)lpParameter;
	CHTTPServer *pServer = (CHTTPServer *)pClientInf->pInstant;
	pServer->OnSessionTimeout(pClientInf);
}

VOID CALLBACK CHTTPServer::SpeedTimeoutCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	// 只要定时器函数被调用,那么 pClientInf 指针一定是有效的.
	PCLIENTINF pClientInf = (PCLIENTINF)lpParameter;
	CHTTPServer *pServer = (CHTTPServer *)pClientInf->pInstant;
	pServer->OnDelaySend(pClientInf);
}

BOOL CHTTPServer::doSend(PCLIENTINF pClientInf)
{
	pClientInf->nOprationType = opp_send;
	DWORD dwTransfered = 0;
	DWORD dwFlags = 0;
	if(SOCKET_ERROR == WSASend(pClientInf->hSocket, &pClientInf->WSABuf, 1, &dwTransfered, dwFlags, (WSAOVERLAPPED *)pClientInf, NULL))
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			TRACE("WSASend Error:%d.\r\n",  WSAGetLastError());
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CHTTPServer::doRecv(PCLIENTINF pClientInf)
{
	pClientInf->nOprationType = opp_recv;
	DWORD dwTransfered = 0;
	DWORD dwFlags = 0;
	if(SOCKET_ERROR == WSARecv(pClientInf->hSocket, &(pClientInf->WSABuf), 1, &dwTransfered, &dwFlags, (LPWSAOVERLAPPED)pClientInf, NULL))
	{
		DWORD dwErr = WSAGetLastError();//WSAEFAULT
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			TRACE("WSARecv Error:%d.\r\n",  WSAGetLastError());
			return FALSE;
		}
	}
	return TRUE;
}

// WSARecv()操作完成
int CHTTPServer::OnRecv(PCLIENTINF pClientInf, DWORD dwBytesTransferred)
{
	ASSERT(pClientInf->nOprationType & opp_recv);

	//记录总共接收的字节数
	DWORD dwLastActiveTimeBak = pClientInf->dwLastActiveTime;
	pClientInf->dwRecved += dwBytesTransferred;
	pClientInf->dwLastActiveTime = GetTickCount(); // 记录最后一次活跃时间.

	// 取消死连接定时器,如果此时超时处理函数被调用也不要紧,套接字被关闭.后面的WSASend()WSARecv()将失败.
	// 如果是本地连接,速度非常快,有可能出现 dwLastActiveTimeBak == pClientInf->dwLastActiveTime 的情况.
	if(pClientInf->hDeadConnectionTimeout != NULL && dwLastActiveTimeBak != pClientInf->dwLastActiveTime)
	{
		ASSERT(m_hDCTimerQueue);
		if(!ChangeTimerQueueTimer(m_hDCTimerQueue, pClientInf->hDeadConnectionTimeout, m_dwDeadConnectionTimeout, 0))
		{
			LOGGER_CERROR(theLogger, _T("无法重置死连接定时器,错误码[%d].\r\n"), GetLastError());
			ASSERT(0);
		}
	}

	// 通知
	if(m_pStatusHandler)
	{
		m_pStatusHandler->OnDataReceived(pClientInf->pszIP, pClientInf->nPort, dwBytesTransferred);
	}

	// 把接收到的数据存压入Request对象.
	if(dwBytesTransferred != pClientInf->pRequest->PushData(pClientInf->WSABuf.buf, dwBytesTransferred) || pClientInf->pRequest->IsEnd())
	{
		// 压入数据失败(说明长度溢出) 或者 压入数据成功,且已经接收到完整的请求头
		OnRequest(pClientInf);

		// 任何一个请求,必须给客户端一个回应. 开始发送数据.
		pClientInf->WSABuf.len = 0;
		pClientInf->nSended = 0;

		// 发送回应包的第一段数据
		DWORD dwSendTransfered = 0;
		int nPopSize = pClientInf->pResponse->PopData(pClientInf->WSABuf.buf, MAX_SOCKBUFF); ASSERT(nPopSize > 0);
		pClientInf->WSABuf.len = nPopSize; 
		if( !doSend(pClientInf) )
		{
			OnClose(pClientInf, RR_SEND_FAILED);
		}
	}
	else
	{
		// 数据压入成功,继续接收数据
		pClientInf->WSABuf.len = MAX_SOCKBUFF;
		if(!doRecv(pClientInf))
		{
			OnClose(pClientInf, RR_RECV_FAILED);
		}
	}
	return 0;
}

int CHTTPServer::OnSend(PCLIENTINF pClientInf, DWORD dwBytesTransferred)
{
	ASSERT(pClientInf->nOprationType & opp_send);

	//// 上个发送完成的收尾工作先做,然后再准备下个包 ////
	// 记录总共发送的字节数
	DWORD dwLastActiveTimeBak = pClientInf->dwLastActiveTime;
	pClientInf->nSended += dwBytesTransferred;
	pClientInf->dwLastActiveTime = GetTickCount();
	pClientInf->dwLastSent = dwBytesTransferred;

	// 通知状态接收接口(接口实现应该非常快速的从OnDataSent()返回,否则可能会导致死连接超时到期.
	if(m_pStatusHandler)
	{
		m_pStatusHandler->OnDataSent(pClientInf->pszIP, pClientInf->nPort, dwBytesTransferred);
	}

	// 检查一下在缓冲区内是否还有数据
	if(dwBytesTransferred < pClientInf->WSABuf.len)
	{
		// 把上一次没有发完的数据移到缓冲区的开头
		memmove(pClientInf->WSABuf.buf, pClientInf->WSABuf.buf + dwBytesTransferred, pClientInf->WSABuf.len - dwBytesTransferred);
		pClientInf->WSABuf.len -= dwBytesTransferred;
	}
	else
	{
		pClientInf->WSABuf.len = 0;
	}

	// 如果缓冲区为空,并且Response到达结尾,说明全部数据都已经发送完毕, 断开客户端的连接.
	if(pClientInf->WSABuf.len <= 0 && pClientInf->pResponse->IsEOF())
	{
		OnClose(pClientInf, RR_SENDCOMPLETE);
		return 0; 
	}
	
	// 数据没有发送完,继续处理
	// 检查是否超出了速度限制
	unsigned long lMaxLength = MAX_SOCKBUFF; // 最大允许发送的包长度.
	DWORD dwDelay = 0; // 延时发送的时间.
	if( m_llMaxSpeed > 0 )
	{
		// 按照最大速度限制,发送 nSended数据完成应该在以下这个时间点完成
		DWORD dwExpectTime = (DWORD)(pClientInf->nSended * 1.0 / m_llMaxSpeed * 1000 + pClientInf->dwStartTime);
		if(dwExpectTime > pClientInf->dwLastActiveTime) // 完成的时间点提前,说明速度超标
		{
			// 计算出定时器触发的时间
			dwDelay = dwExpectTime - pClientInf->dwLastActiveTime;
			if(dwDelay > MAX_WAITTIME_ONSPEEDLIMITED)
			{
				// 超过最长等待时间,下一次发送一个最小包.
				dwDelay = MAX_WAITTIME_ONSPEEDLIMITED;
				lMaxLength = MIN_SIZE_ONSPEEDLIMITED;
			}
			else
			{
				// 下一次可以发送一个最大包.
				lMaxLength = MAX_SOCKBUFF;
			}
		}
	}

	// 重置死连接定时器
	// 如果需要延时发送,则死连接定时器应该延后.(服务器等待的时间不算死连接时间.)
	if(pClientInf->hDeadConnectionTimeout != NULL && pClientInf->dwLastActiveTime != dwLastActiveTimeBak)
	{
		DWORD dwDCTime = (dwDelay == 0) ? m_dwDeadConnectionTimeout : (m_dwDeadConnectionTimeout + dwDelay + DCTIMEOUT_DELAY);
		ASSERT(m_hDCTimerQueue);
		if(!ChangeTimerQueueTimer(m_hDCTimerQueue, pClientInf->hDeadConnectionTimeout, dwDCTime, 0))
		{
			LOGGER_CERROR(theLogger, _T("无法重置死连接定时器,错误码[%d].\r\n"), GetLastError());
			ASSERT(0);
		}
	}

	// 从 Response 对象中读取数据 填充缓冲区
	if( lMaxLength > pClientInf->WSABuf.len )
	{
		// 还允许继续填充.
		int nPopSize = pClientInf->pResponse->PopData(pClientInf->WSABuf.buf + pClientInf->WSABuf.len, lMaxLength - pClientInf->WSABuf.len);
		pClientInf->WSABuf.len += nPopSize;
	}
	ASSERT(pClientInf->WSABuf.len > 0);

	// 执行发送
	ASSERT(pClientInf->hSpeedtimeout == NULL);
	if(dwDelay == 0 || pClientInf->hSpeedtimeout != NULL)
	{
		if(!doSend(pClientInf))
		{
			// 如果正好在这个时刻,客户端关闭了连接,则会导致WSASend()失败.
			OnClose(pClientInf, RR_SEND_FAILED);
		}
	}
	else
	{
		// 延时发送,设置下一次发送的定时器
		if( !CreateTimerQueueTimer(&pClientInf->hSpeedtimeout, m_hSpeedTimerQueue, &CHTTPServer::SpeedTimeoutCallback, pClientInf,  dwDelay, 0, 0))
		{
			ASSERT(0);
			LOGGER_CERROR(theLogger, _T("无法创建速度限制超时触发器,错误码[%d].\r\n"), GetLastError());
			OnClose(pClientInf, RR_UNKNOWN);
		}
	}
	return 0;
}

int CHTTPServer::OnDelaySend(PCLIENTINF pClientInf)
{
	// 删除定时器
	DeleteTimerQueueTimer(m_hSpeedTimerQueue, pClientInf->hSpeedtimeout, NULL);
	pClientInf->hSpeedtimeout = NULL;

	// 延时发送 pClientInf 的信息已经准备好了.
	if(!doSend(pClientInf))
	{
		// 如果正好在这个时刻,客户端关闭了连接,则会导致WSASend()失败.
		OnClose(pClientInf, RR_SEND_FAILED);
	}
	return 0;
}

// 已经完整接收到了一个请求头,处理之
// 目的: 通过分析Request对象,准备好Response对象,并在可能的情况下生成一个Content对象并关联到Response对象中.
void CHTTPServer::OnRequest(PCLIENTINF pSockInf)
{
	ASSERT(pSockInf); 
	ASSERT(pSockInf->pRequest); 
	ASSERT(pSockInf->pResponse);
	std::wstring strUrlObject(L"");
	std::wstring strServerFilePath(L"");

	// 是否是有效的请求头
	if(!pSockInf->pRequest->Verify())
	{
		// 请求头格式不正确,返回HTTP 400和一段关于400的预定义说明文本
		pSockInf->pResponse->SetServerCode(SC_BADREQUEST); 
		CHTTPContent *pContent = new CHTTPContent;
		pContent->OpenText(g_HTTP_Bad_Request, strlen(g_HTTP_Bad_Request));
		pSockInf->pResponse->AttachContent(pContent);
		goto exit;
	}
	
	// 请求的方法是否是 GET 或者 HEAD
	HTTP_METHOD method = pSockInf->pRequest->GetMethod();
	pSockInf->pResponse->SetMethod(method);
	if(method != METHOD_GET && method != METHOD_HEAD)
	{
		// 目前只支持两种HTTP方法
		pSockInf->pResponse->SetServerCode(SC_BADMETHOD);
		CHTTPContent *pContent = new CHTTPContent;
		pContent->OpenText(g_HTTP_Bad_Method, strlen(g_HTTP_Bad_Method));
		pSockInf->pResponse->AttachContent(pContent);
		goto exit;
	}
	
	// 获取客户端请求的对象
	strUrlObject = pSockInf->pRequest->GetUrlObject();
	if(strUrlObject.size() <= 0)
	{
		// URL Object 为空,说明客户端的请求有问题.
		pSockInf->pResponse->SetServerCode(SC_BADREQUEST); // 请求头格式错误
		CHTTPContent *pContent = new CHTTPContent;
		pContent->OpenText(g_HTTP_Bad_Request, strlen(g_HTTP_Bad_Request));
		pSockInf->pResponse->AttachContent(pContent);
		goto exit;
	}
	
	// 映射为服务器文件名.
	mapServerFile(strUrlObject, strServerFilePath);

	// 如果 URL 的最后一个字符是 '/' 说明请求文件列表,否则是请求一个具体的文件.
	if(strUrlObject.back() == L'/')
	{
		// 浏览目录创建目录列表的内容对象,并关联给Response对象
		CHTTPContent *pContent = new CHTTPContent;
		if(m_bNavDir)
		{
			if(pContent->OpenDir(strUrlObject, strServerFilePath))
			{
				pSockInf->pResponse->SetServerCode(SC_OK);
			}
			else
			{
				// 无法列出目录的文件列表.
				pContent->OpenText(g_HTTP_Server_Error, strlen(g_HTTP_Server_Error));
				pSockInf->pResponse->SetServerCode(SC_SERVERERROR);
			}
		}
		else
		{
			// 禁止浏览目录
			// 先尝试打开默认文件
			for(string_vector::iterator iter = m_vecDeftFileNames.begin(); iter != m_vecDeftFileNames.end(); ++iter)
			{
				std::wstring strDftFilePath(strServerFilePath);
				strDftFilePath += *iter;
				if(pContent->OpenFile(strDftFilePath.c_str()))
				{
					break;
				}
			}

			if(pContent->IsOpen())
			{
				// 默认文件打开成功
				pSockInf->pResponse->SetServerCode(SC_OK);
			}
			else
			{
				// 默认文件打开失败,提示禁止浏览目录,而不是404
				pContent->OpenText(g_HTTP_Forbidden, strlen(g_HTTP_Forbidden));
				pSockInf->pResponse->SetServerCode(SC_FORBIDDEN);
			}
		}
		pSockInf->pResponse->AttachContent(pContent);
	}
	else
	{
		// 客户端是否请求了断点续传的内容
		// 创建文件内容对象并关联给Response对象
		__int64 lFrom = 0;
		__int64 lTo = -1;
		if(pSockInf->pRequest->GetRange(lFrom, lTo))
		{
			pSockInf->pResponse->SetServerCode(SC_PARTIAL);
		}
		else
		{
			pSockInf->pResponse->SetServerCode(SC_OK);
		}

		CHTTPContent *pContent = new CHTTPContent;
		if(pContent->OpenFile(strServerFilePath.c_str(), lFrom, lTo))
		{
			// 文件打开成功.
		}
		else
		{
			// 文件不存在或者其它什么原因,打开失败.
			pContent->OpenHtml(g_HTTP_Content_NotFound, strlen(g_HTTP_Content_NotFound));
			pSockInf->pResponse->SetServerCode(SC_NOTFOUND);
		}
		pSockInf->pResponse->AttachContent(pContent);
	}

exit:
	/////////// 准备响应头
	pSockInf->pResponse->CookResponse();

	// 通知状态接收接口
	if(m_pStatusHandler)
	{
		m_pStatusHandler->OnRequested(pSockInf->pszIP, pSockInf->nPort, strUrlObject.c_str(), 
			pSockInf->pRequest->GetMethod(), pSockInf->pResponse->GetServerCode());
	}
	return;
}



BOOL CHTTPServer::mapServerFile(const std::wstring &strUrl, std::wstring &strFilePath)
{
	// 获得根目录
	strFilePath = m_strWorkDir;
	ASSERT(strFilePath.size() > 0);
	if(strFilePath.back() == L'\\') strFilePath.erase(strFilePath.end()--);

	// 获得完整路径
	strFilePath += strUrl;
	for(std::wstring::iterator iter = strFilePath.begin(); iter != strFilePath.end(); ++iter)
	{
		if( *iter == L'/' ) *iter = L'\\'; // URL的正斜杠替换为反斜杠.
	}

	return TRUE;
}

void CHTTPServer::lock()
{
	EnterCriticalSection(&m_cs);
}

void CHTTPServer::unlock()
{
	LeaveCriticalSection(&m_cs);
}

int CHTTPServer::OnDeadConnectionDetected(PCLIENTINF pClientInf)
{
	// 检测到死连接后,把套接字关闭,这样,正在执行的IO操作会失败,从 GetQueuedCompletionStatus() 函数返回.
	// 然后再执行 OnClose() 函数把套接字对应的资源全部回收.

	//LOGGER_CWARNING(theLogger, _T("检测到一个死连接[%s:%d].\r\n"), pClientInf->pszIP, pClientInf->nPort);
	pClientInf->nOprationType = opp_dead;
	shutdown(pClientInf->hSocket, SD_BOTH);
	closesocket(pClientInf->hSocket);
	return 0;
}

int CHTTPServer::OnSessionTimeout(PCLIENTINF pClientInf)
{
	//LOGGER_CWARNING(theLogger, _T("连接[%s:%d]会话超时.\r\n"), pClientInf->pszIP, pClientInf->nPort);
	pClientInf->nOprationType = opp_session_timeout;
	shutdown(pClientInf->hSocket, SD_BOTH);
	closesocket(pClientInf->hSocket);

	// 此处不能把 pClientInf->hSocket 设置为 INVALID_SOCKET, 因为释放资源还需要这个句柄作为KEY.
	return 0;
}

BOOL CHTTPServer::doAccept()
{
	if(m_sockListen == INVALID_SOCKET) return FALSE;

	// 创建一个新的套接字.
	m_sockNewClient = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if( INVALID_SOCKET == m_sockNewClient )
	{
		ASSERT(0);
		LOGGER_CERROR(theLogger, _T("无法为新连接创建套接字,错误码[%d].\r\n"), WSAGetLastError());
		return FALSE;
	}

	// 调用AcceptEx()函数
	DWORD dwBytesReceived = 0;
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	LPFN_ACCEPTEX lpfnAcceptEx = NULL;
	DWORD dwBytes = 0;
	if( 0 != WSAIoctl(m_sockListen, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx, sizeof(GuidAcceptEx),&lpfnAcceptEx, sizeof(lpfnAcceptEx), &dwBytes, NULL, NULL) )
	{
		ASSERT(0);
		LOGGER_CERROR(theLogger, _T("无法获得AcceptEx()函数指针,将导致无法接收到客户端连接,错误码[%d].\r\n"), WSAGetLastError());
		return FALSE;
	}

	memset(&m_AcceptOL, 0, sizeof(m_AcceptOL));
	if( !lpfnAcceptEx(m_sockListen, m_sockNewClient, m_pAcceptBuf, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &dwBytesReceived, &m_AcceptOL) )
	{
		if( WSA_IO_PENDING != WSAGetLastError())
		{
			ASSERT(0);
			LOGGER_CERROR(theLogger, _T("无法接受客户端连接,错误码[%d].\r\n"), WSAGetLastError());
			return FALSE;
		}
		else
		{
			// 正常情况,此时无客户端连接.
		}
	}
	else
	{
		// 立即得到一个新的客户连接,说明此时有很多连接在排队等候服务处理.
		ASSERT(0); // 由于侦听套接字已经关联了一个完成端口,不应该运行到这里.
		OnAcceptEx(FALSE);
	}
	return TRUE;
}

//BOOL CHTTPServer::RegisterXSPHander(LPCTSTR lpszType, IXSPHandler *pHander)
//{
//	//if(NULL == lpszType || NULL == pHander) return FALSE;
//
//	//hander_iter iter;
//	//iter = m_HanderMap.find(std::string(lpszType));
//	//if(iter == m_HanderMap.end())
//	//{
//	//	m_HanderMap.insert(std::make_pair(std::string(lpszType), pHander));
//	//}
//	//else
//	//{
//	//	iter->second = pHander;
//	//}
//	return TRUE;
//}
//
//BOOL CHTTPServer::UnRegisterXSPHander(LPCTSTR lpszType)
//{
//	//if(NULL == lpszType) return FALSE;
//	//
//	//hander_iter iter;
//	//iter = m_HanderMap.find(std::string(lpszType));
//	//if(iter == m_HanderMap.end())
//	//{
//	//	return FALSE;
//	//}
//	//else
//	//{
//	//	m_HanderMap.erase(iter);
//	//}
//	return TRUE;
//}
