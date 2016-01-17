// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "WebServer.h"

#include "MainFrm.h"
#include "SettingDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_STATUS_CONNECTIONS 10001
#define ID_STATUS_SPEED_UP 10002
#define ID_STATUS_SPEED_DOWN 10003

#define IDT_UPDATE_SPEED 10005 /*刷新带宽数据*/
#define CHECKTIME_SPEED 2000 /*每个多少毫秒刷新一次带宽*/

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_START, &CMainFrame::OnStart)
	ON_UPDATE_COMMAND_UI(ID_START, &CMainFrame::OnUpdateStart)
	ON_COMMAND(ID_STOP, &CMainFrame::OnStop)
	ON_UPDATE_COMMAND_UI(ID_STOP, &CMainFrame::OnUpdateStop)
	ON_COMMAND(ID_SETTING, &CMainFrame::OnSetting)
	ON_UPDATE_COMMAND_UI(ID_SETTING, &CMainFrame::OnUpdateSetting)
	ON_COMMAND(ID_LOG, &CMainFrame::OnLog)
	ON_UPDATE_COMMAND_UI(ID_LOG, &CMainFrame::OnUpdateLog)
	ON_COMMAND(ID_PAUSE, &CMainFrame::OnPause)
	ON_UPDATE_COMMAND_UI(ID_PAUSE, &CMainFrame::OnUpdatePause)
	ON_COMMAND(ID_APP_EXIT, &CMainFrame::OnAppExit)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_NOTIFY_ICON, &CMainFrame::OnTrayIcon)
	ON_MESSAGE(WM_LOGMESSAGE, &CMainFrame::OnLogMessage)
	ON_COMMAND(ID_CLEAR_LOG, &CMainFrame::OnClearLog)
	ON_UPDATE_COMMAND_UI(ID_CLEAR_LOG, &CMainFrame::OnUpdateClearLog)
	ON_COMMAND(ID_ENABLE_LOG, &CMainFrame::OnEnableLog)
	ON_UPDATE_COMMAND_UI(ID_ENABLE_LOG, &CMainFrame::OnUpdateEnableLog)
	ON_WM_TIMER()
	ON_MESSAGE(WM_CONNECTION_NUMBER, &CMainFrame::OnConnectionNumber)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame() : m_ll(LL_ALL), m_nWindowLogger(0), m_nFileLogger(0)
{
	// TODO: 在此添加成员初始化代码
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_uTimer = 0;
	InitializeCriticalSection(&m_cs);
}

CMainFrame::~CMainFrame()
{
	DeleteObject(m_hIcon);
	DeleteCriticalSection(&m_cs);
}

void CMainFrame::lock()
{
	EnterCriticalSection(&m_cs);
}

