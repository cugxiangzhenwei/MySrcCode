// RePrintToWndTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RePrintToWndTest.h"
#include "RePrintToWndTestDlg.h"
#include <vector>
// using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD g_ExeProcessId = 0;

HANDLE g_hRead = NULL;

static const int g_TimerElapse = 20;

static int g_TimerId = 0;

#define  WM_SUB_THREAD_MSG WM_USER + 200

static string g_strSubThread = "";

CRITICAL_SECTION g_CS;
static DWORD WINAPI ThreadProc(LPVOID pParamater)
{
	static bool bProcessOK = true;
	char buffer[1024] = {0};  
	DWORD bytesRead; 

	while(ReadFile(g_hRead,buffer,1023,&bytesRead,NULL)) //读取管道
	{
		EnterCriticalSection(&g_CS);
		 g_strSubThread = buffer;
		::SendMessage((HWND)pParamater,WM_SUB_THREAD_MSG,0,0);
	}
	return 0;
}

string StrTrim(const char *pszInStr)
{
	int iStart = 0;
	char * strPtr = (char*)pszInStr;
	while(*strPtr==32 || *strPtr==10 || *strPtr==9)
	{
		iStart ++;
		strPtr++;
	}

	int iEnd = strlen(pszInStr) -1;
	strPtr = (char*)&pszInStr[iEnd];
	while(*strPtr==32 || *strPtr==10 || *strPtr==9 )
	{
		iEnd--;
		strPtr--;
	}

	int iCount = iEnd - iStart +1;
	string strRev = string(pszInStr + iStart).substr(0,iCount);
	return strRev;
}

void StrSplit(const char *pszStr,vector<string> &vStr,const char* strDelim)
{
	int iLen = strlen(pszStr) + 1;
	char * pszCopy = new char[iLen];
	memset(pszCopy,0,iLen);
	strcpy_s(pszCopy,iLen,pszStr);
	char *toKen;
	char *Out = NULL;
	toKen = strtok_s(pszCopy,strDelim,&Out);
	string str = toKen;
	str = StrTrim(str.c_str());
	if (!str.empty())
		vStr.push_back(str);
	while(toKen!=NULL)
	{
		toKen = strtok_s(NULL,strDelim,&Out);
		if (toKen)
		{	str = toKen;
		str = StrTrim(str.c_str());
		if (!str.empty())
			vStr.push_back(str);
		}
	}
	delete pszCopy;
}

// 此函数只能用于界面线程
static DWORD WaitForObjectEx( HANDLE hHandle, DWORD dwMilliseconds )
{
	BOOL bRet;
	MSG msg;
	INT iWaitRet;
	int nTimeOut = 0;

	
	while( (bRet = ::GetMessage( &msg, NULL, 0, 0 )) != 0)
	{ 
		if (dwMilliseconds != INFINITE)
		{
			if(nTimeOut++ * 20 >= dwMilliseconds)
				break;
		}

		iWaitRet = WaitForSingleObject(hHandle, 20);
		if(iWaitRet != WAIT_TIMEOUT)
		{
			break;
		}
		if (bRet == -1)
		{
			break;
		}
		else
		{
			::TranslateMessage(&msg); 
			::DispatchMessage(&msg); 
		}
	}

	return iWaitRet;
}
void CRePrintToWndTestDlg::CallExe(const char * pszCmd,bool bShowWnd)
{
	SECURITY_ATTRIBUTES sa;  
	HANDLE hRead,hWrite;  
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);  
	sa.lpSecurityDescriptor = NULL; //使用系统默认的安全描述符  
	sa.bInheritHandle = TRUE; //创建的进程继承句柄  

	if (!CreatePipe(&hRead,&hWrite,&sa,0)) //创建匿名管道  
	{  
		MessageBox("CreatePipe Failed!","提示",MB_OK | MB_ICONWARNING);  
		return;  
	}  

	STARTUPINFO si;  
	PROCESS_INFORMATION pi;  

	ZeroMemory(&si,sizeof(STARTUPINFO));  

	si.cb = sizeof(STARTUPINFO);  
	GetStartupInfo(&si);  
	si.hStdError = hWrite;  
	si.hStdOutput = hWrite; //新创建进程的标准输出连在写管道一端  
	if (bShowWnd)
		si.wShowWindow = SW_SHOW; //显示窗口  
	else
		si.wShowWindow = SW_HIDE; //隐藏窗口 

	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  


	char cmdline[200];  
	sprintf(cmdline,"%s",pszCmd);  

	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) //创建子进程  
	{  
		MessageBox("CreateProcess Failed!","提示",MB_OK | MB_ICONWARNING);  
		return;  
	}
	g_ExeProcessId = pi.dwProcessId;
	CloseHandle(hWrite); //关闭管道句柄  
	g_hRead = hRead;

	InitializeCriticalSection(&g_CS);
	HANDLE hThread = CreateThread(NULL,0,ThreadProc,GetSafeHwnd(),0,NULL);
	WaitForObjectEx(hThread,INFINITE);
	//MsgWaitForMultipleObjects(1,&hThread,TRUE,INFINITE,-1);
	//SetTimer(g_TimerId,g_TimerElapse,NULL);
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


// CRePrintToWndTestDlg 对话框




CRePrintToWndTestDlg::CRePrintToWndTestDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRePrintToWndTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRePrintToWndTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListBox);
}

