
// NetDiskUI.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "NetDiskUI.h"
#include "NetDiskUIDlg.h"
#include "DlgLoginMain.h"
#include "CrashHandler.h"
#include "TaskSerialization.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetDiskUIApp

BEGIN_MESSAGE_MAP(CNetDiskUIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNetDiskUIApp 构造

CNetDiskUIApp::CNetDiskUIApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	m_strServerIp = "";
	m_hMutex = NULL;
	m_pPageNetDisk = NULL;
	m_bRestart = FALSE;
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CNetDiskUIApp 对象

CNetDiskUIApp theApp;


// CNetDiskUIApp 初始化

BOOL CNetDiskUIApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	CCrashHandler ch;
	ch.SetProcessExceptionHandlers();
	ch.SetThreadExceptionHandlers();
	m_hMutex = CreateMutex(NULL,TRUE,"NetDiskUI");
	if (m_hMutex)
	{
		if(ERROR_ALREADY_EXISTS==GetLastError())
		{
			CloseHandle(m_hMutex);
			m_hMutex = NULL;
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

	char szDir[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(),szDir,MAX_PATH);
	CString strConfigINI = szDir;
	strConfigINI.MakeLower();
	strConfigINI = strConfigINI.Left(strConfigINI.ReverseFind('.') + 1) + "ini";
	GetPrivateProfileString("setting","ServerIp","",m_strServerIp.GetBuffer(100),100,strConfigINI);
	m_strServerIp.ReleaseBuffer();
	if (m_strServerIp.IsEmpty())
	{
		m_strServerIp = "127.0.0.1";
		WritePrivateProfileString("setting","ServerIp",m_strServerIp,strConfigINI);
	}

	GetPrivateProfileString("setting","CacheDir","",m_strCacheDir.GetBuffer(500),500,strConfigINI);
	m_strCacheDir.ReleaseBuffer();
	if (m_strCacheDir.IsEmpty())
	{
		m_strCacheDir = strConfigINI.Mid(0,strConfigINI.ReverseFind('\\')+1) + "CacheDir";
		WritePrivateProfileString("setting","CacheDir",m_strCacheDir,strConfigINI);
	}

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	//CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CDlgLoginMain DlgLogin;
	if (DlgLogin.DoModal()!=IDOK)
		return FALSE;

	m_strUsrId = DlgLogin.m_strUsrID;
	m_strUsrName = DlgLogin.m_strUsrName;
	m_iAllSpace = DlgLogin.m_iAllSpace;
	m_iUsedSpace = DlgLogin.m_iUsedSpace;
	m_strPasswd = DlgLogin.m_strPasswd;

	CNetDiskUIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (dlg.m_bLogOut)
		m_bRestart = TRUE;
	else
		m_bRestart = FALSE;

	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	/*if (pShellManager != NULL)
	{
		delete pShellManager;
	}*/

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}



int CNetDiskUIApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

	SerializationManager.SaveXmlFile();

	if (m_bRestart)
	{
		char szExe[MAX_PATH] = {0};
		GetModuleFileName(AfxGetInstanceHandle(),szExe,MAX_PATH);
		ShellExecute(NULL,"open",szExe,NULL,NULL,SW_SHOWNORMAL);
	}
	
	return CWinApp::ExitInstance();
}
