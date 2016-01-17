// MyListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "MyListBox.h"


// CMyListBox

IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{
	m_nMaxWidth   =   0;  
}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
END_MESSAGE_MAP()



// CMyListBox 消息处理程序

int   CMyListBox::AddString(LPCTSTR   lpszItem)  
{  
	//此处加入字符串宽度跟踪、水平滚动条显示等代码  

	int   nRet   =   CListBox::AddString(lpszItem);  

	/*	----   接下来调用GetScrollInfo()以获得垂直滚动条的相关信息。这些信息是  
	通过一个SCROLLINFO结构传递的，下面是对该结构初始化并调用  
	GetScrollInfo()的代码：   */ 

	SCROLLINFO   scrollInfo;  
	memset(&scrollInfo,   0,   sizeof(SCROLLINFO));  
	scrollInfo.cbSize   =   sizeof(SCROLLINFO);  
	scrollInfo.fMask     =   SIF_ALL;  
	GetScrollInfo(SB_VERT,   &scrollInfo,   SIF_ALL);  

	/*----   在调试器内观察SCROLLINFO，可以发现要获得nMax和nPage的正确数值，  
	列表框至少应含有一个字符串。SCROLLINFO的成员nPage保存了列表框“每页”  
	能够显示的项目数，nMax是列表框内项目总数。当nMax大于或等于nPage，就  
	出现了垂直滚动条。我们需要知道垂直滚动条的宽度以正确计算列表框的有效  
	显示宽度。这里使用一个初始值为0的整数nScrollWidth表示，并在垂直滚动条  
	显示时将它赋值：    */

	int   nScrollWidth   =   0;  
	if(GetCount() >1 && ((int)scrollInfo.nMax >= (int)scrollInfo.nPage))  
	{  
		nScrollWidth   =   GetSystemMetrics(SM_CXVSCROLL);  
	}  

	/*----   接下来声明一个SIZE变量sSize，并实例化对话框的CClientDC：    */

	SIZE             sSize;  
	CClientDC   myDC(this);  

	/*----   对话框所采用的字体，有可能是缺省字体，也有可能是有目的的选择。  
	在对话框编辑器中右击对话框，并选择Properties可以查看当前值。虽然  
	MyDC是从列表框取得的，但列表框字体信息并未包含在MyDC中。也就是说，  
	对话框创建时所用字体并没有“选入”CClientDC。要从  
	GetTextExtentPoint32()获得真正的字符串大小，应该先调用GetFont()获  
	得列表框的字体信息，然后将此字体选入MyDC，代码为：    */

	CFont*   pListBoxFont   =   GetFont();  
	if(pListBoxFont   !=   NULL)  
	{  
		CFont*   pOldFont   =    
			myDC.SelectObject(pListBoxFont);  

		/*----   现在可以调用GetTextExtendPoint32()函数来获得字符串的宽度了。  
		字符串的宽度由sSize结构的cx成员返回，将该值和已有最大宽度相比较：*/    

		GetTextExtentPoint32(myDC.m_hDC,    
			lpszItem,   strlen(lpszItem),   &sSize);  
		m_nMaxWidth   =   max(m_nMaxWidth,   (int)sSize.cx);  

		/*----   剩下的重要工作之一，就是设置水平滚动条的大小了。这可以通过调  
		用SetHorizontalExtent()完成。如果传递给它的整形参数比列表框本身宽度小，  
		则水平滚动条被隐藏。    

		----   这里有一个容易被忽略的地方。如果仔细观察CListBox，可以发现文  
		本左边有一栏小小的空白，它的大小为3   。这部分宽度应该加到文本宽度  
		上。如果希望在文本右边也同样空出一栏，则可以在文本宽度上再加3。 */   

		SetHorizontalExtent(m_nMaxWidth   +   3);  

		/*----   在结束之前，我们需要为MyDC选入原有字体。原有字体保存在pOldFont  
		中：   */ 

		myDC.SelectObject(pOldFont);  
	}  

	return   nRet;  
}  
void   CMyListBox::ResetContent()  
{  
	CListBox::ResetContent();  

	m_nMaxWidth   =   0;  
	SetHorizontalExtent(0);  
}   

