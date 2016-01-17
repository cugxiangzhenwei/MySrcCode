
// PeraSqlToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PeraSqlTool.h"
#include "PeraSqlToolDlg.h"
#include "afxdialogex.h"
#include "Tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  WM_CMD_OUT_NEW WM_USER + 1
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

bool SortFolder(const CString &Dir1,CString &Dir2)
{
	#pragma TODO(排序函数)
	int iPosMid1  = Dir1.ReverseFind('\\')+1;
	int iPosMid2 = Dir2.ReverseFind('\\')+1;
	if (iPosMid1 <=0 || iPosMid2 <=0)
		return false;

	CString strSub1 = Dir1.Mid(iPosMid1);
	CString strSub2 = Dir2.Mid(iPosMid2);

	if (strSub1.Compare(strSub2) >0)
		return false;

	return true;
}
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


// CPeraSqlToolDlg 对话框




CPeraSqlToolDlg::CPeraSqlToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPeraSqlToolDlg::IDD, pParent)
	, m_StrUser(_T(""))
	, m_StrPassWd(_T(""))
	, m_strCaseName(_T(""))
	, m_StrSqlDir(_T(""))
	,m_strRunDir(_T(""))
{
	#pragma TODO(CPeraSqlToolDlg构造函数)
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCmdOutFile = NULL;
}
CPeraSqlToolDlg::~CPeraSqlToolDlg()
{
	/*if (!m_strRunDir.IsEmpty())
	{
	RemoveDir(m_strRunDir);
	}*/
}
void CPeraSqlToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER, m_StrUser);
	DDV_MaxChars(pDX, m_StrUser, 50);
	DDX_Text(pDX, IDC_EDIT_PSWD, m_StrPassWd);
	DDV_MaxChars(pDX, m_StrPassWd, 50);
	DDX_Text(pDX, IDC_EDIT_CASE_NAME, m_strCaseName);
	DDX_Text(pDX, IDC_EDIT_SQL_DIR, m_StrSqlDir);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
}

BEGIN_MESSAGE_MAP(CPeraSqlToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEL_SQL, &CPeraSqlToolDlg::OnBnClickedButtonSelSql)
	ON_BN_CLICKED(IDC_BUTTON_EXCUTE, &CPeraSqlToolDlg::OnBnClickedButtonExcute)
	ON_MESSAGE(WM_CMD_OUT_NEW,&CPeraSqlToolDlg::OnNewCmdMsg)
END_MESSAGE_MAP()


// CPeraSqlToolDlg 消息处理程序

