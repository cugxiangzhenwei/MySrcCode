// 绘图程序View.h : interface of the CMyView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEW_H__78C22989_0591_4FC4_88DE_5A1B4091A6C1__INCLUDED_)
#define AFX_VIEW_H__78C22989_0591_4FC4_88DE_5A1B4091A6C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyView : public CView
{
protected: // create from serialization only
	CMyView();
	DECLARE_DYNCREATE(CMyView)

// Attributes
public:
	CMyDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF m_color;
	int m_endy;
	int m_endx;
	int m_nStep;
	int m_starty;
	int m_startx;
	BOOL m_isLine;
	BOOL m_isPoint;
	int m_nLineWidth;  //记录线宽
	int m_nLineStyle;  //记录线型
	virtual ~CMyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CMyView)
	afx_msg void OnGraphPoint();
	afx_msg void OnGraphLine();
	afx_msg void OnUpdateGraphPoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGraphLine(CCmdUI* pCmdUI);
	afx_msg void OnDaraRound();
	afx_msg void OnDrawEllipse();
	afx_msg void OnDrawPolygon();
	afx_msg void OnDrawPolyline();
	afx_msg void OnDrawRectangle();
	afx_msg void OnBitmapShow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetLinewidth();
	afx_msg void OnSetColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in 绘图程序View.cpp
inline CMyDoc* CMyView::GetDocument()
   { return (CMyDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW_H__78C22989_0591_4FC4_88DE_5A1B4091A6C1__INCLUDED_)
