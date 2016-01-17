// 绘图程序View.cpp : implementation of the CMyView class
//

#include "stdafx.h"
#include "绘图程序.h"

#include "绘图程序Doc.h"
#include "绘图程序View.h"
#include "resource.h"
#include "LineWidthDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyView

IMPLEMENT_DYNCREATE(CMyView, CView)

BEGIN_MESSAGE_MAP(CMyView, CView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CMyView)
	ON_COMMAND(ID_GRAPH_POINT, OnGraphPoint)
	ON_COMMAND(ID_GRAPH_LINE, OnGraphLine)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_POINT, OnUpdateGraphPoint)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_LINE, OnUpdateGraphLine)
	ON_COMMAND(ID_DARA_ROUND, OnDaraRound)
	ON_COMMAND(ID_DRAW_ELLIPSE, OnDrawEllipse)
	ON_COMMAND(ID_DRAW_POLYGON, OnDrawPolygon)
	ON_COMMAND(ID_DRAW_POLYLINE, OnDrawPolyline)
	ON_COMMAND(ID_DRAW_RECTANGLE, OnDrawRectangle)
	ON_COMMAND(ID_BITMAP_SHOW, OnBitmapShow)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_SET_LINEWIDTH, OnSetLinewidth)
	ON_COMMAND(ID_SET_COLOR, OnSetColor)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyView construction/destruction

CMyView::CMyView()
{
	// TODO: add construction code here
 m_isPoint=FALSE;
 m_isLine=FALSE;
 m_startx=0;
 m_starty=0;
 m_nStep=0;
 m_endx=0;
 m_endy=0;
 m_nLineWidth=1;
 m_nLineStyle=PS_SOLID;
 m_color=RGB(0,0,0);
}

CMyView::~CMyView()
{
}

BOOL CMyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyView drawing

void CMyView::OnDraw(CDC* pDC)
{
	CMyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	////////////////////////
	//////////重绘点
	for (int i=0;i<pDoc->m_nPointNum;i++)
	{
		int x=pDoc->pointarry[i][0];
		int y=pDoc->pointarry[i][1];
		pDC->Ellipse(x-2,y-2,x+2,y+2);
	}
	///////////////////////
	/////////重绘线段
   for (int i=0;i<pDoc->m_nLineNum;i++)
   {
	   int x0=pDoc->linearry[i][0];
	   int y0=pDoc->linearry[i][1];
	   int x1=pDoc->linearry[i][2];
	   int y1=pDoc->linearry[i][3];
	   pDC->MoveTo(x0,y0);
	   pDC->LineTo(x1,y1);
   }
}

/////////////////////////////////////////////////////////////////////////////
// CMyView printing

BOOL CMyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyView diagnostics

#ifdef _DEBUG
void CMyView::AssertValid() const
{
	CView::AssertValid();
}

void CMyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyDoc* CMyView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyDoc)));
	return (CMyDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyView message handlers

void CMyView::OnGraphPoint() 
{
	// TODO: Add your command handler code here
	m_isPoint=TRUE;
	m_isLine=FALSE;
	/*
	CClientDC dc(this);
	dc.TextOut(200,100,"画点");
	*/
}

void CMyView::OnGraphLine() 
{
	// TODO: Add your command handler code here
	m_isLine=TRUE;
	m_isPoint=FALSE;
	/*
	CClientDC dc(this);
	dc.TextOut(200,200,"画线");
	*/
}

void CMyView::OnUpdateGraphPoint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->SetRadio(TRUE);  //在画点菜单项前面打点
//	pCmdUI->SetCheck(TRUE);    //在画菜单项前面打勾
//	pCmdUI->SetText("你好");   //改变菜单标题
	pCmdUI->SetCheck(m_isPoint);
//	pCmdUI->Enable(FALSE);    //禁用某个菜单和对应的工具栏中的按钮
}

void CMyView::OnUpdateGraphLine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_isLine);
}

void CMyView::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(CG_IDR_POPUP_MY_VIEW));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
	
}

void CMyView::OnDaraRound() 
{
	// TODO: Add your command handler code here
	CDC *PDC;
	//CPen mypen(PS_DOT,1,RGB(0,255,0));
	CPen mypen(PS_DASH,1,RGB(0,255,0));
	PDC=GetDC();
	CPen *pOldPen=PDC->SelectObject(&mypen);
	PDC->Ellipse(10,20,110,120);      //画圆
	PDC->SelectObject(pOldPen);
	ReleaseDC(PDC);
}

void CMyView::OnDrawEllipse() 
{
	// TODO: Add your command handler code here
	CDC *pDC;
	pDC=GetWindowDC();

    pDC->SetROP2(R2_XORPEN);

	CBrush mybrush(HS_CROSS,RGB(0,255,0));
	CBrush *pOldBrush=pDC->SelectObject(&mybrush);

	pDC->Ellipse(35,110,225,210);       //画椭圆

	pDC->SelectObject(pOldBrush);
	ReleaseDC(pDC);
}

void CMyView::OnDrawPolygon() 
{
	// TODO: Add your command handler code here
CBrush mybrush(HS_DIAGCROSS,RGB(0,0,255));
CClientDC dc(this);
CBrush *pOldBrush=dc.SelectObject(&mybrush);
POINT pointzoon[4]={{380,400},{530,260},{500,360},{200,59}};
dc.Polygon(pointzoon,4);   //画多边形
dc.SelectObject(pOldBrush);
}

void CMyView::OnDrawPolyline() 
{
	// TODO: Add your command handler code here
	CClientDC dc(this);
	POINT pointzoon[]={{0,20},{40,80},{100,60},{56,100},{89,200}};
    dc.Polyline(pointzoon,5);   //画线簇
}