BOOL CPeraSqlToolDlg::OnInitDialog()
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
	ShowProgressMsg("导入进度");
	m_ProgressCtrl.SendMessage(PBM_SETBKCOLOR, 0, RGB(0, 0, 0));//背景色
	m_ProgressCtrl.SendMessage(PBM_SETBARCOLOR, 0, RGB(0, 255, 0));//前景色
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPeraSqlToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPeraSqlToolDlg::OnPaint()
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
HCURSOR CPeraSqlToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);
		HWND hFind = ::GetWindow(hwnd,GW_CHILD);
		while(hFind)
		{
			char szName[256] ={0};
			GetClassName(hFind,szName,256);
			if (_stricmp(szName,"Edit")==0)
			{
				CEdit *pEdit = (CEdit*)CWnd::FromHandle(hFind);
				//pEdit->EnableWindow(FALSE);
				pEdit->SetReadOnly(TRUE); //将控件设置为只读，方便复制，但不允许修改
				break;
			}
			hFind = ::GetWindow(hFind,GW_HWNDNEXT);
		}

		break;
	}
	return 0;
}
void CPeraSqlToolDlg::OnBnClickedButtonSelSql()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 保存用户输入界面数据
	BROWSEINFO bi;  
	ZeroMemory(&bi,sizeof(BROWSEINFO)); 
	bi.hwndOwner = m_hWnd;   
	bi.lpszTitle = "请选择SQL目录：";  

	char strLastDir[1000] = {0};
	GetPrivateProfileString("PeraSqlTool","SQL_DIR_LAST","",strLastDir,1000,"PeraSqlTool.ini");
	bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)(LPCTSTR)strLastDir;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  

	if(NULL == pidl )
		return;

	CString strFolderOut = "";

	LPMALLOC pMalloc;
	SHGetPathFromIDList(pidl,strFolderOut.GetBuffer(1000));  
	if(SUCCEEDED(SHGetMalloc(&pMalloc))) 
	{  
		pMalloc->Free(pidl);  
		pMalloc->Release();  
	}
	m_StrSqlDir = strFolderOut;
	strFolderOut.ReleaseBuffer();
	UpdateData(FALSE);
	if (!m_StrSqlDir.IsEmpty())
	{
		WritePrivateProfileString("PeraSqlTool","SQL_DIR_LAST",m_StrSqlDir,"PeraSqlTool.ini");
	}
}
bool CPeraSqlToolDlg::IsItemOk(const CString & strItemDir)
{
	CString strSQlBat = strItemDir + "\\sql.bat";
	if(!PathFileExists(strSQlBat))
	{
		CString strErrMsg;
		strErrMsg.Format("在目录【%s】下未找到sql.bat文件",strItemDir);
		MessageBox(strErrMsg,"提示",MB_OK|MB_ICONINFORMATION);
		return false;
	}

	if (!PathFileExists(strItemDir + "\\sql"))
	{
		CString strErrMsg;
		strErrMsg.Format("在目录【%s】下未找到sql目录",strItemDir);
		MessageBox(strErrMsg,"提示",MB_OK|MB_ICONINFORMATION);
		return false;
	}
	CString strPeraSQl = strItemDir + "\\sql\\pera.sql";
	if( !PathFileExists(strPeraSQl))
	{
		CString strErrMsg;
		strErrMsg.Format("在目录【%s\\sql】下未找到pera.sql文件",strItemDir);
		MessageBox(strErrMsg,"提示",MB_OK|MB_ICONINFORMATION);
		return false;
	}
	return true;
}
BOOL CPeraSqlToolDlg::CheckFolder()
{
	ShowProgressMsg("正在校验输入数据...");
	CString strWildCard  = m_StrSqlDir;
	strWildCard +="\\*.*";
	CFileFind finder;
	BOOL bFind = FALSE;
	// 查找文件
	BOOL bErrOccured = FALSE;
	bFind = finder.FindFile(strWildCard);
	while(bFind)
	{
		// 查找下一个文件
		bFind = finder.FindNextFile();
		if (finder.IsDots())
			continue;

		//找到文件的路径
		CString strPathName = finder.GetFilePath();
		if (!finder.IsDirectory())
			continue;
		
		if (!IsItemOk(strPathName))
		{
			bErrOccured = TRUE;
			break;
		}
	}
	finder.Close();
	if (bErrOccured)
	{
		m_vStrDirItem.clear();
		return FALSE;
	}
	ShowProgressMsg("数据校验完毕...");
	return TRUE;
}
void CPeraSqlToolDlg::ShowProgressMsg(CString strMsg)
{
	SetDlgItemText(IDC_STATIC_PRO,strMsg);
}
BOOL CPeraSqlToolDlg::CopyDir(const char * pSrc,const char *pDes,int iLevelFolder)
{
	if (NULL == pSrc || NULL == pDes)	return -1;
	if (!PathFileExists(pDes))
	{
		if (CreateDirectory(pDes,NULL)==0)
		{
			ZTools::WriteZToolsFormatLog("创建目录[%s]失败",pDes);
			return FALSE;
		}
	}

	CString strWildCard  = pSrc;
	strWildCard +="\\*.*";
	CFileFind finder;
	BOOL bFind = FALSE;
	// 查找文件
	BOOL bErrOccured = FALSE;
	bFind = finder.FindFile(strWildCard);
	while(bFind)
	{
		// 查找下一个文件
		bFind = finder.FindNextFile();
		if (finder.IsDots())
			continue;

		//找到文件的路径
		CString strPathName = finder.GetFilePath();
		CString strDest = pDes;
		strDest += "\\";
		strDest += finder.GetFileName();

		if (finder.IsDirectory())
		{
			int iLevel = iLevelFolder + 1;
			if (!CopyDir(strPathName,strDest,iLevel))
			{
				ZTools::WriteZToolsFormatLog("复制目录[%s]到[%s]失败",strPathName,strDest);
				finder.Close();
				return FALSE;
			}
			if (iLevelFolder ==1)
				m_vStrDirItem.push_back(strDest);
		}
		else
		{
			CString strMsg;
			strMsg.Format("复制文件[%s]到[%s]",strPathName,strDest);
			ShowProgressMsg(strMsg);
			if (!::CopyFile(strPathName,strDest,FALSE))
			{
				ZTools::WriteZToolsFormatLog("复制文件[%s]到[%s]失败",strPathName,strDest);
				finder.Close();
				return FALSE;
			}
		}
	}
	finder.Close();
	return TRUE;
}
BOOL CPeraSqlToolDlg::RemoveDir(const CString & strDir)
{
	CString strWildCard  = strDir;
	strWildCard +="\\*.*";
	CFileFind finder;
	BOOL bFind = FALSE;
	// 查找文件
	BOOL bErrOccured = FALSE;
	bFind = finder.FindFile(strWildCard);
	while(bFind)
	{
		// 查找下一个文件
		bFind = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		//找到文件的路径
		CString strPathName = finder.GetFilePath();
		if (finder.IsDirectory())
			RemoveDir(strPathName);
		else
		{
			if (!::DeleteFile(strPathName))
			{
				ZTools::WriteZToolsFormatLog("删除文件%s失败",strPathName);
				finder.Close();
				return FALSE;
			}
		}
	}
	finder.Close();
	if (!::RemoveDirectory(strDir))
	{
		ZTools::WriteZToolsFormatLog("删除文件%s失败",strDir);
		return FALSE;
	}

	return TRUE;
}
void CPeraSqlToolDlg::OnBnClickedButtonExcute()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_StrUser.IsEmpty())
	{
		MessageBox("用户名不能为空","提示",MB_OK|MB_ICONINFORMATION);
		return;
	}

	if (m_StrPassWd.IsEmpty())
	{
		MessageBox("密码不能为空","提示",MB_OK|MB_ICONINFORMATION);
		return;
	}

	if (m_strCaseName.IsEmpty())
	{
		MessageBox("实例名不能为空","提示",MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (m_StrSqlDir.IsEmpty())
	{
		MessageBox("SQL目录不能为空","提示",MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (!PathFileExists(m_StrSqlDir))
	{
		CString strErrMsg;
		strErrMsg.Format("目录【%s】不存在",m_StrSqlDir);
		MessageBox(strErrMsg,"提示",MB_OK|MB_ICONERROR);
		return;
	}

	m_StrSqlDir.Replace("/","\\");
	m_StrSqlDir.Replace("\\\\","\\");
	ZTools::WriteZToolsFormatLog("输入参数：\n用户名：%s\n密码：%s\n实例名称:%s\nSQL目录：%s",
		m_StrUser,m_StrPassWd,m_strCaseName,m_StrSqlDir);

	m_ProgressCtrl.SetPos(0);
	HANDLE h = CreateThread(NULL,0,ThreadImportPro,this,0,NULL);
	CloseHandle(h); // 将内核对象的引用计数减少一个
}
BOOL CPeraSqlToolDlg::UpdateSqlBatFile(const CString & strBatFile)
{
	ZTools::WriteZToolsFormatLog("开始修改文件%s",strBatFile);
	FILE *pFile = NULL;
	fopen_s(&pFile,strBatFile,"rb"); // 以只读方式打开文件

	if (pFile==NULL)
	{
		ZTools::WriteZToolsFormatLog("打开文件[%s]失败...\n",strBatFile);
		return FALSE;
	}

	fseek(pFile,0,SEEK_END);
	int iFileSize = ftell(pFile);
	char * pBuf = new char[iFileSize+1];
	memset(pBuf,0,sizeof(char)*(iFileSize+1));
	fseek(pFile,0,SEEK_SET);
	size_t iRead = fread_s(pBuf,iFileSize,sizeof(char),iFileSize,pFile);
	fclose(pFile); // 读完之后关闭文件
	pFile = NULL;
	if (iRead != iFileSize)
	{
		delete pBuf;
		ZTools::WriteZToolsFormatLog("读取文件[%s]内容出错...\n",strBatFile);
		return FALSE;
	}

	pBuf[iFileSize] = 0;
	CString strSourceContent = pBuf;
	delete pBuf;
	strSourceContent.Replace("pause .....","");
	fopen_s(&pFile,strBatFile,"wb"); // 以写方式打开文件
	if (pFile==NULL)
	{
		ZTools::WriteZToolsFormatLog("写文件[%s]失败...\n",strBatFile);
		return FALSE;
	}

	fseek(pFile,0,SEEK_SET);
	size_t iLength = strSourceContent.GetLength();
	size_t iWritten = fwrite(strSourceContent,1,iLength,pFile);
	if (iLength!=iWritten)
	{
		ZTools::WriteZToolsFormatLog("写入文件[%s]内容出错...\n",strBatFile);
		return false;
	}

	fclose(pFile);
	pFile = NULL;
	ZTools::WriteZToolsFormatLog("修改文件%s完成",strBatFile);
	return TRUE;
}
BOOL CPeraSqlToolDlg::UpateSqlFile(const CString &strSqlFile)
{
	ZTools::WriteZToolsFormatLog("开始修改文件%s",strSqlFile);
	FILE *pFile = NULL;
	fopen_s(&pFile,strSqlFile,"rb"); // 以只读方式打开文件

	if (pFile==NULL)
	{
		ZTools::WriteZToolsFormatLog("打开文件[%s]失败...\n",strSqlFile);
		return FALSE;
	}

	fseek(pFile,0,SEEK_END);
	int iFileSize = ftell(pFile);
	char * pBuf = new char[iFileSize+1];
	memset(pBuf,0,sizeof(char)*(iFileSize+1));
	fseek(pFile,0,SEEK_SET);
	size_t iRead = fread_s(pBuf,iFileSize,sizeof(char),iFileSize,pFile);
	fclose(pFile); // 读完之后关闭文件
	pFile = NULL;
	if (iRead != iFileSize)
	{
		delete pBuf;
		ZTools::WriteZToolsFormatLog("读取文件[%s]内容出错...\n",strSqlFile);
		return FALSE;
	}

	pBuf[iFileSize] = 0;
	CString strSourceContent = pBuf;
	delete pBuf;
	int iPosConn = strSourceContent.Find("conn ");
	int iLineEnd = strSourceContent.Find("\n",iPosConn);
	CString strOld = strSourceContent.Mid(iPosConn,iLineEnd - iPosConn + 1);
	CString strConnNew;
	strConnNew.Format("conn %s/%s@%s\r\n",m_StrUser,m_StrPassWd,m_strCaseName);
	strSourceContent.Replace(strOld,strConnNew);
	fopen_s(&pFile,strSqlFile,"wb"); // 以写方式打开文件

	fseek(pFile,0,SEEK_SET);
	size_t iLength = strSourceContent.GetLength();
	size_t iWritten = fwrite(strSourceContent,1,iLength,pFile);
	if (iLength!=iWritten)
	{
		ZTools::WriteZToolsFormatLog("写入文件[%s]内容出错...\n",strSqlFile);
		return false;
	}

	fclose(pFile);
	pFile = NULL;
	ZTools::WriteZToolsFormatLog("修改文件%s完成",strSqlFile);
	return TRUE;
}
void CPeraSqlToolDlg::ExcuteBatImport()
{
	CString strMsg;
	CString strOldWordDir;
	ShowProgressMsg("正在执行批量导入...");
	GetCurrentDirectory(1000,strOldWordDir.GetBuffer(1000));
	CString strCmdOutFile = m_strRunDir + "_cmd.log";
	fopen_s(&m_pCmdOutFile,strCmdOutFile,"w+");
	m_List.ResetContent();
	m_ProgressCtrl.SetRange32(0,m_vStrDirItem.size());
	m_ProgressCtrl.ShowWindow(SW_SHOW);
	for (size_t i=0;i<m_vStrDirItem.size();i++)
	{
		CString strBatFile = m_vStrDirItem[i] + "\\sql.bat";
		CString strSqlFile =  m_vStrDirItem[i] + "\\sql\\pera.sql";
		strMsg.Format("导入%s",strSqlFile);
		ShowProgressMsg(strMsg);
		UpdateSqlBatFile(strBatFile);
		UpateSqlFile(strSqlFile);
		SetCurrentDirectory(m_vStrDirItem[i]); //切换工作目录到sql.bat所在目录
		ExcuteOneItem(m_vStrDirItem[i]);
		m_ProgressCtrl.SetPos(i+1); //更新进度条
	}
	if (m_pCmdOutFile)
	{
		fclose(m_pCmdOutFile);
		m_pCmdOutFile = NULL;
	}
	SetCurrentDirectory(strOldWordDir); // 还原工作目录
	strOldWordDir.ReleaseBuffer();
	ShowProgressMsg("清空临时目录...");
	m_bImportFinished = TRUE;
	/*BOOL bOK = RemoveDir(m_strRunDir);*/
	ShowProgressMsg("批量导入SQL完成!");
}
void CPeraSqlToolDlg::ExcuteOneItem(const CString & strItemDir)
{
	CString strMsg;
	strMsg.Format("执行%s\\sql.bat",strItemDir);
	ZTools::WriteZToolsFormatLog(strMsg);
	fprintf(m_pCmdOutFile,"%s\n",strMsg);
	m_List.AddString(strMsg);
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	//创建匿名管道
	if (!CreatePipe(&hRead,&hWrite,&sa,0)) 
	{
		ZTools::WriteZToolsFormatLog("创建匿名管道失败");
		return;
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;//数据输出用的文件句柄
	si.hStdOutput = hWrite;//数据输出用的文件句柄
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	//执行cmd命令,并在命名中管道中写入cmd命令返回的串
	if (!CreateProcess(NULL,"sql.bat",NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
	{
		ZTools::WriteZToolsLog("CreateProcess启动cmd进程失败!");
		return;
	}
	CloseHandle(hWrite);

	char buffer[4096] ={0} ;
	DWORD bytesRead;
	int iSKipCharCount = 0;

	CString strTmp; //用于保存非完整行
	while(1)
	{
		memset(buffer,0,4096);
		if(ReadFile(hRead,buffer,4096,&bytesRead,NULL) != NULL)//从命名管道中读取数据
		{
			iSKipCharCount = 0;
			CString str = buffer;
			if (str.GetLength()>=2)
			{
				if (buffer[0]=='\n' || buffer[0]=='\r')
				{
					iSKipCharCount++;
					if (buffer[1]=='\n' || buffer[2]=='\r')
						iSKipCharCount++;
				}
			}
			str = buffer + iSKipCharCount;
			if (str.IsEmpty())
				continue;

			strTmp +=str;
			if (strTmp.Right(1)=="\n")
			{
				fprintf(m_pCmdOutFile,"%s",strTmp);
				CString *pMsg = new CString;
				*pMsg = strTmp;
				PostMessage(WM_CMD_OUT_NEW,0,(LPARAM)pMsg);
				strTmp.Empty(); //清空临时保存的非完整行
			}
			else
				continue;
		}
		else
		{
			break;
		}
	}
	CloseHandle(hRead);
	fflush(m_pCmdOutFile);
	//ShellExecute(NULL,"open","sql.bat","",strItemDir,SW_HIDE);
}
void CPeraSqlToolDlg::UpdateUI(BOOL bEnable)
{
	GetDlgItem(IDC_BUTTON_SEL_SQL)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_EXCUTE)->EnableWindow(bEnable);
	GetDlgItem(IDCANCEL)->EnableWindow(bEnable);
}
DWORD WINAPI CPeraSqlToolDlg::ThreadImportPro(LPVOID pThreadParam)
{
	CPeraSqlToolDlg *pThis = (CPeraSqlToolDlg*)pThreadParam;
	pThis->UpdateUI(FALSE);
	if (!pThis->CheckFolder())
	{
		pThis->UpdateUI(TRUE);
		return 0; // 此处不弹出错误消息，因为里面会弹
	}

	pThis->ShowProgressMsg("正在拷贝数据到运行目录...");
	char szDir[500] = {0};
	GetModuleFileName(NULL,szDir,500);
	CString strRunDir = szDir;
	strRunDir = strRunDir.Left(strRunDir.ReverseFind('\\') + 1) + "Run_";

	CString strTime = ZTools::GetNowTimeStr().c_str();
	strTime.Replace(":","-");
	strRunDir +=strTime;
	pThis->m_strRunDir = strRunDir;
	/*if (PathFileExists(pThis->m_strRunDir))
		pThis->RemoveDir(pThis->m_strRunDir);*/

	pThis->m_vStrDirItem.clear();
	ZTools::WriteZToolsFormatLog("复制%s目录到%s",pThis->m_StrSqlDir,pThis->m_strRunDir);
	pThis->CopyDir(pThis->m_StrSqlDir,strRunDir,1);
	if (pThis->m_vStrDirItem.empty())
	{
		pThis->UpdateUI(TRUE);
		pThis->MessageBox("未找到SQL导入文件,请指定正确的SQL目录！","提示",MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	ZTools::WriteZToolsFormatLog("共找到sql.bat目录%d个",pThis->m_vStrDirItem.size());
	std::sort(pThis->m_vStrDirItem.begin(),pThis->m_vStrDirItem.end(),SortFolder);

	{ //将排序结果写入日志
		CString strLogText;
		for (size_t i=0;i< pThis->m_vStrDirItem.size();i++)
		{
			strLogText += pThis->m_vStrDirItem[i];
			strLogText +="\n";
		}
		ZTools::WriteZToolsFormatLog("文件夹排序结果为:\n%s",strLogText);
	}
	pThis->m_bImportFinished = FALSE;
	pThis->ExcuteBatImport();
	pThis->UpdateUI(TRUE);
	return 0;
}
LRESULT CPeraSqlToolDlg::OnNewCmdMsg(WPARAM wParam,LPARAM lParam)
{
	CString *pMsg = (CString*)lParam;
	m_List.AddString(*pMsg);
	delete pMsg;
	m_List.SetCurSel(m_List.GetCount()-1);
	return NULL;
}