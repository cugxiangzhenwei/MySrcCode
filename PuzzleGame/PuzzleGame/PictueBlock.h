#pragma once
class CPictueBlock
{
public:
	CPictueBlock(int iPosID,int iCurPosID,CRect PicturRect);
	~CPictueBlock(void);
	virtual void DrawBlock(CDC & dcAllBlock);
public:
	int m_iPosID;   // 正确的位置编号
	int m_iCurPosID; // 实际位置编号
	CRect m_RectBlock; //数据块在整个绘图区中的位置
};

