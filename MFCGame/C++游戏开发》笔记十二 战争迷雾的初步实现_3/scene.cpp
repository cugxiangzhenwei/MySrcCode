#include"stdafx.h"
#include"scene.h"

CScene::CScene(char *bg)
{
	m_bg.Load(bg);
	m_black.Load("black.png");
	//将数组清0,0表示为黑色迷雾状态
	memset(m_fogArray,0,sizeof(m_fogArray));
}

//绘制背景
void CScene::DrawBG(CDC &cDC)
{
	m_bg.Draw(cDC,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
}

//绘制战争迷雾
void CScene::DrawFog(CDC &cDC)
{
	for(int i=0;i<40;i++)
		for(int j=0;j<30;j++)
		{
			if(m_fogArray[i][j]==0)
				m_black.Draw(cDC,i*20,j*20,20,20);
		}
}

bool CheckFog(int xBox,int yBox,int xMouse,int yMouse)
{
	//出界了返回false
	if(xBox<0 || xBox>=40 || yBox<0 || yBox>=30)
		return false;
	//未出界，则距离鼠标点击中心小于一定的范围内可见
	if( (xBox-xMouse)*(xBox-xMouse) + (yBox-yMouse)*(yBox-yMouse) <=16)
		return true;
	else return false;
}
//更新迷雾区域
void CScene::UpdateFogArea(int x,int y)
{
	//首先计算出鼠标所在的格子
	int xPosBox=x/20;
	int yPosBox=y/20;
	//将迷雾区域复原
	memset(m_fogArray,0,sizeof(m_fogArray));
	//设置可见区域
	for(int xBox=xPosBox-8;xBox<xPosBox+8;xBox++)
	{
		for(int yBox=yPosBox-8;yBox<yPosBox+8;yBox++)
		{
			if(CheckFog(xBox,yBox,xPosBox,yPosBox))
				m_fogArray[xBox][yBox]=1;
		}
	}
}

