#include "stdafx.h"
#include "DataDefine.h"

COLORREF g_ColorNullBlock = RGB(205,194,181);
COLORREF g_ColorBackGround = RGB(189,174,156);
map<int,COLORREF> g_mapColor;

int CGameNumBlock::GetUpBlockID(int iCur)
{
	int iRow   = (iCur / 4);
	int iColum = (iCur % 4);
	if (iRow==0)
		return -1;

	int iID = (iRow - 1)*4 + iColum;
	return iID;
}
int CGameNumBlock::GetDownBlockID(int iCur)
{
	int iRow   = (iCur / 4);
	int iColum = (iCur % 4);
	if (iRow==3)
		return -1;

	int iID = (iRow + 1)*4 + iColum;
	return iID;
}
int CGameNumBlock::GetLeftBlockID(int iCur)
{
	int iRow   = (iCur / 4);
	int iColum = (iCur % 4);
	if (iColum==0)
		return -1;

	int iID = iRow *4 + iColum-1;
	return iID;
}
int CGameNumBlock::GetRightBlockID(int iCur)
{
	int iRow   = (iCur / 4);
	int iColum = (iCur % 4);
	if (iColum==3)
		return -1;

	int iID = iRow *4 + iColum+1;
	return iID;
}