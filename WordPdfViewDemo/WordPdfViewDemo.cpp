
// WordPdfViewDemo.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "WordPdfViewDemo.h"
#include "MainFrm.h"

#include "WordDoc.h"
#include "WordView.h"

#include "PdfDoc.h"
#include "PdfView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWordPdfViewDemoApp

BEGIN_MESSAGE_MAP(CWordPdfViewDemoApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CWordPdfViewDemoApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CWordPdfViewDemoApp 构造

CWordPdfViewDemoApp::CWordPdfViewDemoApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("WordPdfViewDemo.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CWordPdfViewDemoApp 对象

CWordPdfViewDemoApp theApp;
// 生成的此标识符在统计上对于您的应用程序是唯一的
// 如果您更愿选择一个特定的标识符，则可以更改它

// {84D33554-6E4F-4A60-B3F2-4A409F361F8C}
static const CLSID clsid =
{ 0x84D33554, 0x6E4F, 0x4A60, { 0xB3, 0xF2, 0x4A, 0x40, 0x9F, 0x36, 0x1F, 0x8C } };

const GUID CDECL _tlid = { 0x6A9DA469, 0xDC93, 0x4C33, { 0x8B, 0x32, 0x82, 0x2, 0xB5, 0x7F, 0xCF, 0xD2 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CWordPdfViewDemoApp 初始化

BOOL CWordPdfViewDemoApp::InitInstance()
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

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

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
		RUNTIME_CLASS(CWordDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CWordView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	CSingleDocTemplate* pDocTemplatePdf;
	pDocTemplatePdf = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPdfDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CPdfView));
	if (!pDocTemplatePdf)
		return FALSE;
	AddDocTemplate(pDocTemplatePdf);
	// 将 COleTemplateServer 连接到文档模板
	// COleTemplateServer 通过使用
	// 文档模板中指定的信息来为请求 OLE 容器
	// 创建新文档
	m_server.ConnectTemplate(clsid, pDocTemplate, TRUE);
		// 注意: 仅当命令行中存在 /Embedding 或 /Automation 时，SDI 应用程序
		//   才注册服务器对象


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 应用程序是用 /Embedding 或 /Automation 开关启动的。
	//使应用程序作为自动化服务器运行。
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// 将所有的 OLE 服务器工厂注册为正在运行。这将启用
		//  OLE 库以从其他应用程序中创建对象
		COleTemplateServer::RegisterAll();

		// 不显示主窗口
		return TRUE;
	}
	// 使用 /Unregserver 或 /Unregister 开关启动应用程序。注销
	// 类型库。其他注销操作在 ProcessShellCommand() 中发生。
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT, NULL, NULL, FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
	}
	// 应用程序是以独立方式或用其他开关(如 /Register
	// 或 /Regserver)启动的。更新注册表项，包括类型库。
	else
	{
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
	}

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

int CWordPdfViewDemoApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CWordPdfViewDemoApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CWordPdfViewDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CWordPdfViewDemoApp 自定义加载/保存方法

void CWordPdfViewDemoApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CWordPdfViewDemoApp::LoadCustomState()
{
}

void CWordPdfViewDemoApp::SaveCustomState()
{
}

// CWordPdfViewDemoApp 消息处理程序



