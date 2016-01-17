// PageNetDisk.cpp : 实现文件
//

#include "stdafx.h"
#include "NetDiskUI.h"
#include "PageNetDisk.h"
#include "afxdialogex.h"
#include "NetDiskOperation.h"
#include "FileSelectDlg.h"
#include "ThreadPoolManager.h"
#include "UploadTask.h"
#include "DownloadTask.h"
#include "UploadDirRecusion.h"
#include "DownloadDirTask.h"
#include "OpenFileTask.h"
#include "LogTools.h"
#include "TaskSerialization.h"
// CPageNetDisk 对话框

IMPLEMENT_DYNAMIC(CPageNetDisk, CDialogEx)

CPageNetDisk::CPageNetDisk(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageNetDisk::IDD, pParent)
	, m_strNewName(_T(""))
{
	m_brushBkrect = ::CreateSolidBrush(RGB(255,255,255));
	m_brushBkrectMid = ::CreateSolidBrush(RGB(221,228,228));
	for (int i=0;i<4;i++)
	{
		m_SortUp[i] = FALSE;
	}
	m_pMenuSortType = NULL;
	m_pMenuShowFileType = NULL;
	m_pMenuPath = NULL;
	m_bQueryIsRunning = FALSE;
	m_iPosition = 0;
}

CPageNetDisk::~CPageNetDisk()
{
	if (m_pMenuSortType)
	{
		delete m_pMenuSortType;
		m_pMenuSortType = NULL;
	}
	if (m_pMenuShowFileType)
	{
		delete m_pMenuShowFileType;
		m_pMenuShowFileType = NULL;
	}
	thePoolManager.ExitAll();
	DeleteCriticalSection(&m_csUploadTaskList);
}

void CPageNetDisk::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_MAIN, m_btnMain);
	DDX_Control(pDX, IDC_BTN_Back, m_btnGoBack);
	DDX_Control(pDX, IDC_BTN_Forward, m_btnGoForward);
	DDX_Control(pDX, IDC_MENU_BTN_SHOW_TYPE, m_menuBtnShowFileType);
	DDX_Control(pDX, IDC_LIST_NETDISK, m_ListCtrl);
	DDX_Control(pDX, IDC_MENU_BTN_SORT, m_MenubtnSort);
	DDX_Control(pDX, IDC_MENU_BTN_PATH, m_MenuBtnPath);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_EditSearch);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BTN_UPLOAD, m_btnUpload);
	DDX_Control(pDX, IDC_MENU_BTN_NEW, m_MenuBtnNew);
	DDX_Control(pDX, IDC_BTN_DOWNLOAD, m_btnDownLoad);
	DDX_Control(pDX, IDC_BTN_SHARE, m_btnShare);
	DDX_Control(pDX, IDC_BTN_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_MENU_BTN_CHANGE_VIEW, m_MenuBtnChangeView);
	DDX_Control(pDX, IDC_EDIT_RENAME, m_editRename);
	DDX_Text(pDX, IDC_EDIT_RENAME, m_strNewName);
}


BEGIN_MESSAGE_MAP(CPageNetDisk, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_MAIN, &CPageNetDisk::OnBnClickedBtnMain)
	ON_BN_CLICKED(IDC_BTN_Back, &CPageNetDisk::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_Forward, &CPageNetDisk::OnBnClickedBtnForward)
	ON_BN_CLICKED(IDC_MENU_BTN_SHOW_TYPE, &CPageNetDisk::OnBnClickedMenuBtnShowType)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_NETDISK, &CPageNetDisk::OnLvnColumnclickListNetdisk)
	ON_BN_CLICKED(IDC_MENU_BTN_SORT, &CPageNetDisk::OnBnClickedMenuBtnSort)
	ON_BN_CLICKED(IDC_BTN_UPLOAD, &CPageNetDisk::OnBnClickedBtnUpload)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD, &CPageNetDisk::OnBnClickedBtnDownload)
	ON_MESSAGE(WM_DWONLOAD_FINISH, &CPageNetDisk::OnDownloadFinishMsg)
ON_WM_TIMER()
ON_NOTIFY(NM_RCLICK, IDC_LIST_NETDISK, &CPageNetDisk::OnNMRClickListNetdisk)
ON_COMMAND(IDM_LIST_DOWN_TO_DESKTOP, &CPageNetDisk::OnListDownToDesktop)
ON_COMMAND(IDM_LIST_DELETE, &CPageNetDisk::OnListDelete)
ON_COMMAND(IDC_BTN_OPEN, &CPageNetDisk::OnBtnOpen)
ON_COMMAND(IDM_LIST_PROP, &CPageNetDisk::OnListProp)
ON_COMMAND(IDM_LIST_RENAME, &CPageNetDisk::OnListRename)
ON_COMMAND(IDM_LIST_CUT, &CPageNetDisk::OnListCut)
ON_COMMAND(IDM_LIST_COPY, &CPageNetDisk::OnListCopy)
ON_UPDATE_COMMAND_UI(IDM_LIST_DELETE, &CPageNetDisk::OnUpdateRightPopMenu)
ON_UPDATE_COMMAND_UI(IDM_LIST_RENAME, &CPageNetDisk::OnUpdateRightPopMenu)
ON_UPDATE_COMMAND_UI(IDM_LIST_DOWN_TO_DESKTOP, &CPageNetDisk::OnUpdateRightPopMenu)
ON_UPDATE_COMMAND_UI(IDM_LIST_CUT, &CPageNetDisk::OnUpdateRightPopMenu)
ON_UPDATE_COMMAND_UI(IDM_LIST_COPY, &CPageNetDisk::OnUpdateRightPopMenu)
ON_UPDATE_COMMAND_UI(IDM_LIST_PROP, &CPageNetDisk::OnUpdateRightPopMenu)
ON_WM_INITMENUPOPUP()
ON_NOTIFY(NM_DBLCLK, IDC_LIST_NETDISK, &CPageNetDisk::OnNMDblclkListNetdisk)
ON_EN_KILLFOCUS(IDC_EDIT_RENAME, &CPageNetDisk::OnEnKillfocusEditRename)
END_MESSAGE_MAP()


// CPageNetDisk 消息处理程序


void CPageNetDisk::OnBnClickedBtnMain()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateFileList("/");
	AddDirChangeHistory("/");
}


void CPageNetDisk::OnBnClickedBtnBack()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_vListNetDiskDirHistory.empty()) return;
	m_iPosition = m_iPosition - 1;
	if (m_iPosition <0)
		m_iPosition = 0;

	CString strNetDiskDir = m_vListNetDiskDirHistory[m_iPosition];
	m_btnGoForward.EnableWindow(TRUE);
	UpdateFileList(strNetDiskDir);

	if (m_iPosition ==0)
		m_btnGoBack.EnableWindow(FALSE);
}

void CPageNetDisk::OnBnClickedBtnForward()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_vListNetDiskDirHistory.empty()) return;
	m_iPosition = m_iPosition + 1;
	if (m_iPosition== m_vListNetDiskDirHistory.size()) 
	{
		m_iPosition = m_vListNetDiskDirHistory.size() - 1;
		m_btnGoForward.EnableWindow(FALSE);
	}

	CString strNetDiskDir = m_vListNetDiskDirHistory[m_iPosition];
	UpdateFileList(strNetDiskDir);
	
	if (m_iPosition == m_vListNetDiskDirHistory.size()-1)
		m_btnGoForward.EnableWindow(FALSE);

	m_btnGoBack.EnableWindow(TRUE);
}


