
//-----------------------------------【程序说明】----------------------------------------------
// 【MFC游戏开发】笔记九 碰撞检测和运动型AI 配套源代码
// VS2010环境
// 更多内容请访问雾央CSDN博客 http://blog.csdn.net/u011371356/article/category/1497651
// 雾央的新浪微博： @七十一雾央
//------------------------------------------------------------------------------------------------


// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "GameMFC.h"
#include "ChildView.h"

#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")//导入声音头文件库

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
	mciSendString("stop bgMusic ",NULL,0,NULL);
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
	m_bg.Load("bg.png");
	//加载英雄图片
	MyHero.character.Load("heroMove.png");
	TransparentPNG(&MyHero.character);
	
	MyHero.width=80;
	MyHero.height=80;
	//初始化英雄状态
	MyHero.direct=UP;
	MyHero.frame=0;
	//设置英雄初始位置
	MyHero.x=80;    
	MyHero.y=400;

	//加载怪物
	Monster.character.Load("monster.png");
	TransparentPNG(&Monster.character);
	Monster.width=96;
	Monster.height=96;
	Monster.direct=LEFT;
	Monster.frame=0;
	Monster.x=700;
	Monster.y=100;

	//打开音乐文件
	mciSendString("open background.mp3 alias bgMusic ", NULL, 0, NULL);
	mciSendString("play bgMusic repeat", NULL, 0, NULL);

	return TRUE;
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

	//————————————————————开始绘制——————————————————————
	//贴背景,现在贴图就是贴在缓冲DC：m_cache中了
	m_bg.Draw(m_cacheDC,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
	//贴英雄
	MyHero.character.Draw(m_cacheDC,MyHero.x,MyHero.y,80,80,
								MyHero.frame*80,MyHero.direct*80,80,80);
	//贴怪物
	Monster.character.Draw(m_cacheDC,Monster.x,Monster.y,96,96,
								Monster.frame*96,Monster.direct*96,96,96);
	//怪物状态更新
	//水平方向上靠近
	if(Monster.x<MyHero.x)
	{
		Monster.x++;
		Monster.direct=RIGHT;
	}
	else if(Monster.x>MyHero.x)
	{
		Monster.x--;
		Monster.direct=LEFT;
	}
	//竖直方向上靠近
	if(Monster.y<MyHero.y)
		Monster.y++;
	else 
		Monster.y--;

	//判断是否碰撞
	MyHero.Xcenter=MyHero.x+MyHero.width/2;
	MyHero.Ycenter=MyHero.y+MyHero.height/2;
	Monster.Xcenter=Monster.x+Monster.width/2;
	Monster.Ycenter=Monster.y+Monster.height/2;
	//设置文字背景透明
	m_cacheDC.SetBkMode(TRANSPARENT);
	//设置文字为红色
	m_cacheDC.SetTextColor(RGB(255,0,0));

	//假定我们将英雄作为图中的黑色矩形
	if(Monster.Xcenter< MyHero.Xcenter+(MyHero.width/2+Monster.width/2) &&
       Monster.Xcenter> MyHero.Xcenter-(MyHero.width/2+Monster.width/2) &&
	   Monster.Ycenter< MyHero.Ycenter+(MyHero.height/2+Monster.height/2) &&
       Monster.Ycenter> MyHero.Ycenter-(MyHero.height/2+Monster.height/2) )
	   m_cacheDC.TextOut(0,0,"发生碰撞");   //在窗口左上角显示碰撞信息
	else
	   m_cacheDC.TextOut(0,0,"没有碰撞");

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
			Monster.frame++;
			if(Monster.frame==4)         //以后雾央会对代码进行封装，就不会像这里这么重复了。
				Monster.frame=0;
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
