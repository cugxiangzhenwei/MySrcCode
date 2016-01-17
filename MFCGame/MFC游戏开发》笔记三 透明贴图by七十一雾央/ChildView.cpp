
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "GameMFC2.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//将png贴图透明
void TransparentPNG(CImage *png)
{
	for(int i = 0; i <png->GetWidth(); i++)
	{
		for(int j = 0; j <png->GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char *>(png->GetPixelAddress(i , j));
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
}

// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	//创建兼容的DC
	m_bgcDC.CreateCompatibleDC(NULL);
	//从资源中加载位图
	m_bgBitmap.LoadBitmap(IDB_BITMAP1);
	//选择位图对象
	m_bgcDC.SelectObject(&m_bgBitmap);
	//加载英雄图片
	m_hero.Load("hero.png");
	TransparentPNG(&m_hero);


	return TRUE;
}

void CChildView::OnPaint() 
{
	//获取窗口DC指针
	CDC *cDC=this->GetDC();
	//获取窗口大小
	GetClientRect(&m_client);
	//贴图
	cDC->BitBlt(0,0,m_client.Width(),m_client.Height(),&m_bgcDC,0,0,SRCCOPY);
	//释放DC
	m_hero.Draw(*cDC,100,400,60,60);
	ReleaseDC(cDC);
}