BOOL CPageNetDisk::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	theApp.m_pPageNetDisk = this;
	// TODO:  在此添加额外的初始化
	InitializeCriticalSection(&m_csUploadTaskList); 
	SetBtnImage();

	//m_ListCtrl.Create(LVS_REPORT | LVS_ALIGNLEFT |WS_BORDER | WS_TABSTOP|WS_VISIBLE,CRect(0,0,1,1),this,IDC_LIST_NETDISK);
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES); // 添加网格属性

	SHFILEINFO sfi;
	// 获得系统图像列表
	HIMAGELIST sys_small_icon = (HIMAGELIST)SHGetFileInfo(NULL,0,&sfi,sizeof(sfi),SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	m_IconList.Attach(sys_small_icon);
	m_ListCtrl.SetImageList(&m_IconList,LVSIL_SMALL); // 设置列表视图图像列表
	m_IconList.Detach();
	m_ListCtrl.InsertColumn(0,"名称",LVCFMT_LEFT,350);
	m_ListCtrl.InsertColumn(1,"修改时间",LVCFMT_LEFT,120);
	m_ListCtrl.InsertColumn(2,"类型",LVCFMT_LEFT,168);
	m_ListCtrl.InsertColumn(3,"大小",LVCFMT_LEFT,120);
	m_ListCtrl.InsertColumn(4,"",LVCFMT_LEFT,200);

	m_pMenuSortType = new CMenu;
	m_pMenuSortType->LoadMenu(IDR_MENU_SORT);
	m_MenubtnSort.m_hMenu = m_pMenuSortType->GetSubMenu(0)->GetSafeHmenu();
	m_MenubtnSort.SizeToContent();
	m_MenubtnSort.m_bOSMenu = FALSE;

	m_pMenuShowFileType = new CMenu;
	m_pMenuShowFileType->LoadMenu(IDR_MENU_SHOW_FILE_TYPE);

	CString strExePath;
	GetModuleFileName(NULL,strExePath.GetBuffer(MAX_PATH),MAX_PATH);
	strExePath.ReleaseBuffer();
	strExePath = strExePath.Left(strExePath.ReverseFind('\\'));

	CString strImgDir = strExePath + "\\Images\\menu\\";
	SetMenuImage(m_pMenuShowFileType,m_MenuBitMapAllType,strImgDir + "type_all.ico",IDM_SHOW_ALL_TYPE);
	SetMenuImage(m_pMenuShowFileType,m_MenuBitMapDocumentType,strImgDir + "type_document.ico",IDM_SHOW_TYPE_DOCUMENT);
	SetMenuImage(m_pMenuShowFileType,m_MenuBitMapPictureType,strImgDir + "type_picture.ico",IDM_SHOW_TYPE_PICTURE);
	SetMenuImage(m_pMenuShowFileType,m_MenuBitMapMusicType,strImgDir + "type_music.ico",IDM_SHOW_TYPE_MUSIC);
	SetMenuImage(m_pMenuShowFileType,m_MenuBitMapVideoType,strImgDir + "type_video.ico",IDM_SHOW_TYPE_VIDEO);
	SetMenuImage(m_pMenuShowFileType,m_MenuBitMapRecycleBinType,strImgDir + "type_recyclebin.ico",IDM_SHOW_TYPE_RECYCLEBIN);
	
	SetMenuBtnShowTypeIcon("type_all.ico");

	m_menuBtnShowFileType.m_hMenu = m_pMenuShowFileType->GetSubMenu(0)->GetSafeHmenu();
	m_menuBtnShowFileType.SizeToContent();
	m_menuBtnShowFileType.m_bOSMenu = FALSE;

	AdjustNavagiateBarLayout();
	AdjustUploadBarLayout();
	
	SerializationManager.InitConfigXml();
	AddDirChangeHistory("/");
	m_btnGoForward.EnableWindow(FALSE);
	m_btnGoBack.EnableWindow(FALSE);

	SetTimer(WM_TMIMER_LOAD_HIST,500,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CPageNetDisk::SetMenuBtnShowTypeIcon(const CString & strImage)
{
	CString strExePath;
	GetModuleFileName(NULL,strExePath.GetBuffer(MAX_PATH),MAX_PATH);
	strExePath.ReleaseBuffer();
	strExePath = strExePath.Left(strExePath.ReverseFind('\\'));

	CString strImgDir = strExePath + "\\Images\\menu\\";
	CImage image;
	HRESULT hr = image.Load(strImgDir + strImage);
	HBITMAP hbitmap = image.Detach();
	m_menuBtnShowFileType.SetBitmap(hbitmap);

}
void CPageNetDisk::SetMenuImage(CMenu *pMenu,CBitmap & bmp,const CString & strImage,UINT iCMd)
{
	CImage image;
	HRESULT hr = image.Load(strImage);
	HBITMAP hbitmap = image.Detach();
	bmp.Attach(hbitmap);
	pMenu->GetSubMenu(0)->SetMenuItemBitmaps(iCMd,MF_BYCOMMAND,&bmp,NULL);
}
void CPageNetDisk::SetBtnImage()
{
	char szFileDir[MAX_PATH] = {0};
	GetModuleFileName(NULL,szFileDir,MAX_PATH);
	CString strDir = szFileDir;
	strDir = strDir.Left(strDir.ReverseFind('\\')+1);

	// 主页按钮
	CString strImgNormal = strDir + "Images\\Main.bmp";
	CString strImgHover = strDir + "Images\\Main2.bmp";
	CString strImgDown = strDir + "Images\\Main.bmp";
	m_btnMain.SetImage(strImgNormal,strImgHover,strImgHover,strImgDown);
	m_btnMain.SetWindowText("");

	//后退按钮
	strImgNormal = strDir + "Images\\back.bmp";
	strImgHover = strDir + "Images\\back2.bmp";
	strImgDown = strDir + "Images\\back.bmp";
	CString strImgDisabled = strDir + "Images\\back_disable.bmp";
	m_btnGoBack.SetImage(strImgNormal,strImgHover,strImgHover,strImgDisabled);
	m_btnGoBack.SetWindowText("");

	// 前进按钮
	strImgNormal = strDir + "Images\\forward.bmp";
	strImgHover = strDir + "Images\\forward2.bmp";
	strImgDown = strDir + "Images\\forward.bmp";
	strImgDisabled = strDir + "Images\\forward_disable.bmp";
	m_btnGoForward.SetImage(strImgNormal,strImgHover,strImgHover,strImgDisabled);
	m_btnGoForward.SetWindowText("");

	// 刷新按钮
	strImgNormal = strDir + "Images\\btn_refresh.bmp";
	m_btnRefresh.SetImage(strImgNormal,strImgNormal,strImgNormal,strImgNormal);
	m_btnRefresh.SetWindowText("");

	// 上传按钮
	strImgNormal = strDir + "Images\\upload_normal.bmp";
	strImgHover = strDir + "Images\\upload_hover.bmp";
	strImgDown = strDir + "Images\\upload_normal.bmp";
	m_btnUpload.SetImage(strImgNormal,strImgHover,strImgHover,strImgDown);
	m_btnUpload.SetWindowText("");

	// 下载按钮
	strImgNormal = strDir + "Images\\download_normal.bmp";
	strImgHover = strDir + "Images\\download_hover.bmp";
	strImgDown = strDir + "Images\\download_normal.bmp";
	m_btnDownLoad.SetImage(strImgNormal,strImgHover,strImgHover,strImgDown);
	m_btnDownLoad.SetWindowText("");

	// 新建按钮
	strImgNormal = strDir + "Images\\btn_new_normal.bmp";
	strImgHover = strDir + "Images\\btn_new_hover.bmp";
	strImgDown = strDir + "Images\\btn_new_normal.bmp";
	m_MenuBtnNew.SetImage(strImgNormal,strImgHover,strImgHover,strImgDown);
	m_MenuBtnNew.SetWindowText("");

	// 分享按钮
	strImgNormal = strDir + "Images\\btn_share_normal.bmp";
	strImgHover = strDir + "Images\\btn_share_hover.bmp";
	strImgDown = strDir + "Images\\btn_share_normal.bmp";
	m_btnShare.SetImage(strImgNormal,strImgHover,strImgHover,strImgDown);
	m_btnShare.SetWindowText("");

	// 打开按钮
	strImgNormal = strDir + "Images\\btn_open_normal.bmp";
	strImgHover = strDir + "Images\\btn_open_hover.bmp";
	strImgDown = strDir + "Images\\btn_open_normal.bmp";
	m_btnOpen.SetImage(strImgNormal,strImgHover,strImgHover,strImgDown);
	m_btnOpen.SetWindowText("");
}

void CPageNetDisk::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	CRect rcClient;
	GetClientRect(&rcClient);

	CBitmap bitmapMain;
	bitmapMain.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	MemDC.SelectObject(&bitmapMain);


	HGDIOBJ oldBrush=MemDC.SelectObject(m_brushBkrect);
	MemDC.Rectangle(rcClient);
	MemDC.SelectObject(oldBrush);

	CRect rcMid = rcClient;
	rcMid.top = rcClient.top  + TOOLBAR_HEIGHT_NAVAGITE +  TOOLBAR_HEIGHT_NAVAGITE;

	oldBrush=MemDC.SelectObject(m_brushBkrectMid);
	MemDC.Rectangle(rcMid);
	MemDC.SelectObject(oldBrush);

	MemDC.MoveTo(rcClient.left,TOOLBAR_HEIGHT_NAVAGITE);
	MemDC.LineTo(rcClient.right,TOOLBAR_HEIGHT_NAVAGITE); //绘制分割线

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.DeleteDC();
	bitmapMain.DeleteObject();
}
void CPageNetDisk::AdjustNavagiateBarLayout()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rectBtn = rcClient;
	int iLeft = rcClient.left + BORDER_WIDTH;
	if (m_btnMain.m_hWnd)
	{
		rectBtn.left = iLeft;
		rectBtn.right = rectBtn.left + 44;
		rectBtn.top  = rcClient.top + BORDER_WIDTH;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE;
		m_btnMain.MoveWindow(rectBtn,FALSE);

		iLeft += rectBtn.Width();
	}

	if (m_btnGoBack.m_hWnd)
	{
		rectBtn = rcClient;
		iLeft += BORDER_WIDTH;
		rectBtn.left =iLeft;
		rectBtn.right = rectBtn.left + 50;
		rectBtn.top  = rcClient.top + BORDER_WIDTH;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE;
		m_btnGoBack.MoveWindow(rectBtn,FALSE);
		iLeft += rectBtn.Width();
	}
	
	if (m_btnGoForward.m_hWnd)
	{
		rectBtn = rcClient;
		iLeft += BORDER_WIDTH;
		rectBtn.left =iLeft;
		rectBtn.right = rectBtn.left + 47;
		rectBtn.top  = rcClient.top + BORDER_WIDTH;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE;
		m_btnGoForward.MoveWindow(rectBtn,FALSE);
		iLeft += rectBtn.Width();
	}
	if (m_menuBtnShowFileType.m_hWnd)
	{
		rectBtn = rcClient;
		iLeft += BORDER_WIDTH;
		rectBtn.left = iLeft;
		rectBtn.right = rectBtn.left + 47;
		rectBtn.top  = rcClient.top + BORDER_WIDTH + 3;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - 9;
		m_menuBtnShowFileType.MoveWindow(rectBtn,FALSE);
		iLeft += rectBtn.Width();
	}

	int iRight = rcClient.right - BORDER_WIDTH;
	if (m_btnSearch.m_hWnd)
	{
		rectBtn = rcClient;
		rectBtn.right = iRight;
		rectBtn.left = rectBtn.right - 45;
		rectBtn.top  = rcClient.top + BORDER_WIDTH + 3;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - 9;
		m_btnSearch.MoveWindow(rectBtn,FALSE);
		iRight -= rectBtn.Width();
	}
	if (m_EditSearch.m_hWnd)
	{
		rectBtn = rcClient;
		rectBtn.right = iRight;
		rectBtn.left = rectBtn.right - 150;
		rectBtn.top  = rcClient.top + BORDER_WIDTH + 3;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - 9;
		m_EditSearch.MoveWindow(rectBtn,FALSE);
		iRight -= rectBtn.Width();
	}

	if (m_btnRefresh.m_hWnd)
	{
		rectBtn = rcClient;
		rectBtn.right = iRight - BORDER_WIDTH;
		rectBtn.left = rectBtn.right - 45;
		rectBtn.top  = rcClient.top + BORDER_WIDTH + 3;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - 9;
		m_btnRefresh.MoveWindow(rectBtn,FALSE);
		iRight -= rectBtn.Width();
	}
	if (m_MenuBtnPath.m_hWnd)
	{
		rectBtn = rcClient;
		rectBtn.left  = iLeft;
		rectBtn.right = iRight;
		rectBtn.top  = rcClient.top + BORDER_WIDTH + 3;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - 9;
		m_MenuBtnPath.MoveWindow(rectBtn,FALSE);
		iLeft += rectBtn.Width();
	}

	if (m_ListCtrl.m_hWnd)
	{
		CRect rcMid = rcClient;
		rcMid.top = rcClient.top  + TOOLBAR_HEIGHT_NAVAGITE*2;
		m_ListCtrl.MoveWindow(rcMid,TRUE);
	}
}

