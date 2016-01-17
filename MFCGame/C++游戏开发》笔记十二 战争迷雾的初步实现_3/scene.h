#ifndef _SCENE_H
#define _SCENE_H

class CScene
{
private:
	CImage m_bg;      //背景图片
	CImage m_black;	
	//每块迷雾大小为20*20,对于800*600的窗口即有40*30个小迷雾块组成
	int m_fogArray[40][30];
public:
	CScene(char *bg);
	~CScene();
public:
	//绘制背景
	void DrawBG(CDC &cDC);
	//绘制迷雾
	void DrawFog(CDC &cDC);
	//更新迷雾区域
	void UpdateFogArea(int x,int y);
};

#endif