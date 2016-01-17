// RsProcView.cpp : implementation of the CRsProcView class
//

#include "stdafx.h"
#include "RsProc.h"
#include <math.h>
#include "RsProcDoc.h"
#include "RsProcView.h"
#include "ShowModelDlg.h"
#include "RasterTranszDlg.h"
#include "CannyAlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRsProcView

IMPLEMENT_DYNCREATE(CRsProcView, CScrollView)

BEGIN_MESSAGE_MAP(CRsProcView, CScrollView)
	//{{AFX_MSG_MAP(CRsProcView)
	ON_COMMAND(ID_SHOW_MODEL, OnShowModel)
	ON_COMMAND(ID_HIST_EQUAL_SHOW, OnHistEqualShow)
	ON_COMMAND(ID_SELF_FIT_STRECT, OnSelfFitStrect)
	ON_COMMAND(ID_Laplasian, OnLaplasian)
	ON_COMMAND(ID_Laplasian_Outline, OnLaplasianOutline)
	ON_COMMAND(ID_Sobel_Outline, OnSobelOutline)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_PreWitt, &CRsProcView::OnPrewitt)
	ON_COMMAND(ID_PrintScreen, &CRsProcView::OnPrintscreen)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_TransRaster, &CRsProcView::OnTransraster)
	ON_COMMAND(ID_MenuCanny, &CRsProcView::OnMenucanny)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsProcView construction/destruction

CRsProcView::CRsProcView()
{
	// TODO: add construction code here
	PointStart.x=PointStart.y=0;
	PointEnd.y=PointEnd.y=0;
	PointPriews.x=PointPriews.y=0;
	int start_x=0;
	int start_y=0;
	m_nStep=0;
	IsMouseDown=false;
	Isprintscreen=false;
	int show_width=0;
	int show_height=0;

}

CRsProcView::~CRsProcView()
{
}

BOOL CRsProcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRsProcView drawing

void CRsProcView::OnDraw(CDC* pDC)
{
	CRsProcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	LONG m_width;
	LONG m_height;
	m_width=pDoc->m_nWidth;
	m_height=pDoc->m_nHeigth;
	BITMAPINFO *pBitmapInfo=(BITMAPINFO*)new char[sizeof(BITMAPINFO)];

	pBitmapInfo->bmiHeader.biBitCount=24;
	pBitmapInfo->bmiHeader.biClrImportant=0;
	pBitmapInfo->bmiHeader.biClrUsed=0;
	pBitmapInfo->bmiHeader.biCompression=BI_RGB;
	pBitmapInfo->bmiHeader.biWidth=m_width;
	pBitmapInfo->bmiHeader.biHeight=m_height;
	pBitmapInfo->bmiHeader.biPlanes=1;
	pBitmapInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	pBitmapInfo->bmiHeader.biSizeImage=(m_width*24+31)/32*4*m_height;
	pBitmapInfo->bmiHeader.biXPelsPerMeter=0;
	pBitmapInfo->bmiHeader.biYPelsPerMeter=0;

	LONG LineBytes=(m_width*24+31)/32*4;
	LPBYTE pData=(LPBYTE)new char [LineBytes*m_height*3];
	int k;
	LONG i,j;
	for (i=0;i<m_height;i++)
	{
		for(j=0,k=0;j<m_width,k<3*m_width;j++,k+=3)
		{
			/*if(pDoc->pBandBlock[2][i*m_width+j]<150||pDoc->pBandBlock[2][i*m_width+j]<150||pDoc->pBandBlock[0][i*m_width+j]<150)
			{
				pData[(m_height-i-1)*LineBytes+k]= 0;
				pData[(m_height-i-1)*LineBytes+k+1]=0;
				pData[(m_height-i-1)*LineBytes+k+2]=0;
			}
			else
			{
				pData[(m_height-i-1)*LineBytes+k]= 255;
				pData[(m_height-i-1)*LineBytes+k+1]=255;
				pData[(m_height-i-1)*LineBytes+k+2]=255;
			}*/

			pData[(m_height-i-1)*LineBytes+k]=pDoc->pBandBlock[2][i*m_width+j];
			pData[(m_height-i-1)*LineBytes+k+1]=pDoc->pBandBlock[1][i*m_width+j];
			pData[(m_height-i-1)*LineBytes+k+2]=pDoc->pBandBlock[0][i*m_width+j];
		}
	}
	CSize sizeTotal;
	sizeTotal.cx=m_width;
	sizeTotal.cy=m_height;
	SetScrollSizes(MM_TEXT,sizeTotal);
	SetStretchBltMode(pDC->m_hDC,COLORONCOLOR);
	
	if (m_width<=1024)
	{
		show_width=m_width;
		show_height=m_height;
	}
	else
	{
		show_width=1024;
		show_height=1024*m_height/m_width;
	}
	CRect rect;
	GetClientRect(&rect);
	start_x=rect.Width()/2-show_width/2;
	start_y=rect.Height()/2-show_height/2;
	StretchDIBits(pDC->m_hDC,start_x,start_y,show_width,show_height,0,0,m_width,m_height,
		pData,pBitmapInfo,DIB_RGB_COLORS,SRCCOPY);
	UpdateWindow();
	delete pBitmapInfo;
	delete pData;
}

