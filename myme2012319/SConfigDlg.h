#if !defined(AFX_SCONFIGDLG_H__012CEA50_4B2C_4461_B177_5E3C1BC7FB36__INCLUDED_)
#define AFX_SCONFIGDLG_H__012CEA50_4B2C_4461_B177_5E3C1BC7FB36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SConfigDlg.h : header file
//
#include "MyStatic.h"
#include "SkinBtn.h"
/////////////////////////////////////////////////////////////////////////////
// CSConfigDlg dialog

class CSConfigDlg : public CDialog
{
// Construction
public:
	CSConfigDlg(CWnd* pParent = NULL);   // standard constructor

	void initDlg(void);
// Dialog Data
	//{{AFX_DATA(CSConfigDlg)
	enum { IDD = IDD_DIALOG_S_CONF_DLG };
	CButton	m_BtnCheck16;
	CButton	m_BtnCheck17;
	CButton	m_BtnCheck15;
	CButton	m_BtnCheck14;
	CButton	m_BtnCheck13;
	CButton	m_BtnCheck12;
	CButton	m_BtnCheck11;
	CButton	m_BtnCheck10;
	CSkinBtn	m_BtnBindwb;
	CSkinBtn	m_BtnSSave;
	CSkinBtn	m_BtnSCancle;
	CButton	m_Check2Btn;
	CButton	m_Check1Btn;
	CMyStatic	m_Static30;
	CMyStatic	m_Static31;
	CMyStatic	m_Static32;
	CMyStatic	m_Static33;
	CMyStatic	m_Static34;
	CMyStatic	m_Static35;
	CMyStatic	m_Static36;
	CMyStatic	m_Static37;
	CMyStatic	m_Static38;
	CMyStatic	m_Static39;
	CMyStatic	m_Static40;
	CMyStatic	m_Static41;
	CMyStatic	m_Static42;
	CMyStatic	m_Static43;
	CMyStatic	m_Static44;

	CMyStatic	m_Static45;
	CMyStatic	m_Static46;
	CMyStatic	m_Static47;
	CMyStatic	m_Static48;
	CMyStatic	m_Static49;
	CMyStatic	m_Static50;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSConfigDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSSave();
	afx_msg void OnButtonBind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

int m_wbbindflag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCONFIGDLG_H__012CEA50_4B2C_4461_B177_5E3C1BC7FB36__INCLUDED_)
