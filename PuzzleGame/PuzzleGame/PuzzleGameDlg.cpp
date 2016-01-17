// PuzzleGameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PuzzleGame.h"
#include "PuzzleGameDlg.h"
#include "BlockManager.h"
#include <io.h>
#include "GameRecord.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  GRID_COUNT  2

#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")
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


// CPuzzleGameDlg dialog




CPuzzleGameDlg::CPuzzleGameDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CPuzzleGameDlg::IDD, pParent),
	m_GameCtrler(this),
	m_bShowGridLine(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CPuzzleGameDlg::~CPuzzleGameDlg()
{
	mciSendString("stop bgMusic ",NULL,0,NULL);
}
void CPuzzleGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICTURE_WND, m_PictureWnd);
	DDX_Control(pDX, IDC_COMBO1, m_ComboboxGridCount);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRID, m_Check);
}

BEGIN_MESSAGE_MAP(CPuzzleGameDlg, CBCGPDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_PRE, &CPuzzleGameDlg::OnBnClickedBtnPre)
	ON_BN_CLICKED(IDC_BTN_RESTART, &CPuzzleGameDlg::OnBnClickedBtnRestart)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CPuzzleGameDlg::OnBnClickedBtnNext)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPuzzleGameDlg::OnCbnSelchangeComboGridCount)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRID, &CPuzzleGameDlg::OnBnClickedCheckShowGrid)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPuzzleGameDlg message handlers

BOOL CPuzzleGameDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

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
	m_Check.SetCheck(TRUE);
	LoadGameImage();
	m_iCurBackImageIndex = 0;
	m_ComboboxGridCount.SetCurSel(3);
	m_iGridCount = 5;
	if (!IntilizeGameUseRecord())
		IntilizeGame(m_strBackImgFileList[m_iCurBackImageIndex]);

	UpdateBtnEnable();
	//打开音乐文件
	mciSendString("open sound\\background.mp3 alias bgMusic ", NULL, 0, NULL);
	mciSendString("play bgMusic repeat", NULL, 0, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPuzzleGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPuzzleGameDlg::OnPaint() 
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
		CPaintDC dc(this);
		CDC memDC; //内存设备
		memDC.CreateCompatibleDC(&dc); //创建设备兼容性
		CBitmap bmp;
		//创建与设备兼容的位图，大小同客户区

		CRect pictureRect;
		m_PictureWnd.GetClientRect(&pictureRect);
		bmp.CreateCompatibleBitmap(&dc,pictureRect.Width(),pictureRect.Height());
		memDC.SelectObject(&bmp); //将位图选入内存设备

		CBlockManager::GetInstance()->DrawAllBlock(memDC);
		if (m_bShowGridLine)
		{
			CBlockManager::GetInstance()->DrawGridLine(memDC);
		}

		CPoint ptLeftTop = pictureRect.TopLeft();
		m_PictureWnd.MapWindowPoints(this,&ptLeftTop,1);
		// 将内存设备上的内容复制到界面上
		dc.StretchBlt(ptLeftTop.x,ptLeftTop.y,pictureRect.Width(),pictureRect.Height(),
			&memDC,0,0,pictureRect.Width(),pictureRect.Height(),SRCCOPY);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPuzzleGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPuzzleGameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->MapWindowPoints(&m_PictureWnd,&point,1);
	CRect rectPicWnd;
	m_PictureWnd.GetClientRect(rectPicWnd);
	if (rectPicWnd.PtInRect(point))
	{
		int iID = m_GameCtrler.GetClickBlockID(point);
		CString strTmp;
		strTmp.Format("点击%d\n",iID);
		OutputDebugString(strTmp);

		m_iSwapCount ++;
		CString strInfo;
		strInfo.Format("当前点击%d次",m_iSwapCount);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(strInfo);
		BOOL bFinished = m_GameCtrler.MouseClick(point); // 把鼠标点击事件交给游戏控制器处理
		Invalidate(FALSE);

		if(bFinished)
		{
			GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
			MessageBox("恭喜你,你已经完成本级任务!","提示",MB_OK|MB_ICONINFORMATION);
		}
	}

	CBCGPDialog::OnLButtonDown(nFlags, point);
}
void CPuzzleGameDlg::LoadGameImage()
{
	char szBuf[256] ={0};
	GetModuleFileName(NULL,szBuf,256);
	CString strBackImageFolder = szBuf;
	strBackImageFolder = strBackImageFolder.Left(strBackImageFolder.ReverseFind('\\'));
	strBackImageFolder += "\\BackImage\\";
	CString strFind = strBackImageFolder + "*.*";

	struct _finddata_t c_file;
	intptr_t handle =_findfirst(strFind,&c_file);	

	int iErr =0;
	if (handle == -1)
		iErr = -1;

	string strItemName;
	while (iErr==0)
	{
		bool bIsFolder = (c_file.attrib & _A_SUBDIR) == _A_SUBDIR;
		if (strcmp(c_file.name,".")==0 ||strcmp(c_file.name,"..")==0 || bIsFolder)
		{
			iErr =  _findnext(handle,&c_file);
			continue;
		}

		CString strFile = strBackImageFolder + c_file.name;
		strFile.MakeLower();
		if (strFile.Right(4)==".bmp"|| strFile.Right(4)==".png" || strFile.Right(4)==".jpg")
		{
			m_strBackImgFileList.push_back(strFile);
		}

		iErr =  _findnext(handle,&c_file);
	}
	_findclose(handle);
}
void CPuzzleGameDlg::IntilizeGameUseRecord(const CString & strImage,vector<int> & vIDs)
{
	_bstr_t bstr(strImage);
	CStringW strW =(WCHAR*)bstr;
	CBlockManager::GetInstance()->Clear();
	CRect pictureRect;
	m_PictureWnd.GetClientRect(&pictureRect);
	CBlockManager::GetInstance()->IntilizeBlockWithRecord(pictureRect,m_iGridCount,m_iGridCount,vIDs);
	CBlockManager::GetInstance()->LoadPuzzlePicture(strW,CPaintDC(this));
	m_GameCtrler.ResetClickCount();
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText("当前点击0次");
	Invalidate(FALSE);
}
BOOL CPuzzleGameDlg::IntilizeGameUseRecord()
{
	CGameRecord record;
	CString strImg;
	vector<int> vIds;

	BOOL bShowGrid = FALSE;
	BOOL bSuccess = record.LoadGameRecord(strImg,vIds,bShowGrid,m_iGridCount,m_iSwapCount);
	if (!bSuccess)
		return FALSE;

	for (size_t i=0;i<m_strBackImgFileList.size();i++)
	{
		if (strImg == m_strBackImgFileList[i])
		{
			m_iCurBackImageIndex = i;
			break;
		}
	}
	m_bShowGridLine = bShowGrid;
	m_Check.SetCheck(bShowGrid);
	IntilizeGameUseRecord(strImg,vIds);
	int iSel = 3;
	CString strCmp;
	strCmp.Format("%d*%d",m_iGridCount,m_iGridCount);
	int iCount = m_ComboboxGridCount.GetCount();
	for (int i=0;i< iCount;i++)
	{
		CString strTmp;
		m_ComboboxGridCount.GetLBText(i,strTmp);
		if (strTmp==strCmp)
		{
			iSel = i;
			break;
		}

	}
	m_ComboboxGridCount.SetCurSel(iSel);

	CString strInfo;
	strInfo.Format("当前点击%d次",m_iSwapCount);
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(strInfo);
	return TRUE;
}
void  CPuzzleGameDlg::IntilizeGame(CString strImage)
{
	_bstr_t bstr(strImage);
	CStringW strW =(WCHAR*)bstr;
	CBlockManager::GetInstance()->Clear();
	CRect pictureRect;
	m_PictureWnd.GetClientRect(&pictureRect);
	CBlockManager::GetInstance()->IntilizeBlock(pictureRect,m_iGridCount,m_iGridCount);
	CBlockManager::GetInstance()->LoadPuzzlePicture(strW,CPaintDC(this));
	m_GameCtrler.ResetClickCount();
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText("当前点击0次");
	Invalidate(FALSE);
	m_iSwapCount = 0;
}
void CPuzzleGameDlg::OnBnClickedBtnPre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iCurBackImageIndex >0)
	{
		m_iCurBackImageIndex--;
		IntilizeGame(m_strBackImgFileList[m_iCurBackImageIndex]);
		UpdateBtnEnable();
	}
	/*else
	{
	MessageBox("没有上一关啦!","提示",MB_OK|MB_ICONINFORMATION);
	}*/

}
void CPuzzleGameDlg::UpdateBtnEnable()
{
	if (m_iCurBackImageIndex >0)
		GetDlgItem(IDC_BTN_PRE)->EnableWindow(TRUE); 
	else
		GetDlgItem(IDC_BTN_PRE)->EnableWindow(FALSE); 

	if (m_iCurBackImageIndex < int(m_strBackImgFileList.size() -1 ))
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE); 
	else
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE); 
}
void CPuzzleGameDlg::OnBnClickedBtnRestart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDNO == MessageBox("重新开始游戏？","提示",MB_YESNO|MB_ICONQUESTION))
		return;

	IntilizeGame(m_strBackImgFileList[m_iCurBackImageIndex]);
}