void CRsProcView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	CRsProcDoc *pDoc=GetDocument();
	sizeTotal.cx=pDoc->m_nWidth;
	sizeTotal.cy=pDoc->m_nHeigth;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CRsProcView printing

BOOL CRsProcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRsProcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRsProcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRsProcView diagnostics

#ifdef _DEBUG
void CRsProcView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CRsProcView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CRsProcDoc* CRsProcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRsProcDoc)));
	return (CRsProcDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRsProcView message handlers
/************************************************************************/
/* 显示模式设置                                                                     */
/************************************************************************/
void CRsProcView::OnShowModel() 
{
	// TODO: Add your command handler code here
	CShowModelDlg *pdlg=new CShowModelDlg();
	pdlg->Create(IDD_SHOW_MODEL,this);
	pdlg->ShowWindow(SW_SHOW);
	pdlg->CenterWindow();
}
/************************************************************************/
/* 直方图均衡化显示                                                                     */
/************************************************************************/
void CRsProcView::OnHistEqualShow() 
{
	// TODO: Add your command handler code here
	CRsProcDoc *pdoc=GetDocument();
	double fq[3][256]; //原频率
	double f[3][256]; //累计频率
	int newcount[3][256];
	int i,j,k;
	//统计频率
	for(k=0;k<3;k++)
	{
		for (i=0;i<256;i++)
		{
			fq[k][i]=(double)pdoc->count[k][i]/(double)(pdoc->m_nWidth*pdoc->m_nHeigth);
		}
	}
	//计算累计直方图分布
	for (i=0;i<3;i++)
	{
		for (j=0;j<256;j++)
		{
			if (j==0)
			{
				f[i][0]=fq[i][0];
			}
			else
			{
				f[i][j]=f[i][j-1]+fq[i][j];
			}
			newcount[i][j]=int(255*f[i][j]+0.5);
		}
	}
	//灰度变换
	for (k=0;k<3;k++)
	{
		for (i=0;i<pdoc->m_nHeigth;i++)
		{
			for (j=0;j<pdoc->m_nWidth;j++)
			{
				pdoc->pBandBlock[k][i*pdoc->m_nWidth+j]=newcount[k][pdoc->pBandBlock[k][i*pdoc->m_nWidth+j]];
			}
		}
	}
	pdoc->UpdateAllViews(NULL);
}
/************************************************************************/
/* 自适应灰度拉伸显示                                                                     */
/************************************************************************/
void CRsProcView::OnSelfFitStrect() 
{
	// TODO: Add your command handler code here
	CRsProcDoc *pdoc=GetDocument();
	int max[3],min[3];
	int i,j,k;
	for (i=0;i<3;i++)
	{
		max[i]=pdoc->max_grey[i];
		min[i]=pdoc->min_grey[i];
	}
	for(i=0;i<pdoc->m_nHeigth;i++)
	{
		for(j=0;j<pdoc->m_nWidth;j++)
		{
			for (k=0;k<3;k++)
			{
				BYTE a=pdoc->pBandBlock[k][i*pdoc->m_nWidth+j];
				pdoc->pBandBlock[k][i*pdoc->m_nWidth+j]=BYTE((a-min[k])/((max[k]-min[k])*1.0f)*255*1.0f);
			}
		}
	}
	pdoc->UpdateAllViews(NULL);
}
/************************************************************************/
/* Laplasian算子实现图像边缘增强                                                                     */
/************************************************************************/
void CRsProcView::OnLaplasian() 
{
	// TODO: Add your command handler code here
	CRsProcDoc *pdoc=GetDocument();
	int i,j,k;
	//复制新的临时数据域
	BYTE *pNewbandBlock[3];
	for(i=0;i<3;i++)
	{
		pNewbandBlock[i]=(BYTE*)CPLMalloc(sizeof(BYTE)*pdoc->m_nHeigth*pdoc->m_nWidth);  //申请内存空间
	}

	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
			for (k=0;k<3;k++)
			{
				pNewbandBlock[k][i*pdoc->m_nWidth+j]=pdoc->pBandBlock[k][i*pdoc->m_nWidth+j];
			}
		}
	}
	//求边缘
	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
			for (k=0;k<3;k++)
			{
				int left=pdoc->pBandBlock[k][i*pdoc->m_nWidth+j-1];
				int right=pdoc->pBandBlock[k][i*pdoc->m_nWidth+j+1];
				int up=pdoc->pBandBlock[k][(i-1)*pdoc->m_nWidth+j];
				int down=pdoc->pBandBlock[k][(i+1)*pdoc->m_nWidth+j];
				int own=pdoc->pBandBlock[k][i*pdoc->m_nWidth+j];
				int result=4*own-left-right-up-down;
				if (abs(result)>255)
				{
					pNewbandBlock[k][i*pdoc->m_nWidth+j]=255;
				}
				else
				{
					pNewbandBlock[k][i*pdoc->m_nWidth+j]=abs(result);
				}
			}
		}
	}
	//锐化
	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
			for (k=0;k<3;k++)
			{
				pdoc->pBandBlock[k][i*pdoc->m_nWidth+j]=pdoc->pBandBlock[k][i*pdoc->m_nWidth+j]-
					pNewbandBlock[k][i*pdoc->m_nWidth+j];
			}
		}
	}
	pdoc->UpdateAllViews(NULL);
}

