#pragma once
#include "DlgResLib.h"
#include "MineDockingControlBar.h"

class CResLibDockBar:public CMineDockingControlBar
{
	DECLARE_DYNCREATE(CResLibDockBar)
public:
	CResLibDockBar();
	~CResLibDockBar();
protected:
	CDlgResLib * m_pDlgInner;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};