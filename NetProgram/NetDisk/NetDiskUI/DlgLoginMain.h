#pragma once
#include "afxwin.h"
#include "PageLogin.h"
#include "PageRegister.h"

// CDlgLoginMain 对话框

class CDlgLoginMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLoginMain)

public:
	CDlgLoginMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLoginMain();

// 对话框数据
	enum { IDD = IDD_DLG_LOGIN_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_wndPosPage;
	CStatic m_wndPosImg;
	CPageLogin m_pageLogin;
	CPageRegister m_pageRegister;


	CButton m_btnChangePage;
	CString m_strUsrID;
	CString m_strUsrName;
	CString m_strPasswd;
	long long m_iAllSpace;
	long long m_iUsedSpace;
private:
	CBitmap m_BitMapBkImg;
	BOOL m_bCurPageIsLogin;
	void DrawBkImg(CDC *pDC,CRect rect);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnLoginChange();
	afx_msg LRESULT OnMsgLoginSuccess(WPARAM,LPARAM);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};
