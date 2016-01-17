
// TestClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestClient.h"
#include "TestClientDlg.h"
#include "afxdialogex.h"
#include "Core.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CTestClientDlg 对话框




CTestClientDlg::CTestClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CHECK_LICENSE, &CTestClientDlg::OnBnClickedButtonCheckLicense)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_KEY, &CTestClientDlg::OnBnClickedButtonSaveKey)
	ON_BN_CLICKED(IDC_BUTTON_GET_PC_CODE, &CTestClientDlg::OnBnClickedButtonGetPcCode)
	ON_BN_CLICKED(IDC_BUTTON_Get_LICENSE, &CTestClientDlg::OnBnClickedButtonGetLicense)
END_MESSAGE_MAP()


// CTestClientDlg 消息处理程序

BOOL CTestClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	char * pszKeyVal = GetRegsiterKeyFromReg("TestClient.exe");
	string strKeyVal  = pszKeyVal;
	FreeStrMen(pszKeyVal);
	if (strKeyVal.empty())
	{
		SetWindowText("未注册版本！");
	}
	else
	{
		char *pszId = GetPcIdStr();
		string strId = pszId;
		FreeStrMen(pszId);
		string strErrMsg;
		if (!IsValidCode(strId.c_str(),"TestClient.exe",strKeyVal.c_str(),strErrMsg))   // 校验许可
		{
			SetWindowText(strErrMsg.c_str());
		}
		else
			SetWindowText("正式版本！");

	}
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestClientDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestClientDlg::OnBnClickedButtonCheckLicense()
{
	// TODO: 在此添加控件通知处理程序代码
	char *pszKey = GetRegsiterKeyFromReg("TestClient.exe");
	string strKey = pszKey;
	FreeStrMen(pszKey);

	if (strKey.empty())
	{
		MessageBox("许可不存在！","提示",MB_ICONERROR|MB_OK);
	}
	else
	{
		string strErrMsg;
		char *pszId = GetPcIdStr();
		string strId = pszId;
		FreeStrMen(pszId);
		if (!IsValidCode(strId.c_str(),"TestClient.exe",strKey.c_str(),strErrMsg))   // 校验许可
		{
			MessageBox(strErrMsg.c_str(),"提示",MB_ICONINFORMATION|MB_OK);
		}
		else
			MessageBox("许可有效！","提示",MB_ICONINFORMATION|MB_OK);

	}
}


void CTestClientDlg::OnBnClickedButtonSaveKey()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_strLicenseData.IsEmpty())
	{
		MessageBox("许可不存在！","提示",MB_ICONERROR|MB_OK);
		return;
	}
	char *pszId = GetPcIdStr();
	string strId = pszId;
	FreeStrMen(pszId);
	string strErrMsg;
	if (!IsValidCode(strId.c_str(),"TestClient.exe",m_strLicenseData,strErrMsg))
	{
		MessageBox(strErrMsg.c_str(),"提示",MB_ICONERROR|MB_OK);
		return;
	}

	if (!WriteLicenseToReg(m_strLicenseData,"TestClient.exe"))
	{
		MessageBox("许可导入失败！","提示",MB_ICONERROR|MB_OK);
		return;
	}

	MessageBox("导入成功！","提示",MB_ICONINFORMATION|MB_OK);
	SetWindowText("正式版本！");
}


void CTestClientDlg::OnBnClickedButtonGetPcCode()
{
	// TODO: 在此添加控件通知处理程序代码
	char *pszId = GetPcIdStr();
	string strId = pszId;
	FreeStrMen(pszId);
	CString str;
	str.Format("注册码为:%s",strId.c_str());
	MessageBox(str,"提示",MB_ICONINFORMATION|MB_OK);
}


void CTestClientDlg::OnBnClickedButtonGetLicense() // 申请许可
{
	// TODO: 在此添加控件通知处理程序代码
	char *pszId = GetPcIdStr();
	string strId = pszId;
	FreeStrMen(pszId);

	
	char * pszKey = GetRegsiterKey(strId.c_str(),"TestClient.exe",1);
	string strVal = pszKey;
	FreeStrMen(pszKey);

	m_strLicenseData = strVal.c_str();
	CString strMsg;
	strMsg.Format("许可：%s",m_strLicenseData);
	MessageBox(strMsg,"提示",MB_ICONINFORMATION|MB_OK);
}
