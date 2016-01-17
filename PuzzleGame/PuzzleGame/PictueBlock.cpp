#include "StdAfx.h"
#include "PictueBlock.h"
#include "BlockManager.h"

CPictueBlock::CPictueBlock(int iPosID,int iCurPosID,CRect PicturRect)
{
	m_iPosID = iPosID;
	m_iCurPosID = iCurPosID;
	m_RectBlock = PicturRect;
}
CPictueBlock::~CPictueBlock(void)
{
}
void CPictueBlock::DrawBlock(CDC &dcAllBlock)
{
	CBlockManager::GetInstance()->GetBlockPicture(m_iPosID,dcAllBlock);
}