
// ChildView.h : CChildView 类的接口
//

#pragma once
#include "particle.h"
#include "scene.h"

// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:
	//保存客户区大小
	CRect m_client;   
	//雪花
	CParticle *m_snow;
	//场景
	CScene *m_scene;
	//缓冲DC
	CDC m_cacheDC;  
	//缓冲位图
	CBitmap m_cacheCBitmap;
// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

