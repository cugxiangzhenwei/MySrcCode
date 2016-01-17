#pragma once
#include "CompressionDlg.h"
#include "DCompressionDlg.h"
#include "afxcmn.h"
#include "MyTabCtrl.h"
// CMainDlg 对话框

class CCLientUIMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCLientUIMainDlg)

public:
	CCLientUIMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCLientUIMainDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CCompressionDlgPage m_ComDlg;
	CDCompressionDlgPage m_DComDlg;
	CTabCtrl m_TabCtrl;
	afx_msg void OnTcnSelchangeTabPage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
