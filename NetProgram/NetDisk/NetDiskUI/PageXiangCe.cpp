// PageXiangCe.cpp : 实现文件
//

#include "stdafx.h"
#include "NetDiskUI.h"
#include "PageXiangCe.h"
#include "afxdialogex.h"


// CPageXiangCe 对话框

IMPLEMENT_DYNAMIC(CPageXiangCe, CDialogEx)

CPageXiangCe::CPageXiangCe(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageXiangCe::IDD, pParent)
{

}

CPageXiangCe::~CPageXiangCe()
{
}

void CPageXiangCe::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageXiangCe, CDialogEx)
END_MESSAGE_MAP()


// CPageXiangCe 消息处理程序


BOOL CPageXiangCe::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
