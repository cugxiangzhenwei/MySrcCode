// FileManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileManager.h"
#include "FileManagerDlg.h"
#include "SqliteBase.h"
#include "FilePro.h"
#include "sqlite3.h"
#include "CodeFunc.h"
#include "ShellContextMenu.h"
#include "ThreadPoolManager.h"
#include "MonitorDiskTask.h"
#include "WriteMonitorResult.h"
#include "LogTools.h"
#include "FileManagerUnity.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4996)

#define  KB 1024
#define  MB (1024*KB)
#define  GB (1024*MB)
#define  TB (1024*GB)


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
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


// CFileManagerDlg dialog


long g_AllCount = 0;

CFileManagerDlg::CFileManagerDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CFileManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDataBase = NULL;
	m_bExitScanDisk = FALSE;
	m_pScanThreadHandleArry = NULL;
	m_iHandleCount = 0;
	m_iFinishedThreadCount = 0;
	m_bRunAtBack = FALSE;
	m_bIsEditingSelect = FALSE;
	m_bStartWithWindow = FALSE;
}
CFileManagerDlg::~CFileManagerDlg()
{
	if (m_pDataBase)
	{
		delete m_pDataBase;
		m_pDataBase = NULL;
	}
	if (m_pScanThreadHandleArry)
	{
		delete [] m_pScanThreadHandleArry;
		m_pScanThreadHandleArry = NULL;
	}
	DeleteCriticalSection(&(FileProTool::g_cs));
}
void CFileManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditSelect);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_btnSelect);
}

BEGIN_MESSAGE_MAP(CFileManagerDlg, CBCGPDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_OPEN_ITEM, &CFileManagerDlg::OnOpenItem)
	ON_COMMAND(ID_OPEN_DIR, &CFileManagerDlg::OnOpenDir)
	ON_COMMAND(ID_SHOW_ATTRIBUTE, &CFileManagerDlg::OnShowAttribute)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CFileManagerDlg::OnNMRClickList1)
	ON_MESSAGE(WM_SCAN_RESULT,&CFileManagerDlg::OnMsgScanResult)
	ON_MESSAGE(WM_SCAN_FINISHED,&CFileManagerDlg::OnMsgScanFinished)
	ON_MESSAGE(WM_UPDATE_SEARCH,&CFileManagerDlg::OnMsgUpdateList)
	ON_WM_INITMENUPOPUP()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CFileManagerDlg::OnBnClickedButtonSelect)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CFileManagerDlg::OnNMDblclkList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CFileManagerDlg::OnNMClickList1)
	ON_UPDATE_COMMAND_UI(ID_OPEN_ITEM, &CFileManagerDlg::OnUpdateOpenItem)
	ON_UPDATE_COMMAND_UI(ID_OPEN_DIR, &CFileManagerDlg::OnUpdateOpenDir)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ATTRIBUTE, &CFileManagerDlg::OnUpdateShowAttribute)
	ON_UPDATE_COMMAND_UI(WM_START_AFTER_WINDOWS, &CFileManagerDlg::OnUpdateStartWithWindow)
	ON_EN_CHANGE(IDC_EDIT1, &CFileManagerDlg::OnEnChangeEdit1)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_MAIN_NOTIFY_MSG,&CFileManagerDlg::OnMsgNotifyIcon)
	ON_COMMAND(WM_START_AFTER_WINDOWS,&CFileManagerDlg:: OnMsgNotifyStartWithWindow)
	ON_COMMAND(WM_MAIN_SHOW,&CFileManagerDlg::OnMsgNotifyOpen)
	ON_COMMAND(WM_EXIT_APP,&CFileManagerDlg::OnExitApp)
	ON_COMMAND(ID_COPY_FILE, &CFileManagerDlg::OnCopyFile)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_CUST, &CFileManagerDlg::OnFileCust)
	ON_UPDATE_COMMAND_UI(ID_FILE_CUST, &CFileManagerDlg::OnUpdateFileCust)
	ON_COMMAND(ID_FILE_DELETE, &CFileManagerDlg::OnFileDelete)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE, &CFileManagerDlg::OnUpdateFileDelete)
END_MESSAGE_MAP()


// CFileManagerDlg message handlers

