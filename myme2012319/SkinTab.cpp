/************************************************************************ 
* 文件名：    SkinTab.cpp 
* 文件描述：  图片TAB 
* 创建人：    黄锐坤(Nuk), 2006年08月28日
* 版本号：    1.0 
************************************************************************/ 
// SkinTab.cpp : 实现文件
//

#include "stdafx.h"

#include "SkinTab.h"


// CSkinTab



IMPLEMENT_DYNAMIC(CSkinTab, CStatic)

CSkinTab::CSkinTab()
{
	m_pTabRect= NULL;
	m_nTabCount=0;
	m_nCurDownIndex=-1;
	m_nCurOverIndex=-1;
	lpET=new   TRACKMOUSEEVENT;  
	m_pTabBmpNormal=NULL;
	m_pTabBmpOver=NULL;
	m_pTabBmpDown=NULL;
	m_phIcon = NULL;
	m_bDCStored = false;

	m_szTab_COLOR.m_ColorR = 190;
	m_szTab_COLOR.m_ColorG = 243;
	m_szTab_COLOR.m_ColorB = 232;
	m_szTab_COLOR.R_OFFSET = 83;
	m_szTab_COLOR.G_OFFSET = 221;
	m_szTab_COLOR.B_OFFSET = 187;
	for(int i = 0;i < MAX_TEXT_COUNT;i++)
	{
		memset(m_TextTitle[i],0x00,sizeof(m_TextTitle[i]));
	}
}

CSkinTab::~CSkinTab()
{
	if (m_pTabRect)
		delete []m_pTabRect;
	if(lpET)
		delete lpET;
	if (m_pTabBmpNormal)
		delete []m_pTabBmpNormal;
	if (m_pTabBmpOver)
		delete []m_pTabBmpOver;
	if (m_pTabBmpDown)
		delete []m_pTabBmpDown;
	if (m_phIcon)
		delete []m_phIcon;
	m_memDC.DeleteDC();
}


BEGIN_MESSAGE_MAP(CSkinTab, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CSkinTab 消息处理程序



void CSkinTab::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0, SS_NOTIFY);

	CStatic::PreSubclassWindow();
}

void CSkinTab::SetSkinColor(DLG_COLOR_SKIN szDLG_COLOR_SKIN)
{ 
	m_szTab_COLOR = szDLG_COLOR_SKIN;


	m_bDCStored = false;
	m_memDC.DeleteDC();

	this->Invalidate(FALSE);
}

void CSkinTab::OnPaint()
{
	int i = 0;
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect r;
	GetClientRect(&r);	

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, r.Width(), r.Height());
	memDC.SelectObject(&btScreen);
	btScreen.DeleteObject();

	//画背景 
	memDC.BitBlt(r.left, r.top, r.Width(), r.Height(), &m_memDC, 0, 0, SRCCOPY);

	//memDC.Rectangle(r);
	for( i=0;i<m_nTabCount;i++)
	{
		CRect rect=m_pTabRect[m_nTabCount-i-1];
		
		rect.top +=22;
		DrawRangeImage(&m_BitmapBack[0],&memDC,rect);

	}
	if(m_nCurDownIndex!=-1)
	{
		CRect rect=m_pTabRect[m_nCurDownIndex];
		DrawRangeImage(&m_BitmapBack[1],&memDC,rect);

	
	}

	for(i = 0; i < MAX_TEXT_COUNT;i++)
	{
		if(strlen(m_TextTitle[i]) > 0)
		{
			CRect rect=m_pTabRect[i];
			CSize sizeTemp=memDC.GetTextExtent(m_TextTitle[i],_tcslen(m_TextTitle[i]));
			int xPos = (rect.Width() - sizeTemp.cx )/ 2 + rect.left;
			int YPos = (rect.Height() - sizeTemp.cy )/ 2 + rect.top + 11 ;
			if(xPos < 0)
				xPos = rect.left + 2;
			if(YPos < 0)
				YPos = rect.top + 2;
			memDC.SetBkMode(TRANSPARENT);
			memDC.TextOut(xPos ,YPos,m_TextTitle[i]);
		}
	}

	dc.BitBlt(r.left, r.top, r.Width(), r.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();
	// 不为绘图消息调用 CStatic::OnPaint()
}

