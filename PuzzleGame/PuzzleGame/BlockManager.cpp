#include "StdAfx.h"
#include "BlockManager.h"

CBlockManager * CBlockManager::m_pManager = NULL;
CBlockManager::GabageCollector CBlockManager::m_gc;

CBlockManager::CBlockManager(void)
{
	m_BlockMap.clear();
	m_iRowBlockCount = 0;
	m_iColBlockCount = 0;
	m_WndPictureRect.SetRectEmpty();
}


CBlockManager::~CBlockManager(void)
{
	Clear();
}
CBlockManager * CBlockManager::GetInstance()
{
	if(m_pManager==NULL)
		m_pManager = new CBlockManager;

	return m_pManager;
}
void CBlockManager::Clear()
{
	m_BlockMap.clear();
	m_iRowBlockCount = 0;
	m_iColBlockCount = 0;
	m_WndPictureRect.SetRectEmpty();
	if (m_MemPictureAll.GetSafeHdc())
	{
		m_MemPictureAll.DeleteDC();
	}
}
BOOL CBlockManager::IntilizeBlockWithRecord(const CRect & rectPictureWnd,int iRow,int iCol,const vector<int> & vIDs)
{
	m_WndPictureRect = rectPictureWnd;
	int iBlockH = m_WndPictureRect.Height() / iRow;
	int iBlockW = m_WndPictureRect.Width() / iRow;

	m_iRowBlockCount = iRow;
	m_iColBlockCount = iCol;

	for (int iR=0;iR < iRow;iR++)
	{
		for (int iC =0;iC < iCol;iC++)
		{
			int iID = iR*iRow + iC;
			int iCurPosID = vIDs[iID];
			int iCurRow   = int(iCurPosID / m_iColBlockCount);
			int iCurCol   = iCurPosID%m_iColBlockCount;

			CPictueBlock blk = CPictueBlock(iID,iCurPosID,CRect(CPoint(iCurCol*iBlockW,iCurRow*iBlockH),CSize(iBlockW,iBlockH)));
			m_BlockMap.insert(make_pair(iID,blk));
		}
	}
	return TRUE;
}
 BOOL CBlockManager::IntilizeBlock(const CRect & rectPictureWnd,int iRow,int iCol)
 {
	 m_WndPictureRect = rectPictureWnd;
	 int iBlockH = m_WndPictureRect.Height() / iRow;
	 int iBlockW = m_WndPictureRect.Width() / iRow;

	 m_iRowBlockCount = iRow;
	 m_iColBlockCount = iCol;

	 srand(time(NULL));
	 vector<int> PrimePosIDList; // 初始筛选顺序
	 for (int i=0;i<m_iRowBlockCount*m_iColBlockCount;i++)
		 PrimePosIDList.push_back(i);

	 for (int iR=0;iR < iRow;iR++)
	 {
		 for (int iC =0;iC < iCol;iC++)
		 {
			 int iID = iR*iRow + iC;
			 int iInditify = rand()%PrimePosIDList.size();
			 int iCurPosID = PrimePosIDList[iInditify];
			 int iCurRow   = int(iCurPosID / m_iColBlockCount);
			 int iCurCol   = iCurPosID%m_iColBlockCount;
			 PrimePosIDList.erase(PrimePosIDList.begin() + iInditify);

			 CPictueBlock blk = CPictueBlock(iID,iCurPosID,CRect(CPoint(iCurCol*iBlockW,iCurRow*iBlockH),CSize(iBlockW,iBlockH)));
			 m_BlockMap.insert(make_pair(iID,blk));
		 }
	 }
	 return TRUE;
 }
 void CBlockManager::DrawAllBlock(CDC & dc)
 {
	 map<int,CPictueBlock>::iterator iter = m_BlockMap.begin();
	 for (; iter!=m_BlockMap.end(); iter++)
	 {
		 CPictueBlock & picBlock = iter->second;
		 picBlock.DrawBlock(dc);
	 }
 }
 void CBlockManager::DrawGridLine(CDC & dc)
 {
	 int iBlockH = m_WndPictureRect.Height() / m_iRowBlockCount;
	 int iBlockW = m_WndPictureRect.Width() / m_iColBlockCount;

	 int iXMax = m_WndPictureRect.Width();
	 int iYmax = m_WndPictureRect.Height();
	 for (int iY=0; iY <m_iRowBlockCount;iY++) // 画横线
	 {
		 dc.MoveTo(0,iY*iBlockH);
		 dc.LineTo(iXMax,iY*iBlockH);
	 }

	 for (int iX=0; iX <m_iColBlockCount;iX++) // 画竖线
	 {
		 dc.MoveTo(iX*iBlockW,0);
		 dc.LineTo(iX*iBlockW,iYmax);
	 }

 }
 BOOL  CBlockManager::LoadPuzzlePicture(const WCHAR * pszImg,CDC &dc)
 {
	 CDC * pDC = CDC::FromHandle(dc.GetSafeHdc());
	 m_MemPictureAll.CreateCompatibleDC(pDC); //创建设备兼容的内存DC
	 CBitmap bmp;
	 //创建与设备兼容的位图，大小同客户区
	 bmp.CreateCompatibleBitmap(pDC,m_WndPictureRect.Width(),m_WndPictureRect.Height());
	 m_MemPictureAll.SelectObject(&bmp); //将位图选入内存设备

	 Graphics g(m_MemPictureAll.GetSafeHdc());
	 Gdiplus::Image * pImage = Image::FromFile(pszImg);
	 Gdiplus::Rect recGid(0,0,m_WndPictureRect.Width(),m_WndPictureRect.Height());
	 g.DrawImage(pImage,recGid,0,0,pImage->GetWidth(),pImage->GetHeight(),Gdiplus::UnitPixel);
	 return TRUE;
 }
 BOOL  CBlockManager::GetBlockPicture(int iID,CDC & dcAllBlock)
 {
	map<int,CPictueBlock>::iterator iter = m_BlockMap.find(iID);
	if (iter==m_BlockMap.end()) return FALSE;
	int iRow = iID / m_iColBlockCount; //图片所在块的行号
	int iCol = iID % m_iColBlockCount; //图片所在块的列号
	int iBlockH = m_WndPictureRect.Height() / m_iRowBlockCount;
	int iBlockW = m_WndPictureRect.Width() / m_iColBlockCount;
	int iSrcX = iBlockW * iCol;
	int iSrcY = iBlockH * iRow;
	CRect & rectBlock = iter->second.m_RectBlock;
	dcAllBlock.BitBlt(rectBlock.left,rectBlock.top,rectBlock.Width(),rectBlock.Height(),&m_MemPictureAll,iSrcX,iSrcY,SRCCOPY);
	return TRUE;
 }
 void CBlockManager::SwapBlock(const int &iID1,const int &iID2)
 {
	 map<int,CPictueBlock>::iterator iterBlk1 = m_BlockMap.begin();
	 for (;iterBlk1!=m_BlockMap.end();iterBlk1++)
	 {
		 if (iterBlk1->second.m_iCurPosID == iID1)
			 break;

	 }
	 if (iterBlk1==m_BlockMap.end()) return;

	 map<int,CPictueBlock>::iterator iterBlk2 = m_BlockMap.begin();
	 for (;iterBlk2!=m_BlockMap.end();iterBlk2++)
	 {
		 if (iterBlk2->second.m_iCurPosID == iID2)
			 break;
	 }
	 if (iterBlk2==m_BlockMap.end()) return;

	 CPictueBlock & Blk1 = iterBlk1->second;
	 CPictueBlock & blk2 =  iterBlk2->second;
	 std::swap(Blk1.m_iCurPosID,blk2.m_iCurPosID);
	 std::swap(Blk1.m_RectBlock,blk2.m_RectBlock);
 }