void CRsProcView::OnLaplasianOutline() 
{
	// TODO: Add your command handler code here
	CRsProcDoc *pdoc=GetDocument();
	int i,j,k;
	//复制新的临时数据域
	BYTE *pNewbandBlock[1];
	pNewbandBlock[0]=(BYTE*)CPLMalloc(sizeof(BYTE)*pdoc->m_nHeigth*pdoc->m_nWidth);  //申请内存空间
	
	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
			pNewbandBlock[0][i*pdoc->m_nWidth+j]=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j];
		}
	}
	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
			
				int left=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j-1];
				int right=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j+1];
				int up=pdoc->pBandBlock[0][(i-1)*pdoc->m_nWidth+j];
				int down=pdoc->pBandBlock[0][(i+1)*pdoc->m_nWidth+j];
				int own=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j];
				int result=4*own-left-right-up-down;
				if (abs(result)>255)
					pNewbandBlock[0][i*pdoc->m_nWidth+j]=255;
				else
					pNewbandBlock[0][i*pdoc->m_nWidth+j]=abs(result);
		}
	}
	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
			for (k=0;k<3;k++)
			{
				pdoc->pBandBlock[k][i*pdoc->m_nWidth+j]=pNewbandBlock[0][i*pdoc->m_nWidth+j];
			}
		}
	}
	pdoc->UpdateAllViews(NULL);
}
/************************************************************************/
/* 求边缘模板                                                                     */
/************************************************************************/
BOOL CRsProcView::Tempalte(BYTE *pSrcBandBlock[3],int width,int height,int TemX,int TemY,
						   int *fpArry,float fcoef)
{
	CRsProcDoc *pdoc=GetDocument();
	int i,j,k,m,n;
	BYTE *pNewBandBlock[3];
	for (k=0;k<3;k++)
	{
		pNewBandBlock[k]=(BYTE*)CPLMalloc(sizeof(BYTE)*width*height); //分配内存
		if (pNewBandBlock[k]==NULL)
		{
			MessageBox("分配内存失败","提示",MB_ICONERROR);
			return FALSE;
		}
	}
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			for (k=0;k<3;k++)
			{  
				pNewBandBlock[k][i*width+j]=0;
				pNewBandBlock[k][i*width+j]=pSrcBandBlock[k][i*width+j];  //拷贝图像数据
			}
		}
	}
	for (i=TemY/2;i<height-TemY/2;i++)
	{
		for (j=TemX/2;j<width-TemX/2;j++)
		{
			for(k=0;k<3;k++)
			{
				float result=0;
				for(m=0;m<TemY;m++)
					for (n=0;n<TemX;n++)
					{
						result+=pNewBandBlock[k][i*width+j]*fpArry[m*TemX+n]; //模板内权重累加？
					}
					result*=fcoef;//乘以系数
					if (abs(result)>255)
					{
						pSrcBandBlock[k][i*width+j]=255;
					}
					else
						pSrcBandBlock[k][i*width+j]=int(result+0.5);
			}
		}
	}
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			for (k=0;k<3;k++)
			{  
				pdoc->pBandBlock[k][i*width+j]=pSrcBandBlock[k][i*width+j];
			}
		}
	}
	pdoc->UpdateAllViews(NULL);
	return TRUE;
}
/************************************************************************/
/* Soble 算子提取边缘                                                                     */
/************************************************************************/
void CRsProcView::OnSobelOutline() 
{
	// TODO: Add your command handler code here
	CRsProcDoc *pdoc=GetDocument();
	int i,j,k;
	//复制新的临时数据域
	BYTE *pNewbandBlock[1];
	pNewbandBlock[0]=(BYTE*)CPLMalloc(sizeof(BYTE)*pdoc->m_nHeigth*pdoc->m_nWidth);  //申请内存空间
	
	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
			pNewbandBlock[0][i*pdoc->m_nWidth+j]=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j];
		}
	}
	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
				int left=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j-1];
				int right=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j+1];
				int up=pdoc->pBandBlock[0][(i-1)*pdoc->m_nWidth+j];
				int down=pdoc->pBandBlock[0][(i+1)*pdoc->m_nWidth+j];
				int own=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j];
				int left_top=pdoc->pBandBlock[0][(i-1)*pdoc->m_nWidth+j-1];
				int right_top=pdoc->pBandBlock[0][(i-1)*pdoc->m_nWidth+j+1];
				int left_down=pdoc->pBandBlock[0][(i+1)*pdoc->m_nWidth+j-1];
				int right_down=pdoc->pBandBlock[0][(i+1)*pdoc->m_nWidth+j+1];

				int result=8*own-left-right-up-down-left_down-left_top-right_down-right_top;
				if (abs(result)>255)
					pNewbandBlock[0][i*pdoc->m_nWidth+j]=255;
				else
					pNewbandBlock[0][i*pdoc->m_nWidth+j]=abs(result);
		}
	}
	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
			for (k=0;k<3;k++)
			{
				pdoc->pBandBlock[k][i*pdoc->m_nWidth+j]=pNewbandBlock[0][i*pdoc->m_nWidth+j];
			}
		}
	}
	pdoc->UpdateAllViews(NULL);

}
/************************************************************************/
/* PreWitt算子边缘提取                                                                 */
/************************************************************************/
void CRsProcView::OnPrewitt()
{
	// TODO: 在此添加命令处理程序代码
	CRsProcDoc *pdoc=GetDocument();
	int i,j;
	//复制新的临时数据域
	BYTE *pNewbandBlock[1];
	pNewbandBlock[0]=(BYTE*)CPLMalloc(sizeof(BYTE)*pdoc->m_nHeigth*pdoc->m_nWidth);  //申请内存空间
	
	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
			pNewbandBlock[0][i*pdoc->m_nWidth+j]=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j];
		}
	}
	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
				int left=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j-1];
				int right=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j+1];
				int up=pdoc->pBandBlock[0][(i-1)*pdoc->m_nWidth+j];
				int down=pdoc->pBandBlock[0][(i+1)*pdoc->m_nWidth+j];
				int own=pdoc->pBandBlock[0][i*pdoc->m_nWidth+j];
				int left_top=pdoc->pBandBlock[0][(i-1)*pdoc->m_nWidth+j-1];
				int right_top=pdoc->pBandBlock[0][(i-1)*pdoc->m_nWidth+j+1];
				int left_down=pdoc->pBandBlock[0][(i+1)*pdoc->m_nWidth+j-1];
				int right_down=pdoc->pBandBlock[0][(i+1)*pdoc->m_nWidth+j+1];

				int result=(left_top-right_top+2*left-2*right+left_down-right_down)+
					(left_top-left_down+2*up-2*down+right_top-right_down);
				if (abs(result)>255)
					pNewbandBlock[0][i*pdoc->m_nWidth+j]=255;
				else
					pNewbandBlock[0][i*pdoc->m_nWidth+j]=abs(result);
		}
	}
	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
			for (int k=0;k<3;k++)
			{
				pdoc->pBandBlock[k][i*pdoc->m_nWidth+j]=pNewbandBlock[0][i*pdoc->m_nWidth+j];
			}
		}
	}
	pdoc->UpdateAllViews(NULL);
}

