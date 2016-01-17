
// NetDiskUIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetDiskUI.h"
#include "NetDiskUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetDiskUIDlg 对话框




CNetDiskUIDlg::CNetDiskUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetDiskUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_brushBkrect = ::CreateSolidBrush(RGB(38,59,90));
	m_brushBkrectMid = ::CreateSolidBrush(RGB(221,228,228));
	m_pPageNetDisk = NULL;
	m_pPageXiangce  = NULL;
}
CNetDiskUIDlg::~CNetDiskUIDlg()
{
	if (m_pPageNetDisk)
	{
		m_pPageNetDisk->DestroyWindow();
		delete m_pPageNetDisk;
		m_pPageNetDisk = NULL;
	}

	if (m_pPageXiangce)
	{
		m_pPageXiangce->DestroyWindow();
		delete m_pPageXiangce;
		m_pPageXiangce = NULL;
	}
}

void CNetDiskUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_NETDSIK, m_btnNetDisk);
	DDX_Control(pDX, IDC_BUTTON_XIANGCE, m_btnXiangCe);
	DDX_Control(pDX, IDC_BTB_SETTING, m_btnSetting);
	DDX_Control(pDX, IDC_BTN_MIN, m_btnMin);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
}

BEGIN_MESSAGE_MAP(CNetDiskUIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_NETDSIK, &CNetDiskUIDlg::OnBnClickedButtonNetdisk)
	ON_BN_CLICKED(IDC_BUTTON_XIANGCE, &CNetDiskUIDlg::OnBnClickedButtonXiangce)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CNetDiskUIDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_MIN, &CNetDiskUIDlg::OnBnClickedBtnMin)
	ON_BN_CLICKED(IDC_BTB_SETTING, &CNetDiskUIDlg::OnBnClickedBtbSetting)
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_MENU_SETTING_LOGOUT, &CNetDiskUIDlg::OnMenuSettingLogout)
	ON_COMMAND(ID_MENU_SETTING_QUIT, &CNetDiskUIDlg::OnMenuSettingQuit)
	ON_MESSAGE(WM_STATUS_TEXT_CHANGE,&CNetDiskUIDlg::OnStatusChange)
END_MESSAGE_MAP()


// CNetDiskUIDlg 消息处理程序