void CMyView::OnDrawRectangle() 
{
	// TODO: Add your command handler code here
	CPen mypen(PS_DASHDOTDOT,3,RGB(255,0,0));
	CClientDC dc(this);
	//dc.SetROP2(R2_BLACK);
	dc.SetROP2(R2_NOTXORPEN);
	CPen *pOldPen=dc.SelectObject(&mypen);
	dc.Rectangle(10,300,200,400);           //画矩形
	dc.SelectObject(pOldPen);
}

void CMyView::OnBitmapShow() 
{
	// TODO: Add your command handler code here
	//显示位图
	CDC *pDC;
	pDC=GetDC();
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP1);

	CDC memDC;
    memDC.CreateCompatibleDC(pDC);

	memDC.SelectObject(&bitmap);
	BITMAP bminfo;
	bitmap.GetObject(sizeof(bminfo),&bminfo);
	pDC->BitBlt(0,0,bminfo.bmWidth,bminfo.bmHeight,&memDC,0,0,SRCCOPY);
	/*
	CRect rect;
    GetClientRect(rect);
	pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,bminfo.bmWidth,bminfo.bmHeight,SRCCOPY);
	*/
	ReleaseDC(pDC);
}

void CMyView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMyDoc *pdoc=GetDocument();
	CClientDC dc(this);

	CPen mypen(m_nLineStyle,m_nLineWidth,m_color);
	 CPen *pOldpen=dc.SelectObject(&mypen);

//	dc.TextOut(point.x,point.y,"你单击了鼠标左键");
	if (nFlags&MK_SHIFT)
	{
			dc.TextOut(point.x,point.y+20,"你按下了shift键");
	}
	if (nFlags&MK_CONTROL)
	{
		dc.TextOut(point.x,point.y+40,"您按下了Ctrl键");
	}
	if (m_isPoint)
	{
		/*
		CMenu *pmenu=GetParent()->GetMenu()->GetSubMenu(4);
		pmenu->EnableMenuItem(ID_SET_LINEWIDTH,MF_DISABLED);
		GetParent()->DrawMenuBar();
		*/
		int i=pdoc->m_nPointNum;
		dc.Ellipse(point.x-2,point.y-2,point.x+2,point.y+2);
		pdoc->pointarry[i][0]=point.x;
		pdoc->pointarry[i][1]=point.y;
		pdoc->m_nPointNum++;
		pdoc->SetModifiedFlag(TRUE);
	}

  	if(m_isLine)
	{
		
    	if (m_nStep==0)
		{
			SetCapture();
		m_startx=point.x;
		m_starty=point.y;
		m_nStep++;
		}
		else
		{
		dc.MoveTo(m_startx,m_starty);
		dc.LineTo(point);
		int i=pdoc->m_nLineNum;
		pdoc->linearry[i][0]=m_startx;        //线段起点x坐标
		pdoc->linearry[i][1]=m_starty;        //线段起点y坐标
        pdoc->linearry[i][2]=m_endx;          //线段终点x坐标
        pdoc->linearry[i][3]=m_endy;          //线段终点y坐标
		pdoc->m_nLineNum++;
		m_nStep=0;
		m_startx=m_starty=0;
		m_endx=m_endy=0;
		::ReleaseCapture();
		pdoc->SetModifiedFlag(TRUE);
		}
	
	}
	dc.SelectObject(pOldpen);
	// AfxMessageBox("使用全局Afxmessagebox函数",MB_ABORTRETRYIGNORE|MB_ICONEXCLAMATION);
	CView::OnLButtonDown(nFlags, point);
}

BOOL CMyView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(m_isPoint)
	{ 
        HCURSOR hcursor;
		hcursor=AfxGetApp()->LoadCursor(IDC_POINTER);
		SetCursor(hcursor);
		return TRUE;
	}
	if (m_isLine)
	{
	  HCURSOR hcursor;
      hcursor=AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	  SetCursor(hcursor);
	  return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CMyView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar==VK_ESCAPE)
	{
		if (MessageBox("要取消画图吗?","提示消息",MB_YESNO|MB_ICONQUESTION)==IDYES)
		{
			m_isPoint=FALSE;
			m_isLine=FALSE;
			m_nStep=0;
			m_startx=m_starty=0;
			m_endx=m_endy=0;
		::ReleaseCapture();
		}	
		else return;
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	if (m_isLine)
	{
		if (m_nStep!=0)
		{
			dc.SetROP2(R2_NOTXORPEN); //设置绘图模式
			if (m_endx!=0)      //如果不是第一次画线，则擦除上一线条
			{
				dc.MoveTo(m_startx,m_starty);
				dc.LineTo(m_endx,m_endy);
			}
			//更新终点坐标
			m_endx=point.x;
			m_endy=point.y;
			//画出新的线段
			dc.MoveTo(m_startx,m_starty);
			dc.LineTo(m_endx,m_endy);
		}
	}
CString str;
CStatusBar *pStatusbar;
pStatusbar=(CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
if (pStatusbar)
{
	str.Format("x =%4d , y =%4d",point.x,point.y);   //%后面的数字设置等号与其后数字的间隔大小
	pStatusbar->SetPaneText(1,str);     //设置状态栏第二窗格显示当前坐标
}
	CView::OnMouseMove(nFlags, point);
}


void CMyView::OnSetLinewidth() 
{
	// TODO: Add your command handler code here
	CLineWidthDlg dlg;
	if (dlg.DoModal()==IDOK)
	{
		m_nLineWidth=dlg.m_nLineWidth;	}
	
}

void CMyView::OnSetColor() 
{
	// TODO: Add your command handler code here
	CColorDialog dlg;    //颜色设置通用对话框
	if (dlg.DoModal()==IDOK)
	{
		m_color=dlg.GetColor();
	}
}