BOOL CFileManagerDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	GetUsrSettings();
	// 设置对话框的大小和位置
	CRect rectScreen;
	::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);
	rectScreen.InflateRect(-1,-1,-1,-1);
	MoveWindow(rectScreen); 

	CRect rectClient;
	GetClientRect(rectClient);
	// 设置搜索框和按钮的大小
	m_EditSelect.MoveWindow(0,0,rectClient.Width()-60,25);
	m_btnSelect.MoveWindow(rectClient.Width()-60,0,60,25);

	// 设置按钮列表框的大小和位置
	rectClient.top = rectClient.top + 25;
	rectClient.bottom = rectClient.bottom - 22;
	m_ListCtrl.Create(LVS_REPORT | LVS_ALIGNLEFT /*|LVS_SHOWSELALWAYS*/|WS_BORDER |LVS_OWNERDRAWFIXED| WS_TABSTOP|LVS_OWNERDATA |WS_VISIBLE,rectClient,this,IDC_LIST1);
	m_ListCtrl.MoveWindow(rectClient);

	GetClientRect(rectClient);
	rectClient.top=rectClient.bottom-20; //设置状态栏的矩形区域
	m_StatusBar.Create(WS_CHILD|WS_VISIBLE, rectClient, this, 0);

	int strPartDim[3]= {300, 100, -1}; //分割数量
	m_StatusBar.SetParts(3, strPartDim);

	//设置状态栏文本
	m_StatusBar.SetText("扫描状态:", 0, 0);
	m_StatusBar.SetText("文件数量：", 2, 0);
	HICON hICon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_StatusBar.SetIcon(0,hICon );//为第一个分栏中加的图标

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES); // 添加网格属性

	SHFILEINFO sfi;
	// 获得系统图像列表
	HIMAGELIST sys_small_icon = (HIMAGELIST)SHGetFileInfo(NULL,0,&sfi,sizeof(sfi),SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	m_IconList.Attach(sys_small_icon);
	m_ListCtrl.SetImageList(&m_IconList,LVSIL_SMALL); // 设置列表视图图像列表
	m_IconList.Detach();
	m_ListCtrl.InsertColumn(0,"名称",LVCFMT_LEFT,350);
	m_ListCtrl.InsertColumn(1,"路径",LVCFMT_LEFT,600);
	m_ListCtrl.InsertColumn(2,"大小",LVCFMT_LEFT,60);
	m_ListCtrl.InsertColumn(4,"创建时间",LVCFMT_LEFT,160);
	m_ListCtrl.InsertColumn(4,"更新时间",LVCFMT_LEFT,160);

	ShowNotifyIcon(TRUE);
	if (m_bRunAtBack)
	{
		SetTimer(1,50,NULL);
	}
	LoadDataBase();
	SetWindowText("文件快速搜索工具");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CBCGPDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileManagerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBCGPDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CFileManagerDlg::TransferFileSize(ULONGLONG fSize)
{
	CString strSize;
	if (fSize >= GB)
	{
		double dGB = fSize * 1.0 / GB;
		strSize.Format("%.2fGB",dGB);
	}
	else if (fSize >= MB)
	{
		double dMB = fSize * 1.0 / MB;
		strSize.Format("%.2fMB",dMB);
	}
	else if (fSize >= KB)
	{
		double dKB = fSize * 1.0 / KB;
		strSize.Format("%.2fKB",dKB);
	}
	else
		strSize.Format("%lldByte",fSize);

	return strSize;
}
BOOL CFileManagerDlg::GetFileInfo(const CString & strFile,CFileInfo & fInfo)
{
	SHFILEINFO sfi;
	SHGetFileInfo(strFile,0,&sfi,sizeof(sfi),SHGFI_DISPLAYNAME|SHGFI_SMALLICON|SHGFI_SYSICONINDEX);
	fInfo.m_iImageID = sfi.iIcon;
	int iPos = strFile.ReverseFind(_T('\\'));
	fInfo.m_strDir = strFile.Left(iPos);
	fInfo.m_strName = strFile.Mid(iPos+1);


	WIN32_FIND_DATAW DataFind;
	CStringW strFileW = (WCHAR*)_bstr_t(strFile);
	HANDLE hFind = FindFirstFileW(strFileW,&DataFind);
	if(DataFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		FindClose(hFind);
		return TRUE;
	}

	SYSTEMTIME  st;
	FileTimeToSystemTime(&DataFind.ftLastWriteTime,&st);
	fInfo.m_strModifyTime.Format("%04d/%02d/%02d %02d:%02d:%02d",st.wYear,st.wMonth,st.wDay, st.wHour, st.wMinute, st.wSecond);
	FileTimeToSystemTime(&DataFind.ftCreationTime,&st);
	fInfo.m_strCreateTime.Format("%04d/%02d/%02d %02d:%02d:%02d",st.wYear,st.wMonth,st.wDay, st.wHour, st.wMinute, st.wSecond);
	ULONGLONG lSize = ULONGLONG(DataFind.nFileSizeHigh)<< 32;
	lSize = lSize + DataFind.nFileSizeLow;
	fInfo.m_strFileSize = TransferFileSize(lSize);
	FindClose(hFind);
	return TRUE;
}
void CFileManagerDlg::InsertFileItem(const CFileInfo & ifo)
{
	int iItem = m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(),(LPCTSTR)ifo.m_strName,ifo.m_iImageID);
	m_ListCtrl.SetItemText(iItem,1,ifo.m_strDir);
	m_ListCtrl.SetItemText(iItem,2,ifo.m_strFileSize);
	m_ListCtrl.SetItemText(iItem,3,ifo.m_strCreateTime);
	m_ListCtrl.SetItemText(iItem,4,ifo.m_strModifyTime);
}

void CFileManagerDlg::OnOpenItem()
{
	// TODO: 在此添加命令处理程序代码
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_NO_UI;
	ShExecInfo.hwnd = GetSafeHwnd();
	ShExecInfo.lpVerb = "Open";      
	ShExecInfo.lpParameters = NULL;  
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;	

	POSITION p = m_ListCtrl.GetFirstSelectedItemPosition();
	CString strPath;
	while(p)
	{
		int iRow = m_ListCtrl.GetNextSelectedItem(p);
		strPath = m_ListCtrl.GetItemTextEx(iRow,1) + "\\";
		strPath += m_ListCtrl.GetItemTextEx(iRow,0);
		ShExecInfo.lpFile = strPath;  
		ShellExecuteEx(&ShExecInfo);
	}
}


