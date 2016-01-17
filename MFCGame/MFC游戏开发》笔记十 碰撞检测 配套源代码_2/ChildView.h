
// ChildView.h : CChildView 类的接口
//


#pragma once

#define SNOW_NUMBER 100  //雪花例子的数量
// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:
	//人物结构体
	struct charcter
	{
		CImage character;     //保存人物的图像
		int x;             //保存人物的位置
		int y;
		int direct;        //人物的方向
		int frame;         //运动到第几张图片
		int width;          //图片的宽度和高度，用于碰撞判定
		int height;
	}MyHero;
	
	CRect m_client;    //保存客户区大小
	CImage m_bg;      //背景图片
	CImage m_bgblack; //背景蒙版图

	CDC m_cacheDC;   //缓冲DC
	CBitmap m_cacheCBitmap;//缓冲位图
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
	bool CanPass();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

