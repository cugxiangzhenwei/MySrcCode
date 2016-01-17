#include "stdafx.h"
#include "MineStructTreeDockBar.h"

IMPLEMENT_DYNCREATE(CStructTreeDockBar,CMineDockingControlBar)
CStructTreeDockBar::CStructTreeDockBar()
{

}
BEGIN_MESSAGE_MAP(CStructTreeDockBar, CMineDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CStructTreeDockBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMineDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_TreeDir.Create("目录树",WS_CHILD|WS_VISIBLE,CRect(0,0,500,1000),this,1000);
	return 0;
}


void CStructTreeDockBar::OnSize(UINT nType, int cx, int cy)
{
	CMineDockingControlBar::OnSize(nType, cx, cy);
	
	// TODO: 在此处添加消息处理程序代码
	m_TreeDir.SetWindowPos (NULL, 1, -1, cx - 2, cy 
		, SWP_NOACTIVATE | SWP_NOZORDER);
}