BOOL CNetDiskUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_pPageNetDisk = new CPageNetDisk;
	m_pPageNetDisk->Create(CPageNetDisk::IDD,this);

	m_pPageXiangce = new CPageXiangCe;
	m_pPageXiangce->Create(CPageXiangCe::IDD,this);

	SetBackImage();
	AdjustLocation();
	m_btnNetDisk.SetCurrentShow(1); //启动时网盘按钮高亮
	m_pPageNetDisk->ShowWindow(SW_SHOW);
	m_pPageXiangce->ShowWindow(SW_HIDE);
	SetWindowText("网盘客户端"); // 系统工具栏显示名称，不加这句话就只有图标了
	m_bLogOut = FALSE;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CNetDiskUIDlg::DrawUsrInfo(CDC *pDC)
{
	LOGFONT lf;
	pDC->GetCurrentFont()->GetLogFont(&lf);

	lf.lfCharSet = 134;
	lf.lfHeight = 12;
	lf.lfWidth  = 0;
	strcpy_s(lf.lfFaceName,"宋体");

	CFont font;
	font.CreateFontIndirect(&lf);
	CFont *pOldFont = pDC->SelectObject(&font);

	pDC->SetTextColor(RGB(255,255,255));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(CAPTON_BTN_START_X - 5,40,theApp.m_strUsrId);
	pDC->TextOut(CAPTON_BTN_START_X - 5,65,theApp.m_strUsrName);

	CString strSpace;
	CString strSizeUsed = TransferFileSize(theApp.m_iUsedSpace);
	CString strSizeAll  = TransferFileSize(theApp.m_iAllSpace);
	strSpace.Format("%s/%s",strSizeUsed,strSizeAll);
	pDC->TextOut(CAPTON_BTN_START_X - 5,90,strSpace);

}
void CNetDiskUIDlg::OnPaint()
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
		CPaintDC dc(this); // device context for painting

		CRect rcClient;
		GetClientRect(&rcClient);


		//内存画图//////////////////////////
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);

		CBitmap bitmapMain;
		bitmapMain.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

		MemDC.SelectObject(&bitmapMain);


		HGDIOBJ oldBrush=MemDC.SelectObject(m_brushBkrect);
		MemDC.Rectangle(rcClient);
		MemDC.SelectObject(oldBrush);


		CRect rcMid = rcClient;
		rcMid.left = rcClient.left + BORDER_WIDTH;
		rcMid.top = rcClient.top  + CAPTION_HEIGHT - BORDER_WIDTH;
		rcMid.bottom = rcClient.bottom - STATUS_HEIGHT + BORDER_WIDTH;
		rcMid.right = rcClient.right - BORDER_WIDTH;

		oldBrush=MemDC.SelectObject(m_brushBkrectMid);
		MemDC.Rectangle(rcMid);
		MemDC.SelectObject(oldBrush);

		CRect rectHeader = rcClient;
		rectHeader.bottom = CAPTION_HEIGHT;
		DrawImage(m_BitmapHeader,&MemDC,rectHeader);

		CRect rectStatus = rcClient;
		rectStatus.top = rcClient.bottom - STATUS_HEIGHT;
		DrawImage(m_BitmapStatus,&MemDC,rectStatus);

	
		LOGFONT lf;
		dc.GetCurrentFont()->GetLogFont(&lf);

		lf.lfCharSet = 134;
		lf.lfHeight = 13;
		lf.lfWidth  = 0;
		strcpy_s(lf.lfFaceName,"楷体");

		CFont font;
		font.CreateFontIndirect(&lf);
		CFont *pOldFont = MemDC.SelectObject(&font);

		MemDC.SetTextColor(RGB(255,255,255));
		MemDC.SetBkMode(TRANSPARENT);
		MemDC.TextOut(10,5,_T("网盘客户端"));

		DrawUsrInfo(&MemDC);
		DrawStatusBar(&MemDC);

		dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &MemDC, 0, 0, SRCCOPY);

		MemDC.SelectObject(pOldFont);
		MemDC.DeleteDC();
		bitmapMain.DeleteObject();

	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNetDiskUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNetDiskUIDlg::OnBnClickedButtonNetdisk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btnNetDisk.SetCurrentShow(1);
	m_btnXiangCe.SetCurrentShow(0);
	m_pPageNetDisk->ShowWindow(SW_SHOW);
	m_pPageXiangce->ShowWindow(SW_HIDE);
}


void CNetDiskUIDlg::OnBnClickedButtonXiangce()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btnNetDisk.SetCurrentShow(0);
	m_btnXiangCe.SetCurrentShow(1);
	m_pPageNetDisk->ShowWindow(SW_HIDE);
	m_pPageXiangce->ShowWindow(SW_SHOW);
}

void CNetDiskUIDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::EndDialog(IDOK);
}
void CNetDiskUIDlg::OnBnClickedBtnMin()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_MINIMIZE);
}


