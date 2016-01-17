#if !defined(AFX_HXYQDLG_H__27C20844_D550_4400_AECA_E3E059B3EF26__INCLUDED_)
#define AFX_HXYQDLG_H__27C20844_D550_4400_AECA_E3E059B3EF26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HxyqDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHxyqDlg dialog

class CHxyqDlg : public CDialog
{
// Construction
public:
	CHxyqDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHxyqDlg)
	enum { IDD = IDD_DIALOG_YHHY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHxyqDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHxyqDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HXYQDLG_H__27C20844_D550_4400_AECA_E3E059B3EF26__INCLUDED_)
