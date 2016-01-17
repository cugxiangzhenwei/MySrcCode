#if !defined(AFX_SHOWMODELDLG_H__AF0FA8A0_1658_45F6_8E6F_E0A609982E80__INCLUDED_)
#define AFX_SHOWMODELDLG_H__AF0FA8A0_1658_45F6_8E6F_E0A609982E80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "ColorDlg.h"
#include "GreyDlg.h"
#endif // _MSC_VER > 1000
// ShowModelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShowModelDlg dialog

class CShowModelDlg : public CDialog
{
	// Construction
public:
	CShowModelDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CShowModelDlg)
	enum { IDD = IDD_SHOW_MODEL };
	CTabCtrl	m_tab;
	CColorDlg m_colordlg;
	CGreyDlg  m_greydlg;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowModelDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShowModelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabShowModel(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWMODELDLG_H__AF0FA8A0_1658_45F6_8E6F_E0A609982E80__INCLUDED_)
