// DlgResLibTree.cpp : 实现文件
//

#include "stdafx.h"
#include "CugWorkSpace.h"
#include "DlgResLibTree.h"
#include "afxdialogex.h"


// CDlgResLibTree 对话框

IMPLEMENT_DYNAMIC(CDlgResLibTree, CDialogEx)

CDlgResLibTree::CDlgResLibTree(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgResLibTree::IDD, pParent)
{

}

CDlgResLibTree::~CDlgResLibTree()
{
}

void CDlgResLibTree::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgResLibTree, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDlgResLibTree 消息处理程序


BOOL CDlgResLibTree::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ResLibTree.ModifyStyleEx(WS_EX_CLIENTEDGE | NM_CUSTOMDRAW, 0);

	ModifyStyle(DS_3DLOOK, 0);
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgResLibTree::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient(3,5,cx,cy);
	if ( IsWindow(m_ResLibTree.GetSafeHwnd()))
	{
		m_ResLibTree.MoveWindow(&rcClient);
	}
}


int CDlgResLibTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	m_ResLibTree.Create("目录树",WS_CHILD|WS_VISIBLE,CRect(0,0,500,1000),this,IDC_RESLIB_TREE);
	return 0;
}
