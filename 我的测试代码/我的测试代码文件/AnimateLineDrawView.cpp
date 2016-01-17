
// AnimateLineDrawView.cpp : CAnimateLineDrawView 类的实现
//

#include "stdafx.h"
#include "AnimateLineDraw.h"

#include "AnimateLineDrawDoc.h"
#include "AnimateLineDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimateLineDrawView

IMPLEMENT_DYNCREATE(CAnimateLineDrawView, CView)

BEGIN_MESSAGE_MAP(CAnimateLineDrawView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAnimateLineDrawView::OnFilePrintPreview)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CAnimateLineDrawView 构造/析构

CAnimateLineDrawView::CAnimateLineDrawView()
{
	// TODO: 在此处添加构造代码

}

CAnimateLineDrawView::~CAnimateLineDrawView()
{
}

BOOL CAnimateLineDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CAnimateLineDrawView 绘制

void CAnimateLineDrawView::OnDraw(CDC* pDC)
{
	CAnimateLineDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	// 在客户屏幕上建立一个曲线绘制区域，区域大小为200 x 100,背景为方格黑色背景
	CRect rect(200,100,400,200);
	CBrush bkBrush(HS_CROSS,RGB(0,128,0));
	pDC->SetBkColor(RGB(0,0,0));
	pDC->FillRect(rect,&bkBrush);

	//创建一个和设备描述表兼容的位图，尺寸大小为195 x 100，用来保存需要移动的曲线图
	if(m_dc.GetSafeHdc()==NULL)
	{

		CRect rect1(0,0,195,100);
		m_dc.CreateCompatibleDC(pDC);
		m_bitmap.CreateCompatibleBitmap(pDC,195,100);
		m_dc.SelectObject(m_bitmap);
		m_dc.SetBkColor(RGB(0,0,0));
		m_dc.FillRect(rect1,&bkBrush);
	}
}


// CAnimateLineDrawView 打印


void CAnimateLineDrawView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CAnimateLineDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CAnimateLineDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CAnimateLineDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CAnimateLineDrawView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAnimateLineDrawView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CAnimateLineDrawView 诊断

#ifdef _DEBUG
void CAnimateLineDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimateLineDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimateLineDrawDoc* CAnimateLineDrawView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimateLineDrawDoc)));
	return (CAnimateLineDrawDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnimateLineDrawView 消息处理程序

void CAnimateLineDrawView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	GetParentFrame()->RecalcLayout();
	m_offsetx = 5;
	SetTimer(1,100,NULL);
}

void CAnimateLineDrawView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CClientDC dc(this);
	static int x = 200;
	static int y = 200;
	CPen pen1(PS_SOLID,0,RGB(255,0,8));
	CPen *pOldPen = dc.SelectObject(&pen1);

	//以下代码实现动态曲线
	x = x+ m_offsetx;

	// 如果曲线没有超过屏幕图片框右边界线，直接在屏幕图片框内绘制曲线，不用平移
	if( x <= 400)
	{
		dc.MoveTo(x-m_offsetx,y);
		y = 200 - rand()%90; //模拟数据采样值
		dc.LineTo(x,y);
	}
	else // 如果超过屏幕图片框右边界线，直需要平移曲线图
	{
		CRect rect(200,100,400,200);
		CBrush bkBrush(HS_CROSS,RGB(0,128,0));

		// 保存需要移动的曲线图像
		m_dc.BitBlt(0,0,195,100,&dc,205,100,SRCCOPY);

		// 用背景图重绘屏幕图片框，消隐其中的所有曲线
		dc.SetBkColor(RGB(0,0,0));
		dc.FillRect(rect,&bkBrush);

		//将保存的曲线图向左平移5个单位
		dc.BitBlt(200,100,195,100,&m_dc,0,0,SRCCOPY);

		// 用新数据继续绘制曲线
		dc.MoveTo(395,y);
		y = 200 - rand()%90; //模拟数据采样值
		dc.LineTo(400,y);
	}

	dc.SelectObject(pOldPen);
	CView::OnTimer(nIDEvent);
}
