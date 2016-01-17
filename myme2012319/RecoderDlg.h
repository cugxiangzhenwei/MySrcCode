#if !defined(AFX_RECODERDLG_H__B0FB8928_3EC1_44F2_92C0_20512488CA91__INCLUDED_)
#define AFX_RECODERDLG_H__B0FB8928_3EC1_44F2_92C0_20512488CA91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecoderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecoderDlg dialog

#include"SkinBtn.h"
class CMeYeDlg;
class CRecoderDlg : public CDialog
{
// Construction
public:
	CRecoderDlg(CWnd* pParent = NULL);   // standard constructor
	void SetMainCall(CMeYeDlg *pWnd)
	{
		m_pMainDlg = pWnd;
	}
private:
	CMeYeDlg * m_pMainDlg;
// Dialog Data
	//{{AFX_DATA(CRecoderDlg)
	enum { IDD = IDD_DIALOG_RECODER };
	CDateTimeCtrl	m_DataStar;
	CSkinBtn	m_BtnSerchVideo;
	CTime	m_DataEnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecoderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecoderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnButtonSerchVideo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECODERDLG_H__B0FB8928_3EC1_44F2_92C0_20512488CA91__INCLUDED_)
