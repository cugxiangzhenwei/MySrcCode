#include "stdafx.h"
#include "GameCtrler.h"
#include "BlockManager.h"
CGameCtrler::CGameCtrler(CWnd *pMainWnd)
{
	m_ptCFirstClick = CPoint(-1,-1);
	m_pMainWnd = pMainWnd;
	m_iClickCount = 0;
}
CGameCtrler::~CGameCtrler(void)
{

}
void CGameCtrler::ResetClickCount()
{
	m_iClickCount = 0;
}
BOOL CGameCtrler::MouseClick(CPoint ptInPicWnd)
{
	m_iClickCount++;
	if (m_iClickCount==1)
	{
		m_ptCFirstClick = ptInPicWnd;
	}
	else if (m_iClickCount==2)
	{
		SwapBlock(ptInPicWnd);
		ResetClickCount(); // 点击两次后归零
		m_ptCFirstClick = CPoint(-1,-1);
	}
	return CheckFinish();
}
BOOL CGameCtrler::CheckFinish()
{
	map<int,CPictueBlock> & blkMap =  CBlockManager::GetInstance()->m_BlockMap;
	BOOL bok = TRUE;
	map<int,CPictueBlock>::iterator iter = blkMap.begin();
	for (;iter!=blkMap.end();iter++)
	{
		if (iter->second.m_iCurPosID != iter->second.m_iPosID)
			return FALSE;
	}

	return TRUE;
}
void CGameCtrler::SwapBlock(CPoint ptSecondClick)
{
	int iFirstID  = GetClickBlockID(m_ptCFirstClick);
	int iSecondID = GetClickBlockID(ptSecondClick);
	if (iSecondID== -1 || iSecondID == -1)
		return;

	if (!IsNeighbourBlock(iFirstID,iSecondID))
	{
		MessageBox(m_pMainWnd->GetSafeHwnd(),"亲，只能交换相邻的图片哦~","提示",MB_OK|MB_ICONINFORMATION);
		return;
	}

	 CBlockManager::GetInstance()->SwapBlock(iFirstID,iSecondID);
	 OutputDebugString("交换一次\n");
}
int CGameCtrler::GetClickBlockID(const CPoint & pt)
{
	map<int,CPictueBlock> & blkMap =  CBlockManager::GetInstance()->m_BlockMap; 
	map<int,CPictueBlock>::iterator iter = blkMap.begin();
	for (;iter!=blkMap.end();iter++)
	{
		if (iter->second.m_RectBlock.PtInRect(pt))
			return iter->second.m_iCurPosID;
	}

	return -1;
}
void CGameCtrler::GetNeighbourBloks(const int &iId1,vector<int> & vNeighbours)
{
	int & iRowCount = CBlockManager::GetInstance()->m_iRowBlockCount;
	int & iColCount = CBlockManager::GetInstance()->m_iColBlockCount;
	int iCurRow = int(iId1/iRowCount);
	int iCurCol = iId1%iColCount;

	if (iCurRow-1 >=0) // 上邻居
	{
		int iTop = (iCurRow-1)*iColCount + iCurCol;
		vNeighbours.push_back(iTop);
	}

	if (iCurRow+1 < iRowCount) // 下邻居
	{
		int iBottom = (iCurRow+1)*iColCount + iCurCol;
		vNeighbours.push_back(iBottom);
	}

	if (iCurCol-1 >=0 ) // 左邻居
	{
		int iLeft = iCurRow*iColCount + (iCurCol-1);
		vNeighbours.push_back(iLeft);
	}

	if (iCurCol+1 < iColCount ) // 右邻居
	{
		int iRight = iCurRow*iColCount + (iCurCol+1);
		vNeighbours.push_back(iRight);
	}
}
BOOL CGameCtrler::IsNeighbourBlock(const int &id1,const int & id2)
{
	vector<int> vNeighbours;
	GetNeighbourBloks(id1,vNeighbours);
	for (size_t i=0;i<vNeighbours.size();i++)
	{
		if (vNeighbours[i] == id2)
			return TRUE;
	}
	return FALSE;
}