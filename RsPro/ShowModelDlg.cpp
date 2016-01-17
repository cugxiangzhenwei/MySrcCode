// ShowModelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RsProc.h"
#include "ShowModelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowModelDlg dialog


CShowModelDlg::CShowModelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowModelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowModelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CShowModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowModelDlg)
	DDX_Control(pDX, IDC_TAB_SHOW_MODEL, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowModelDlg, CDialog)
	//{{AFX_MSG_MAP(CShowModelDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SHOW_MODEL, OnSelchangeTabShowModel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowModelDlg message handlers

BOOL CShowModelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_tab.InsertItem(0,"彩色显示");
	m_tab.InsertItem(1,"灰度显示");
	m_colordlg.Create(IDD_DIALOG_COLOR,GetDlgItem(IDC_TAB_SHOW_MODEL));
	m_greydlg.Create(IDD_DIALOG_GREY,GetDlgItem(IDC_TAB_SHOW_MODEL));
	CRect rect;
	m_tab.GetClientRect(&rect);
	rect.top+=30;
	m_colordlg.MoveWindow(&rect);
	m_greydlg.MoveWindow(&rect);
	m_colordlg.ShowWindow(TRUE);
	m_greydlg.ShowWindow(FALSE);
	m_tab.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShowModelDlg::OnSelchangeTabShowModel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int Cursel=m_tab.GetCurSel();
	switch (Cursel)
	{
	case 0:
		m_colordlg.ShowWindow(TRUE);
		m_greydlg.ShowWindow(FALSE);
		break;
	case 1:
		m_colordlg.ShowWindow(FALSE);
		m_greydlg.ShowWindow(TRUE);
	}
	*pResult = 0;
}

void CShowModelDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
}
