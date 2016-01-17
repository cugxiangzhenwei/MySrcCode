
//-----------------------------------【程序说明】----------------------------------------------
// 【MFC游戏开发】笔记八 粒子系统 配套源代码
// VS2010环境
// 更多内容请访问雾央CSDN博客 http://blog.csdn.net/u011371356/article/category/1497651
// 雾央的新浪微博： @七十一雾央
//------------------------------------------------------------------------------------------------


// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "GameMFC.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//定时器的名称用宏比较清楚
#define TIMER_PAINT 1
#define TIMER_HEROMOVE 2
//四个方向
#define DOWN 0
#define LEFT 1
#define RIGHT 2
#define UP 3
//窗口大小
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
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
	
	//-----------------------------------游戏数据初始化部分-------------------------
	
	//加载背景
	m_bg.Load("bigbg.png");
	//获取背景地图的宽度
	m_mapWidth=m_bg.GetWidth();
	//加载英雄图片
	MyHero.hero.Load("heroMove.png");
	TransparentPNG(&MyHero.hero);
	//初始化英雄状态
	MyHero.direct=UP;
	MyHero.frame=0;
	//设置英雄初始位置
	MyHero.x=80;    
	MyHero.y=400;
	//设置地图初始从最左端开始显示
	m_xMapStart=0;
	//加载雪花图像
	char buf[20];
	for(int i=0;i<7;i++)    //加载七种图像
	{
		sprintf(buf,"Snow//%d.png",i);
		m_snowMap[i].Load(buf);
	}
	//初始化雪花粒子
	for(int i=0;i<SNOW_NUMBER;i++)
	{
		Snow[i].x=rand()% WINDOW_WIDTH;   //最初雪花在水平方向上随机出现
		Snow[i].y=rand()% WINDOW_HEIGHT; //垂直方向上也是随机出现
		Snow[i].number=rand()%7;         //七种雪花中的一种
	}
	
	return TRUE;
}
//计算地图左端x开始位置
void CChildView::GetMapStartX()
{
	//如果人物不在最左边和最右边半个屏幕内时，地图的起始坐标是需要根据人物位置计算的。
	if(MyHero.x<m_mapWidth-WINDOW_WIDTH/2 && MyHero.x>WINDOW_WIDTH/2)
		m_xMapStart=MyHero.x-WINDOW_WIDTH/2;
}
//获取人物在屏幕上的坐标
int GetScreenX(int xHero,int mapWidth)
{
	//如果人物在最左边和最右边半个屏幕内时，那么人物就处在屏幕中间
	if(xHero<mapWidth-WINDOW_WIDTH/2 && xHero>WINDOW_WIDTH/2)
		return WINDOW_WIDTH/2;
	else if(xHero<=WINDOW_WIDTH/2)     //在最左边半个屏幕时，人物在屏幕上的位置就是自己的x坐标了
		return xHero;
	else 
		return WINDOW_WIDTH-(mapWidth-xHero);  //在最右边半个屏幕
}
void CChildView::OnPaint() 
{
	//获取窗口DC指针
	CDC *cDC=this->GetDC();
	//获取窗口大小
	GetClientRect(&m_client);
	//创建缓冲DC
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(cDC,m_client.Width(),m_client.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);
	//计算背景地图起始位置
	GetMapStartX();
	//————————————————————开始绘制——————————————————————
	//贴背景,现在贴图就是贴在缓冲DC：m_cache中了
	m_bg.Draw(m_cacheDC,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,m_xMapStart,0,WINDOW_WIDTH,WINDOW_HEIGHT);
	//贴英雄
	MyHero.hero.Draw(m_cacheDC,GetScreenX(MyHero.x,m_mapWidth),MyHero.y,80,80,MyHero.frame*80,MyHero.direct*80,80,80);
	//绘制雪花粒子
	for(int i=0;i<SNOW_NUMBER;i++)
	{
		//画出粒子
		m_snowMap[Snow[i].number].Draw(m_cacheDC,Snow[i].x,Snow[i].y,32,32);
		//对粒子的位置进行更新
		Snow[i].y+=1;
		if(Snow[i].y>=600)    //当落到最下面后，再回到最上面去
			Snow[i].y=0;
		//为了更自然，在水平方向上也发生位移，就像有风一样
		if(rand()%2==0)
			Snow[i].x+=1;
		else 
			Snow[i].x-=1;
		if(Snow[i].x<0)
			Snow[i].x=WINDOW_WIDTH;      //水平方向上出界后到另一边去
		else if(Snow[i].x>=WINDOW_WIDTH)
			Snow[i].x=0;
	}
	//最后将缓冲DC内容输出到窗口DC中
	cDC->BitBlt(0,0,m_client.Width(),m_client.Height(),&m_cacheDC,0,0,SRCCOPY);

	//————————————————————绘制结束—————————————————————
	
	//在绘制完图后,使窗口区有效
	ValidateRect(&m_client);
	//释放缓冲DC
	m_cacheDC.DeleteDC();
	//释放对象
	m_cacheCBitmap.DeleteObject();
	//释放窗口DC
	ReleaseDC(cDC);
}

//按键响应函数
void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//nChar表示按下的键值
	switch(nChar)
	{
	case 'd':         //游戏中按下的键当然应该不区分大小写了
	case 'D':
		MyHero.direct=RIGHT;
		MyHero.x+=5;
		break;
	case 'a':
	case 'A':
		MyHero.direct=LEFT;
		MyHero.x-=5;
		break;
	case 'w':
	case 'W':
		MyHero.direct=UP;
		MyHero.y-=5;
		break;
	case 's':
	case 'S':
		MyHero.direct=DOWN;
		MyHero.y+=5;
		break;
	}
}

//鼠标左键单击响应函数
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	char bufPos[50];
	sprintf(bufPos,"你单击了点X:%d,Y:%d",point.x,point.y);
	AfxMessageBox(bufPos);
}

//定时器响应函数
void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	
	switch(nIDEvent)
	{
	case TIMER_PAINT:OnPaint();break;  //若是重绘定时器，就执行OnPaint函数
	case TIMER_HEROMOVE:               //控制人物移动的定时器
		{
			MyHero.frame++;              //每次到了间隔时间就将图片换为下一帧
			if(MyHero.frame==4)          //到最后了再重头开始
				MyHero.frame=0;
		}
		break;
	}
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	//创建一个10毫秒产生一次消息的定时器
	SetTimer(TIMER_PAINT,10,NULL);
	//创建人物行走动画定时器
	SetTimer(TIMER_HEROMOVE,100,NULL);
	return 0;
}