void CRsProcView::OnPrintscreen()
{
	// TODO: 在此添加命令处理程序代码
	Isprintscreen=true;
}

void CRsProcView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (Isprintscreen)
	{
		PointStart=point;
		IsMouseDown=true;
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

void CRsProcView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	if (Isprintscreen)
	{
		Invalidate();
		PointEnd=point;
		IsMouseDown=false;
		Isprintscreen=false;
		CRect rect(PointStart.x+1,PointStart.y+1,PointEnd.x-1,PointEnd.y-1);
		ClientToScreen(rect);
		CRsProcDoc *pdoc=GetDocument();
		HBITMAP hmap=pdoc->CopyScreenToBitmap(rect);
		pdoc->SaveBitmapToFile(hmap,"C:\\Documents and Settings\\Administrator\\桌面\\截图.bmp");

		::ReleaseCapture();
		PointStart.x=PointStart.y=0;
		PointEnd.x=PointEnd.y=0;
		PointPriews.x=PointPriews.y=0;
	}
	CScrollView::OnLButtonUp(nFlags, point);
}

void CRsProcView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	if (Isprintscreen)
	{
		if (IsMouseDown)
		{
			SetCapture();
			CClientDC dc(this);
			CPen pen(PS_SOLID,1,RGB(255,0,0));
			CPen *pOldPen=dc.SelectObject(&pen);
			if (PointPriews.x==0)
			{
				PointPriews=point;
			}
			dc.SetROP2(R2_NOTXORPEN);
			CRect rectpriew(PointStart,PointPriews);
			dc.Rectangle(rectpriew);
			CRect rect(PointStart,point);
			dc.Rectangle(rect);
			dc.SelectObject(pOldPen);
			PointPriews=point;
		}
	}
	CScrollView::OnMouseMove(nFlags, point);
}

