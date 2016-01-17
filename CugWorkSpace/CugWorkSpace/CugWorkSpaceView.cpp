// CugWorkSpaceView.cpp : implementation of the CCugWorkSpaceView class
//

#include "stdafx.h"
#include "CugWorkSpace.h"
#include "UserDefine.h"
#include "CugWorkSpaceDoc.h"
#include "CugWorkSpaceView.h"
#include "SvgDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCugWorkSpaceView

IMPLEMENT_DYNCREATE(CCugWorkSpaceView, CView)

BEGIN_MESSAGE_MAP(CCugWorkSpaceView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CCugWorkSpaceView construction/destruction

CCugWorkSpaceView::CCugWorkSpaceView()
{
	// TODO: add construction code here

}

CCugWorkSpaceView::~CCugWorkSpaceView()
{
}

BOOL CCugWorkSpaceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

// CCugWorkSpaceView drawing

void CCugWorkSpaceView::OnDraw(CDC* /*pDC*/)
{
	CCugWorkSpaceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CCugWorkSpaceView printing

void CCugWorkSpaceView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CCugWorkSpaceView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCugWorkSpaceView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCugWorkSpaceView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CCugWorkSpaceView diagnostics

#ifdef _DEBUG
void CCugWorkSpaceView::AssertValid() const
{
	CView::AssertValid();
}

void CCugWorkSpaceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCugWorkSpaceDoc* CCugWorkSpaceView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCugWorkSpaceDoc)));
	return (CCugWorkSpaceDoc*)m_pDocument;
}
#endif //_DEBUG


// CCugWorkSpaceView message handlers


int CCugWorkSpaceView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (NULL == theApp.m_pSvgDlg)
		theApp.m_pSvgDlg = new CSvgDlg();

	if (theApp.m_pSvgDlg && !IsWindow(theApp.m_pSvgDlg->GetSafeHwnd()))
	{
		theApp.m_pSvgDlg->Create(theApp.m_pSvgDlg->IDD, this);
		theApp.m_pSvgDlg->ModifyStyle(DS_3DLOOK, 0);
		theApp.m_pSvgDlg->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
		return 0;
	}
	if (NULL == theApp.m_pWorkHistViewDlg)
	{
		theApp.m_pWorkHistViewDlg  = new CSvgDlg;
	}

	if (theApp.m_pWorkHistViewDlg && !IsWindow(theApp.m_pWorkHistViewDlg->GetSafeHwnd()))
	{
		theApp.m_pWorkHistViewDlg->Create(theApp.m_pWorkHistViewDlg->IDD, this);
		theApp.m_pWorkHistViewDlg->ModifyStyle(DS_3DLOOK, 0);
		theApp.m_pWorkHistViewDlg->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	}
	
	return 0;
}


void CCugWorkSpaceView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CString strTitle = GetDocument()->GetTitle();
	if (strTitle == g_strTitleJianMo)
	{
		if (theApp.m_pSvgDlg && IsWindow(theApp.m_pSvgDlg->GetSafeHwnd()))
		{
			theApp.m_pSvgDlg->MoveWindow(0,0,cx-1,cy-1);
			theApp.m_pSvgDlg->ShowWindow(SW_SHOW);
		}
	}
	else if (strTitle == g_strTitleWorkHistory)
	{
		if (theApp.m_pWorkHistViewDlg && IsWindow(theApp.m_pWorkHistViewDlg->GetSafeHwnd()))
		{
			theApp.m_pWorkHistViewDlg->MoveWindow(0,0,cx-1,cy-1);
			theApp.m_pWorkHistViewDlg->ShowWindow(SW_SHOW);
		}
	}
}
