
// AnimateLineDrawView.h : CAnimateLineDrawView 类的接口
//


#pragma once


class CAnimateLineDrawView : public CView
{
protected: // 仅从序列化创建
	CAnimateLineDrawView();
	DECLARE_DYNCREATE(CAnimateLineDrawView)

// 属性
public:
	CAnimateLineDrawDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CAnimateLineDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	int m_offsetx;
	CDC m_dc;
	CBitmap m_bitmap;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // AnimateLineDrawView.cpp 中的调试版本
inline CAnimateLineDrawDoc* CAnimateLineDrawView::GetDocument() const
   { return reinterpret_cast<CAnimateLineDrawDoc*>(m_pDocument); }
#endif

