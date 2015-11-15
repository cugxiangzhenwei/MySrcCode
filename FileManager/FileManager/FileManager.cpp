// FileManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FileManager.h"
#include "FileManagerDlg.h"
#include "LogTools.h"
#include "CrashHandler.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CFileManagerApp
BEGIN_MESSAGE_MAP(CFileManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFileManagerApp construction

CFileManagerApp::CFileManagerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bRunAtBack = FALSE;
}


// The one and only CFileManagerApp object

CFileManagerApp theApp;


// CFileManagerApp initialization
extern void InitConsoleWindow();
BOOL CFileManagerApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();
#if CONSOLE_WND
	InitConsoleWindow();
#endif

	AfxEnableControlContainer();
	LogTools::InitZToolsLog();
	CCrashHandler ch;
	ch.SetProcessExceptionHandlers();
	ch.SetThreadExceptionHandlers();

	globalData.SetDPIAware ();
	ProcessCmd();
	HANDLE hMetux = CreateMutex(NULL,TRUE,"FileManager_Sqlite3");
	if (hMetux)
	{
		if(ERROR_ALREADY_EXISTS==GetLastError())
		{
			CloseHandle(hMetux);
			CWnd * pDeskTopWnd = CWnd::GetDesktopWindow();
			CWnd *pFind = pDeskTopWnd->GetWindow(GW_CHILD);
			while(pFind)
			{
				if (::GetProp(pFind->m_hWnd,m_pszExeName))
				{
					if (::IsIconic(pFind->m_hWnd))
						pFind->ShowWindow(SW_RESTORE); // 如果主窗口已经最小话，则恢复其大小
					pFind->ShowWindow(SW_SHOW);
					pFind->SetForegroundWindow();
					::SetForegroundWindow(::GetLastActivePopup(pFind->m_hWnd));
					return FALSE; // 前一个运行实例已经存在，退出本实例
				}
				pFind = pFind->GetWindow(GW_HWNDNEXT);
			}
		}
	}

	CFileManagerDlg dlg;
	dlg.m_bRunAtBack = m_bRunAtBack;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	BCGCBProCleanUp ();
	CloseHandle(hMetux);
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
void CFileManagerApp::ProcessCmd()
{
	int argc = 0;
	LPWSTR *lpszArgv = NULL;
	LPWSTR szCmdLine = GetCommandLineW(); //获取命令行参数；
	lpszArgv = CommandLineToArgvW(szCmdLine, &argc); //拆分命令行参数字符串；
	LogMsg("开始解析命令行...\n");
	if (argc==1)
		return;
	CStringW strW = lpszArgv[1];
	if (strW.CollateNoCase(L"-runAtBack")==0)
		m_bRunAtBack =TRUE;
}