void CSkinTab::SetCount(int nCount)
{
	m_nTabCount = nCount;

	if (m_pTabBmpNormal)
		delete []m_pTabBmpNormal;
	m_pTabBmpNormal = new CBitmap[m_nTabCount];

	if (m_pTabBmpOver)
		delete []m_pTabBmpOver;
	m_pTabBmpOver = new CBitmap[m_nTabCount];

	if (m_pTabBmpDown)
		delete []m_pTabBmpDown;
	m_pTabBmpDown = new CBitmap[m_nTabCount];

	if (m_phIcon)
		delete []m_phIcon;
	m_phIcon = new HICON[m_nTabCount];

	if (m_pTabRect)
		delete []m_pTabRect;
	m_pTabRect = new CRect[m_nTabCount];


	SetBackImage(0,_T("分图\\9按钮_1.bmp"));
	SetBackImage(1,_T("分图\\9按钮_2.bmp"));
	
}

void CSkinTab::SetRect(int nIndex, CRect rc)
{
	ASSERT(nIndex < m_nTabCount);
	m_pTabRect[nIndex] = rc;
}

void CSkinTab::SetIcon(int nIndex, CString strIcon)
{
	ASSERT(nIndex < m_nTabCount);
	m_phIcon[nIndex] = (HICON)::LoadImage(AfxGetApp()->m_hInstance,strIcon,IMAGE_ICON,24,24,LR_LOADFROMFILE); 
}

void CSkinTab::SetIcon(int nIndex, UINT nIconID)
{
}