void CNetDiskUIDlg::OnBnClickedBtbSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CMenu menuSet;
	menuSet.LoadMenu(IDR_MENU_SETTING);
	CMenu *pMenu = menuSet.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN,pt.x,pt.y,this);
}
void CNetDiskUIDlg::SetBackImage()
{
	char szFileDir[MAX_PATH] = {0};
	GetModuleFileName(NULL,szFileDir,MAX_PATH);
	CString strDir = szFileDir;
	strDir = strDir.Left(strDir.ReverseFind('\\')+1);
	SetImage(m_BitmapHeader,strDir + "Images\\header.bmp");
	SetImage(m_BitmapStatus,strDir + "Images\\status.bmp");
	//关闭按钮
	CString strCloseImgNormal = strDir + "Images\\close_normal.bmp";
	CString strCloseImgHover = strDir + "Images\\close_hover.bmp";
	CString strCloseImgDown = strDir + "Images\\close_down.bmp";
	m_btnClose.SetImage(strCloseImgNormal,strCloseImgHover,strCloseImgDown,strCloseImgNormal);

	// 最小化按钮
	CString strMinImgNormal = strDir + "Images\\min_normal.bmp";
	CString strMinImgHover = strDir + "Images\\min_hover.bmp";
	CString strMinImgDown = strDir + "Images\\min_down.bmp";
	m_btnMin.SetImage(strMinImgNormal,strMinImgHover,strMinImgDown,strMinImgNormal);

	// 设置按钮
	CString strSettingImgNormal = strDir + "Images\\setting_normal.bmp";
	CString strSettingImgHover = strDir + "Images\\setting_hover.bmp";
	CString strSettingImgDown = strDir + "Images\\setting_down.bmp";
	m_btnSetting.SetImage(strSettingImgNormal,strSettingImgHover,strSettingImgDown,strSettingImgNormal);

	// 网盘按钮
	CString strNetdiskImg =  strDir + "Images\\main_btn_netdisk.bmp";
	CString strNetdiskHover =  strDir + "Images\\main_btn_netdisk_hover.bmp";
	m_btnNetDisk.SetImage(strNetdiskImg,strNetdiskHover,strNetdiskHover,strNetdiskImg);
	
	// 相册按钮
	CString strXiangceImg =  strDir + "Images\\main_btn_xiangce.bmp";
	CString strXiangceHoverImg =  strDir + "Images\\main_xiangce_hover.bmp";
	m_btnXiangCe.SetImage(strXiangceImg,strXiangceHoverImg,strXiangceHoverImg,strXiangceImg);
}
void CNetDiskUIDlg::AdjustLocation()
{
	CRect rcClient;
	GetClientRect(rcClient);
	// 关闭按钮
	CRect tempUp = rcClient;
	tempUp.left = rcClient.right - (34 + 16);//21
	tempUp.right = tempUp.left  +  34 ;//21
	tempUp.top = rcClient.top + 5;
	tempUp.bottom = rcClient.top + 24 ;
	m_btnClose.SetWindowText(_T(""));
	m_btnClose.MoveWindow(tempUp,TRUE);

	// 最小化按钮
	tempUp = rcClient;
	tempUp.left = rcClient.right - (34 + 34 + 16);//21
	tempUp.right = tempUp.left  +  34 ;//21
	tempUp.top = rcClient.top + 5;
	tempUp.bottom = rcClient.top + 24 ;
	m_btnMin.SetWindowText(_T(""));
	m_btnMin.MoveWindow(tempUp,TRUE);

	// 设置按钮
	tempUp = rcClient;
	tempUp.left = rcClient.right - (34 + 34 +34  + 16);//21
	tempUp.right = tempUp.left + 34 ;//21
	tempUp.top = rcClient.top + 5;
	tempUp.bottom = rcClient.top + 24 ;
	m_btnSetting.SetWindowText(_T(""));
	m_btnSetting.MoveWindow(tempUp,TRUE);


	// 网盘按钮
	tempUp = rcClient;
	tempUp.left = CAPTON_BTN_START_X + 10 + CAPTON_BTN_WIDTH + CAPTION_BTN_GAP;
	tempUp.right = tempUp.left + CAPTON_BTN_WIDTH ;
	tempUp.top = rcClient.top + CAPTON_BTN_START_Y;
	tempUp.bottom = tempUp.top + CAPTON_BTN_HEIGHT ;
	m_btnNetDisk.SetWindowText(_T(""));
	m_btnNetDisk.MoveWindow(tempUp,TRUE);

	// 相册按钮
	tempUp = rcClient;
	tempUp.left = CAPTON_BTN_START_X + (CAPTON_BTN_WIDTH + CAPTION_BTN_GAP + 10)*2;
	tempUp.right = tempUp.left + CAPTON_BTN_WIDTH ;
	tempUp.top = rcClient.top + CAPTON_BTN_START_Y;
	tempUp.bottom = tempUp.top + CAPTON_BTN_HEIGHT ;
	m_btnXiangCe.SetWindowText(_T(""));
	m_btnXiangCe.MoveWindow(tempUp,TRUE);

	int iWidth  = rcClient.Width() - BORDER_WIDTH*2;
	int iHeight = rcClient.Height() - CAPTION_HEIGHT - STATUS_HEIGHT -BORDER_WIDTH*2;
	m_pPageNetDisk->MoveWindow(BORDER_WIDTH,CAPTION_HEIGHT,iWidth,iHeight,SWP_NOZORDER);
	m_pPageXiangce->MoveWindow(BORDER_WIDTH,CAPTION_HEIGHT,iWidth,iHeight,SWP_NOZORDER);
}
void CNetDiskUIDlg::SetImage(CBitmap &bitmap, CString strName)
{
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)::LoadImage(NULL, strName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	//ASSERT(hBitmap);

	if(hBitmap == NULL)
	{
		CString strErr;
		strErr.Format("无法加载位图资源【%s】",strName);
		MessageBox(strErr,"错误",MB_OK|MB_ICONERROR);
		PostQuitMessage(0);
	}

	if (bitmap.m_hObject)
		bitmap.Detach();

	bitmap.Attach(hBitmap);
}
void CNetDiskUIDlg::DrawImage(CBitmap &bitmap,CDC * pDC,CRect rect)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = MemDC.SelectObject(&bitmap);

	BITMAP bm;
	bitmap.GetBitmap(&bm);
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}

