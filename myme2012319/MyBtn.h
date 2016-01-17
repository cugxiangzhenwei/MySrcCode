#if !defined(AFX_MYBTN_H__4433A212_3772_4FFD_977A_5CE278EDD02D__INCLUDED_)
#define AFX_MYBTN_H__4433A212_3772_4FFD_977A_5CE278EDD02D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyBtn window

class CMyBtn : public CStatic
{
// Construction
public:
	CMyBtn();

	void SetBtnText(CString str);
//?	void CreateMsgWindow(HWND hWnd,RECT rc,long nDlgID)  ;     // 创建窗口
// Attributes
public:

// Operations
	void SetBackImage(int index,CString strName);
    
	void SetBtnFlag(BOOL bFlag);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBtn)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyBtn)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	LPARAM  OnMouseLeave(WPARAM wp, LPARAM lp);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_BitmapBack[2];
	CSize   m_szBitmapBack[2];

	BOOL bTrackLeave;
	BOOL m_btnMove;

	CString m_btnText;
private:
	
	void DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc);
	void SetImageSize(CBitmap *pBitmap, CSize &sz);
    //载入硬盘文件位图
    void SetImage(CBitmap &bitmap, CString strName);

	// 设置透明度
	void SetLayeredWindowTrans(int nTrans);
	void ChangeWindowRgn(CDC *pDC);
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBTN_H__4433A212_3772_4FFD_977A_5CE278EDD02D__INCLUDED_)
