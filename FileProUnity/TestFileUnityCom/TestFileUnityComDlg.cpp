
// TestFileUnityComDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestFileUnityCom.h"
#include "TestFileUnityComDlg.h"
#include "afxdialogex.h"


//JSON 头文件
#include "value.h" 
#include "reader.h"
#include "writer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <string>

#import "../Debug/FileProUnity.dll" no_namespace

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


// CTestFileUnityComDlg 对话框




CTestFileUnityComDlg::CTestFileUnityComDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestFileUnityComDlg::IDD, pParent)
	, m_StrCopyInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestFileUnityComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_StrCopyInfo);
}

BEGIN_MESSAGE_MAP(CTestFileUnityComDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestFileUnityComDlg::OnBnClickedOpenFileDlg)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestFileUnityComDlg::OnBtnSelectDir)
	ON_BN_CLICKED(IDC_BTN_FILE_INFO, &CTestFileUnityComDlg::OnBnClickedBtnFileInfo)
	ON_BN_CLICKED(IDC_BTN_DeleteFile, &CTestFileUnityComDlg::OnBnClickedBtnDeletefile)
	ON_BN_CLICKED(IDC_BTN_Get_MD5, &CTestFileUnityComDlg::OnBnClickedBtnGetMd5)
	ON_BN_CLICKED(IDC_BTN_OPEN_FILE, &CTestFileUnityComDlg::OnBnClickedBtnOpenFile)
	ON_BN_CLICKED(IDC_BTN_CopyFile, &CTestFileUnityComDlg::OnBnClickedBtnCopyfile)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTestFileUnityComDlg 消息处理程序

BOOL CTestFileUnityComDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestFileUnityComDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestFileUnityComDlg::OnPaint()
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
HCURSOR CTestFileUnityComDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestFileUnityComDlg::OnBnClickedOpenFileDlg()
{
	// TODO: 在此添加控件通知处理程序代码
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("获取com对象失败!","提示");
		return;
	}

	Json::Value obj;
	CString str =  "Word File(*doc;*.docx)|*.doc;*.docx|文本文件(*.txt)|*.txt|可执行程序(*exe)|*exe|所有文件(*.*)|*.*||";
	obj["filter"] = Json::Value( (LPCTSTR)str );
	obj["selectmode"] = Json::Value("single"); //"multi"多选,"single"单选，不设置则默认单选
	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();
	BSTR bsName;
	bsName = FileProObj->OpenFileDlg(bst);
	CString strOutMsg = (LPCTSTR)_bstr_t(bsName);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBtnSelectDir()
{
	// TODO: 在此添加控件通知处理程序代码
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("获取com对象失败!","提示");
		return;
	}
	BSTR bsName;
	bsName = FileProObj->SelectDirDlg("");
	CString strOutMsg =  (LPCTSTR)_bstr_t(bsName);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBnClickedBtnFileInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("获取com对象失败!","提示");
		return;
	}

	Json::Value obj;
	CString str =  "E:\\WorkDir\\FileProUnity\\libd\\jsond.lib";
	obj["file"] = Json::Value( (LPCTSTR)str );
	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();
	BSTR bsName;
	bsName = FileProObj->GetFileInfo(bst);
	CString strOutMsg =  (LPCTSTR)_bstr_t(bsName);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBnClickedBtnDeletefile()
{
	// TODO: 在此添加控件通知处理程序代码
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("获取com对象失败!","提示");
		return;
	}

	Json::Value obj;
	CString str =  "E:\\新建文本文档.txt";
	obj["file"] = Json::Value( (LPCTSTR)str );
	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();
	BSTR bstOut;
	bstOut = FileProObj->DeleteLocalFile(bst);

	CString strOutMsg =  (LPCTSTR)_bstr_t(bstOut);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBnClickedBtnGetMd5()
{
	// TODO: 在此添加控件通知处理程序代码
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("获取com对象失败!","提示");
		return;
	}

	Json::Value obj;
	CString str =  "E:\\新建文本文档.txt";
	obj["file"] = Json::Value( (LPCTSTR)str );
	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();

	BSTR bstOut;
	bstOut = FileProObj->GetFileMd5(bst);
	CString strOutMsg = (LPCTSTR)_bstr_t(bstOut);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBnClickedBtnOpenFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("获取com对象失败!","提示");
		return;
	}

	Json::Value obj;
	CString str =  "E:\\pera13.1分支及参数列表说明文档（赵飞）.docx";
	obj["file"] = Json::Value( (LPCTSTR)str );
	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();

	BSTR bstOut;
	bstOut = FileProObj->OpenFile(bst);
	CString strOutMsg =  (LPCTSTR)_bstr_t(bstOut);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBnClickedBtnCopyfile()
{
	// TODO: 在此添加控件通知处理程序代码
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("获取com对象失败!","提示");
		return;
	}

	Json::Value obj;
	CString str =  "Z:\\oarcle 10G_win32.zip";
	CString strDest = "E:\\oarcle.zip";
	obj["Source"] = Json::Value( (LPCTSTR)str );
	obj["Dest"]   = Json::Value( (LPCTSTR)strDest);

	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();

	BSTR bstOut;
	bstOut = FileProObj->CopyFileCom(bst);

	CString strOutMsg =  (LPCTSTR)_bstr_t(bstOut);
	SetTimer(1000,1500,NULL);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("获取com对象失败!","提示");
		return;
	}

	Json::Value obj;
	CString strDest = "E:\\oarcle.zip";
	obj["Dest"]   = Json::Value( (LPCTSTR)strDest);

	CString str = obj.toStyledString().c_str();
	BSTR bst = _bstr_t(str).GetBSTR();

	BSTR bstOut;
	bstOut = FileProObj->GetCopyProgress(bst);

	m_StrCopyInfo = (LPCTSTR)_bstr_t(bstOut);
	UpdateData(FALSE);

	CDialogEx::OnTimer(nIDEvent);
}
