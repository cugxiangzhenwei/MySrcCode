#if !defined(AFX_CLIENTCONFIGDLG_H__658BCD12_3EAE_4E5A_8D1A_952FBC3158FE__INCLUDED_)
#define AFX_CLIENTCONFIGDLG_H__658BCD12_3EAE_4E5A_8D1A_952FBC3158FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClientConfigDlg dialog
#include "SkinTab.h"
#include "MyStatic.h"
#include "SkinBtn.h"

class CClientConfigDlg : public CDialog
{
// Construction
public:
	CClientConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CClientConfigDlg)
	enum { IDD = IDD_DIALOG_C_CONF_DLG };
	CSkinBtn	m_CBtnSave;
	CSkinBtn	m_CBtnCancle;
	CMyStatic	m_StaticC23;
	CMyStatic	m_StaticC22;
	CMyStatic	m_StaticC21;
	CEdit	m_EditC23;
	CEdit	m_EditC22;
	CEdit	m_EditC21;
	CMyStatic	m_StaticC31;
	CMyStatic	m_StaticC3;
	CMyStatic	m_StaticC2;
	CMyStatic	m_StaticC1;
	CEdit	m_EditC31;
	CEdit	m_EditC3;
	CEdit	m_EditC2;
	CEdit	m_EditC1;
	CSkinTab	m_TabConfig;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CClientConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnButtonCSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void SetDevIndex(long nIndex);

private:
	afx_msg LRESULT OnTabChange(WPARAM wParam, LPARAM lParam);

private:
	CRect m_rcHeadTab[3];
	long  m_CurrentTabSel;

	long   m_CurrentDev;  //当前设置名称
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTCONFIGDLG_H__658BCD12_3EAE_4E5A_8D1A_952FBC3158FE__INCLUDED_)
