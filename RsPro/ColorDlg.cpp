// ColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RsProc.h"
#include "ColorDlg.h"
#include "RsProcDoc.h"
#include "RsProcView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog


CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorDlg)
	//}}AFX_DATA_INIT
	
	pdoc=NULL;
	width=0;
	height=0;
	bandnum=0;
	m_sel_r=0;
	m_sel_g=0;
	m_sel_b=0;
}


void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorDlg)
	DDX_Control(pDX, IDC_COMBO_R, m_combo_r);
	DDX_Control(pDX, IDC_COMBO_G, m_combo_g);
	DDX_Control(pDX, IDC_COMBO_B, m_combo_b);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
	//{{AFX_MSG_MAP(CColorDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_R, OnSelchangeComboR)
	ON_CBN_SELCHANGE(IDC_COMBO_G, OnSelchangeComboG)
	ON_CBN_SELCHANGE(IDC_COMBO_B, OnSelchangeComboB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers

BOOL CColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMDIFrameWnd *pMainFrame=(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	if (pMainFrame!=NULL)
	{
		CMDIChildWnd *pChildFrame=(CMDIChildWnd*)pMainFrame->GetActiveFrame();
		if (pChildFrame!=NULL)
		{
			CRsProcView *pView=(CRsProcView*)pChildFrame->GetActiveView();
			if (pView!=NULL)
			{
			  pdoc=pView->GetDocument();
			}
		}
	}
	if (pdoc!=NULL)
	{
     	bandnum=pdoc->m_nbands;
		width=pdoc->m_nWidth;
		height=pdoc->m_nHeigth;

		CString strText1="";
		CString strText2="";
		strText1.Format("Ó°Ïñ²¨¶ÎÊýÎª: %d",bandnum);
	    SetDlgItemText(IDC_STATIC_BAND_NUM,strText1);
		for (int i=0;i<bandnum;i++)
		{
			strText2.Format("²¨¶Î%d",i+1);
			m_combo_r.AddString(strText2);
			m_combo_g.AddString(strText2);
			m_combo_b.AddString(strText2);
		}
		if (bandnum==1)
		{
			m_combo_r.EnableWindow(FALSE);
			m_combo_g.EnableWindow(FALSE);
			m_combo_b.EnableWindow(FALSE);
		}
		else
		{
			m_combo_r.SetCurSel(0);
			m_combo_g.SetCurSel(1);
			m_combo_b.SetCurSel(2);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CColorDlg::OnSelchangeComboR() 
{
	// TODO: Add your control notification handler code here
	int Cursel=m_combo_r.GetCurSel();
	switch (Cursel)
	{
	case 0:
		m_sel_r=0;
		break;
	case 1:
		m_sel_r=1;
		break;
	case 2:
		m_sel_r=2;
		break;
	default:
		m_sel_r=0;
		break;
	}
	pdoc->pbands[m_sel_r]->RasterIO(GF_Read,0,0,width,height,pdoc->pBandBlock[0],
		width,height,pdoc->pbands[m_sel_r]->GetRasterDataType(),0,0);
	pdoc->UpdateAllViews(NULL);
}

void CColorDlg::OnSelchangeComboG() 
{
	// TODO: Add your control notification handler code here
	int Cursel=m_combo_g.GetCurSel();
	switch (Cursel)
	{
	case 0:
		m_sel_g=0;
		break;
	case 1:
		m_sel_g=1;
		break;
	case 2:
		m_sel_g=2;
		break;
	default:
		m_sel_g=0;
		break;
	}
	pdoc->pbands[m_sel_g]->RasterIO(GF_Read,0,0,width,height,pdoc->pBandBlock[1],
		width,height,pdoc->pbands[m_sel_g]->GetRasterDataType(),0,0);
	pdoc->UpdateAllViews(NULL);
}

void CColorDlg::OnSelchangeComboB() 
{
	// TODO: Add your control notification handler code here
	int Cursel=m_combo_b.GetCurSel();
	switch (Cursel)
	{
	case 0:
		m_sel_b=0;
		break;
	case 1:
		m_sel_b=1;
		break;
	case 2:
		m_sel_b=2;
		break;
	default:
		m_sel_b=0;
		break;
	}
	pdoc->pbands[m_sel_b]->RasterIO(GF_Read,0,0,width,height,pdoc->pBandBlock[2],
		width,height,pdoc->pbands[m_sel_b]->GetRasterDataType(),0,0);
	pdoc->UpdateAllViews(NULL);
}

