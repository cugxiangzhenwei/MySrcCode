#if !defined(AFX_GREYGLG_H__9FCF8C04_324E_4D37_AC7B_3D11D8B2F1F5__INCLUDED_)
#define AFX_GREYGLG_H__9FCF8C04_324E_4D37_AC7B_3D11D8B2F1F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GreyGlg.h : header file
//
#include "RsProcDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CGreyDlg dialog

class CGreyDlg : public CDialog
{
// Construction
public:
	CGreyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGreyDlg)
	enum { IDD = IDD_DIALOG_GREY };
	CComboBox	m_combo_grey;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGreyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGreyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboGrey();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		CRsProcDoc *pdoc;
	int m_sel_band;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GREYGLG_H__9FCF8C04_324E_4D37_AC7B_3D11D8B2F1F5__INCLUDED_)
