
// CookieUserView.cpp : CCookieUserView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CookieUser.h"
#endif

#include "CookieUserDoc.h"
#include "CookieUserView.h"

#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCookieUserView

IMPLEMENT_DYNCREATE(CCookieUserView, CHtmlView)

BEGIN_MESSAGE_MAP(CCookieUserView, CHtmlView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_COMMAND(ID_REFRESH, &CCookieUserView::OnRefresh)
END_MESSAGE_MAP()

// CCookieUserView 构造/析构

CCookieUserView::CCookieUserView()
{
	// TODO: 在此处添加构造代码

}

CCookieUserView::~CCookieUserView()
{
}

BOOL CCookieUserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CHtmlView::PreCreateWindow(cs);
}

void CCookieUserView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	SetSilent(TRUE);
	Navigate2(_T("http://www.jd.com/"),NULL,NULL);
}


// CCookieUserView 打印



// CCookieUserView 诊断

#ifdef _DEBUG
void CCookieUserView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CCookieUserView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CCookieUserDoc* CCookieUserView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCookieUserDoc)));
	return (CCookieUserDoc*)m_pDocument;
}
#endif //_DEBUG


// CCookieUserView 消息处理程序


void CCookieUserView::OnRefresh()
{
	// TODO: 在此添加命令处理程序代码
	Refresh();
}


void CCookieUserView::OnNavigateComplete2(LPCTSTR strURL)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (strcmp(strURL, "about:blank"))
	{
		CMainFrame *pFrameWnd = dynamic_cast<CMainFrame*>(GetParentFrame());
		if (pFrameWnd)
			pFrameWnd->m_editUrlAddress.SetWindowText(strURL);
	}

	CHtmlView::OnNavigateComplete2(strURL);
}
