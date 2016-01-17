#pragma once


// CCxSplitterView 视图

class CMineSplitterView : public CView
{
	DECLARE_DYNCREATE(CMineSplitterView)

protected:
	CMineSplitterView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMineSplitterView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	int m_nSpliterViewFlag;
	HWND m_hwndInner;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


