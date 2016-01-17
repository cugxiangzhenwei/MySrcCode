#include "stdafx.h"
#include "MineResLibDockBar.h"

IMPLEMENT_DYNCREATE(CResLibDockBar,CMineDockingControlBar)
CResLibDockBar::CResLibDockBar()
{
	m_pDlgInner = NULL;
}
CResLibDockBar::~CResLibDockBar()
{
	if (m_pDlgInner)
	{
		if ( IsWindow( m_pDlgInner->GetSafeHwnd() ) )
			m_pDlgInner->DestroyWindow();

		delete m_pDlgInner;
		m_pDlgInner = NULL;
	}
}


BEGIN_MESSAGE_MAP(CResLibDockBar, CMineDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CResLibDockBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMineDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (NULL==m_pDlgInner)
		m_pDlgInner = new CDlgResLib(this);

	if (NULL==m_pDlgInner->GetSafeHwnd())
	{
		m_pDlgInner->Create(m_pDlgInner->IDD, this);
		m_pDlgInner->ShowWindow(SW_SHOW);
	}
	return 0;
}


void CResLibDockBar::OnSize(UINT nType, int cx, int cy)
{
	CMineDockingControlBar::OnSize(nType, cx, cy);

	if ( m_pDlgInner != NULL && IsWindow(m_pDlgInner->GetSafeHwnd()))
		m_pDlgInner->SetWindowPos (NULL, 1, -1, cx - 2, cy
		, SWP_NOACTIVATE | SWP_NOZORDER);
}
