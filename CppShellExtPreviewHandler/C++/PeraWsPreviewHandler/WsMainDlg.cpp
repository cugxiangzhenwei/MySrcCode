// WsMainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WsMainDlg.h"
#include "afxdialogex.h"


// CWsMainDlg 对话框

IMPLEMENT_DYNAMIC(CWsMainDlg, CDialogEx)

CWsMainDlg::CWsMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWsMainDlg::IDD, pParent)
{
}

CWsMainDlg::~CWsMainDlg()
{
}

void CWsMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WS_TITLE, m_StaticTitle);
	DDX_Control(pDX, IDC_STATIC_ZOOM_NUM, m_StaticZoomNum);
	DDX_Control(pDX, IDC_STATIC_WS_CONTAINER, m_WsContainer);
	DDX_Control(pDX, IDC_SLIDER_ZOOM, m_SliderZoom);
}


BEGIN_MESSAGE_MAP(CWsMainDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWsMainDlg 消息处理程序


void CWsMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_StaticTitle.m_hWnd)
	{
		m_StaticTitle.SetWindowPos(NULL,1,8,cx - 150,20,SWP_NOZORDER);
	}
	if (m_StaticZoomNum.m_hWnd)
	{
		m_StaticZoomNum.SetWindowPos(NULL,cx -150,8,150,20,SWP_NOZORDER);
	}
	if (m_WsContainer.m_hWnd)
	{
		m_WsContainer.SetWindowPos(NULL,1,30,cx-2,cy - 56,SWP_NOZORDER);
		if (m_MapCore.m_hWnd)
		{
			m_MapCore.SetWindowPos(NULL,1,30,cx-2,cy - 56,SWP_NOZORDER);
		}
	}
	if (m_SliderZoom.m_hWnd)
	{
		m_SliderZoom.SetWindowPos(NULL,1,cy - 19,cx-2,15,SWP_NOZORDER);
	}
}


BOOL CWsMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_MapCore.Create(L"MapCore",WS_VISIBLE|WS_CHILD,CRect(0,0,10,10),&m_WsContainer,5000);
	m_MapCore.ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
