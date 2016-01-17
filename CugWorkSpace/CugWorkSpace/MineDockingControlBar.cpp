#include "stdafx.h"
#include "MineDockingControlBar.h"

IMPLEMENT_DYNCREATE(CMineDockingControlBar,CBCGPDockingControlBar)

CMineDockingControlBar::CMineDockingControlBar()
{

}
BEGIN_MESSAGE_MAP(CMineDockingControlBar, CBCGPDockingControlBar)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CMineDockingControlBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}
