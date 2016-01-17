// RsProcView.h : interface of the CRsProcView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSPROCVIEW_H__5705BCA5_3ED7_465C_B256_8EB46F316725__INCLUDED_)
#define AFX_RSPROCVIEW_H__5705BCA5_3ED7_465C_B256_8EB46F316725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RsProcDoc.h"
class CRsProcView : public CScrollView
{
protected: // create from serialization only
	CRsProcView();
	DECLARE_DYNCREATE(CRsProcView)

// Attributes
public:
	CRsProcDoc* GetDocument();
	BOOL Tempalte(BYTE *pNew1BandBlock[3],int width,int height,int TemX,int TemY,
		int *fpArry,float fcoef);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsProcView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	CPoint PointStart;
	CPoint PointEnd;
	CPoint PointPriews;
	bool IsMouseDown;
	bool Isprintscreen;
	int m_nStep;
   int start_x;
   int start_y;
   int show_width;
   int show_height;
   HCURSOR hcursor;

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRsProcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRsProcView)
	afx_msg void OnShowModel();
	afx_msg void OnHistEqualShow();
	afx_msg void OnSelfFitStrect();
	afx_msg void OnLaplasian();
	afx_msg void OnLaplasianOutline();
	afx_msg void OnSobelOutline();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPrewitt();
	afx_msg void OnFileSaveAs();
	afx_msg void OnPrintscreen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTransraster();
	afx_msg void OnMenucanny();
};

#ifndef _DEBUG  // debug version in RsProcView.cpp
inline CRsProcDoc* CRsProcView::GetDocument()
   { return (CRsProcDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSPROCVIEW_H__5705BCA5_3ED7_465C_B256_8EB46F316725__INCLUDED_)