void CFileManagerDlg::OnOpenDir()
{
	// TODO: 在此添加命令处理程序代码
	POSITION p = m_ListCtrl.GetFirstSelectedItemPosition();
	CString strPath;
	while(p)
	{
		int iRow = m_ListCtrl.GetNextSelectedItem(p);
		strPath = m_ListCtrl.GetItemTextEx(iRow,1) + "\\";
		strPath += m_ListCtrl.GetItemTextEx(iRow,0);
		CString strShellParam;
		strShellParam.Format(" /select ,%s",strPath);
		ShellExecuteA(0,"open","explorer.exe",strShellParam,NULL,SW_SHOWNORMAL);
	}
}


void CFileManagerDlg::OnShowAttribute()
{
	// TODO: 在此添加命令处理程序代码
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask =  SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI | SEE_MASK_INVOKEIDLIST;
	ShExecInfo.hwnd = GetSafeHwnd();
	ShExecInfo.lpVerb = "properties";      
	ShExecInfo.lpParameters = NULL;  
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOWNORMAL;
	ShExecInfo.hInstApp = NULL;	

	POSITION p = m_ListCtrl.GetFirstSelectedItemPosition();
	CString strPath;
	while(p)
	{
		int iRow = m_ListCtrl.GetNextSelectedItem(p);
		strPath = m_ListCtrl.GetItemTextEx(iRow,1) + "\\";
		strPath += m_ListCtrl.GetItemTextEx(iRow,0);
		ShExecInfo.lpFile = strPath;  
		ShellExecuteEx(&ShExecInfo);
	}
}


void CFileManagerDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	// TODO: 在此添加控件通知处理程序代码

	POSITION p = m_ListCtrl.GetFirstSelectedItemPosition();
	CStringArray strArray;
	while(p)
	{
		int iRow = m_ListCtrl.GetNextSelectedItem(p);
		CString strPath = m_ListCtrl.GetItemTextEx(iRow,1) + "\\";
		strPath += m_ListCtrl.GetItemTextEx(iRow,0);
		strArray.Add(strPath);
	}

	CMenu popMenu;
	popMenu.LoadMenu(IDR_MENU1);
	CMenu *pPopupMenu = popMenu.GetSubMenu(0);
	//为菜单项设置图标

	CString strExePath;
	GetModuleFileName(NULL,strExePath.GetBuffer(MAX_PATH),MAX_PATH);
	strExePath.ReleaseBuffer();
	strExePath = strExePath.Left(strExePath.ReverseFind('\\'));

	CImage imageOpen;
	CString strImg = strExePath + "\\res\\book_open.png";
	HRESULT hr = imageOpen.Load(strImg);
	HBITMAP hbitmap = imageOpen.Detach();
	CBitmap MenuBitMapOpen;
	MenuBitMapOpen.Attach(hbitmap);
	pPopupMenu->SetMenuItemBitmaps(ID_OPEN_ITEM,MF_BYCOMMAND,&MenuBitMapOpen,NULL);


	CImage imageOpenDir;
	strImg = strExePath + "\\res\\folder_go.png";
	hr = imageOpenDir.Load(strImg);
	hbitmap = imageOpenDir.Detach();
	CBitmap MenuBitMapOpenDir;
	MenuBitMapOpenDir.Attach(hbitmap);
	pPopupMenu->SetMenuItemBitmaps(ID_OPEN_DIR,MF_BYCOMMAND,&MenuBitMapOpenDir,NULL);

	CImage imageInfo;
	strImg = strExePath + "\\res\\information.png";
	hr = imageInfo.Load(strImg);
	hbitmap = imageInfo.Detach();
	CBitmap MenuBitMap3;
	MenuBitMap3.Attach(hbitmap);
	pPopupMenu->SetMenuItemBitmaps(ID_SHOW_ATTRIBUTE,MF_BYCOMMAND,&MenuBitMap3,NULL);

	CImage imageCopy;
	strImg = strExePath + "\\res\\edit-copy.png";
	hr = imageCopy.Load(strImg);
	hbitmap = imageCopy.Detach();
	CBitmap MenuBitMapCopy;
	MenuBitMapCopy.Attach(hbitmap);
	pPopupMenu->SetMenuItemBitmaps(ID_COPY_FILE,MF_BYCOMMAND,&MenuBitMapCopy,NULL);

	CImage imageCut;
	strImg = strExePath + "\\res\\edit-cut.png";
	hr = imageCut.Load(strImg);
	hbitmap = imageCut.Detach();
	CBitmap MenuBitMapCut;
	MenuBitMapCut.Attach(hbitmap);
	pPopupMenu->SetMenuItemBitmaps(ID_FILE_CUST,MF_BYCOMMAND,&MenuBitMapCut,NULL);

	CImage imageDelete;
	strImg = strExePath + "\\res\\edit-delete.png";
	hr = imageDelete.Load(strImg);
	hbitmap = imageDelete.Detach();
	CBitmap MenuBitMapDelete;
	MenuBitMapDelete.Attach(hbitmap);
	pPopupMenu->SetMenuItemBitmaps(ID_FILE_DELETE,MF_BYCOMMAND,&MenuBitMapDelete,NULL);

	CPoint pt;
	GetCursorPos(&pt);
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN,pt.x,pt.y,this);
}


void CFileManagerDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CBCGPDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: 在此处添加消息处理程序代码
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child Windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}
void CFileManagerDlg::QueryFromDataBase(const CString & strKeyWords,BOOL bMatchEnd,BOOL bMatchStart)
{
	CString strSql;
	if (strKeyWords=="*" || strKeyWords== "*.*" || strKeyWords== ".*")
	{
		strSql.Format("select * from FILE;",strKeyWords);
	}
	else if (bMatchStart && bMatchEnd)
	{
		char * zSQL = sqlite3_mprintf("select * from FILE where path ='%q';",strKeyWords);
		strSql = zSQL;
		sqlite3_free(zSQL);
	}
	else if (bMatchStart)
	{
		char * zSQL = sqlite3_mprintf("select * from FILE where path like '%q%%';",strKeyWords);
		strSql = zSQL;
		sqlite3_free(zSQL);
	}
	else if (bMatchEnd)
	{
		char * zSQL = sqlite3_mprintf("select * from FILE where path like '%%%q';",strKeyWords);
		strSql = zSQL;
		sqlite3_free(zSQL);
	}
	else
	{
		char * zSQL = sqlite3_mprintf("select * from FILE where path like '%%%q%%';",strKeyWords);
		strSql = zSQL;
		sqlite3_free(zSQL);
	}

	m_StatusBar.SetText("",2,0);
	m_StatusBar.SetText("正在搜索...",0,0);

	int nRows,nColums;
	char ** pszDbResult = NULL;
	m_pDataBase->SelectRecord(strSql,nRows,nColums,pszDbResult);
	LogMsg("查询关键字【%s】,匹配行[%d],匹配列【%d】\n",strKeyWords,nRows,nColums);
	m_ListCtrl.InsertAll(nRows,nColums,pszDbResult);

	CString strNum;
	strNum.Format("文件数量:%ld",nRows);
	m_StatusBar.SetText(strNum,2,0);
	m_StatusBar.SetText("加载完毕！",0,0);

	m_pDataBase->FreeSelectResult(pszDbResult);
}
void CFileManagerDlg::StartDiskMonitor()
{
	InitializeCriticalSection(&g_FileActionListCS); // 初始化监控互斥区

	// 开启监控结果写入数据库的线程
	CWriteMonitorResult TaskWriteDB(m_pDataBase,GetSafeHwnd());
	thePoolManager.AddTask(TaskWriteDB,FALSE);

	// 开启磁盘监控线程
	CStringArray diskList;
	FileProTool::GetAllDisk(diskList);
	for (int i=0;i< diskList.GetCount();i++)
	{
		CMonitorDiskTask TaskMonitor(diskList[i]);
		thePoolManager.AddTask(TaskMonitor,FALSE);
	}
	LogMsg("开启全盘监控成功...\n");
}
DWORD WINAPI ThreadFuncInsertToDB(LPVOID pParam)
{
	CFileManagerDlg *pData  = (CFileManagerDlg*)(pParam);
	ASSERT(pData);
	while(!pData->m_bExitScanDisk)
	{
		vector<CString> vFiles;
		EnterCriticalSection(&FileProTool::g_cs);
		vFiles.assign(FileProTool::m_gFileList.begin(),FileProTool::m_gFileList.end());
		FileProTool::m_gFileList.clear();
		if (vFiles.empty())
		{
			LeaveCriticalSection(&FileProTool::g_cs);
			if(pData->m_iFinishedThreadCount == pData->m_iHandleCount)
			{
				LogMsg("所有磁盘数据插入数据库完毕!\n");
				pData->PostMessage(WM_SCAN_FINISHED,0,0);
				break;
			}

			Sleep(100);
			continue;
		}

		int iSize  = vFiles.size();
		LogMsg("正在生成插入SQL语句【%ld】条...\n",iSize);
		ConvertFileToSqlInsert(vFiles);
		LogMsg("正在批量插入记录%ld...\n",iSize);

		// 批量插入，现在内存中缓存，然后插入
		FileProTool::g_pDataBase->StartTransaction();
		int iRev = 0;
		char *cErrMsg = NULL;
		for (int i=0;i<iSize;i++)
		{
			if (pData->m_bExitScanDisk)
			{
				LeaveCriticalSection(&FileProTool::g_cs);
				pData->m_InsetDbFinished = TRUE;
				return 0;
			}

			iRev = sqlite3_exec(FileProTool::g_pDataBase->GetDB() , vFiles[i] ,0 ,0, &cErrMsg);
			if (iRev!=SQLITE_OK)
			{
				LogMsg("执行SQL【%s】失败,%s\n",vFiles[i],cErrMsg);
			}

			if (i % 5000 ==0)
			{
				FileProTool::g_pDataBase->EndTransaction();
				FileProTool::g_pDataBase->StartTransaction();
				LogMsg("已经缓存5000条记录，执行COMMIT...\n");
			}
		}
		g_AllCount += iSize;
		if (iSize%5000!=0)
			FileProTool::g_pDataBase->EndTransaction();
		
		LogMsg("批量插入完成,当前共插入数据库【%ld】条记录\n",g_AllCount);
		LeaveCriticalSection(&FileProTool::g_cs);
	}
	pData->m_InsetDbFinished = TRUE;
	return 0;
}
void CFileManagerDlg::LoadDataBase()
{
	AfxGetModuleFileName(NULL,m_strDB);
	m_strDB.Replace(".exe",".db");
	//CSqliteBase *  pMemDataBase =  CSqliteBase::OpenDataBase(":memory:"); // 创建内存数据库
	if (PathFileExists(m_strDB)) // 如果数据库文件存在
	{
		m_pDataBase = CSqliteBase::OpenDataBase(m_strDB);
		if (m_pDataBase && m_pDataBase->IsTableExist("FILE"))
		{
			//CSqliteBase::loadOrSaveDb(pMemDataBase,m_strDB,FALSE);
			//delete m_pDataBase;
			//m_pDataBase = pMemDataBase; // 程序中操作内存数据库
			//DeleteFile(m_strDB);
			SetWindowText("");
			QueryFromDataBase("*",FALSE,FALSE);
			StartDiskMonitor();
			return;
		}
		else
		{	
			if (m_pDataBase) // 原来的数据库不完整，需重新创建
			{
				delete m_pDataBase;
				m_pDataBase = NULL;
				DeleteFile(m_strDB);
			}
			
		}
	}

	m_pDataBase = CSqliteBase::OpenDataBase(m_strDB);
	m_pDataBase->ExcuteSqlCmd("create table FILE(path varchar,type integer);");
	m_pDataBase->ExcuteSqlCmd("create index file_path_index on FILE(path);"); //创建查询索引
	sqlite3_exec(m_pDataBase->GetDB(), "PRAGMA synchronous = OFF; ", 0,0,0);

	m_bExitScanDisk = FALSE;
	CStringArray diskList;
	FileProTool::GetAllDisk(diskList);

	FileProTool::g_pDataBase = m_pDataBase;
	InitializeCriticalSection(&(FileProTool::g_cs));

	m_iFinishedThreadCount = 0;
	m_iHandleCount = diskList.GetCount();
	m_pScanThreadHandleArry = new HANDLE[m_iHandleCount];

	m_StatusBar.SetText("磁盘扫描中...",0,0);
	LogMsg("开始时间：%ld\n",GetTickCount());

	HANDLE hDb = CreateThread(NULL,0,ThreadFuncInsertToDB,this,CREATE_SUSPENDED,NULL);

	CFileInfo info;
	m_InsetDbFinished = FALSE;
	for (int i=0;i<m_iHandleCount;i++)
	{
		CString & strItem = diskList[i];
		ThreadScanDiskData *pData = new ThreadScanDiskData;
		pData->pManagerDlg = this;
		pData->m_strDisk = strItem;
		pData->m_bScanFinished = FALSE;
		m_pScanThreadHandleArry[i] = CreateThread(NULL,0,CFileManagerDlg::ThreadFuncScanDisk,pData,0,NULL);
	}
	ResumeThread(hDb);
	CloseHandle(hDb);
}
DWORD WINAPI CFileManagerDlg::ThreadFuncScanDisk(LPVOID pParam)
{
	ThreadScanDiskData *pData  = (ThreadScanDiskData*)(pParam);
	CString strWildCard  = pData->m_strDisk;
	CFileManagerDlg *pThis = pData->pManagerDlg;
	pData->m_bScanFinished = FALSE;
	FileProTool::GetAllItemsUseUSN(strWildCard,pThis->m_bExitScanDisk);
	pData->m_bScanFinished = TRUE;
	pData->pManagerDlg->m_iFinishedThreadCount++;
	delete pData;
	return 0;
}