void CMainFrame::unlock()
{
	LeaveCriticalSection(&m_cs);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	else
	{
		m_wndStatusBar.SetPaneInfo(1, ID_STATUS_CONNECTIONS, SBPS_NORMAL, 110);
		m_wndStatusBar.SetPaneInfo(2, ID_STATUS_SPEED_UP, SBPS_NORMAL, 100);
		m_wndStatusBar.SetPaneInfo(3, ID_STATUS_SPEED_DOWN, SBPS_NORMAL, 100);
	}

	// 设置图标
	SetIcon(m_hIcon, TRUE);

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// 初始化日志模块
	theLogger.open(true, LL_NONE);
	
	// 添加托盘图标.
	memset(&m_IconData, 0, sizeof(m_IconData));
	m_IconData.cbSize = sizeof(m_IconData);
	m_IconData.hWnd = m_hWnd;
	m_IconData.uID = 100;
	m_IconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_IconData.uCallbackMessage = WM_NOTIFY_ICON;
	m_IconData.uTimeout = 500;
	_tcscpy(m_IconData.szTip, _T("Que's HTTP Server - by Que's C++ Studio"));
	m_IconData.hIcon = m_hIcon;
	Shell_NotifyIcon(NIM_ADD, &m_IconData);

	// 初始化状态栏
	ResetStatus();

	//  是否输出日志
	if(!AfxGetApp()->GetProfileInt(INI_SESSION, _T("DisableScreenLog"), 0))
	{
		m_nWindowLogger = theLogger.addHwndAppender(m_hWnd, WM_LOGMESSAGE);
	}
	if( !AfxGetApp()->GetProfileInt(INI_SESSION, _T("DisableFileLog"), 0))
	{
		m_nFileLogger = theLogger.addFileAppender(getLogFileName());
	}
	if(m_nWindow != 0 || m_nFileLogger != 0)
	{
		theLogger.setLogLevel(LL_ALL);
	}

	// 启动
	if(AfxGetApp()->GetProfileInt(INI_SESSION, _T("AutoRun"), 0))
	{
		LOGGER_CINFO(theLogger, _T("Que's HTTP Server 启动中...\r\n"));
		OnStart();
	}
	else
	{
		LOGGER_CINFO(theLogger, _T("Que's HTTP Server 准备就绪.\r\n"));
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

std::wstring CMainFrame::getLogFileName()
{
	TCHAR szFilePath[MAX_PATH] = {0};
	if( 0 == GetModuleFileName(NULL, szFilePath, MAX_PATH))
	{
	}
	else
	{
		TCHAR* pEnd = _tcsrchr(szFilePath, _T('\\'));
		if(pEnd == NULL)
		{
		}
		else
		{
			pEnd++;
			_tcscpy(pEnd, _T("HTTPServerLOG.TXT"));
		}
	}

	return szFilePath;
}


void CMainFrame::OnStart()
{
	HTTPSTARTDESC startDesc;
	memset(&startDesc, 0, sizeof(HTTPSTARTDESC));

	TCHAR szFilePath[MAX_PATH] = {0};
	if( 0 == GetModuleFileName(NULL, szFilePath, MAX_PATH))
	{
	}
	else
	{
		TCHAR* pEnd = _tcsrchr(szFilePath, _T('\\'));
		if(pEnd == NULL)
		{
		}
		else
		{
			pEnd[0] = 0;
		}
	}
	
	CString strRoot = AfxGetApp()->GetProfileString(INI_SESSION, _T("Root"), szFilePath);
	_tcsncpy(startDesc.szRootDir, (LPCTSTR)strRoot, MAX_PATH);
	startDesc.nPort = AfxGetApp()->GetProfileInt(INI_SESSION, _T("Port"), 80);
	startDesc.nMaxConnection =  AfxGetApp()->GetProfileInt(INI_SESSION, _T("MaxConnections"), 5000);
	startDesc.bNavDir = AfxGetApp()->GetProfileInt(INI_SESSION, _T("ListFile"), TRUE);
	startDesc.dwDeadConnectionTimeout = AfxGetApp()->GetProfileInt(INI_SESSION, _T("DeadConnectionTimeout"), 30) * 1000;
	startDesc.dwSessionTimeout = AfxGetApp()->GetProfileInt(INI_SESSION, _T("SessionTimeout"), 0) * 1000;
	if(startDesc.dwDeadConnectionTimeout < 0) startDesc.dwDeadConnectionTimeout = 0;
	if(startDesc.dwSessionTimeout < 0) startDesc.dwSessionTimeout = 0;
	startDesc.pStatusHandler = this;
	CString strSpeed = AfxGetApp()->GetProfileString(INI_SESSION, _T("MaxSpeed"), _T("0"));
	double dSpeed = _ttof((LPCTSTR)strSpeed);
	startDesc.llMaxSpeed = (__int64)(dSpeed * 1024);
	startDesc.nMaxClientConn = AfxGetApp()->GetProfileInt(INI_SESSION, _T("MaxClientConnections"), 0);
	CString strDefName = AfxGetApp()->GetProfileString(INI_SESSION, _T("DefaultFileName"), _T("index.html,index.htm,default.html,default.htm"));
	_tcsncpy(startDesc.szDefaultFileName, (LPCTSTR)strDefName, MAX_PATH);

	if( SE_SUCCESS == m_Server.Run(&startDesc))
	{
		ResetStatus();
		m_uTimer = SetTimer(IDT_UPDATE_SPEED, CHECKTIME_SPEED, NULL);

		LOGGER_CINFO(theLogger, _T("Que's HTTP Server 启动成功,根目录[%s],端口[%d],最大连接数[%d].\r\n"), strRoot, startDesc.nPort, startDesc.nMaxConnection);
	}
	else
	{
		LOGGER_CINFO(theLogger, _T("Que's HTTP Server 启动失败,已经在运行或者端口[%d]被占用..\r\n"), startDesc.nPort);
	}
}

void CMainFrame::ResetStatus()
{
	SetConnectionsNumber(0);
	m_lBytesSent = 0;
	m_lBytesRecv = 0;
	m_dwLastTick = 0;

	if(m_uTimer != 0)
	{
		KillTimer(m_uTimer);
		m_uTimer = 0;
	}

	m_wndStatusBar.SetPaneText(2, _T("上行: 0.00 B/s"), TRUE);
	m_wndStatusBar.SetPaneText(3, _T("下行: 0.00 B/s"), TRUE);
}

void CMainFrame::OnUpdateStart(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_Server.IsRuning());
}

void CMainFrame::OnStop()
{
	HTTP_SERVER_ERROR_TYPE hm = m_Server.Stop();
	if(hm == SE_SUCCESS)
	{
		ResetStatus();
		LOGGER_CINFO(theLogger, _T("Que's HTTP Server 停止服务.\r\n"));
	}
	else
	{
		LOGGER_CINFO(theLogger, _T("暂时无法停止服务,请查看日志.\r\n"));
	}
}

void CMainFrame::OnUpdateStop(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_Server.IsRuning());
}

