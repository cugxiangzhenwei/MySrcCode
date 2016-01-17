// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "7zClientUI.h"
#include "CLientUIMainDlg.h"
#include "afxdialogex.h"


// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CCLientUIMainDlg, CDialogEx)

CCLientUIMainDlg::CCLientUIMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCLientUIMainDlg::IDD, pParent)
{

}

CCLientUIMainDlg::~CCLientUIMainDlg()
{
}

void CCLientUIMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PAGE, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CCLientUIMainDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PAGE, &CCLientUIMainDlg::OnTcnSelchangeTabPage)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainDlg 消息处理程序


BOOL CCLientUIMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_TabCtrl.InsertItem(0,"压缩"); 
	m_TabCtrl.InsertItem(1,"解压"); 

	m_TabCtrl.SetMinTabWidth(77);
	m_TabCtrl.SetItemSize(CSize(77,22));
	//建立各页 
	m_ComDlg.Create(m_ComDlg.IDD,&m_TabCtrl); 
	m_DComDlg.Create(m_DComDlg.IDD,&m_TabCtrl); 

	//设置页面的位置在m_tab控件范围内 
	CRect rect; 
	GetClientRect(rect);
	m_TabCtrl.MoveWindow(rect);
	m_TabCtrl.GetClientRect(&rect); 
	rect.top+=23; 

	rect.InflateRect(-1,-5,-1,-1);
	m_ComDlg.MoveWindow(&rect);
	m_DComDlg.MoveWindow(&rect); 
	m_ComDlg.ShowWindow(SW_SHOW); 
	m_DComDlg.ShowWindow(SW_HIDE);
	m_TabCtrl.SetCurSel(0);
	//m_ComDlg.SetFocus();
	return TRUE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCLientUIMainDlg::OnTcnSelchangeTabPage(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int CurSel; 
	CurSel=m_TabCtrl.GetCurSel(); 
	switch(CurSel) 
	{ 
	case 0: 
		m_ComDlg.ShowWindow(SW_SHOW); 
		m_DComDlg.ShowWindow(SW_HIDE); 
		break; 
	case 1: 
		m_ComDlg.ShowWindow(SW_HIDE); 
		m_DComDlg.ShowWindow(SW_SHOW); 
		break; 
	default: ; 
	} 

	*pResult = 0; 
}


void CCLientUIMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_TabCtrl.m_hWnd)
	{
		m_TabCtrl.MoveWindow(0,0,cx,cy);
	}
}


BOOL CCLientUIMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if (pMsg->wParam == VK_RETURN)
			return TRUE;
		else if (pMsg->wParam == VK_SPACE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