void  CPageNetDisk::AdjustUploadBarLayout()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rectBtn = rcClient;
	int iLeft = rcClient.left + BORDER_WIDTH*2;
	if (m_btnUpload.m_hWnd)
	{
		rectBtn.left = iLeft;
		rectBtn.right = rectBtn.left + 85;
		rectBtn.top  = rcClient.top + TOOLBAR_HEIGHT_NAVAGITE + BORDER_WIDTH;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - BORDER_WIDTH;
		m_btnUpload.MoveWindow(rectBtn,FALSE);
		iLeft += rectBtn.Width();
	}

	if (m_MenuBtnNew.m_hWnd)
	{
		rectBtn = rcClient;
		iLeft += BORDER_WIDTH;
		rectBtn.left =iLeft;
		rectBtn.right = rectBtn.left + 84;
		rectBtn.top  = rcClient.top + TOOLBAR_HEIGHT_NAVAGITE + BORDER_WIDTH;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - BORDER_WIDTH;
		m_MenuBtnNew.MoveWindow(rectBtn,FALSE);
		iLeft += rectBtn.Width();
	}

	if (m_btnDownLoad.m_hWnd)
	{
		rectBtn = rcClient;
		iLeft += BORDER_WIDTH;
		rectBtn.left =iLeft;
		rectBtn.right = rectBtn.left + 62;
		rectBtn.top  = rcClient.top  + TOOLBAR_HEIGHT_NAVAGITE + BORDER_WIDTH;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - BORDER_WIDTH;
		m_btnDownLoad.MoveWindow(rectBtn,FALSE);
		iLeft += rectBtn.Width();
	}

	if (m_btnShare.m_hWnd)
	{
		rectBtn = rcClient;
		iLeft += BORDER_WIDTH;
		rectBtn.left =iLeft;
		rectBtn.right = rectBtn.left + 68;
		rectBtn.top  = rcClient.top  + TOOLBAR_HEIGHT_NAVAGITE + BORDER_WIDTH;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - BORDER_WIDTH;
		m_btnShare.MoveWindow(rectBtn,FALSE);
		iLeft += rectBtn.Width();
	}

	if (m_btnOpen.m_hWnd)
	{
		rectBtn = rcClient;
		iLeft += BORDER_WIDTH;
		rectBtn.left =iLeft;
		rectBtn.right = rectBtn.left + 70;
		rectBtn.top  = rcClient.top  + TOOLBAR_HEIGHT_NAVAGITE + BORDER_WIDTH;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - BORDER_WIDTH;
		m_btnOpen.MoveWindow(rectBtn,FALSE);
		iLeft += rectBtn.Width();
	}

	int iRight = rcClient.right - BORDER_WIDTH*2;
	if (m_MenuBtnChangeView.m_hWnd)
	{
		rectBtn = rcClient;
		rectBtn.right = iRight;
		rectBtn.left = rectBtn.right - 80;
		rectBtn.top  = rcClient.top + TOOLBAR_HEIGHT_NAVAGITE + BORDER_WIDTH*2 + 1;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - 9;
		m_MenuBtnChangeView.MoveWindow(rectBtn,FALSE);
		iRight -= rectBtn.Width();
	}

	if (m_MenubtnSort.m_hWnd)
	{
		rectBtn = rcClient;
		rectBtn.right = iRight;
		rectBtn.left = rectBtn.right - 60;
		rectBtn.top  = rcClient.top + TOOLBAR_HEIGHT_NAVAGITE + BORDER_WIDTH*2 +1 ;
		rectBtn.bottom = rectBtn.top + TOOLBAR_HEIGHT_NAVAGITE - 9;
		m_MenubtnSort.MoveWindow(rectBtn,FALSE);
		iRight -= rectBtn.Width();
	}
	
}