void CMainFrame::OnSetting()
{
	// TODO: Add your command handler code here
	do_setting();
}

void CMainFrame::OnUpdateSetting(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFrameWnd::PreTranslateMessage(pMsg);
}

#define REMOVE_LINE_COUNT 100
void CMainFrame::do_log(const TCHAR* strLog)
{
	if (m_wndView.m_edt.m_hWnd == NULL)
	{
	}
	else
	{
		int nLmt = m_wndView.m_edt.GetLimitText();
		int nTextLength = m_wndView.m_edt.GetWindowTextLength();
		if( (int)(nTextLength + _tcslen(strLog)) >= nLmt )
		{
			int nPos = m_wndView.m_edt.LineIndex(REMOVE_LINE_COUNT);
			m_wndView.m_edt.SetSel(0, nPos);
			m_wndView.m_edt.ReplaceSel(_T(""), FALSE);  // 不能用 Clear(), 由于 Undo() 的原因, 用 Clear() 并没有正在删除数据.
		}

		m_wndView.m_edt.SetSel(nTextLength, nTextLength);
		m_wndView.m_edt.ReplaceSel(strLog);
	}
}

void CMainFrame::OnLog()
{
}

LRESULT CMainFrame::OnLogMessage(WPARAM w, LPARAM l)
{
	LPCTSTR log_msg = reinterpret_cast<LPCTSTR>(l);
	do_log(log_msg);
	delete []log_msg;
	return 0;
}

