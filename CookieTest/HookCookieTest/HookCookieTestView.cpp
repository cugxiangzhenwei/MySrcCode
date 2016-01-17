
// HookCookieTestView.cpp : CHookCookieTestView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HookCookieTest.h"
#endif

#include "HookCookieTestDoc.h"
#include "HookCookieTestView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHookCookieTestView

IMPLEMENT_DYNCREATE(CHookCookieTestView, CHtmlView)

BEGIN_MESSAGE_MAP(CHookCookieTestView, CHtmlView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_COMMAND(ID_REFRESH, &CHookCookieTestView::OnRefresh)
END_MESSAGE_MAP()

// CHookCookieTestView 构造/析构

CHookCookieTestView::CHookCookieTestView()
{
	// TODO: 在此处添加构造代码

}

CHookCookieTestView::~CHookCookieTestView()
{
}

BOOL CHookCookieTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CHtmlView::PreCreateWindow(cs);
}

void CHookCookieTestView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	SetSilent(TRUE);
	//char *szURL = "http://192.168.50.165/portal/erm/portal/index";
	char *szURL = "http://www.jd.com";
	//char *szURL = "http://www.taobao.com/";
	//char *szURL = "about:blank";
	Navigate2(szURL,NULL,NULL);
}


// CHookCookieTestView 打印



// CHookCookieTestView 诊断

#ifdef _DEBUG
void CHookCookieTestView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHookCookieTestView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CHookCookieTestDoc* CHookCookieTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHookCookieTestDoc)));
	return (CHookCookieTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CHookCookieTestView 消息处理程序


void CHookCookieTestView::OnRefresh()
{
	// TODO: 在此添加命令处理程序代码
	Refresh();
}


void CHookCookieTestView::OnNavigateComplete2(LPCTSTR strURL)
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
