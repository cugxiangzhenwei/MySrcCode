// MineListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "PeraSqlTool.h"
#include "MineListBox.h"


// CMineListBox

IMPLEMENT_DYNAMIC(CMineListBox, CListBox)

	CMineListBox::CMineListBox()
{
	m_nMaxWidth   =   0;
}

CMineListBox::~CMineListBox()
{
}


BEGIN_MESSAGE_MAP(CMineListBox, CListBox)
END_MESSAGE_MAP()



// CMineListBox 消息处理程序


int CMineListBox::AddString(LPCTSTR lpszItem)
{
	int   nRet   =   CListBox::AddString(lpszItem);
	SCROLLINFO   scrollInfo;  
	memset(&scrollInfo,   0,   sizeof(SCROLLINFO));  
	scrollInfo.cbSize   =   sizeof(SCROLLINFO);  
	scrollInfo.fMask     =   SIF_ALL;  
	GetScrollInfo(SB_VERT,   &scrollInfo,   SIF_ALL); 
	int   nScrollWidth   =   0;  
	if(GetCount()> 1 && ((int)scrollInfo.nMax >=   (int)scrollInfo.nPage))  
	{  
		nScrollWidth   =   GetSystemMetrics(SM_CXVSCROLL);  
	}
	SIZE             sSize;  
	CClientDC   myDC(this);
	CFont*   pListBoxFont   =   GetFont();  
	if(pListBoxFont   !=   NULL)  
	{  
		CFont*   pOldFont   =    
			myDC.SelectObject(pListBoxFont); 

		GetTextExtentPoint32(myDC.m_hDC, lpszItem,   strlen(lpszItem),   &sSize);  
		m_nMaxWidth   =   max(m_nMaxWidth,   (int)sSize.cx);
		SetHorizontalExtent(m_nMaxWidth   +   3); 
		myDC.SelectObject(pOldFont);  
	}

	return   nRet; 
}
int CMineListBox::DeleteString(UINT nIndex)
{
	 return CListBox::DeleteString(nIndex); 
}
int CMineListBox::InsertString(int nIndex, LPCTSTR lpszItem)
{
	 return CListBox::InsertString(nIndex,lpszItem); 
}
void CMineListBox::ResetContent()
{
	m_nMaxWidth   =   0;  
	SetHorizontalExtent(0);
	CListBox::ResetContent();
}