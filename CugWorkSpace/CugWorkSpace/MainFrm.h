// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "MineToolBar.h"
#include"MineBCGPMenuBar.h"
class CMainFrame : public CBCGPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CBCGPStatusBar			m_wndStatusBar;
	CMineBCGPMenuBar		m_wndMenuBar;
	CMimeToolBar            m_MineToolbar;

	CBCGPToolBarImages	m_UserImages;

	void OpenJianMoHuanJing();
	void CreateMineToolBar();
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	void OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	afx_msg void OnWindowManager();
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	afx_msg void OnMdiMoveToNextGroup();
	afx_msg void OnMdiMoveToPrevGroup();
	afx_msg void OnMdiNewHorzTabGroup();
	afx_msg void OnMdiNewVertGroup();
	afx_msg void OnMdiCancel();
	afx_msg void OnClose();
	// 以下是非向导创建的消息
public:
	afx_msg void OnNewShejiGuoCheng();  // 新建设计过程
protected:
	afx_msg void OnOpenShejiGuoCheng(); // 打开设计过程
	afx_msg void OnMineToolbarClicked(UINT iID); // 工具栏按钮点击响应事件
	afx_msg void OnViewMineToolbar(); // 切换工具栏是否显示
	afx_msg void OnUpdateViewMineToolbar(CCmdUI *pCmdUI); // 切换工具栏显示时更新菜单前显示的对钩
	afx_msg void OnToolHistoryMonitor(); // 打开作业管理页面
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop);
	virtual CBCGPMDIChildWnd* CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* /*pObj*/);
	UINT	m_nAppLook;
};


