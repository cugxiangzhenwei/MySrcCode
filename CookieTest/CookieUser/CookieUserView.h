
// CookieUserView.h : CCookieUserView 类的接口
//

#pragma once


class CCookieUserView : public CHtmlView
{
protected: // 仅从序列化创建
	CCookieUserView();
	DECLARE_DYNCREATE(CCookieUserView)

// 特性
public:
	CCookieUserDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CCookieUserView();
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

#ifndef _DEBUG  // CookieUserView.cpp 中的调试版本
inline CCookieUserDoc* CCookieUserView::GetDocument() const
   { return reinterpret_cast<CCookieUserDoc*>(m_pDocument); }
#endif

