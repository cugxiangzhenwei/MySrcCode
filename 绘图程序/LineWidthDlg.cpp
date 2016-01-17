// LineWidthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "»æÍ¼³ÌÐò.h"
#include "LineWidthDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineWidthDlg dialog


CLineWidthDlg::CLineWidthDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineWidthDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineWidthDlg)
	m_nLineWidth = 0;
	//}}AFX_DATA_INIT
}


void CLineWidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineWidthDlg)
	DDX_Text(pDX, IDC_EDIT_LINEWIDTH, m_nLineWidth);
	DDV_MinMaxInt(pDX, m_nLineWidth, 0, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineWidthDlg, CDialog)
	//{{AFX_MSG_MAP(CLineWidthDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineWidthDlg message handlers