void CSkinTab::SetSkin(int nIndex, CString strNormal, CString strOver, CString strDown)
{
	ASSERT(nIndex < m_nTabCount);

	if (m_pTabBmpNormal[nIndex].m_hObject)
		m_pTabBmpNormal[nIndex].Detach();
	if (m_pTabBmpOver[nIndex].m_hObject)
		m_pTabBmpOver[nIndex].Detach();
	if (m_pTabBmpDown[nIndex].m_hObject)
		m_pTabBmpDown[nIndex].Detach();

	HBITMAP bm = (HBITMAP)::LoadImage(NULL,strNormal,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_pTabBmpNormal[nIndex].Attach(bm);
	bm = (HBITMAP)::LoadImage(NULL,strOver,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_pTabBmpOver[nIndex].Attach(bm);
	bm = (HBITMAP)::LoadImage(NULL,strDown,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_pTabBmpDown[nIndex].Attach(bm);
}

void CSkinTab::SetSkin(int nIndex, UINT nNormalID, UINT nOverID, UINT nDownID)
{
	ASSERT(nIndex < m_nTabCount);

	if (m_pTabBmpNormal[nIndex].m_hObject)
		m_pTabBmpNormal[nIndex].Detach();
	if (m_pTabBmpOver[nIndex].m_hObject)
		m_pTabBmpOver[nIndex].Detach();
	if (m_pTabBmpDown[nIndex].m_hObject)
		m_pTabBmpDown[nIndex].Detach();

	m_pTabBmpNormal[nIndex].LoadBitmap(nNormalID);
	m_pTabBmpOver[nIndex].LoadBitmap(nOverID);
	m_pTabBmpDown[nIndex].LoadBitmap(nDownID);
}

int CSkinTab::GetBitmapWidth (HBITMAP hBitmap)
{ 
	BITMAP bm;
	GetObject(hBitmap,sizeof(BITMAP),(PSTR)&bm); 
	return bm.bmWidth;
}

int CSkinTab::GetBitmapHeight (HBITMAP hBitmap)
{ 
	BITMAP bm;
	GetObject(hBitmap,sizeof(BITMAP),(PSTR)&bm); 
	return bm.bmHeight;
}

void CSkinTab::DrawIcon(CDC* dc, HICON hIcon,CRect rect , UINT align)
{
	RECT r;
	r.left = rect.left;
	r.top = rect.top;
	r.right = rect.right;
	r.bottom = rect.bottom;
	if(hIcon!=NULL)
	{
		int ix,iy;
		switch(align)
		{
		case 2:
			ix=(r.right-r.left-24+1)/2;//水平居中
			iy=r.top+2;//垂直置顶
			break;
		case 4:
			ix=r.left+2;//水平左置
			iy=(r.bottom-r.top-24+1)/2;//垂直居中
			break;
		default:
			ix =0;
			iy =0;
		}
		DrawIconEx(dc->m_hDC,ix,iy,hIcon,24,24,0, NULL, DI_NORMAL); 
	}
}

void CSkinTab::SetTextFile(char *pText,int nCount)
{
	if(nCount > MAX_TEXT_COUNT)
	{
        AfxMessageBox("数组大定义的最大长度！");
		return;
	}
	sprintf(m_TextTitle[nCount],_T("%s"),pText);
}

void CSkinTab::DrawBitmap(CDC* dc, HBITMAP hbmp, CRect rect)
{
	RECT r;
	r.left = rect.left;
	r.top = rect.top;
	r.right = rect.right;
	r.bottom = rect.bottom;

	if(!hbmp) 
		return;

	int cx=r.right  - r.left;
	int cy=r.bottom - r.top;
	int xbmp = GetBitmapWidth(hbmp);
	int ybmp = GetBitmapHeight(hbmp);

	CDC		 l_BufferDC ;
			 l_BufferDC.CreateCompatibleDC( dc) ;
	CBitmap  l_BufferBitmap ;
			 l_BufferBitmap.CreateCompatibleBitmap( dc, cx, cy ) ;
	CBitmap* l_pOldBufferBitmap = l_BufferDC.SelectObject( &l_BufferBitmap ) ;

	CDC		 l_MaskDC ;
			 l_MaskDC.CreateCompatibleDC( dc ) ;
	CBitmap  l_MaskBitmap ;
			 l_MaskBitmap.CreateBitmap( cx, cy, 1, 1, NULL ) ;
	CBitmap* l_pOldMaskBitmap = l_MaskDC.SelectObject( &l_MaskBitmap ) ;

	#define ROP4_TRANSPARENTBLIT		0xCCAA0020
	#define SRCMASK						0x00220326

	l_BufferDC.FillSolidRect( r.left,r.top,r.right,r.bottom, RGB( 255, 0, 255 ) ) ;

	CDC		 l_MemoryDC ;
			 l_MemoryDC.CreateCompatibleDC( dc) ;
	l_MemoryDC.SelectObject(hbmp) ;

	////画图标
	//RECT rico;
	//rico.left=0;
	//rico.top = 0;
	//rico.right = xbmp;
	//rico.bottom = ybmp;
	//DrawIcon(&l_MemoryDC,rico,D_TOP);

	////画标题
	//RECT rbmp;
	//rbmp.left=0;
	//rbmp.top = 0;
	//rbmp.right = xbmp;
	//rbmp.bottom = ybmp;
	//DrawTitle(&l_MemoryDC, rbmp);

	l_BufferDC.BitBlt( 0, 0, cx, cy, &l_MemoryDC, 0, 0, SRCCOPY ) ;

	COLORREF l_crOldBack = l_BufferDC.SetBkColor( RGB( 255, 0, 255 ) ) ;

	l_MaskDC.BitBlt( 0, 0, cx, cy, &l_BufferDC, 0, 0, SRCCOPY ) ;

	l_BufferDC.SetBkColor( l_crOldBack ) ;

	if(!dc->MaskBlt( r.left, r.top, cx,cy,&l_BufferDC,0,0,l_MaskBitmap,0,0,ROP4_TRANSPARENTBLIT ) )
	{
		CDC		 l_CopyDC ;
				 l_CopyDC.CreateCompatibleDC( dc ) ;
		CBitmap  l_CopyBitmap ;
				 l_CopyBitmap.CreateCompatibleBitmap(dc, cx, cy ) ;
		CBitmap* l_pOldCopyBitmap = l_CopyDC.SelectObject( &l_CopyBitmap ) ;

		l_CopyDC.BitBlt(   0, 0, cx, cy, dc,  0, 0, SRCCOPY  ) ;
		l_CopyDC.BitBlt(   0, 0, cx, cy, &l_MaskDC,   0, 0, SRCAND   ) ;
		l_BufferDC.BitBlt( 0, 0, cx, cy, &l_MaskDC,   0, 0, SRCMASK  ) ;
		l_CopyDC.BitBlt(   0, 0, cx, cy, &l_BufferDC, 0, 0, SRCPAINT ) ;
		dc->BitBlt(  0, 0, cx, cy, &l_CopyDC,   0, 0, SRCCOPY  ) ;

		l_CopyDC.SelectObject( l_pOldCopyBitmap ) ;
	}
}

void CSkinTab::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	for(int i=0; i<m_nTabCount; i++)
	{
		if (m_pTabRect[i].PtInRect(point) && m_nCurOverIndex != i)
		{
			m_nCurOverIndex = i;
			Invalidate();
			break;
		}
	}
 
	lpET->cbSize=sizeof(TRACKMOUSEEVENT);   
	lpET->dwFlags=TME_LEAVE;   
	lpET->dwHoverTime=NULL;   
	lpET->hwndTrack=this->m_hWnd;  
	_TrackMouseEvent(lpET);   

	CStatic::OnMouseMove(nFlags, point);
}
void CSkinTab::SetSelTab()
{
	CPoint point;
    point.x = 13;
	point.y = 14;
	OnLButtonDown(1, point);
	OnMouseMove(0,  point);
}
void CSkinTab::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	for(int i=0; i<m_nTabCount; i++)
	{
		if (m_pTabRect[i].PtInRect(point) && m_nCurDownIndex != i)
		{
			m_nCurDownIndex = i;
			Invalidate();
			GetParent()->PostMessage(WM_TAB_SELECT,NULL,m_nCurDownIndex);
			break;
		}
	}
	CStatic::OnLButtonDown(nFlags, point);
}

BOOL CSkinTab::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_MOUSELEAVE)   
	{   
		m_nCurOverIndex=-1; 
		Invalidate();
	}   

	return CStatic::PreTranslateMessage(pMsg);
}