void CPuzzleGameDlg::OnBnClickedBtnNext()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iCurBackImageIndex < int(m_strBackImgFileList.size()-1))
	{
		m_iCurBackImageIndex++;
		IntilizeGame(m_strBackImgFileList[m_iCurBackImageIndex]);
		UpdateBtnEnable();
	}
	/*else
	{
	MessageBox("没有下一关啦!","提示",MB_OK|MB_ICONINFORMATION);
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	}*/
}


void CPuzzleGameDlg::OnCbnSelchangeComboGridCount()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSel = m_ComboboxGridCount.GetCurSel();
	if (iSel <0) return;

	CString str ="";
	m_ComboboxGridCount.GetLBText(iSel,str);
	ASSERT(str.Find("*")>0);
	str = str.Left(str.Find("*"));
	m_iGridCount = atoi(str);
	OnBnClickedBtnRestart();
}


void CPuzzleGameDlg::OnBnClickedCheckShowGrid()
{
	// TODO: 在此添加控件通知处理程序代码
	int iState = m_Check.GetCheck();
	m_bShowGridLine = BOOL(iState);
	Invalidate(FALSE);
}


void CPuzzleGameDlg::OnDestroy()
{
	CBCGPDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	CGameRecord record;
	BOOL bShowGrid = (BOOL)m_Check.GetCheck();
	record.WriteGameRecord(m_strBackImgFileList[m_iCurBackImageIndex],bShowGrid,m_iGridCount,m_iSwapCount,CBlockManager::GetInstance()->GetBlocks());
}