void CFileManagerDlg::OnDestroy()
{
	// TODO: 在此处添加消息处理程序代码
	m_bExitScanDisk = TRUE;
	m_StatusBar.SetText("正在等待线程安全结束...",0,0);
	thePoolManager.ExitAll();
	if (!g_MonotorFileActionList.empty()) // 可能存在写数据库的线程已经结束后，监控线程继续添加CFileAction 到g_MonotorFileActionList
	{
		for (size_t i=0;i<g_MonotorFileActionList.size();i++)
		{
			delete g_MonotorFileActionList[i]; // 删除new的CFileAction,防止内存泄漏
		}
	}
	WaitForMultipleObjects(m_iHandleCount,m_pScanThreadHandleArry,TRUE,INFINITE);
	DeleteCriticalSection(&g_FileActionListCS); // 删除监控互斥区
	ShowNotifyIcon(FALSE);
	RemoveProp(GetSafeHwnd(),AfxGetApp()->m_pszExeName); //移除窗体标志

	//CSqliteBase::loadOrSaveDb(m_pDataBase,m_strDB,true); // 退出时对内存数据库进行保存

	CBCGPDialog::OnDestroy();
}
HRESULT CFileManagerDlg::OnMsgScanResult(WPARAM wp,LPARAM lp)
{
	CString *pMsg = (CString*)(lp);
	CString msg = *(CString*)(lp);
	delete pMsg;
	m_StatusBar.SetText(msg,0,0);
	CString strNum;
	strNum.Format("当前文件数量:%ld",wp);
	m_StatusBar.SetText(strNum,2,0);
	return S_OK;
}
HRESULT  CFileManagerDlg::OnMsgScanFinished(WPARAM wp,LPARAM lp)
{
	LogMsg("结束时间：%ld\n",GetTickCount());
	SetWindowText("");
	QueryFromDataBase("*",FALSE,FALSE);
	LogMsg("所有磁盘扫描完成,共有%d个项目!",g_AllCount);
	m_StatusBar.SetText("所有磁盘扫描完成！",0,0);
	CString strNum;
	strNum.Format("当前文件数量:%ld",g_AllCount);
	m_StatusBar.SetText(strNum,2,0);
	StartDiskMonitor(); // 所有磁盘扫描完成后开启监控
	return S_OK;
}
void CFileManagerDlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsEditingSelect = FALSE;
	CString str = "";
	m_EditSelect.GetWindowText(str);
	if (str.IsEmpty())
	{
		MessageBox("请输入检索关键字","提示",MB_OK|MB_ICONINFORMATION);
		return;
	}
	m_ListCtrl.DeleteAllItems();

	BOOL bMatchEnd = FALSE;
	if (str.GetLength() >2)
	{
		if (str.Left(2)=="*.")
		{
			str = str.Mid(1);
			if (str!="*")
				bMatchEnd = TRUE;
		}
	}
	QueryFromDataBase(str,bMatchEnd,FALSE);
}

void CFileManagerDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int iRow = pNMItemActivate->iItem;
	if (iRow <0)
		return;


	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_NO_UI;
	ShExecInfo.hwnd = GetSafeHwnd();
	ShExecInfo.lpVerb = "Open";      
	ShExecInfo.lpParameters = NULL;  
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;	

	CString strPath = m_ListCtrl.GetItemTextEx(iRow,1) + "\\";
	strPath += m_ListCtrl.GetItemTextEx(iRow,0);
	ShExecInfo.lpFile = strPath;  
	ShellExecuteEx(&ShExecInfo);
}


BOOL CFileManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedButtonSelect();
			return TRUE;
		}
	}
	return CBCGPDialog::PreTranslateMessage(pMsg);
}
void CFileManagerDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
	return;

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);  

	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;  
	lvinfo.flags = LVHT_ABOVE;  

	UINT nFlag;  
	int nItem = m_ListCtrl.HitTest(point, &nFlag)-1; 
	if (nItem<0)
		return;

	BOOL bMultiSelect = FALSE;
	if (::GetKeyState(VK_CONTROL)<0 || ::GetKeyState(VK_SHIFT)<0  )
		bMultiSelect = TRUE;

	if (bMultiSelect == FALSE) //单选的时候取消所有的选中，之后再重新选择
	{
		POSITION p = m_ListCtrl.GetFirstSelectedItemPosition();
		while(p)
		{
			int iSelID  = m_ListCtrl.GetNextSelectedItem(p);
			m_ListCtrl.SetItemState(iSelID,0,LVIS_SELECTED);
		}

		m_ListCtrl.SetSelectionMark(-1);
		m_ListCtrl.SetFocus();
	}

	if( m_ListCtrl.GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED)
	{
		m_ListCtrl.SetItemState(nItem,0,LVIS_SELECTED);
		return;
	}

	m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);   //选中行
	m_ListCtrl.SetSelectionMark(nItem);
}

void CFileManagerDlg::OnUpdateOpenItem(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (m_ListCtrl.GetSelectedCount()==0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);

}


