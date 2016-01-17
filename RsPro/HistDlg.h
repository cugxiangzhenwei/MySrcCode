#if !defined(AFX_HISTDLG_H__56C9B65D_8CE6_41C9_B37B_25D079EFE6A1__INCLUDED_)
#define AFX_HISTDLG_H__56C9B65D_8CE6_41C9_B37B_25D079EFE6A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistDlg.h : header file
//
#include "RsProcDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CHistDlg dialog

class CHistDlg : public CDialog
{
// Construction
public:
	CHistDlg(CWnd* pParent = NULL);   // standard constructo
	int Select();
// Dialog Data
	//{{AFX_DATA(CHistDlg)
	enum { IDD = IDD_HIST_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
CRsProcDoc *pdoc;
int m_sel_band;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistDlg)
	afx_msg void OnPaint();
	afx_msg void OnRadioR();
	afx_msg void OnRadioG();
	afx_msg void OnRadioB();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTDLG_H__56C9B65D_8CE6_41C9_B37B_25D079EFE6A1__INCLUDED_)
