
// PdfView.h : CWordPdfViewDemoView 类的接口
//

#pragma once
/************************************************************************/
/* Word 文档视图类                                                                     */
/************************************************************************/
#include "CAcroAXDocShim.h"
class CPdfView : public CView
{
protected: // 仅从序列化创建
	CPdfView();
	DECLARE_DYNCREATE(CPdfView)

// 特性
public:
	CPdfDoc* GetDocument() const;

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
	virtual ~CPdfView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CAcroAXDocShim m_drvPDF;
	CWnd           m_wndPDF;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnOpenPdfDoc();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // WordPdfViewDemoView.cpp 中的调试版本
inline CPdfDoc* CPdfView::GetDocument() const
   { return reinterpret_cast<CPdfDoc*>(m_pDocument); }
#endif