void CFileManagerDlg::OnUpdateOpenDir(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (m_ListCtrl.GetSelectedCount()==0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CFileManagerDlg::OnUpdateShowAttribute(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (m_ListCtrl.GetSelectedCount()==0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CFileManagerDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str = "";
	m_EditSelect.GetWindowText(str);
	m_bIsEditingSelect = TRUE;
}


void CFileManagerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	HideMain();
	//CBCGPDialog::OnClose();
}
void  CFileManagerDlg::HideMain()
{
	ShowWindow(SW_HIDE);
}
void CFileManagerDlg::OnUpdateStartWithWindow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bStartWithWindow);
}
void CFileManagerDlg::OnMsgNotifyStartWithWindow()
{
	m_bStartWithWindow = !m_bStartWithWindow;
	if (m_bStartWithWindow)
		WriteStartAtWindowsLogin();
	else
		RemoveStartAtWindowsLoginKey();
}
void CFileManagerDlg::OnMsgNotifyOpen()
{
	if (IsIconic())
		ShowWindow(SW_RESTORE);

	ShowWindow(SW_SHOW);
	SetForegroundWindow();
}
void CFileManagerDlg::OnExitApp()
{
	if (IDNO == MessageBox("确定要退出快速搜索工具？手动退出下次启动将重新扫描磁盘!","提示",MB_YESNO|MB_ICONQUESTION))
		return;

	CBCGPDialog::EndDialog(IDOK);
}
void  CFileManagerDlg::ShowNotifyIcon( BOOL bAdd)
{
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = GetSafeHwnd();
	m_nid.uID = IDR_MAINFRAME;
	m_nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	m_nid.uCallbackMessage = WM_MAIN_NOTIFY_MSG; // 自定义消息
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy_s(m_nid.szTip,"文件快速搜索工具");
	DWORD dwMessage;
	if (bAdd) 
		dwMessage = NIM_ADD;
	else
		dwMessage = NIM_DELETE;

	Shell_NotifyIcon(dwMessage,&m_nid);
}
HRESULT CFileManagerDlg::OnMsgNotifyIcon(WPARAM wp,LPARAM lp)
{
	if (wp!=IDR_MAINFRAME)
		return S_OK;

	switch(lp)
	{
	case WM_RBUTTONUP:
		{
			CPoint pos;
			GetCursorPos(&pos);
			CMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING,WM_START_AFTER_WINDOWS,"开机启动");
			menu.AppendMenu(MF_STRING,WM_MAIN_SHOW,"打开");
			menu.AppendMenu(MF_STRING,WM_EXIT_APP,"退出");
			menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN,pos.x,pos.y,this);
			// 回收资源
			HMENU hMenu = menu.Detach();
			menu.DestroyMenu();
		}
		break; // 请求托盘右键菜单
	case WM_LBUTTONDBLCLK:
		ShowWindow(SW_SHOW);
		break; // 双击托盘图标
	}
	return S_OK;
}
void CFileManagerDlg::GetUsrSettings()
{
	HKEY hKey;
	LSTATUS ls = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_READ,&hKey);
	if (ls!= ERROR_SUCCESS)
	{
		LogMsg("打开开机启动键值【%s】失败\n","Software\\Microsoft\\Windows\\CurrentVersion\\Run");
		return;
	}
	DWORD dwType;
	DWORD dwSize = 1000;
	char  szString[1000] = {0};
	ls = ::RegQueryValueEx (hKey,"FileManager", NULL,&dwType, (BYTE *) szString, &dwSize);
	if (ls== ERROR_SUCCESS)
		m_bStartWithWindow = TRUE;
	else if (ls== ERROR_FILE_NOT_FOUND)
		m_bStartWithWindow = FALSE;
	else
		m_bStartWithWindow = FALSE;

	LogMsg("读入开机启动配置为【%s】！\n",m_bStartWithWindow ? "开机启动":"不随Windows启动");
	RegCloseKey(hKey);
}
void CFileManagerDlg::WriteStartAtWindowsLogin()
{
	WCHAR wPath[MAX_PATH];
	GetModuleFileNameW(NULL,wPath,MAX_PATH*sizeof(WCHAR));
	CString strPath = (LPCTSTR)_bstr_t(wPath);

	HKEY hKey;
	LSTATUS ls = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_SET_VALUE,&hKey);
	if (ls!= ERROR_SUCCESS)
	{
		LogMsg("打开开机启动键值【%s】失败\n","Software\\Microsoft\\Windows\\CurrentVersion\\Run");
		return;
	}
	strPath += " -runAtBack";// 指定后台运行的命令行参数
	const char *pszPath = (LPCTSTR)strPath;
	ls = RegSetValueEx(hKey,"FileManager",0,REG_SZ,(const unsigned char*)pszPath,strlen(pszPath));
	LogMsg("写入开机启动项【%s】\n",ls==ERROR_SUCCESS ? "成功":"失败");
	RegCloseKey(hKey);
}
void CFileManagerDlg::RemoveStartAtWindowsLoginKey()
{
	HKEY hKey;
	LSTATUS ls = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_SET_VALUE,&hKey);
	if (ls!= ERROR_SUCCESS)
	{
		LogMsg("打开开机启动键值【%s】失败\n","Software\\Microsoft\\Windows\\CurrentVersion\\Run");
		return;
	}
	//HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Run
	RegDeleteValue(hKey,"FileManager");
	LogMsg("删除开机启动项成功！\n");
	RegCloseKey(hKey);
}
void CFileManagerDlg::OnCopyFile()
{
	// TODO: 在此添加命令处理程序代码
	POSITION p = m_ListCtrl.GetFirstSelectedItemPosition();
	CString strPath;
	CStringArray strFileList;
	while(p)
	{
		int iRow = m_ListCtrl.GetNextSelectedItem(p);
		strPath = m_ListCtrl.GetItemTextEx(iRow,1) + "\\";
		strPath += m_ListCtrl.GetItemTextEx(iRow,0);
		strFileList.Add(strPath);
	}

	//复制文件到剪切板,对于多个文件,没有参数指定文件个数,
	//每个文件名用 0 分隔,所有文件名结束用 两个0表示
	DROPFILES *dropFiles;
	UINT memLen;
	HGLOBAL Gmem;
	char *pMem,*szFileList;

	//记算所需内存大小。
	memLen=sizeof(DROPFILES);
	for(int i=0;i<strFileList.GetCount();i++)
		memLen+=strFileList[i].GetLength()+1;

	memLen++; //最后一个0的位置。

	Gmem= GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE, memLen);
	pMem=(char*)GlobalLock(Gmem);
	if(pMem==NULL)
	{
		GlobalFree(Gmem);
		LogMsg("GlobalLock失败,%s,%s,%s\n",__FILE__,__LINE__,__FUNCTION__);
		return;
	}

	dropFiles=(DROPFILES*)pMem;
	szFileList=pMem+sizeof(DROPFILES);

	dropFiles->pFiles =sizeof(DROPFILES);
	dropFiles->pt.x=0;
	dropFiles->pt.y=0;
	dropFiles->fNC =false;
	dropFiles->fWide =false;

	int nameLen=0;
	for(int i=0;i<strFileList.GetCount();i++)
	{
		nameLen+= sprintf(szFileList+nameLen,strFileList[i]);
		nameLen++;//字符结尾0的位置。
	}
	szFileList[nameLen]=0;//最后一个0。

	GlobalUnlock(Gmem);

	if(!OpenClipboard() )
	{
		GlobalFree(Gmem);
		LogMsg("OpenClipboard失败,%s,%s,%s\n",__FILE__,__LINE__,__FUNCTION__);
		return;
	}

	EmptyClipboard();

	if(NULL==SetClipboardData( CF_HDROP, Gmem ))
		LogMsg("SetClipboardData失败,%s,%s,%s\n",__FILE__,__LINE__,__FUNCTION__);

	CloseClipboard();
}


void CFileManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent==1)
	{
		ShowWindow(SW_HIDE); // 后台运行，则隐藏窗体
		KillTimer(1);
	}

	CBCGPDialog::OnTimer(nIDEvent);
}


int CFileManagerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetProp(GetSafeHwnd(),AfxGetApp()->m_pszExeName,(HANDLE)1); // 设置窗体标志
	return 0;
}
void CFileManagerDlg::CheckRefreshList(CString strFile,CString strNewFile)
{
	if (m_bIsEditingSelect)
		return;

	CString strKeyWords;
	m_EditSelect.GetWindowText(strKeyWords);
	if (strKeyWords.IsEmpty())
		return;

	if (m_ListCtrl.GetItemCount()>1000)
		return;

	if (strKeyWords.Left(1)=="*")
		strKeyWords.TrimLeft("*");

	if (strFile.Find(strKeyWords)>=0 || strNewFile.Find(strKeyWords) >=0)
		OnBnClickedButtonSelect();
}
HRESULT CFileManagerDlg::OnMsgUpdateList(WPARAM,LPARAM lp)
{
	CFileAction * pAction = (CFileAction*)lp;
	CString strFile = pAction->m_strFile;
	CString strFileNew = pAction->m_strNewFile;
	delete pAction;

	CheckRefreshList(strFile,strFileNew);
	return S_OK;
}

