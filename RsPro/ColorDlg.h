#if !defined(AFX_COLORDLG_H__F094C4E6_16C2_4669_B06D_9850D0155C4A__INCLUDED_)
#define AFX_COLORDLG_H__F094C4E6_16C2_4669_B06D_9850D0155C4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorDlg.h : header file
//
#include "RsProcDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

class CColorDlg : public CDialog
{
// Construction
public:
	CColorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColorDlg)
	enum { IDD = IDD_DIALOG_COLOR };
	CComboBox	m_combo_r;
	CComboBox	m_combo_g;
	CComboBox	m_combo_b;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboR();
	afx_msg void OnSelchangeComboG();
	afx_msg void OnSelchangeComboB();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		CRsProcDoc *pdoc;
	int width;
	int height;
	int bandnum;
	int m_sel_r;
	int m_sel_g;
	int m_sel_b;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORDLG_H__F094C4E6_16C2_4669_B06D_9850D0155C4A__INCLUDED_)
