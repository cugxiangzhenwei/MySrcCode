
// RePrintCallExeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RePrintCallExe.h"
#include "RePrintCallExeDlg.h"
#include<vector>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD  g_ExeProcessID = 0;

static const int g_iTimerElpase = 20; //  定时器时间间隔
static UINT_PTR  g_TimerID = 0;
HANDLE g_hRead = NULL;

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

// CRePrintCallExeDlg 对话框
void CRePrintCallExeDlg::CallExe(const char *pszCmd,bool bShowWnd)
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
	si.wShowWindow = SW_HIDE; //隐藏窗口  
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  


	char cmdline[200];  
	sprintf(cmdline,"%s",pszCmd);  

	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) //创建子进程  
	{  
		MessageBox("CreateProcess Failed!","提示",MB_OK | MB_ICONWARNING);  
		return;  
	}  
	CloseHandle(hWrite); //关闭管道句柄  

	g_hRead = hRead;
	g_ExeProcessID = pi.dwProcessId;

	SetTimer(g_TimerID,g_iTimerElpase,NULL); // 设置定时器
}

CRePrintCallExeDlg::CRePrintCallExeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRePrintCallExeDlg::IDD, pParent),m_strLast("")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRePrintCallExeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG, m_ListBox);
}

BEGIN_MESSAGE_MAP(CRePrintCallExeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CALL_EXE, &CRePrintCallExeDlg::OnBnClickedBtnCallExe)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRePrintCallExeDlg 消息处理程序

BOOL CRePrintCallExeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRePrintCallExeDlg::OnPaint()
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
HCURSOR CRePrintCallExeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRePrintCallExeDlg::OnBnClickedBtnCallExe()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strLast = "";
	m_ListBox.ResetContent(); // 清空内容
	g_ExeProcessID  = 0;
	const char * pszCmd = "\"D:\\Program Files (x86)\\NSIS\\makensis.exe\"  F:\\RsSrcDir\\Release\\国土大区域打包安装脚本.nsi";
	CallExe(pszCmd,false);
}

void CRePrintCallExeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	char buffer[1024] = {0};  
	DWORD bytesRead;  
	if (ReadFile(g_hRead,buffer,1023,&bytesRead,NULL) == NULL) //读取管道
	{
		CloseHandle(g_hRead);  
		g_hRead = NULL;
		SetTimer(g_TimerID,-1,NULL);
		g_ExeProcessID = 0;
	}

	string strThisTime  = m_strLast;
	strThisTime += buffer;

	strThisTime = StrTrim(strThisTime.c_str());
	if (strThisTime.empty())
		return CDialog::OnTimer(nIDEvent); // 未捕捉到消息

	SetTimer(g_TimerID,-1,NULL);

	vector<string> vStrLine;
	StrSplit(strThisTime.c_str(),vStrLine,"\n");

	bool bBadEnd = false;
	int iLen = strThisTime.length();

	int iCount = vStrLine.size();
	if (strThisTime[iLen-1]!='\n')
	{
		m_strLast = vStrLine[iCount-1]; // 最后一行消息不完整，记录下来，下次输出 
		bBadEnd = true;
	}

	if (bBadEnd)
		iCount --; // 最后一行消息不完整,最后一行本次不输出

	for(int i=0;i< iCount;i++)
	{
		m_ListBox.AddString(vStrLine[i].c_str());
		int iListSize = m_ListBox.GetCount();
		m_ListBox.SetCaretIndex(iListSize-1,TRUE);
		m_ListBox.SetCurSel(iListSize-1);
	}

	SetTimer(g_TimerID,g_iTimerElpase,NULL);

	CDialog::OnTimer(nIDEvent);
}
