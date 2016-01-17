// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "绘图程序.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_GRAPHTOOL, OnViewGraphtool)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRAPHTOOL, OnUpdateViewGraphtool)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MYOPENDLG, OnMyopendlg)
	ON_COMMAND(ID_SET_LINESTYLE, OnSetLinestyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,    //新增加的状态提示窗格
	ID_INDICATOR_TIME,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
     //////////////////////////
	 //自己创建的工具栏
	if (!m_newtoolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_newtoolbar.LoadToolBar(IDR_GRAPHTOOL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//结束创建
	//////////////////////////////////
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetWindowText("标准");
	m_newtoolbar.SetWindowText("绘图工具栏");    //设置工具栏标题
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
   ////////////////////////////
	//自己创建的工具栏的显示
	m_newtoolbar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_newtoolbar);
	///////////////
	//设定计时器
	SetTimer(1,1000,NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewGraphtool() 
{
	// TODO: Add your command handler code here
	
	if (m_newtoolbar.IsWindowVisible())
	{
		m_newtoolbar.ShowWindow(SW_HIDE);  //如果可见，就隐藏
	}
	else 
	{
		m_newtoolbar.ShowWindow(SW_SHOW);
	}
	DockControlBar(&m_newtoolbar);    //使工具栏以停靠方式显示
	RecalcLayout(TRUE);  //默认为true

//	ShowControlBar(&m_newtoolbar,!m_newtoolbar.IsWindowVisible(),FALSE);
}

void CMainFrame::OnUpdateViewGraphtool(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_newtoolbar.IsWindowVisible());
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	/////////////////////获取本地系统当前时间
	/*
	CTime time;
	time=CTime::GetCurrentTime();   //调用CTime静态成员函数获取系统时间
	CString str;
	str=time.Format("%H:%M:%S");
	m_wndStatusBar.SetPaneText( //在窗格中输出字符串
		m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME),str);
   */
	/////////////////////
	//获取格林威治时间
	SYSTEMTIME systemtime;
	::GetSystemTime(&systemtime); //获取格林威治时间
    CTime time(systemtime);
	CString str=time.Format("格林威治时间 %H:%M:%S");
	m_wndStatusBar.SetPaneText(
		m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME),str);
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	if (MessageBox("要退出程序吗?","提示",MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		CFrameWnd::OnClose();
	}
	else return;
}

void CMainFrame::OnMyopendlg() 
{
	// TODO: Add your command handler code here
	char szFilter[]="文本文件(*.txt)|*.txt|word文件(*.doc,*.docx)|*.doc;*.docx|可执行文件(*.exe)|*.exe||";
	CFileDialog filedlg(TRUE,"exe","测试.txt",OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,szFilter,this);
	filedlg.m_ofn.lpstrTitle="打开图像文件";        //m_ofn为一个结构体，提供对CFileDialog对象基本参数的设置
	filedlg.DoModal();
}

void CMainFrame::OnSetLinestyle() 
{
	// TODO: Add your command handler code here
	m_LineStyleDlg.Create(IDD_LINESTYLE,this);
	m_LineStyleDlg.ShowWindow(SW_SHOW);  //显示对话框
}
