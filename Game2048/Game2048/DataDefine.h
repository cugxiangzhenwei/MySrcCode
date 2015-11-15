#pragma once
#include <map>
using namespace std;

class CGameNumBlock
{
public:
	int m_iBlockID;
	int m_iGameNumShow;
	CRect m_RectBlock;
	int   m_iFlashCount;
	static int GetUpBlockID(int iCur);
	static int GetDownBlockID(int iCur);
	static int GetLeftBlockID(int iCur);
	static int GetRightBlockID(int iCur);
};

extern COLORREF g_ColorNullBlock;
extern COLORREF g_ColorBackGround;
extern map<int,COLORREF> g_mapColor;