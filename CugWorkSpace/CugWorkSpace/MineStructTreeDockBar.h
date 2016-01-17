#pragma once

#include "MineDockingControlBar.h"
#include"CDDirTreeActiveX0.h"
class CStructTreeDockBar:public CMineDockingControlBar
{
	DECLARE_DYNCREATE(CStructTreeDockBar)
public:
	CStructTreeDockBar();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	CDDirTreeActiveX0  m_TreeDir;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};