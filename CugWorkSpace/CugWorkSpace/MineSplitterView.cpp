// CxSplitterView.cpp : 实现文件
//

#include "stdafx.h"
#include "CugWorkSpace.h"
#include "MineSplitterView.h"
#include "CDDirTreeActiveX0.h"

const int nHorzMargin = 2;
const int nVertMargin = 2;
// CCxSplitterView

IMPLEMENT_DYNCREATE(CMineSplitterView, CView)

CMineSplitterView::CMineSplitterView()
{
	m_nSpliterViewFlag = -1;
}

CMineSplitterView::~CMineSplitterView()
{
}

BEGIN_MESSAGE_MAP(CMineSplitterView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCxSplitterView 绘图

void CMineSplitterView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	//CRect rectClient;
	//GetClientRect (rectClient);

}

// CCxSplitterView 诊断

#ifdef _DEBUG
void CMineSplitterView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMineSplitterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCxSplitterView 消息处理程序


BOOL CMineSplitterView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


int CMineSplitterView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_nSpliterViewFlag = theApp.m_nSpliterViewFlag;
	switch (theApp.m_nSpliterViewFlag)
	{
	case theApp.SPLVF_COMPONENT_TREE:
		if (theApp.m_pDlgResLibTree == NULL)
		{
			theApp.m_pDlgResLibTree = new CDlgResLibTree(this);
			theApp.m_pDlgResLibTree->Create(theApp.m_pDlgResLibTree->IDD, this);
			theApp.m_pDlgResLibTree->ShowWindow(SW_SHOW);
		}
		break;
	case theApp.SPLVF_COMPONENT_LIST:
		if (theApp.m_pDlgResLibImage == NULL)
		{
			theApp.m_pDlgResLibImage = new CDlgResLibImage(this);
			theApp.m_pDlgResLibImage->Create(theApp.m_pDlgResLibImage->IDD, this);
			theApp.m_pDlgResLibImage->ShowWindow(SW_SHOW);
		}
		break;

	default:
		break;
	}

	return 0;
}


void CMineSplitterView::OnDestroy()
{
	switch (m_nSpliterViewFlag)
	{
	case theApp.SPLVF_COMPONENT_TREE:
		if (theApp.m_pDlgResLibTree != NULL)
		{
			theApp.m_pDlgResLibTree->DestroyWindow();
			delete theApp.m_pDlgResLibTree;
			theApp.m_pDlgResLibTree = NULL;
		}
		break;
	case theApp.SPLVF_COMPONENT_LIST:
		if (theApp.m_pDlgResLibImage != NULL)
		{
			theApp.m_pDlgResLibImage->DestroyWindow();
			delete theApp.m_pDlgResLibImage;
			theApp.m_pDlgResLibImage = NULL;
		}
		break;

	default:
		break;
	}
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


void CMineSplitterView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CRect rcClient(0,0,cx,cy);

	switch (m_nSpliterViewFlag)
	{
	case theApp.SPLVF_COMPONENT_TREE:
		if ( theApp.m_pDlgResLibTree != NULL 
			&& IsWindow(theApp.m_pDlgResLibTree->GetSafeHwnd()) )
		{
			//rcClient.DeflateRect(0,0,1,0);
			theApp.m_pDlgResLibTree->MoveWindow(&rcClient);
		}
		break;
	case theApp.SPLVF_COMPONENT_LIST:
		if (theApp.m_pDlgResLibImage != NULL 
			&& IsWindow(theApp.m_pDlgResLibImage->GetSafeHwnd()) )
		{
			//rcClient.DeflateRect(0,0,1,0);
			theApp.m_pDlgResLibImage->MoveWindow(&rcClient);
		}
		break;

	default:
		break;
	}
}
