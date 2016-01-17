
// WordPdfViewDemoView.h : CWordPdfViewDemoView 类的接口
//

#pragma once
/************************************************************************/
/* Word 文档视图类                                                                     */
/************************************************************************/
#include "CWebBrowser2.h"
class CWordView : public CView
{
protected: // 仅从序列化创建
	CWordView();
	DECLARE_DYNCREATE(CWordView)

// 特性
public:
	CWordDoc* GetDocument() const;
	void OpenDocFile(CString strFile);
	BOOL IsWordFile(const CString &strFile);
	BOOL ConvertWordToHtml(CString strWordFile,CString strHtml);
	void RemoveTempHtmlFile();

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
	virtual ~CWordView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CWebBrowser2 m_web;
	CString m_strLastTmpFile;
// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOpenWordDoc();
};

#ifndef _DEBUG  // WordPdfViewDemoView.cpp 中的调试版本
inline CWordDoc* CWordView::GetDocument() const
   { return reinterpret_cast<CWordDoc*>(m_pDocument); }
#endif