void CPageNetDisk::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustNavagiateBarLayout();
	AdjustUploadBarLayout();
}


void CPageNetDisk::OnLvnColumnclickListNetdisk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	BeginWaitCursor();
	/*SortItems(pNMLV->iSubItem);*/
	CHeaderCtrl* pHdrCtrl = m_ListCtrl.GetHeaderCtrl(); 
	int n = pHdrCtrl->GetItemCount();

	HDITEM hdi;  
	for( int i = 0; i < n; i++ )  
	{  
		hdi.mask = HDI_FORMAT;  // 读取排序箭头图标状态，必需指定  
		pHdrCtrl->GetItem(i, &hdi);  
		//Set sort image to this column
		if(i == pNMLV->iSubItem)
		{
			//Windows xp has a easier way to show the sort order
			//in the header: i just have to set a flag and windows
			//will do the drawing. No windows xp, no easy way.
			hdi.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
			hdi.fmt |= m_SortUp[i] ? HDF_SORTUP : HDF_SORTDOWN;
		}
		//Remove sort image (if exists)
		//from other columns.
		else
		{
			hdi.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
		}
		pHdrCtrl->SetItem( i, &hdi );  
	} 
	EndWaitCursor();
	m_ListCtrl.Invalidate(FALSE);
	m_SortUp[pNMLV->iSubItem] = ! m_SortUp[pNMLV->iSubItem];
}

void CPageNetDisk::OnBnClickedMenuBtnSort()
{
	// TODO: 在此添加控件通知处理程序代码
	switch (m_MenubtnSort.m_nMenuResult)
	{
	case  ID_SORT_BY_NAME:
	case  ID_SORT_BY_TIME:
	case  ID_SORT_BY_TYPE:
	case  ID_SORT_BY_SIZE:
		AfxMessageBox("1111");
		break;
	}
}
void CPageNetDisk::OnBnClickedMenuBtnShowType()
{
	switch (m_menuBtnShowFileType.m_nMenuResult)
	{
	case  IDM_SHOW_ALL_TYPE:
		//m_menuBtnShowFileType.SetWindowText("所有文件");
		SetMenuBtnShowTypeIcon("type_all.ico");
		break;
	case  IDM_SHOW_TYPE_DOCUMENT:
		//m_menuBtnShowFileType.SetWindowText("文档");
		SetMenuBtnShowTypeIcon("type_document.ico");
		break;
	case  IDM_SHOW_TYPE_PICTURE:
		//m_menuBtnShowFileType.SetWindowText("图片");
		SetMenuBtnShowTypeIcon("type_picture.ico");
		break;
	case  IDM_SHOW_TYPE_MUSIC:
		//m_menuBtnShowFileType.SetWindowText("音乐");
		SetMenuBtnShowTypeIcon("type_music.ico");
		break;
	case  IDM_SHOW_TYPE_VIDEO:
		//m_menuBtnShowFileType.SetWindowText("视频");
		SetMenuBtnShowTypeIcon("type_video.ico");
		break;
	case  IDM_SHOW_TYPE_RECYCLEBIN:
		//m_menuBtnShowFileType.SetWindowText("回收站");
		SetMenuBtnShowTypeIcon("type_recyclebin.ico");
		break;
	}
}

