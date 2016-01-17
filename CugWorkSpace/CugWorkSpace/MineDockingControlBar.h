#pragma once
#include "BCGPDockingControlBar.h"
class CMineDockingControlBar:public CBCGPDockingControlBar
{
	DECLARE_DYNCREATE(CMineDockingControlBar)
public:
	CMineDockingControlBar();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
};