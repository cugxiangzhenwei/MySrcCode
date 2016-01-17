#ifndef _SCENE_H
#define _SCENE_H

class CScene
{
private:
	CImage m_bg;      //背景图片
	CImage m_black[2];	
	int m_mode;  //显示模式
	//每块迷雾大小为128*128,对于1280*640的窗口即有10*5个小迷雾块组成
	int m_fogArray[11][6];
	//每块是否被点击过
	int m_clickArray[11][6];
public:
	CScene(char *bg);
	~CScene();
public:
	void ChangeMode();
	//绘制背景
	void DrawBG(CDC &cDC);
	//绘制迷雾
	void DrawFog(CDC &cDC);
	//更新迷雾区域
	void UpdateFogArea(int x,int y);
};

#endif