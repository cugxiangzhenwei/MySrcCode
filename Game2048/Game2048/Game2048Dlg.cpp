// Game2048Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Game2048.h"
#include "Game2048Dlg.h"
#include "GameRecord.h"
#include <vector>
using namespace std;

#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CGame2048Dlg dialog




CGame2048Dlg::CGame2048Dlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CGame2048Dlg::IDD, pParent)
	,m_GameCtrler(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_mapColor[0] = RGB(205,194,181);
	g_mapColor[2] = RGB(238,226,213);
	g_mapColor[4] = RGB(238,222,197);
	g_mapColor[8] = RGB(238,178,115);
	g_mapColor[16] = RGB(246,149,98);
	g_mapColor[32] = RGB(246,125,90);
	g_mapColor[64] = RGB(246,93,49);
	g_mapColor[128] = RGB(238,206,106);
}
CGame2048Dlg::~CGame2048Dlg()
{
	mciSendString("stop bgMusic ",NULL,0,NULL);
}
void CGame2048Dlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WND_CORE, m_wndCore);
}

BEGIN_MESSAGE_MAP(CGame2048Dlg, CBCGPDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_RESTART, &CGame2048Dlg::OnBnClickedBtnRestart)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGame2048Dlg message handlers

BOOL CGame2048Dlg::OnInitDialog()
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

	CRect rectCore;
	m_wndCore.GetClientRect(rectCore);
	m_BlockGapX = 10;
	m_BlockGapY = 8;
	m_BlockWidth = (rectCore.Width() - m_BlockGapX*5)/4;
	m_BlockHeight = (rectCore.Height() -m_BlockGapY*5)/4;
	int iLeftX(0),iLeftY(0);
	for (int i=0;i<4;i++) // 行
	{
		iLeftY = ((1+i)*m_BlockGapY + i*m_BlockHeight);
		iLeftX = 0;
		for (int j=0;j<4;j++) // 列
		{
			iLeftX = ((j+1)*m_BlockGapX + j*m_BlockWidth);
			m_RectBlocks[i*4+j].m_RectBlock = CRect(CPoint(iLeftX,iLeftY),CSize(m_BlockWidth,m_BlockHeight));
			m_RectBlocks[i*4+j].m_iBlockID = i*4+j;
			m_RectBlocks[i*4+j].m_iGameNumShow = 0;
			m_RectBlocks[i*4+j].m_iFlashCount  = 0;
		}
	}
	CGameRecord gameRecord;
	int iSumSocre = 0;
	if (gameRecord.LoadGameRecord(m_RectBlocks,16,iSumSocre))
	{
		CString strSumSocre;
		strSumSocre.Format("当前总分:%d",iSumSocre);
		m_GameCtrler.SetSumScore(iSumSocre);
		GetDlgItem(IDC_STATIC_SUM_SCORE)->SetWindowText(strSumSocre);
	}
	else
		IntilizeGame();
	
	//打开音乐文件
	mciSendString("open sound\\bgmusic.mp3 alias bgMusic ", NULL, 0, NULL);
	mciSendString("play bgMusic repeat", NULL, 0, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGame2048Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGame2048Dlg::OnPaint() 
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
		
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);

		CRect rectCore;
		m_wndCore.GetClientRect(rectCore);

		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&dc,rectCore.Width(),rectCore.Height());
		MemDC.SelectObject(&bitmap);

		CBrush brush(g_ColorBackGround);
		//CBrush brush;
		//CBitmap bgBitmap;
		//bgBitmap.LoadBitmap(IDB_BITMAP_BG);
		//brush.CreatePatternBrush(&bgBitmap); // 创建位图画刷
		MemDC.FillRect(rectCore,&brush);

		DrawBlocks(MemDC);

		m_wndCore.MapWindowPoints(this,rectCore);
		dc.BitBlt(rectCore.left,rectCore.top,rectCore.Width(),rectCore.Height(),&MemDC,0,0,SRCCOPY);
		MemDC.DeleteDC();

		//CBCGPDialog::OnPaint();
	}
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGame2048Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CGame2048Dlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBCGPDialog::OnEraseBkgnd(pDC);

	CRect rectCore;
	m_wndCore.GetClientRect(rectCore);
	m_wndCore.MapWindowPoints(this,rectCore);
	CBrush brush(g_ColorBackGround);
	//CBitmap bgBitmap;
	//bgBitmap.LoadBitmap(IDB_BITMAP_BG);
	//brush.CreatePatternBrush(&bgBitmap); // 创建位图画刷
	pDC->FillRect(&rectCore,&brush);
	return TRUE;
}
void CGame2048Dlg::IntilizeGame()
{
	for (int i=0;i<16;i++)
	{
		m_RectBlocks[i].m_iGameNumShow = 0;
	}
	// 随机选两块数字重置为2
	static int iIDArry[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

	vector<int> vIds(16,0);
	std::copy(iIDArry,iIDArry+16,vIds.begin());
	vector<int>::iterator iter = vIds.begin();
	int iBlockID[2] = {0,0};
	iBlockID[0] = vIds[rand()%16];
	iter += iBlockID[0];
	vIds.erase(iter);
	iBlockID[1] = vIds[rand()%15];

	m_RectBlocks[iBlockID[0]].m_iGameNumShow = 2;
	m_RectBlocks[iBlockID[1]].m_iGameNumShow = 2;

	GetDlgItem(IDC_STATIC_ADD_SCORE)->SetWindowText("");

	m_GameCtrler.SetSumScore(0);
	GetDlgItem(IDC_STATIC_SUM_SCORE)->SetWindowText("当前总分:0");

}
void  CGame2048Dlg::DrawBlocks(CDC & dc)
{
	for (int i=0;i<16;i++)
	{
		DrawBlock(dc,i);
	}
}
void CGame2048Dlg::DrawBlock(CDC & dc,const int & iBlockID)
{
	CRect rectBlock = m_RectBlocks[iBlockID].m_RectBlock;
	int  iShowNum = m_RectBlocks[iBlockID].m_iGameNumShow;
	if (iShowNum  ==0)
	{
		CBrush brush(g_ColorNullBlock);
		dc.FillRect(rectBlock,&brush);
		return;
	}
	if (m_RectBlocks[iBlockID].m_iFlashCount > 0)
	{
		int iW = m_RectBlocks[iBlockID].m_iFlashCount*FLASH_INFALTE_PIXEL;
		int iH = m_RectBlocks[iBlockID].m_iFlashCount*FLASH_INFALTE_PIXEL;
		rectBlock.InflateRect(-iW,-iH,-iW,-iH);
	}
	int iTemp =iShowNum > 128 ? 128:iShowNum;
	CBrush brush(g_mapColor[iTemp]);
	dc.FillRect(rectBlock,&brush);

	CString strNum;
	strNum.Format("%d",iShowNum);
	if (iShowNum<=4)
		dc.SetTextColor(RGB(0,0,0));
	else
		dc.SetTextColor(RGB(255,255,255));

	int nBKModel = dc.SetBkMode(TRANSPARENT);

	LOGFONT lf;
	dc.GetCurrentFont()->GetLogFont(&lf);
	
	lf.lfCharSet = 134;
	lf.lfHeight = 50;
	lf.lfWidth  = 0;
	strcpy_s(lf.lfFaceName,"宋体");

	CFont font;
	font.CreateFontIndirect(&lf);
	CFont *pOldFont = dc.SelectObject(&font);

	CSize sizeText = dc.GetTextExtent(strNum);
	int iLeftX = rectBlock.CenterPoint().x - sizeText.cx / 2;
	int iLeftY = rectBlock.CenterPoint().y - sizeText.cy / 2;

	dc.TextOut(iLeftX,iLeftY,strNum);
	dc.SetBkMode(nBKModel);
	dc.SelectObject(pOldFont);
}

BOOL CGame2048Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg-> wParam == VK_LEFT|| pMsg-> wParam == 'A') 
			m_GameCtrler.MoveLeft();
		else if(pMsg-> wParam == VK_RIGHT|| pMsg-> wParam == 'D')
			m_GameCtrler.MoveRight();
		else if(pMsg-> wParam == VK_UP || pMsg-> wParam == 'W')
			m_GameCtrler.MoveUp();
		else if(pMsg-> wParam == VK_DOWN|| pMsg-> wParam == 'S')
			m_GameCtrler.MoveDown();
		else if (pMsg->wParam == VK_ESCAPE)
			TRUE;
		else
			return CBCGPDialog::PreTranslateMessage(pMsg);

		CString strAddScore;
		strAddScore.Format("+%d",m_GameCtrler.GetAddScore());
		GetDlgItem(IDC_STATIC_ADD_SCORE)->SetWindowText(strAddScore);

		CString strSumSocre;
		strSumSocre.Format("当前总分:%d",m_GameCtrler.GetSumScore());
		GetDlgItem(IDC_STATIC_SUM_SCORE)->SetWindowText(strSumSocre);
		return TRUE;
	}

	return CBCGPDialog::PreTranslateMessage(pMsg);
}


void CGame2048Dlg::OnDestroy()
{
	CBCGPDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	CGameRecord gameRecord;
	gameRecord.WriteGameRecord(m_RectBlocks,16,m_GameCtrler.GetSumScore());
}


void CGame2048Dlg::OnBnClickedBtnRestart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDNO == MessageBox("重新开始游戏？","提示",MB_YESNO|MB_ICONQUESTION))
		return;

	//mciSendString("stop bgMusic ",NULL,0,NULL);
	mciSendString("close bgMusic",NULL,0,NULL);
	//打开音乐文件
	mciSendString("open sound\\bgmusic.mp3 alias bgMusic ", NULL, 0, NULL);
	mciSendString("play bgMusic repeat", NULL, 0, NULL);
	IntilizeGame();
	Invalidate(FALSE);
}


void CGame2048Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent==TIMER_FLASH_BLOCK)
	{
		m_GameCtrler.UpdateFlashTimeCount();
		Invalidate(FALSE);
	}

	CBCGPDialog::OnTimer(nIDEvent);
}
