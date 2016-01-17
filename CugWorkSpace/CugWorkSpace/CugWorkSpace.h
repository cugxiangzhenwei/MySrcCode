// CugWorkSpace.h : main header file for the CugWorkSpace application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "DlgResLibTree.h"
#include "DlgResLibImage.h"
#include "MineStructTreeDockBar.h"
#include "MineResLibDockBar.h"
#include "SvgDlg.h"

// CCugWorkSpaceApp:
// See CugWorkSpace.cpp for the implementation of this class
//

class CCugWorkSpaceApp : public CWinApp,
							public CBCGPWorkspace
{
public:
	CCugWorkSpaceApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	ULONG_PTR m_gdiplusToken; 

	enum {
		SPLVF_COMPONENT_TREE = 1 //组件树
		, SPLVF_COMPONENT_LIST //组件列表
	};
	int m_nSpliterViewFlag;

	CStructTreeDockBar * m_pBarJianmoTree; //CChildFrame 中创建、删除
	CResLibDockBar * m_pBarJianmoComponentLib; //CChildFrame 中创建、删除
	CDlgResLibTree * m_pDlgResLibTree; //CMineSplitterView 中创建、删除
	CDlgResLibImage * m_pDlgResLibImage; //CMineSplitterView 中创建、删除
	CSvgDlg * m_pSvgDlg;
	CSvgDlg * m_pWorkHistViewDlg;
	CString m_strNewDocTitle;

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCugWorkSpaceApp theApp;