BOOL CRsProcView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (Isprintscreen)
	{
		if (IsMouseDown)
		{
			HCURSOR hcursor;
			hcursor=AfxGetApp()->LoadStandardCursor(IDC_CROSS);
			SetCursor(hcursor);
			return TRUE;
		}
	}
		return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}


void CRsProcView::OnTransraster()
{
	// TODO: 在此添加命令处理程序代码
	CRasterTranszDlg *pdlg=new CRasterTranszDlg;
	pdlg->DoModal();
}
//Canny提取边缘菜单响应入口
void CRsProcView::OnMenucanny()
{
	// TODO: 在此添加命令处理程序代码

	//更改光标形状
	BeginWaitCursor();

	CRsProcDoc *pdoc=GetDocument();

	int nWidth  = pdoc->m_nWidth;
	int nHeight = pdoc->m_nHeigth;

	int i,j,k;
	//复制新的临时数据域
	unsigned char *pNewbandBlock = new unsigned char[nWidth *nHeight];

	Canny(&pdoc->pBandBlock[0][0], nWidth, nHeight, 0.4, 0.4, 0.79, pNewbandBlock);

	for(i=1;i<pdoc->m_nHeigth-1;i++)
	{
		for(j=1;j<pdoc->m_nWidth-1;j++)
		{
			for (k=0;k<3;k++)
			{
				pdoc->pBandBlock[k][i*pdoc->m_nWidth+j]=pNewbandBlock[i*pdoc->m_nWidth+j];
			}
		}
	}

	delete pNewbandBlock;
	pNewbandBlock = NULL;
	
	// 恢复光标形状
	EndWaitCursor(); 

	// 更新视图
	pdoc->UpdateAllViews(NULL);
}
