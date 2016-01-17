#pragma once
#include <vector>
using namespace  std;

class CGameCtrler
{
public:
	CGameCtrler(CWnd *pMainWnd);
	~CGameCtrler(void);
	BOOL MouseClick(CPoint ptInPicWnd);
	BOOL CheckFinish();
	void ResetClickCount();
	int GetClickBlockID(const CPoint &);
private:
	CPoint m_ptCFirstClick;
	CWnd *m_pMainWnd;
	int   m_iClickCount;
private:
	void SwapBlock(CPoint ptSecondClick); // 鼠标第二次点击时交换图片
	BOOL IsNeighbourBlock(const int &id1,const int & id2);
	void GetNeighbourBloks(const int &iId1,vector<int> & );
};