LRESULT CNetDiskUIDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ScreenToClient(&point);

	RECT rtWindow;
	GetClientRect(&rtWindow);

	long wndHeight = rtWindow.bottom - rtWindow.top;
	long wndWidth = rtWindow.right - rtWindow.left;

	RECT rcW = {0,0,wndWidth,CAPTION_HEIGHT};
	if(::PtInRect(&rcW,point))
	{  
		return HTCAPTION;  // 在拖动范围内
	}

	return CWnd::OnNcHitTest(point);
}


void CNetDiskUIDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	int iWidth  = cx - BORDER_WIDTH*2;
	int iHeight = cy - CAPTION_HEIGHT - STATUS_HEIGHT - BORDER_WIDTH*2;
	if (m_pPageNetDisk)
		m_pPageNetDisk->MoveWindow(BORDER_WIDTH,CAPTION_HEIGHT,iWidth,iHeight,SWP_NOZORDER);
		
	if (m_pPageXiangce)
		m_pPageXiangce->MoveWindow(BORDER_WIDTH,CAPTION_HEIGHT,iWidth,iHeight,SWP_NOZORDER);
}


void CNetDiskUIDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	RemoveProp(GetSafeHwnd(),AfxGetApp()->m_pszExeName); //移除窗体标志
}


int CNetDiskUIDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	 SetProp(GetSafeHwnd(),AfxGetApp()->m_pszExeName,(HANDLE)1); // 设置窗体标志
	return 0;
}
BOOL CNetDiskUIDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CNetDiskUIDlg::OnMenuSettingLogout()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedBtnClose();
	m_bLogOut = TRUE;
	BOOL bAutoLogin = FALSE;
	char szDir[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(),szDir,MAX_PATH);
	CString strConfigINI = szDir;
	strConfigINI.MakeLower();
	strConfigINI = strConfigINI.Left(strConfigINI.ReverseFind('.') + 1) + "ini";
	WritePrivateProfileStruct("setting","AutoLogin",&bAutoLogin,sizeof(bAutoLogin),strConfigINI);
}


void CNetDiskUIDlg::OnMenuSettingQuit()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedBtnClose();
}
void CNetDiskUIDlg::DrawStatusBar(CDC *pDC)
{
	CString strText;
	if (m_pPageNetDisk->IsWindowVisible())
	{
		strText = m_pPageNetDisk->GetStatusText();
	}
	else if (m_pPageXiangce->IsWindowVisible())
	{
		strText.Empty();
	}

	if (strText.IsEmpty())
		return;

	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rectStatus = rcClient;
	rectStatus.top = rcClient.bottom - STATUS_HEIGHT;

	LOGFONT lf;
	pDC->GetCurrentFont()->GetLogFont(&lf);
	lf.lfCharSet = 134;
	lf.lfHeight = 12;
	lf.lfWidth  = 0;
	strcpy_s(lf.lfFaceName,"宋体");

	CFont font;
	font.CreateFontIndirect(&lf);
	CFont *pOldFont = pDC->SelectObject(&font);

	pDC->SetTextColor(RGB(255,255,255));
	pDC->SetBkMode(TRANSPARENT);
	CSize si = pDC->GetTextExtent(strText);

	CRect rectOut = rectStatus;
	rectOut.top = rectStatus.top + STATUS_HEIGHT/2 - si.cy /2;
	rectOut.right = rectStatus.right - 40;
	rectOut.left  = rectOut.right  - si.cx;
	pDC->TextOut(rectOut.left,rectOut.top,strText);
}
void CNetDiskUIDlg::RefreshStatusBar()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rectStatus = rcClient;
	rectStatus.top = rcClient.bottom - STATUS_HEIGHT;
	InvalidateRect(rectStatus,FALSE);
}
LRESULT CNetDiskUIDlg::OnStatusChange(WPARAM,LPARAM)
{
	RefreshStatusBar();
	return S_OK;
}