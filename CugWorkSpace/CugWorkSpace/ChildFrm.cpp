// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "CugWorkSpace.h"

#include "ChildFrm.h"
#include "UserDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CBCGPMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CBCGPMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CBCGPMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~WS_SYSMENU;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CBCGPMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers

void CChildFrame::ActivateFrame(int nCmdShow)
{
	CBCGPMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);
}
void CChildFrame::CreateJianMoAndResLibBar()
{
	CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
	if (theApp.m_pBarJianmoTree == NULL)
	{
		theApp.m_pBarJianmoTree = new CStructTreeDockBar();
	}
	if (!IsWindow(theApp.m_pBarJianmoTree->GetSafeHwnd()))
	{
		if (!theApp.m_pBarJianmoTree->Create(
			_T("结构树")
			, this
			, CRect (0, 0, 330, 200)
			, TRUE // Has gripper
			, ID_BAR_JIANMO_TREE
			, WS_CHILD | WS_VISIBLE | CBRS_LEFT
			, CBRS_BCGP_REGULAR_TABS
			, CBRS_BCGP_RESIZE | CBRS_BCGP_AUTOHIDE
			))
		{
			return ; // fail to create
		}
	}
	
	if (theApp.m_pBarJianmoComponentLib == NULL)
	{
		theApp.m_pBarJianmoComponentLib = new CResLibDockBar();
	}
	if (!IsWindow(theApp.m_pBarJianmoComponentLib->GetSafeHwnd()))
	{
		if ( !theApp.m_pBarJianmoComponentLib->Create (
			_T("组件资源库")
			, this
			, CRect (0, 0, 200, 200)
			, TRUE // Has gripper
			, ID_BAR_RES_LIB
			, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM
			, CBRS_BCGP_REGULAR_TABS
			, CBRS_BCGP_RESIZE | CBRS_BCGP_AUTOHIDE
			) )
		{
			return;      // fail to create
		}
	}

	ShowControlBar (theApp.m_pBarJianmoTree
		, TRUE //!(theApp.m_pBarJianmoTree->GetStyle () & WS_VISIBLE)
		, FALSE
		, TRUE );
	ShowControlBar (theApp.m_pBarJianmoComponentLib
		, TRUE //!(theApp.m_pBarJianmoTree->GetStyle () & WS_VISIBLE)
		, FALSE
		, TRUE );

	theApp.m_pBarJianmoTree->EnableDocking(CBRS_ALIGN_ANY);
	theApp.m_pBarJianmoComponentLib->EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CreateJianMoAndResLibBar();
	ModifyStyle(DS_3DLOOK, 0);
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	return 0;
}


void CChildFrame::OnDestroy()
{
	CBCGPMDIChildWnd::OnDestroy();
	char szBuf[100] = {0};
	GetWindowText(szBuf,100);
	if (strcmp(szBuf,"建模环境")==0)
	{
		DestroyJianmoBar();
	}

	// TODO: 在此处添加消息处理程序代码
}
void CChildFrame::DestroyJianmoBar()
{
	if (theApp.m_pBarJianmoTree != NULL)
	{
		if ( IsWindow( theApp.m_pBarJianmoTree->GetSafeHwnd() ) )
			theApp.m_pBarJianmoTree->DestroyWindow();
		delete theApp.m_pBarJianmoTree;
		theApp.m_pBarJianmoTree = NULL;
	}
	if (theApp.m_pBarJianmoComponentLib != NULL)
	{
		if ( IsWindow( theApp.m_pBarJianmoComponentLib->GetSafeHwnd() ) )
			theApp.m_pBarJianmoComponentLib->DestroyWindow();
		delete theApp.m_pBarJianmoComponentLib;
		theApp.m_pBarJianmoComponentLib = NULL;
	}
}

void CChildFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strTitile;
	GetWindowText(strTitile);
	if (strTitile == "建模环境")
	{
		if (theApp.m_pSvgDlg && IsWindow(theApp.m_pSvgDlg->GetSafeHwnd()))
		{
			this->ActivateFrame(SW_SHOW);
			theApp.m_pSvgDlg->ShowWindow(SW_SHOW);
			theApp.m_pSvgDlg->OpenDefaultPage();
		}
		return; // 禁止关闭建模环境
	}

	CBCGPMDIChildWnd::OnClose();
}