void CPageNetDisk::OnBnClickedBtnUpload()
{
	// TODO: 在此添加控件通知处理程序代码

	DWORD flags = OFN_ALLOWMULTISELECT|OFN_PATHMUSTEXIST;;
	CFileSelectDlg dlg(TRUE,NULL,NULL,flags,NULL,NULL,0UL,FALSE);
	dlg.m_ofn.lpstrTitle   =   "选择要添加到云盘的文件";                                    //至少允许选择100个文件
	dlg.m_ofn.lpstrFile = new TCHAR[MAX_PATH * SELECT_MAX_FILE_NUMBER]; //重新定义缓冲区大小          
	memset(dlg.m_ofn.lpstrFile, 0, MAX_PATH * SELECT_MAX_FILE_NUMBER);  //初始化定义的缓冲区
	dlg.m_ofn.nMaxFile = MAX_PATH*SELECT_MAX_FILE_NUMBER; // 指定缓冲区大小

	char szDir[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(),szDir,MAX_PATH);
	CString strConfigINI = szDir;
	strConfigINI.MakeLower();
	strConfigINI = strConfigINI.Left(strConfigINI.ReverseFind('.') + 1) + "ini";
	CHAR szDefaultpath[MAX_PATH];
	SHGetSpecialFolderPath(NULL,szDefaultpath,CSIDL_DESKTOPDIRECTORY,FALSE); // 找不到则默认为桌面路径
	CString strLastDir = "";
	GetPrivateProfileString("setting","LastSelectFolder",szDefaultpath,strLastDir.GetBuffer(512),512,strConfigINI);
	if (!PathFileExists(strLastDir))
		strLastDir = szDefaultpath; // 上次的路径不存在时，使用桌面路径
	dlg.m_ofn.lpstrInitialDir = strLastDir;
	strLastDir.ReleaseBuffer();

	if (dlg.DoModal() != IDOK)
	{
		delete[] dlg.m_ofn.lpstrFile;     //最后别忘了释放内存
		return;
	}

	strLastDir = CFileSelectDlg::m_strCurrendDirectory;
	WritePrivateProfileString("setting","LastSelectFolder",strLastDir,strConfigINI);
	int iSize = CFileSelectDlg::m_SelectedItemList.GetCount();
	vector<CString> vstrSel;
	for (int i=0;i<iSize;i++)
	{
		vstrSel.push_back(CFileSelectDlg::m_SelectedItemList[i]);
	}

	delete[] dlg.m_ofn.lpstrFile;     //最后别忘了释放内存

	map<CString,CString> vMapDirs;

	for (size_t i=0;i<vstrSel.size();i++)
	{
		string strFile = vstrSel[i];
		strFile = RepalceAll(strFile.c_str(),"\\\\","\\");
		CFileInfo info;
		if (!GetFileInfo(strFile.c_str(),info,TRUE))
		{
			CString strMsg;
			strMsg.Format("获取文件【%s】基本信息失败！",strFile.c_str());
			MessageBox(strMsg,"提示",MB_ICONERROR|MB_OK);
			continue;
		}

		if (HasSameNameItem(info.m_strName))
		{
			CString strMsg;
			strMsg.Format("已经存在同名文件【%s】,该文件将跳过！",info.m_strName);
			MessageBox(strMsg,"提示",MB_ICONINFORMATION|MB_OK);
			continue;
		}
		if (info.m_strType == "文件夹")
		{
			SOCKET sockClent = GetConnect(theApp.m_strServerIp);
			if (sockClent == 0)
			{
				MessageBox("连接服务器失败!","提示",MB_ICONERROR|MB_OK);
				continue;
			}
			string strErrMsg;
			if (!CreateNetDiskDir(sockClent,theApp.m_strUsrId,info.m_strName,m_ListCtrl.GetNetDiskDir(),strErrMsg))
			{
				MessageBox("上传目录失败!","提示",MB_ICONERROR|MB_OK);
				continue;
			}
			m_ListCtrl.InsertItemEx(info); //创建文件夹,不查询进度
			vMapDirs[strFile.c_str()] = m_ListCtrl.GetNetDiskDir() + info.m_strName + "/";
			continue;
		}

		info.m_strLocalPath   = strFile.c_str();
		info.m_strNetDiskPath = m_ListCtrl.GetNetDiskDir() + info.m_strName;
		CUploadTask uploadTask(m_ListCtrl.GetNetDiskDir(),info,strFile.c_str());
		if (!thePoolManager.AddTask(uploadTask))
		{
			CString strMsg;
			strMsg.Format("文件\"%s\"前一操作尚未完成，请稍后重试！",info.m_strName);
			MessageBox(strMsg,"提示",MB_OK|MB_ICONINFORMATION);
			continue;
		}
		SerializationManager.AddUploadTaskHistory(theApp.m_strUsrId,info);
		AddRunningUploadTaskIdList(uploadTask.GetTaskId());
		m_ListCtrl.InsertItemEx(info); //上传完成后FileId被赋值
	}

	map<CString,CString>::iterator iter = vMapDirs.begin();
	for (;iter!= vMapDirs.end();iter++) // 递归上传所选目录内的文件及文件夹
		UploadDirRecusion(iter->first,iter->second);

	m_ListCtrl.Invalidate(FALSE);
	StartQueryTaskStatusTimer();
}
void CPageNetDisk::UploadDirRecusion(const CString & strDir,const CString & strNetDiskDir)
{
	CUploadDirRecusion taskUpDir(strNetDiskDir,strDir);
	if (!thePoolManager.AddTask(taskUpDir))
	{
		CString strMsg;
		strMsg.Format("文件夹\"%s\"前一操作尚未完成，请稍后重试！",strNetDiskDir);
		MessageBox(strMsg,"提示",MB_OK|MB_ICONINFORMATION);
	}
	m_ListCtrl.SetProgressText("正在传输",strNetDiskDir);
	AddRunningUploadTaskIdList(taskUpDir.GetTaskId());
}
void CPageNetDisk::OnBnClickedBtnDownload()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT iCount = m_ListCtrl.GetSelectedCount();
	if (iCount<1)
	{
		MessageBox("请先选择要下载的文件或文件夹!","提示",MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strDir = GetSelectedFolder(this->GetSafeHwnd());
	if (strDir.IsEmpty())
		return;

	DownLoadToDir(strDir);
}
void CPageNetDisk::UpdateFileList(const CString & strNetDiskDir)
{
	SOCKET sockClent = GetConnect(theApp.m_strServerIp);
	if (sockClent == 0)
	{
		MessageBox("连接服务器失败!","提示",MB_ICONERROR|MB_OK);
		return;
	}

	vector<FileListItem*> vList;
	bool bOK = GetNetDiskFileList(sockClent,theApp.m_strUsrId,strNetDiskDir,false,vList);
	if (!bOK)
	{
		MessageBox("获取文件列表失败!","提示",MB_ICONERROR|MB_OK);
		return;
	}

	StopQueryTaskStatusTimer();
	m_ListCtrl.m_FileInfoArry.clear();
	LoadLocalRunningTask(strNetDiskDir);

	vector<CFileInfo> vFileInfo;
	for (size_t i=0; i<vList.size();i++)
	{
		CFileInfo info;
		info.m_strNetDiskPath = strNetDiskDir + vList[i]->szItemName;
		if (_stricmp(vList[i]->szItemType,"File")==0)
		{
			info.m_iImageID = info.m_iImageID;
			info.m_strName = vList[i]->szItemName;
			info.m_strModifyTime = TransferFileTime(vList[i]->iLastModifyTime);
			info.m_strFileSize = TransferFileSize(vList[i]->iFileLength);
			info.m_strFileId = vList[i]->szFileId;
			info.m_strMd5 = vList[i]->szMd5;
			GetFileInfo(vList[i]->szItemName,info,FALSE);
		}
		else
		{
			info.m_strNetDiskPath +="/";
			info.m_strType = vList[i]->szItemType;
			GetFileInfo(vList[i]->szItemName,info,FALSE);
			info.m_strName = vList[i]->szItemName;
			info.m_strType ="文件夹";
			CString strSubDirNetDisk = strNetDiskDir + info.m_strName + "/";
			if (thePoolManager.HasFrontMatchTask(strSubDirNetDisk))
			{
				info.m_strProgressText = "正在传输";
				StartQueryTaskStatusTimer();
			}

		}
		vFileInfo.push_back(info);
		delete vList[i];
	}
	vList.clear();
	m_ListCtrl.InsertAll(vFileInfo);
	m_ListCtrl.SetNetDiskDir(strNetDiskDir);

	CString strPath = strNetDiskDir;
	strPath = CString("所有文件/") + strNetDiskDir.Mid(1);
	m_MenuBtnPath.SetWindowText(strPath);

	m_ListCtrl.Invalidate(FALSE);
}
void CPageNetDisk::LoadLocalRunningTask(const CString & strNetDiskDir)
{
	vector<CTaskBase *> vRuningTasks;
	thePoolManager.GetFrontMatchTask(strNetDiskDir,vRuningTasks);
	bool bNeedQueryProgress = false;
	for (size_t i=0;i<vRuningTasks.size();i++)
	{
		if (vRuningTasks[i]->m_Status != CTaskBase::task_finish) // 只加载未完成的任务
		{
			if (vRuningTasks[i]->m_strOperation == "upload")
			{
				CUploadTask *pUploadTask = dynamic_cast<CUploadTask*>(vRuningTasks[i]);
				if (pUploadTask->m_strNetDiskDir.Compare(strNetDiskDir)==0)
				{
					CFileInfo & info = pUploadTask->m_FileInfo;
					m_ListCtrl.InsertItemEx(info);
					AddRunningUploadTaskIdList(pUploadTask->GetTaskId());
					bNeedQueryProgress = true;
				}
			}
			if (vRuningTasks[i]->m_strOperation == "download" && !bNeedQueryProgress)
			{
				CDownloadTask *pDownloadTask = dynamic_cast<CDownloadTask*>(vRuningTasks[i]);
				if (pDownloadTask->m_strNetDiskDir.Compare(strNetDiskDir)==0)
					bNeedQueryProgress = true;
			}
		}
	}
	Invalidate(FALSE);
	if (bNeedQueryProgress)
		StartQueryTaskStatusTimer();
}
BOOL CPageNetDisk::HasSameNameItem(const CString & strItemName)
{
	for (size_t i=0;i<m_ListCtrl.m_FileInfoArry.size();i++)
	{
		if (m_ListCtrl.m_FileInfoArry[i].m_strName.CompareNoCase(strItemName)==0)
			return TRUE;
	}
	return FALSE;
}

void CPageNetDisk::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == WM_TMIMER_QUER_STATUS)
	{
		ProcessUploadProgress();
		ProcessDownloadProgress();

		EnterCriticalSection(&m_csUploadTaskList);
		if (RunningUploadTaskIdList.empty() &&RunningDownloadTaskIdList.empty())
			StopQueryTaskStatusTimer();
		LeaveCriticalSection(&m_csUploadTaskList);
		
		m_ListCtrl.Invalidate(FALSE);
	}
	if (nIDEvent == WM_TMIMER_LOAD_HIST)
	{
		LoadUnFinishedTask();
	}

	CDialogEx::OnTimer(nIDEvent);
}
void CPageNetDisk::StartQueryTaskStatusTimer()
{
	if (m_bQueryIsRunning)
		return;

	SetTimer(WM_TMIMER_QUER_STATUS,1000,NULL);
	m_bQueryIsRunning = TRUE;
}
void CPageNetDisk::StopQueryTaskStatusTimer()
{
	if (!m_bQueryIsRunning)
		return;

	m_bQueryIsRunning = FALSE;
	KillTimer(WM_TMIMER_QUER_STATUS);
	//thePoolManager.ClearAllFinishedTask();
	RefreshUserInfo();
}
void CPageNetDisk::ProcessUploadProgress()
{
	ProcessProgress("upload");
}
void CPageNetDisk::ProcessDownloadProgress()
{
	ProcessProgress("download");
	if (m_DownloadDestDirs.empty())
		return;

	vector<CString>::iterator iterDir = m_DownloadDestDirs.begin();
	for ( ;iterDir!=m_DownloadDestDirs.end();)
	{
		BOOL bFinished = TRUE;
		EnterCriticalSection(&m_csUploadTaskList);
		for (size_t i=0;i<RunningDownloadTaskIdList.size();i++)
		{
			CTaskBase *pTask = thePoolManager.GetTaskFromHistory(RunningDownloadTaskIdList[i]);
			CDownloadTask *pDownload = dynamic_cast<CDownloadTask*>(pTask);
			if(pDownload==NULL)
			{
				bFinished = TRUE;
				break;
			}
			if (pDownload->m_strDestDir.Find(m_DownloadDestDirs[i])==0)
			{
				bFinished = FALSE;
				break;
			}
		}
		LeaveCriticalSection(&m_csUploadTaskList);
		if (bFinished)
		{
			int iBytes = iterDir->GetLength();
			char * pNewDir = new char[iBytes+1];
			memset(pNewDir,0,iBytes+1);
			strcpy_s(pNewDir,iBytes+1,*iterDir);
			this->PostMessage(WM_DWONLOAD_FINISH,0,(LPARAM)pNewDir);
			iterDir = m_DownloadDestDirs.erase(iterDir);
			continue;
		}

		iterDir++;
	}
}
void CPageNetDisk::ProcessProgress(CString strOperationType)
{
	CString strOperation ="";
	vector<CString> * pvTaskList = NULL; 
	if (strOperationType == "upload")
	{
		 strOperation = "上传";
		 pvTaskList = &RunningUploadTaskIdList;
	}
	else if (strOperationType == "download")
	{
		 strOperation = "下载";
		pvTaskList = &RunningDownloadTaskIdList;
	}

	if (pvTaskList==NULL)
		return;

	EnterCriticalSection(&m_csUploadTaskList);
	vector<CString>::iterator iter = pvTaskList->begin();
	for (;iter!=pvTaskList->end();)
	{
		CString strNetDiskDir = *iter;
		if (strNetDiskDir.Right(1)=="/") // 任务为文件夹
		{
			strNetDiskDir.TrimRight("/");
			strNetDiskDir = strNetDiskDir.Left(strNetDiskDir.ReverseFind('/')+1);
		}
		else
		{
			strNetDiskDir = strNetDiskDir.Left(strNetDiskDir.ReverseFind('/')+1);
		}

		CTaskBase *pTask = thePoolManager.GetTaskFromHistory(*iter);
		if (pTask==NULL)
		{
			iter  = pvTaskList->erase(iter);
			continue;
		}
	

		if (pTask->m_Status ==CTaskBase::task_running)
		{
			if (strNetDiskDir.Compare(m_ListCtrl.GetNetDiskDir())==0) //查询项位于当前目录
			{
				if (pTask->m_strOperation == "download" ||pTask->m_strOperation == "upload" )
					m_ListCtrl.UpdateProgress(pTask->m_dProgress,strOperation,*iter);
			}

			iter++;
			continue;
		}
		else if (pTask->m_Status ==CTaskBase::task_finish)
		{
			if (strOperationType=="upload" && pTask->m_strOperation == "upload")
			{
				CUploadTask *pUploadTask = dynamic_cast<CUploadTask*>(pTask);
				SerializationManager.DeleteUploadTaskHistory(theApp.m_strUsrId,pUploadTask->m_FileInfo); // 删除完成的历史任务
				CString strTaskId = *iter;
				if (strNetDiskDir.Compare(m_ListCtrl.GetNetDiskDir())==0) //查询项位于当前目录
				{
					m_ListCtrl.SetItemFileId(pUploadTask->m_FileInfo.m_strName,pUploadTask->m_strFileId);
					m_ListCtrl.ClearProgressMsg(strTaskId);
					iter = pvTaskList->erase(iter);
				}
				else
					iter++;

				thePoolManager.RemoveFinishedTask(strTaskId); // 从线程池任务管理器里移除已经完成的任务记录，加快查询速度
				TellParentDirProgressChanged(strTaskId);
				GetParent()->PostMessage(WM_STATUS_TEXT_CHANGE,0,0);
				continue;
			}
			else if (strOperationType=="download" && pTask->m_strOperation == "download" )
			{
				thePoolManager.RemoveFinishedTask(*iter); // 从线程池任务管理器里移除已经完成的任务记录，加快查询速度
				TellParentDirProgressChanged(*iter);

				if (strNetDiskDir.Compare(m_ListCtrl.GetNetDiskDir())==0) //查询项位于当前目录
				{
					m_ListCtrl.ClearProgressMsg(*iter);
					iter = pvTaskList->erase(iter);
				}
				else
					iter++;

				GetParent()->PostMessage(WM_STATUS_TEXT_CHANGE,0,0);
				continue;
			}
			else if(pTask->m_strOperation == "uploadDir" || pTask->m_strOperation == "downloadDir")
			{
				iter++;
				continue;
			}		
			
		}
		else if (pTask->m_Status == CTaskBase::task_error)
		{
			if (strNetDiskDir.Compare(m_ListCtrl.GetNetDiskDir())==0) //查询项位于当前目录
			{
				m_ListCtrl.SetProgressText("传输错误",*iter);
				iter = pvTaskList->erase(iter);
			}
			else
				iter++;

			continue;
		}
		else if (pTask->m_Status == CTaskBase::task_wait)
		{
			if (strNetDiskDir.Compare(m_ListCtrl.GetNetDiskDir())==0) //查询项位于当前目录
			{
				if (pTask->m_strOperation == "download" ||pTask->m_strOperation == "upload" )
					m_ListCtrl.SetProgressText("等待传输",*iter);
			}

			iter++;
			continue;
		}
		else if (pTask->m_Status == CTaskBase::task_stop)
		{
			m_ListCtrl.SetProgressText("传输停止",*iter);
			iter++;
			continue;
		}
		else
			iter++;

	}
	LeaveCriticalSection(&m_csUploadTaskList);
}

 LRESULT CPageNetDisk::OnDownloadFinishMsg(WPARAM ,LPARAM lp)
 {
	 char *pDir = (char*)lp;
	 CString strDir = pDir;
	 delete []pDir;
	 MessageBox("下载完毕","提示",MB_OK|MB_ICONINFORMATION);
	 return S_OK;
 }

 void CPageNetDisk::OnNMRClickListNetdisk(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	 // TODO: 在此添加控件通知处理程序代码
	 *pResult = 0;
	 CMenu menu;
	 menu.LoadMenu(IDR_MENU_RIGHT_POP_LIST);

	 CMenu *pMenu = menu.GetSubMenu(0);
	 CPoint pt;
	 GetCursorPos(&pt);
	 pMenu->TrackPopupMenu(0,pt.x , pt.y,this);
 }

 void CPageNetDisk::DownLoadToDir(const CString & strDir)
 {
	 CString strErrMsg;
	 BOOL bAddNewDownloadFileSuccess = FALSE;
	 POSITION pt = m_ListCtrl.GetFirstSelectedItemPosition();
	 while(pt)
	 {
		 int iRow = m_ListCtrl.GetNextSelectedItem(pt);
		 CFileInfo &info = m_ListCtrl.m_FileInfoArry[iRow];
		 if (info.m_strFileId.IsEmpty() && info.m_strType == "文件夹")
		 {
			 CString strDirNetDiskPath = m_ListCtrl.GetNetDiskDir() + info.m_strName + "/";
			 CDownloadDirTask downloadDirTask(strDirNetDiskPath,info,strDir);
			 if(!thePoolManager.AddTask(downloadDirTask))
			 {
				 CString strMsg;
				 strMsg.Format("文件\"%s\"前一操作尚未完成，请稍后重试！",info.m_strName);
				 MessageBox(strMsg,"提示",MB_OK|MB_ICONINFORMATION);
				 continue;
			 }
			 m_ListCtrl.SetProgressText("正在传输",strDirNetDiskPath);
			 continue;
		 }

		 CDownloadTask downloadTask(m_ListCtrl.GetNetDiskDir(),info,strDir);
		 if(!thePoolManager.AddTask(downloadTask))
		 {
			 CString strMsg;
			 strMsg.Format("文件\"%s\"前一操作尚未完成，请稍后重试！",info.m_strName);
			 MessageBox(strMsg,"提示",MB_OK|MB_ICONINFORMATION);
			 continue;
		 }
		bAddNewDownloadFileSuccess = TRUE;
		AddRunningDownloadTaskIdList(downloadTask.GetTaskId());
	 }
	 if (bAddNewDownloadFileSuccess)
	 {
		 m_DownloadDestDirs.push_back(strDir);
		 StartQueryTaskStatusTimer();
	 }
 }

 void CPageNetDisk::OnListDownToDesktop()
 {
	 // TODO: 在此添加命令处理程序代码
	 UINT iCount = m_ListCtrl.GetSelectedCount();
	 if (iCount<1)
	 {
		 MessageBox("请先选择要下载的文件或文件夹!","提示",MB_ICONINFORMATION|MB_OK);
		 return;
	 }
	 CString strDeskTopDir;
	 SHGetSpecialFolderPath(0,strDeskTopDir.GetBuffer(MAX_PATH),CSIDL_DESKTOPDIRECTORY,FALSE);
	 strDeskTopDir.ReleaseBuffer();
	 DownLoadToDir(strDeskTopDir);
 }

 void CPageNetDisk::OnListDelete()
 {
	 // TODO: 在此添加命令处理程序代码
	 UINT iCount = m_ListCtrl.GetSelectedCount();
	 if (iCount<1) return;

	 vector<CDataDeleteFile> vFileList;
	 vector<CFileInfo> vLocalDel;
	 POSITION pt = m_ListCtrl.GetFirstSelectedItemPosition();
	 while(pt)
	 {
		 int iRow = m_ListCtrl.GetNextSelectedItem(pt);
		 CFileInfo &info = m_ListCtrl.m_FileInfoArry[iRow];

		 if (!info.m_strProgressText.IsEmpty())
		 {
			 MessageBox("操作正在进行，无法删除！","提示",MB_OK|MB_ICONINFORMATION);
			 return;
		 }

		  if (info.m_strType == "File" && info.m_strFileId.IsEmpty())
		  {
			  vLocalDel.push_back(info);
			  continue;
		  }

		 CDataDeleteFile DFile;
		 if (info.m_strType == "文件夹")
			 DFile.m_strType = "Folder";
		 else
			 DFile.m_strType = "File";

		 DFile.m_strNetDiskDir = m_ListCtrl.GetNetDiskDir();
		 DFile.m_strItemName = info.m_strName;
		 vFileList.push_back(DFile);
	 }

	 if (vFileList.empty() && vLocalDel.empty())
		 return;

	 if (MessageBox("确定要删除选中项？","删除提示",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)
		 return;

	 if(!vLocalDel.empty())
	 {
		 for (size_t i=0;i<vLocalDel.size();i++)
		 {
			 CString strTaskID = m_ListCtrl.GetNetDiskDir() + vLocalDel[i].m_strName;
			 if (vLocalDel[i].m_strType == "文件夹")
				strTaskID +="/";

			 thePoolManager.RemoveFinishedTask(strTaskID);
			 DeleteRunningTaskId(strTaskID);
			 SerializationManager.DeleteUploadTaskHistory(theApp.m_strUsrId,vLocalDel[i]); // 删除错误的任务
		 }
		
	 }

	 if (vFileList.empty())
	 {
		 UpdateFileList(m_ListCtrl.GetNetDiskDir()); //刷新列表
		 RefreshUserInfo();
		 return;
	 }

	 SOCKET sockClent =  GetConnect(theApp.m_strServerIp);
	 if (sockClent == 0)
	 {
		 MessageBox("连接服务器失败!","提示",MB_ICONERROR|MB_OK);
		 return;
	 }

	 BeginWaitCursor();
	 if (!DelFileFromNetDisk(sockClent,theApp.m_strUsrId,vFileList))
	 {
		 
		  MessageBox("删除失败!","提示",MB_ICONERROR|MB_OK);
		  return;
	 }
	 closesocket(sockClent);
	 UpdateFileList(m_ListCtrl.GetNetDiskDir()); //刷新列表
	 RefreshUserInfo();
	 EndWaitCursor();
 }

 void CPageNetDisk::OnBtnOpen()
 {
	 // TODO: 在此添加命令处理程序代码
	  UINT iCount = m_ListCtrl.GetSelectedCount();
	  if (iCount<1) return;

	  vector<CFileInfo> vListSelFile;
	  vector<CFileInfo> vListSelDir;

	  POSITION pt = m_ListCtrl.GetFirstSelectedItemPosition();
	  while(pt)
	  {
		  int iRow = m_ListCtrl.GetNextSelectedItem(pt);
		  CFileInfo &info = m_ListCtrl.m_FileInfoArry[iRow];
		  if (info.m_strType == "文件夹")
			 vListSelDir.push_back(info);
		  else
			  vListSelFile.push_back(info);
	  }

	  if (vListSelDir.size() >1)
	  {
		  MessageBox("无法同时打开多个文件夹！","提示",MB_OK|MB_ICONINFORMATION);
		  return;
	  }
	  if (!vListSelDir.empty())
	  {
		  CString strNetDiskDir = m_ListCtrl.GetNetDiskDir() + vListSelDir[0].m_strName + "/";
		  UpdateFileList(strNetDiskDir);
		  AddDirChangeHistory(strNetDiskDir);
	  }

	  if (!vListSelFile.empty()) // 打开选中的文件
	  {
		  for (size_t i=0;i<vListSelFile.size();i++)
		  {
			  COpenFileTask OpenTask(vListSelFile[i],NULL);
			  thePoolManager.AddTask(OpenTask,TRUE);
		  }
	  }
 }
 void CPageNetDisk::AddDirChangeHistory(const CString & strDir)
 {
	 if (m_vListNetDiskDirHistory.size() > 0)
	 {
		 while((m_vListNetDiskDirHistory.size()-1)!= m_iPosition) // 在历史记录中间开始新的操作时，当前位置之后的历史记录失效
			 m_vListNetDiskDirHistory.erase(m_vListNetDiskDirHistory.begin() + m_iPosition+1);
	 }

	  m_vListNetDiskDirHistory.push_back(strDir);
	  if (m_vListNetDiskDirHistory.size() > 10)
		  m_vListNetDiskDirHistory.erase(m_vListNetDiskDirHistory.begin());

	  m_iPosition = m_vListNetDiskDirHistory.size() - 1;
	  m_btnGoBack.EnableWindow(TRUE);
	  m_btnGoForward.EnableWindow(FALSE);
 }
 void CPageNetDisk::OnListProp()
 {
	 // TODO: 在此添加命令处理程序代码
 }


 void CPageNetDisk::OnListRename()
 {
	 // TODO: 在此添加命令处理程序代码
	 UINT iCount = m_ListCtrl.GetSelectedCount();
	 if (iCount<1) return;

	 if (iCount >1)
	 {
		 MessageBox("无法同时重命名多个项！","提示",MB_OK|MB_ICONINFORMATION);
		 return;
	 }

	
	 POSITION pt = m_ListCtrl.GetFirstSelectedItemPosition();
	 int iRow = m_ListCtrl.GetNextSelectedItem(pt);
	 CFileInfo &info = m_ListCtrl.m_FileInfoArry[iRow];
	 m_strReNameFileNetDiskPath = info.m_strNetDiskPath;
	 m_iReNameFileRowId = iRow;

	 CRect rect;  
	 int width = m_ListCtrl.GetColumnWidth(0);  
	 m_ListCtrl.GetSubItemRect(iRow,0,LVIR_LABEL,rect);
	 m_ListCtrl.ClientToScreen(rect);
	 ScreenToClient(rect);
	 m_strNewName = info.m_strName;
	 m_editRename.MoveWindow(rect);
	 UpdateData(FALSE);
	 int iSelEnd = -1;
	 if(info.m_strType != "文件夹")
	 {
		 iSelEnd = m_strNewName.ReverseFind('.');
		 if (iSelEnd<0) 
			 iSelEnd = -1;
	 }

	 m_editRename.SetSel(0,iSelEnd);
	 m_editRename.ShowWindow(SW_SHOW);
	 m_editRename.SetFocus();
	 m_editRename.SetCapture();
 }

 void CPageNetDisk::OnListCut()
 {
	 // TODO: 在此添加命令处理程序代码
 }

 void CPageNetDisk::OnListCopy()
 {
	 // TODO: 在此添加命令处理程序代码
 }
 void CPageNetDisk::OnUpdateRightPopMenu(CCmdUI *pCmdUI)
 {
	 // TODO: 在此添加命令更新用户界面处理程序代码
	  UINT iCount = m_ListCtrl.GetSelectedCount();
	  if (iCount <1)
		  pCmdUI->Enable(FALSE);
	  else
		  pCmdUI->Enable(TRUE);
 }


 void CPageNetDisk::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
 {
	 CDialogEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

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


 void CPageNetDisk::OnNMDblclkListNetdisk(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	 // TODO: 在此添加控件通知处理程序代码
	 *pResult = 0;
	 OnBtnOpen();
 }
void CPageNetDisk::AddRunningUploadTaskIdList( LPCTSTR strTaskId)
{
	EnterCriticalSection(&m_csUploadTaskList);
	RunningUploadTaskIdList.push_back(strTaskId);
	LeaveCriticalSection(&m_csUploadTaskList);
	StartQueryTaskStatusTimer();
}
void CPageNetDisk::AddRunningDownloadTaskIdList( LPCTSTR strTaskId)
{
	EnterCriticalSection(&m_csUploadTaskList);
	RunningDownloadTaskIdList.push_back(strTaskId);
	LeaveCriticalSection(&m_csUploadTaskList);
	StartQueryTaskStatusTimer();
}
void CPageNetDisk::DeleteRunningTaskId(LPCTSTR  strTaskId)
{
	EnterCriticalSection(&m_csUploadTaskList);
	for (vector<CString>::iterator iter = RunningUploadTaskIdList.begin();iter!=RunningUploadTaskIdList.end();)
	{
		if (iter->Compare(strTaskId)==0)
		{
			iter = RunningUploadTaskIdList.erase(iter);
		}
		else
			iter++;
	}
	for (vector<CString>::iterator iter = RunningDownloadTaskIdList.begin();iter!=RunningDownloadTaskIdList.end();)
	{
		if (iter->Compare(strTaskId)==0)
		{
			iter = RunningDownloadTaskIdList.erase(iter);
		}
		else
			iter++;
	}
	LeaveCriticalSection(&m_csUploadTaskList);
}

void CPageNetDisk::TellParentDirProgressChanged(LPCTSTR strChildTaskId)
{
	CString strParentNetDiskDir = strChildTaskId;
	if (strParentNetDiskDir.Compare("/")==0)
		return;

	strParentNetDiskDir = strParentNetDiskDir.Mid(0,strParentNetDiskDir.ReverseFind('/')+1);

	if (!thePoolManager.HasFrontMatchTask(strParentNetDiskDir))
		m_ListCtrl.ClearProgressMsg(strParentNetDiskDir);
}
void CPageNetDisk::RefreshUserInfo()
{
	SOCKET sockClent = GetConnect(theApp.m_strServerIp);
	if (sockClent == 0)
	{
		MessageBox("连接服务器失败!","提示",MB_ICONERROR|MB_OK);
		return;
	}
	CDataLoginResult LoginRes;
	bool bok = Login(sockClent,theApp.m_strUsrId,theApp.m_strPasswd,LoginRes);
	if (!bok)
		return;

	theApp.m_strUsrName = LoginRes.m_szUserName;
	theApp.m_iAllSpace  = LoginRes.m_iAllSpace;
	theApp.m_iUsedSpace = LoginRes.m_iUsedSpace;
	GetParent()->Invalidate(FALSE);
}

void CPageNetDisk::OnEnKillfocusEditRename()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_editRename.ShowWindow(SW_HIDE);
	if (m_strNewName.Compare(m_ListCtrl.m_FileInfoArry[m_iReNameFileRowId].m_strName)==0 || m_strNewName.IsEmpty())
	{
		m_strNewName.Empty();
		return;
	}
	if (m_strNewName.GetLength() > 512)
	{
		m_strNewName.Empty();
		MessageBox("新文件名过长！","提示",MB_ICONINFORMATION|MB_OK);
		return;
	}

	SOCKET sockClent =  GetConnect(theApp.m_strServerIp);
	if (sockClent == 0)
	{
		MessageBox("连接服务器失败!","提示",MB_ICONERROR|MB_OK);
		return;
	}

	BeginWaitCursor();
	string strErr;
	if (!ReNameFileFromNetDisk(sockClent,theApp.m_strUsrId,m_ListCtrl.m_FileInfoArry[m_iReNameFileRowId].m_strName,
		m_ListCtrl.GetNetDiskDir(),m_strNewName,strErr))
	{
		EndWaitCursor();
		MessageBox(strErr.c_str(),"提示",MB_ICONERROR|MB_OK);
		return;
	}
	else
	{
		EndWaitCursor();
		closesocket(sockClent);
		m_ListCtrl.m_FileInfoArry[m_iReNameFileRowId].m_strName = m_strNewName;
		m_ListCtrl.Invalidate(FALSE);
		m_strNewName.Empty();
		MessageBox("重命名成功！","提示",MB_ICONINFORMATION|MB_OK);
	}
}


BOOL CPageNetDisk::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (m_editRename.IsWindowVisible())
			{
				m_ListCtrl.SetFocus();
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		else if (pMsg->wParam == VK_F2)
		{
			OnListRename();
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CPageNetDisk::LoadUnFinishedTask()
{
	vector<CFileInfo> vList;
	SerializationManager.LoadNotFinishedTask(theApp.m_strUsrId,vList);
	for (size_t i=0;i<vList.size();i++)
	{
		if (!GetFileInfo(vList[i].m_strLocalPath,vList[i],TRUE))
		{
			LogMsg("恢复上传文件%s失败，GetFileInfo返回FALSE",vList[i].m_strLocalPath);
			continue;
		}
		CString strDestDir = vList[i].m_strNetDiskPath;
		strDestDir = strDestDir.Left(strDestDir.ReverseFind('/')+1);
		CUploadTask uploadTask(strDestDir,vList[i],vList[i].m_strLocalPath);
		thePoolManager.AddTask(uploadTask);
		AddRunningUploadTaskIdList(uploadTask.GetTaskId());
	}

	UpdateFileList("/");

	if (vList.size() > 0)
		StartQueryTaskStatusTimer();

	KillTimer(WM_TMIMER_LOAD_HIST);
}
CString CPageNetDisk::GetStatusText()
{
	EnterCriticalSection(&m_csUploadTaskList);
	int iUploadCount = RunningUploadTaskIdList.size();
	int iDownCount  = RunningDownloadTaskIdList.size();
	LeaveCriticalSection(&m_csUploadTaskList);
	CString strStatusMsg;
	strStatusMsg.Format("%d个任务正在上传,%d个任务正在下载",iUploadCount,iDownCount);
	return strStatusMsg;
}