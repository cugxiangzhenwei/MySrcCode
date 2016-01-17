#if !defined(AFX_REALVIDEODLG_H__BA30BAC2_FAEA_415A_9DE9_8B48033788E1__INCLUDED_)
#define AFX_REALVIDEODLG_H__BA30BAC2_FAEA_415A_9DE9_8B48033788E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealVideoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRealVideoDlg dialog
#include"SkinBtn.h"

class  CMeYeDlg;
class CRealVideoDlg : public CDialog
{
// Construction
public:
	CRealVideoDlg(CWnd* pParent = NULL);   // standard constructor

	void SetMainCall(CMeYeDlg *pWnd)
	{
		m_pMainDlg = pWnd;
	}
private:
	CMeYeDlg * m_pMainDlg;
// Dialog Data
	//{{AFX_DATA(CRealVideoDlg)
	enum { IDD = IDD_DIALOG_VIDEO_REAL };
	CSkinBtn	m_Btn_Video_Cap;
	CSkinBtn	m_Btn_Up_Down;
	CSkinBtn	m_Btn_Up;
	CSkinBtn	m_Btn_4Line;
	CSkinBtn	m_Btn_Left_Right;
	CSkinBtn	m_Btn_Right;
	CSkinBtn	m_Btn_Left;
	CSkinBtn	m_Btn_Down;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealVideoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRealVideoDlg)
	afx_msg void OnButtonReal4lineBtn();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonVideoCap();
	afx_msg void OnButtonRealUp();
	afx_msg void OnButtonRealLeft();
	afx_msg void OnButtonRealRight();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonLeftRight();
	afx_msg void OnButtonUpDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetBackImage(int index,CString strName);
    void DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc);
	void SetImageSize(CBitmap *pBitmap, CSize &sz);
	void SetImage(CBitmap &bitmap, CString strName);

	void OnMenuLine1();
	void OnMenuLine2();
	void OnMenuLine3();
	void OnMenuLine4();

private:
	CBitmap m_BitmapBack[3];
	CSize   m_szBitmapBack[3];


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALVIDEODLG_H__BA30BAC2_FAEA_415A_9DE9_8B48033788E1__INCLUDED_)