BOOL CSkinTab::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!m_bDCStored)
	{
    	CRect  rect;
	    GetClientRect(&rect);

		m_memDC.CreateCompatibleDC (pDC);
		CBitmap	btScreen;
		btScreen.CreateCompatibleBitmap (pDC,rect.Width(),rect.Height());
		m_memDC.SelectObject (&btScreen);

		m_memDC.BitBlt (0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);

		m_bDCStored=true;

		btScreen.DeleteObject();
	}
	return TRUE;// CButton::OnEraseBkgnd(pDC);//
	return CStatic::OnEraseBkgnd(pDC);
}

//平铺画位图
void CSkinTab::DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc)
{
	CDC MemDC;
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	int li_Width = bm.bmWidth;
	int li_Height = bm.bmHeight;


	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = MemDC.SelectObject(pBitmap);

	int x=rc.left;
	int y=rc.top;
	while (y < (rc.Height()+rc.top)) 
 	{
 		while(x < (rc.Width()+rc.left)) 
 		{
			pDC->BitBlt(x, y, li_Width, li_Height, &MemDC, 0, 0, SRCCOPY);
 			x += li_Width;
 		}
		x = rc.left;
 		y += li_Height;
 	}

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}

//保存位图尺寸
void CSkinTab::SetImageSize(CBitmap *pBitmap, CSize &sz)
{
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	sz = CSize(bm.bmWidth, bm.bmHeight);
}

//载入资源位图
void CSkinTab::SetBackImage(int index,CString strName)
{
	SetImage(m_BitmapBack[index], strName);
	SetImageSize(&m_BitmapBack[index], m_szBitmapBack[index]);
}

//载入硬盘文件位图
void CSkinTab::SetImage(CBitmap &bitmap, CString strName)
{
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)::LoadImage(NULL, strName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	//ASSERT(hBitmap);

	if(hBitmap == NULL)
	{
		CString str;
		AfxMessageBox(_T("Not Open SetImage Bitmap"));
		PostQuitMessage(0);
	}

	if (bitmap.m_hObject)
		bitmap.Detach();

	bitmap.Attach(hBitmap);
}