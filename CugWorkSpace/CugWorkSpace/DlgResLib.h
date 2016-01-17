#pragma once
#include "resource.h"
#include <BCGPSplitterWnd.h>
#include "MineBCGPSplitterWnd.h"
typedef CFrameWnd  CSlplitFrameWnd;
// CDlgResLib 对话框

class CDlgResLib : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgResLib)

public:
	CDlgResLib(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgResLib();

// 对话框数据
	enum { IDD = IDD_COMPONENT_LIB };

private:
	CSlplitFrameWnd * m_pFrameWnd;
	CMineBCGPSplitterWnd m_wndSplitter;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
};
