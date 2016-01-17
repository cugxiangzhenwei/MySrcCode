// CugWorkSpaceView.h : interface of the CCugWorkSpaceView class
//


#pragma once


class CCugWorkSpaceView : public CView
{
protected: // create from serialization only
	CCugWorkSpaceView();
	DECLARE_DYNCREATE(CCugWorkSpaceView)

// Attributes
public:
	CCugWorkSpaceDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCugWorkSpaceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in CugWorkSpaceView.cpp
inline CCugWorkSpaceDoc* CCugWorkSpaceView::GetDocument() const
   { return reinterpret_cast<CCugWorkSpaceDoc*>(m_pDocument); }
#endif

