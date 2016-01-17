
// RePrintToWndTest.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "RePrintToWndTest.h"
#include "RePrintToWndTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRePrintToWndTestApp

BEGIN_MESSAGE_MAP(CRePrintToWndTestApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRePrintToWndTestApp 构造

CRePrintToWndTestApp::CRePrintToWndTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CRePrintToWndTestApp 对象

CRePrintToWndTestApp theApp;


// CRePrintToWndTestApp 初始化

BOOL CRePrintToWndTestApp::InitInstance()
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

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CRePrintToWndTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
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

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

/*
*根据进程ID杀掉进程
*/
BOOL KillProcess(DWORD nProcessID)
{
	HANDLE hProcessHandle; 
	hProcessHandle =::OpenProcess( PROCESS_TERMINATE,FALSE, nProcessID );
	if (hProcessHandle == NULL)
		return FALSE;

	return ::TerminateProcess( hProcessHandle, 4 );
}

/*
*根据进程显示的标题名称杀掉进程
*/
BOOL KillProcess(LPCSTR pszWindowTitle)
{
	HWND TheWindow;

	TheWindow = ::FindWindow( NULL, pszWindowTitle );
	if (TheWindow == NULL)
		return FALSE;

	DWORD nProcessID;
	::GetWindowThreadProcessId( TheWindow, &nProcessID );

	return KillProcess(nProcessID);
}

int CRePrintToWndTestApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	KillProcess("区域覆盖高分影像产品生产工具");

	if (g_ExeProcessId!=0)
	{
		KillProcess(g_ExeProcessId); // 主窗体退出时杀掉exe进程
	}
	
	return CWinAppEx::ExitInstance();
}