BEGIN_MESSAGE_MAP(CRePrintToWndTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CRePrintToWndTestDlg::OnBnClickedBtnStart)
	ON_MESSAGE(WM_SUB_THREAD_MSG,&CRePrintToWndTestDlg::OnSubThreadMsg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_START2, &CRePrintToWndTestDlg::OnBnClickedBtnStart2)
	ON_BN_CLICKED(IDC_BTN_START3, &CRePrintToWndTestDlg::OnBnClickedBtnStart3)
END_MESSAGE_MAP()


// CRePrintToWndTestDlg 消息处理程序

BOOL CRePrintToWndTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRePrintToWndTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRePrintToWndTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRePrintToWndTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRePrintToWndTestDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strCallMsg.clear();
	m_ListBox.ResetContent();
	g_hRead = NULL;
	const char *pszCmd1 = "D:\\RsSrcDir\\Bin\\AtToolBox.exe -sj9_dp";
	const char *pszCmd2 = "\"C:\\Program Files (x86)\\NSIS\\makensis.exe\" D:\\RsSrcDir\\Release\\国土大区域打包安装脚本.nsi";
	const char *pszCmd3 = "doxygen.exe D:\\doxyWorkdir\\Doxyfile";
	CallExe(pszCmd1,true);
}
LRESULT CRePrintToWndTestDlg::OnSubThreadMsg(WPARAM wparam,LPARAM lparam)
{
	if(g_strSubThread.empty())
	{
		LeaveCriticalSection(&g_CS);
		return 0; // 接收到消息为空时，不处理
	}

	static string strLast ="";
	m_strCallMsg.c_str();
	m_strCallMsg += strLast; //最后一个字符串不完整，需要与下次的合并组成完整的串
	m_strCallMsg += g_strSubThread;


	g_strSubThread.clear(); //清空消息字符串
	LeaveCriticalSection(&g_CS);


	vector<string> vStrMsg;
	StrSplit(m_strCallMsg.c_str(),vStrMsg,"\n");

	int iCurCount = vStrMsg.size();

	int iLen = m_strCallMsg.length();
	bool bBadEnd = false;
	if(m_strCallMsg[iLen-1] != '\n')
	{
		strLast = vStrMsg[iCurCount-1]; // 最后一行不完整时，记录不完整的部分，用于下次输出
		bBadEnd = true;
	}

	if (bBadEnd)
		iCurCount--; // 最后一行不完整时，不输出，等待下次输出

	for (int i= 0;i< iCurCount;i++)
	{
		m_ListBox.AddString(vStrMsg[i].c_str()); // 只增加新的
		int iSize = m_ListBox.GetCount();
		m_ListBox.SetCaretIndex(iSize-1,TRUE);
		m_ListBox.SetCurSel(iSize-1);
	}

	//MessageBox(g_strSubThread.c_str(),"提示",MB_OK|MB_ICONINFORMATION);  
	return 0;
}
void CRePrintToWndTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	static bool bProcessOK = true;
	char buffer[1024] = {0};  
	DWORD bytesRead; 
	static string strLast ="";

	if (ReadFile(g_hRead,buffer,1023,&bytesRead,NULL) == NULL) //读取管道  
	{
		g_ExeProcessId = 0;
		if (g_hRead)
			CloseHandle(g_hRead); 

		bProcessOK  = true;
		SetTimer(g_TimerId,-1,NULL); // 结束定时器
		return CDialog::OnTimer(nIDEvent);
	}

	if (strcmp(buffer,"")==0)
		return CDialog::OnTimer(nIDEvent);

	SetTimer(g_TimerId,-1,NULL); // 结束定时器

	while(!bProcessOK); // 阻止修改字符串

	m_strCallMsg.clear();
	m_strCallMsg += strLast; //最后一个字符串不完整，需要与下次的合并组成完整的串
	m_strCallMsg += buffer;

	bProcessOK = false; // 开始处理字符串
	vector<string> vStrMsg;
	StrSplit(m_strCallMsg.c_str(),vStrMsg,"\n");

	int iCurCount = vStrMsg.size();

	int iLen = m_strCallMsg.length();
	bool bBadEnd = false;
	if(m_strCallMsg[iLen-1] != '\n')
	{
		strLast = vStrMsg[iCurCount-1]; // 最后一行不完整时，记录不完整的部分，用于下次输出
		bBadEnd = true;
	}

	if (bBadEnd)
		iCurCount--; // 最后一行不完整时，不输出，等待下次输出

	for (int i= 0;i< iCurCount;i++)
	{
		m_ListBox.AddString(vStrMsg[i].c_str()); // 只增加新的
		int iSize = m_ListBox.GetCount();
		m_ListBox.SetCaretIndex(iSize-1,TRUE);
		m_ListBox.SetCurSel(iSize-1);
	}
	SetTimer(g_TimerId,g_TimerElapse,NULL); // 重新启动定时器

	bProcessOK = true;

	CDialog::OnTimer(nIDEvent);
}

void CRePrintToWndTestDlg::OnBnClickedBtnStart2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strCallMsg.clear();
	m_ListBox.ResetContent();
	g_hRead = NULL;
	const char *pszCmd = "doxygen.exe D:\\doxyWorkdir\\Doxyfile";
	CallExe(pszCmd,false);
}

void CRePrintToWndTestDlg::OnBnClickedBtnStart3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strCallMsg.clear();
	m_ListBox.ResetContent();
	g_hRead = NULL;
	const char *pszCmd = "\"C:\\Program Files (x86)\\NSIS\\makensis.exe\" /V4 D:\\RsSrcDir\\Release\\国土大区域打包安装脚本.nsi";
	CallExe(pszCmd,false);
}
