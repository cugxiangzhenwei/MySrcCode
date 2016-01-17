// HxyqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MeYe.h"
#include "HxyqDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHxyqDlg dialog


CHxyqDlg::CHxyqDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHxyqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHxyqDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHxyqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHxyqDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHxyqDlg, CDialog)
	//{{AFX_MSG_MAP(CHxyqDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHxyqDlg message handlers

void CHxyqDlg::OnOK() 
{
	// TODO: Add extra validation here
	
}
