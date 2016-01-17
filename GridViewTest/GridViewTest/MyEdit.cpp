// MyEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "GridViewTest.h"
#include "MyEdit.h"


// CMyEdit

IMPLEMENT_DYNAMIC(CMyEdit, CEdit)

CMyEdit::CMyEdit()
{

}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CMyEdit 消息处理程序



void CMyEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	// 向顶级父窗口即对话框发出输入焦点改变的通知
	(GetParent()->GetParent())->SendMessage(GRID_DATA_CHANGED,0);

	// TODO: 在此处添加消息处理程序代码
}