void CFileManagerDlg::OnFileCust()
{
	// TODO: 在此添加命令处理程序代码
	POSITION p = m_ListCtrl.GetFirstSelectedItemPosition();
	CString strPath;
	CStringArray strFileList;
	while(p)
	{
		int iRow = m_ListCtrl.GetNextSelectedItem(p);
		strPath = m_ListCtrl.GetItemTextEx(iRow,1) + "\\";
		strPath += m_ListCtrl.GetItemTextEx(iRow,0);
		strFileList.Add(strPath);
	}

	UINT uDropEffect=RegisterClipboardFormat("Preferred DropEffect");
	HGLOBAL hGblEffect=GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE,sizeof(DWORD));
	DWORD * dwDropEffect=(DWORD*)GlobalLock(hGblEffect);
	if(0)
		*dwDropEffect=DROPEFFECT_COPY;
	else 
		*dwDropEffect=DROPEFFECT_MOVE;
	GlobalUnlock(hGblEffect);
	

	DROPFILES dropFiles;
	UINT uDropFilesLen=sizeof(DROPFILES);

	dropFiles.pFiles =uDropFilesLen;
	dropFiles.pt.x=0;
	dropFiles.pt.y=0;
	dropFiles.fNC =FALSE;
	dropFiles.fWide =FALSE;

	//记算所需内存大小。
	UINT uGblLen=sizeof(DROPFILES);
	for(int i=0;i<strFileList.GetCount();i++)
		uGblLen+=strFileList[i].GetLength()+1;

	uGblLen++; //最后一个0的位置。

	HGLOBAL hGblFiles= GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE, uGblLen);
	char *szData=(char*)GlobalLock(hGblFiles);
	memcpy(szData,(LPVOID)(&dropFiles),uDropFilesLen);
	char * szFileList=szData+uDropFilesLen;

	int nameLen=0;
	for(int i=0;i<strFileList.GetCount();i++)
	{
		nameLen+= sprintf(szFileList+nameLen,strFileList[i]);
		nameLen++;//字符结尾0的位置。
	}
	szFileList[nameLen]=0;//最后一个0。


	GlobalUnlock(hGblFiles);

	if(OpenClipboard())
	{
		EmptyClipboard();
		SetClipboardData( CF_HDROP, hGblFiles );
		SetClipboardData(uDropEffect,hGblEffect);
		CloseClipboard();
	}
}


void CFileManagerDlg::OnUpdateFileCust(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (m_ListCtrl.GetSelectedCount()==0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CFileManagerDlg::OnFileDelete()
{
	// TODO: 在此添加命令处理程序代码
	POSITION p = m_ListCtrl.GetFirstSelectedItemPosition();
	CString strPath;
	CStringArray strFileList;
	while(p)
	{
		int iRow = m_ListCtrl.GetNextSelectedItem(p);
		strPath = m_ListCtrl.GetItemTextEx(iRow,1) + "\\";
		strPath += m_ListCtrl.GetItemTextEx(iRow,0);
		strFileList.Add(strPath);
	}

	SHFILEOPSTRUCT  shDelFile;
	memset(&shDelFile,0,sizeof(SHFILEOPSTRUCT));
	//shDelFile.fFlags |= FOF_SILENT;      // 不显示进度
	//shDelFile.fFlags |= FOF_NOERRORUI;     //不报告错误信息
	//shDelFile.fFlags |= FOF_NOCONFIRMATION;    //直接删除，不进行确认
	// 复制路径到一个以双NULL结束的string变量里

	int iLen = 1;
	for(int i=0;i<strFileList.GetCount();i++)
		iLen +=strFileList[i].GetLength()+1;

	CHAR *pbuf = new CHAR[iLen]; 
	memset(pbuf,0,sizeof(CHAR)*iLen);
	int nameLen = 0;
	for (int i=0;i<strFileList.GetCount();i++)
	{
		nameLen+= sprintf(pbuf+nameLen,strFileList[i]);
		nameLen++;//字符结尾0的位置。
	}
	
	pbuf[iLen-1]=0;   // 在末尾加两个NULL

	// 设置SHFILEOPSTRUCT的参数为删除做准备
	shDelFile.wFunc = FO_DELETE;       // 执行的操作
	shDelFile.pFrom = pbuf;         // 操作的对象，也就是目录
	shDelFile.pTo = NULL;          // 必须设置为NULL
	if (1) //根据传递的bDelete参数确定是否删除到回收站
	{    
		shDelFile.fFlags |= FOF_ALLOWUNDO;    //删除到回收站
	} 
	else 
	{           
		shDelFile.fFlags &= ~FOF_ALLOWUNDO;    //直接删除，不进入回收站
	}

	int iRev = SHFileOperation(&shDelFile);
	delete []pbuf;
	if (iRev==0)
		LogMsg("删除操作成功！\n");
	else
		LogMsg("删除操作失败！\n");
}


void CFileManagerDlg::OnUpdateFileDelete(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (m_ListCtrl.GetSelectedCount()==0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}