void CMainFrame::OnUpdateLog(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMainFrame::OnPause()
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnUpdatePause(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMainFrame::OnAppExit()
{
	// TODO: Add your command handler code here
	if(m_Server.IsRuning()) m_Server.Stop();
	Shell_NotifyIcon(NIM_DELETE, &m_IconData);
	//
	CFrameWnd::OnClose();
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
}

void CMainFrame::do_setting()
{
	CSettingDlg dlg;
	if( IDOK == dlg.DoModal())
	{
		// 应用新的设置(关于日志的),其它设置需要重启服务器

		if(dlg.m_bDisableScreenLog)
		{
			if(m_nWindowLogger != 0) theLogger.removeAppender(m_nWindowLogger);
			m_nWindowLogger = 0;
		}
		else
		{
			if(m_nWindowLogger == 0) m_nWindowLogger = theLogger.addHwndAppender(m_hWnd, WM_LOGMESSAGE);
		}

		if(dlg.m_bDisableFileLog)
		{
			if(m_nFileLogger != 0) theLogger.removeAppender(m_nFileLogger);
			m_nFileLogger = 0;
		}
		else
		{
			if(m_nFileLogger == 0) m_nFileLogger = theLogger.addFileAppender(getLogFileName());
		}

		if(m_nWindowLogger == 0 && m_nFileLogger == 0)
		{
			theLogger.setLogLevel(LL_NONE);
		}
		else
		{
			theLogger.setLogLevel(LL_ALL);
		}
	}
}

LRESULT CMainFrame::OnTrayIcon(WPARAM w, LPARAM l)
{
	if( l == WM_LBUTTONUP || l == WM_RBUTTONUP)
	{
		ShowWindow(SW_SHOW);
	}
	return 0;
}

void CMainFrame::OnClearLog()
{
	m_wndView.m_edt.SetSel(0, -1);
	m_wndView.m_edt.ReplaceSel(_T(""), FALSE);
}


void CMainFrame::OnUpdateClearLog(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CMainFrame::OnEnableLog()
{
	// TODO: 在此添加命令处理程序代码
	if(m_nWindowLogger == 0)
	{
		m_nWindowLogger = theLogger.addHwndAppender(m_hWnd, WM_LOGMESSAGE);
		theLogger.setLogLevel(LL_ALL);
	}
	else
	{
		theLogger.removeAppender(m_nWindowLogger);
		m_nWindowLogger = 0;
		if(m_nFileLogger == 0) theLogger.setLogLevel(LL_NONE);
	}
}


void CMainFrame::OnUpdateEnableLog(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(m_nWindowLogger == 0)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::SetConnectionsNumber(int nTotalConnections)
{
	m_nTotalConnections = nTotalConnections;
	/*wchar_t szConnectionsText[200] = {L'\0'};
	wsprintf(szConnectionsText, _T("目前有: %d个连接"), m_nTotalConnections);
	m_wndStatusBar.SetPaneText(1, szConnectionsText, TRUE);*/

	// 只能以消息的方式刷新文本,因为一定是另外一个线程在调用SetConnectionsNumber()函数
	// 如果是另一个线程调用, IsWindow()将失败.
	PostMessage(WM_CONNECTION_NUMBER, m_nTotalConnections, 0);
}

LRESULT CMainFrame::OnConnectionNumber(WPARAM w, LPARAM l)
{
	int nConnectionNumber = (int)w;
	wchar_t szConnectionsText[200] = {L'\0'};
	wsprintf(szConnectionsText, _T("目前有: %d个连接"), nConnectionNumber);
	m_wndStatusBar.SetPaneText(1, szConnectionsText, TRUE);
	return 0;
}

void CMainFrame::OnNewConnection(const wchar_t *pszIP, unsigned int nPort, BOOL bRefused, BOOL bKicked)
{
	// 需要同步.
	if(bKicked)
	{
		LOGGER_CINFO(theLogger, _T("[%s:%d] - 客户连接被拒绝,超出单IP最大允许的连接数限制.\r\n"), pszIP, nPort);
	}
	else
	{
		if(bRefused)
		{
			LOGGER_CINFO(theLogger, _T("[%s:%d] - 服务器忙,连接被丢弃.\r\n"), pszIP, nPort);
		}
		else
		{
			LOGGER_CINFO(theLogger, _T("[%s:%d] - 新连接被接受.\r\n"), pszIP, nPort);

			lock();
			SetConnectionsNumber(m_nTotalConnections + 1);
			unlock();
		}
	}
}

void CMainFrame::OnConnectionClosed(const wchar_t *pszIP, unsigned int nPort, REMOVE_REASON rr, __int64 nByteSent, unsigned int nTimeUsed)
{
	// 需要同步
	lock();
	SetConnectionsNumber(m_nTotalConnections - 1);
	unlock();

	std::wstring strReason(L"");
	switch(rr)
	{
	case RR_CLIENTCLOSED: { strReason = _T("客户端关闭了连接"); break; }
	case RR_SENDCOMPLETE: { strReason  = _T("数据发送完毕"); break; }
	case RR_RECV_FAILED: { strReason = _T("接收失败"); break; }
	case  RR_SEND_FAILED: { strReason = _T("发送失败"); break; }
	case RR_DEAD: { strReason = _T("不活跃连接"); break; }
	case RR_SESSION_TIMEOUT: { strReason= _T("会话超时"); break; }
	default: { strReason = _T("未知"); break; }
	}

	// 计算以发送数据的长度
	CString strBytes(_T(""));
	if(nByteSent >= G_BYTES)
	{
		strBytes.Format(_T("%.2fGB"), nByteSent * 1.0 / G_BYTES);
	}
	else if(nByteSent >= M_BYTES)
	{
		strBytes.Format(_T("%.2fMB"), nByteSent * 1.0 / M_BYTES);
	}
	else if(nByteSent >= K_BYTES)
	{
		strBytes.Format(_T("%.2fKB"), nByteSent * 1.0 / K_BYTES);
	}
	else
	{
		strBytes.Format(_T("%lldBytes"), nByteSent);
	}

	// 计算平均数据
	CString strSpeed(_T(""));
	if(nTimeUsed <= 0)
	{
		strSpeed = _T("---");
	}
	else
	{
		double llSpeed = nByteSent * 1.0 / nTimeUsed * 1000;
		if(llSpeed >= G_BYTES)
		{
			strSpeed.Format(_T("%.2fGB/s"), llSpeed * 1.0 / G_BYTES);
		}
		else if(llSpeed >= M_BYTES)
		{
			strSpeed.Format(_T("%.2fMB/s"), llSpeed * 1.0 / M_BYTES);
		}
		else if(llSpeed >= K_BYTES)
		{
			strSpeed.Format(_T("%.2fKB/s"), llSpeed * 1.0 / K_BYTES);
		}
		else
		{
			strSpeed.Format(_T("%.2fB/s"), llSpeed);
		}
	}

	LOGGER_CINFO(theLogger, _T("[%s:%d] - 连接被关闭[%s],总计发送数据[%s],用时[%.3fs],平均速度[%s].\r\n"), 
		pszIP, nPort, strReason.c_str(), (LPCTSTR)strBytes, nTimeUsed * 1.0 / 1000, (LPCTSTR)strSpeed);
}

void CMainFrame::OnDataSent(const wchar_t *pszIP, unsigned int nPort, unsigned int nBytesSent)
{
	// 需要做同步 InterLockedExchangeAdd
	// InterlockedExchangeAdd64(&m_lBytesSent, nBytesSent);

	lock(); 
	m_lBytesSent += nBytesSent;
	unlock();
}

void CMainFrame::OnDataReceived(const wchar_t *pszIP, unsigned int nPort, unsigned int nBytesReceived)
{
	// 需要做同步

	// Windows Vista/7 才有这个函数dll
	// InterlockedExchangeAdd64(&m_lBytesRecv, nBytesReceived); 

	// 其它系统用临界段
	lock();
	m_lBytesRecv += nBytesReceived;
	unlock();
}

void CMainFrame::OnRequested(const wchar_t *pszIP, unsigned int nPort, const wchar_t *pszUrl, HTTP_METHOD hm, SERVER_CODE sc)
{
	// 只写一下日志就可以了.
	LOGGER_CINFO(theLogger, _T("[%s:%d] - 请求资源[%s],响应码: %d.\r\n"), pszIP, nPort, pszUrl, sc);  
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( nIDEvent == m_uTimer )
	{
		// 记录数据,后面再计算

		lock();
		__int64 lBytesSent = m_lBytesSent;
		__int64 lBytesRecv = m_lBytesRecv;
		DWORD dwLastTickCount = m_dwLastTick;
		DWORD dwTickCount = GetTickCount();
		
		m_lBytesRecv = 0;
		m_lBytesSent = 0;
		m_dwLastTick = dwTickCount;
		unlock();


		/*
		__int64 lBytesSent = m_lBytesSent;
		__int64 lBytesRecv = m_lBytesRecv;
		DWORD dwLastTickCount = m_dwLastTick;
		DWORD dwTickCount = GetTickCount();

		InterlockedExchange64(&m_lBytesRecv, 0);
		InterlockedExchange64(&m_lBytesSent, 0);
		m_dwLastTick = dwTickCount;
		*/

		// 更新文本显示,没有同步要求
		if(dwLastTickCount == 0 || dwTickCount <= dwLastTickCount)
		{
			// 第一次不作计算
		}
		else
		{
			CString strSpeedText(_T(""));

			// 计算上行带宽 Sent
			double llSpeed = lBytesSent * 1.0 / (dwTickCount - dwLastTickCount) * 1000;
			if(llSpeed >= G_BYTES)
			{
				strSpeedText.Format(_T("上行: %.2f GB/s"), llSpeed * 1.0 / G_BYTES);
			}
			else if(llSpeed >= M_BYTES)
			{
				strSpeedText.Format(_T("上行: %.2f MB/s"), llSpeed * 1.0 / M_BYTES);
			}
			else if(llSpeed >= K_BYTES)
			{
				strSpeedText.Format(_T("上行: %.2f KB/s"), llSpeed * 1.0 / K_BYTES);
			}
			else
			{
				strSpeedText.Format(_T("上行: %.2f B/s"), llSpeed);
			}
			m_wndStatusBar.SetPaneText(2, strSpeedText, TRUE);
			

			// 计算下行带宽 Received
			llSpeed = lBytesRecv * 1.0 / (dwTickCount - dwLastTickCount) * 1000;
			if(llSpeed >= G_BYTES)
			{
				strSpeedText.Format(_T("下行: %.2f GB/s"), llSpeed * 1.0 / G_BYTES);
			}
			else if(llSpeed >= M_BYTES)
			{
				strSpeedText.Format(_T("下行: %.2f MB/s"), llSpeed * 1.0 / M_BYTES);
			}
			else if(llSpeed >= K_BYTES)
			{
				strSpeedText.Format(_T("下行: %.2f KB/s"), llSpeed * 1.0 / K_BYTES);
			}
			else
			{
				strSpeedText.Format(_T("下行: %.2f B/s"), llSpeed);
			}
			m_wndStatusBar.SetPaneText(3, strSpeedText, TRUE);
		}
	}
	else
	{
		ASSERT(0);
	}

	__super::OnTimer(nIDEvent);
}
