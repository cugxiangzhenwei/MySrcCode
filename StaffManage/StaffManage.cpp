
// StaffManage.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "StaffManage.h"
#include "MainFrm.h"

#include "StaffManageSet.h"
#include "StaffManageDoc.h"
#include "StaffManageView.h"
#include "odbcinst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStaffManageApp

BEGIN_MESSAGE_MAP(CStaffManageApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CStaffManageApp::OnAppAbout)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CStaffManageApp 构造

CStaffManageApp::CStaffManageApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CStaffManageApp 对象

CStaffManageApp theApp;


// CStaffManageApp 初始化

BOOL CStaffManageApp::InitInstance()
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

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}


	CString strDir;
	GetCurrentDirectory(MAX_PATH,strDir.GetBuffer(MAX_PATH));
	strDir.ReleaseBuffer();

	CString strAtt;
	strAtt.Format(_T("DSN=人事管理数据库\0 DBQ=人事管理数据库.mdb\0 DEFAULTDIR= %s\0\0"),strDir);

	BOOL bOK = SQLConfigDataSource(NULL,ODBC_ADD_SYS_DSN,_T("Microsoft Access Driver (*.mdb)\0"),strAtt);
	if (!bOK)
	{
		AfxMessageBox(_T("数据源加载失败!"));
	}
	
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CStaffManageDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CStaffManageView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CStaffManageApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CStaffManageApp 自定义加载/保存方法

void CStaffManageApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CStaffManageApp::LoadCustomState()
{
}

void CStaffManageApp::SaveCustomState()
{
}

// CStaffManageApp 消息处理程序





int CStaffManageApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strDir;
	GetCurrentDirectory(MAX_PATH,strDir.GetBuffer(MAX_PATH));
	strDir.ReleaseBuffer();

	CString strAtt;
	strAtt.Format(_T("DSN= 人事管理数据库\0 DBQ=人事管理数据库.mdb\0 DEFAULTDIR= %s\0\0"),strDir);

	BOOL bOK = SQLConfigDataSource(NULL,ODBC_REMOVE_SYS_DSN,_T("Microsoft Access Driver (*.mdb)\0"),strAtt);
	if (!bOK)
	{
		AfxMessageBox(_T("卸载数据源失败!"));
	}
	return CWinAppEx::ExitInstance();
}
