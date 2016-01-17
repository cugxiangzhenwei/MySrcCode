
// WordPdfViewDemoView.cpp : CWordPdfViewDemoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "WordPdfViewDemo.h"
#endif

#include "PdfDoc.h"
#include "PdfView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWordPdfViewDemoView

IMPLEMENT_DYNCREATE(CPdfView, CView)

BEGIN_MESSAGE_MAP(CPdfView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPdfView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPEN_PDF, &CPdfView::OnOpenPdfDoc)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CWordPdfViewDemoView 构造/析构

CPdfView::CPdfView()
{
	// TODO: 在此处添加构造代码

}

CPdfView::~CPdfView()
{
}

BOOL CPdfView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CWordPdfViewDemoView 绘制

void CPdfView::OnDraw(CDC* /*pDC*/)
{
	CPdfDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (m_wndPDF.m_hWnd)
		m_wndPDF.RedrawWindow();
}


// CWordPdfViewDemoView 打印


void CPdfView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPdfView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPdfView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPdfView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CPdfView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPdfView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWordPdfViewDemoView 诊断

#ifdef _DEBUG
void CPdfView::AssertValid() const
{
	CView::AssertValid();
}

void CPdfView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPdfDoc* CPdfView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPdfDoc)));
	return (CPdfDoc*)m_pDocument;
}
#endif //_DEBUG


// CWordPdfViewDemoView 消息处理程序


void CPdfView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	if (m_wndPDF.m_hWnd==NULL)
	{
		CRect rt;
		GetClientRect(&rt);
		// 利用CWnd::CreateControl创建PDF控件窗口
		// Acrobat (Reader) 5.x/6.x的控件类ID为"PDF.PdfCtrl.5"，7.x为"AcroPDF.PDF.1"
		m_wndPDF.CreateControl("AcroPDF.PDF.1", NULL, WS_CHILD | WS_VISIBLE, rt, this, 0);

		// 获取IDispatch接口指针
		LPUNKNOWN lpUnknown = m_wndPDF.GetControlUnknown();
		LPDISPATCH lpDispatch;
		lpUnknown->QueryInterface(IID_IDispatch, (void**)&lpDispatch);
		// 把获得的接口指针传递给组件驱动对象m_drvPDF，通过它调用控件的属性和方法
		// 当m_drvPDF销毁时，会自动释放接口指针
		m_drvPDF.AttachDispatch(lpDispatch,TRUE);
	}
	CPdfDoc* pDoc = GetDocument();
	CString strPdfFile = pDoc->GetPathName();
	if (!strPdfFile.IsEmpty())
	{
		m_drvPDF.LoadFile(strPdfFile);
		//m_drvPDF.setZoom(0.5);
	}
}


void CPdfView::OnOpenPdfDoc()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE,".pdf",NULL,6UL,"PDF(*.pdf)|*.pdf||",this);
	if (dlg.DoModal()!=IDOK)
		return;

	CString strFile = dlg.GetPathName();
	m_drvPDF.LoadFile(strFile);
}


void CPdfView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_wndPDF.m_hWnd)
	{
		CRect rect;
		GetClientRect(&rect);
		m_wndPDF.SetWindowPos(&wndTop,rect.left,rect.top,rect.Width(),rect.Height(),NULL);
		m_wndPDF.RedrawWindow();
	}
}
