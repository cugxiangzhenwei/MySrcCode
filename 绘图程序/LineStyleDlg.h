#if !defined(AFX_LINESTYLEDLG_H__2BF0208C_8A36_4182_AA0C_777B38160578__INCLUDED_)
#define AFX_LINESTYLEDLG_H__2BF0208C_8A36_4182_AA0C_777B38160578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineStyleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineStyleDlg dialog

class CLineStyleDlg : public CDialog
{
// Construction
public:
	CLineStyleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineStyleDlg)
	enum { IDD = IDD_LINESTYLE };
	CComboBox	m_ComboBox;
	int		m_nLineStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineStyleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLineStyleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLinestyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINESTYLEDLG_H__2BF0208C_8A36_4182_AA0C_777B38160578__INCLUDED_)
