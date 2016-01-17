
// HookCookieTestView.h : CHookCookieTestView 类的接口
//

#pragma once


class CHookCookieTestView : public CHtmlView
{
protected: // 仅从序列化创建
	CHookCookieTestView();
	DECLARE_DYNCREATE(CHookCookieTestView)

// 特性
public:
	CHookCookieTestDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CHookCookieTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRefresh();
	virtual void OnNavigateComplete2(LPCTSTR strURL);
};

#ifndef _DEBUG  // HookCookieTestView.cpp 中的调试版本
inline CHookCookieTestDoc* CHookCookieTestView::GetDocument() const
   { return reinterpret_cast<CHookCookieTestDoc*>(m_pDocument); }
#endif

