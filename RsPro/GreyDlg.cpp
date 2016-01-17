// GreyGlg.cpp : implementation file
//

#include "stdafx.h"
#include "RsProc.h"
#include "GreyDlg.h"
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
// CGreyDlg dialog


CGreyDlg::CGreyDlg(CWnd* pParent /*=NULL*/)
 :CDialog(CGreyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGreyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pdoc=NULL;
	m_sel_band=0;
}


void CGreyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGreyDlg)
	DDX_Control(pDX, IDC_COMBO_GREY, m_combo_grey);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGreyDlg, CDialog)
	//{{AFX_MSG_MAP(CGreyDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_GREY, OnSelchangeComboGrey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGreyDlg message handlers

BOOL CGreyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//CRsProcDoc *pdoc=NULL;
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
		int bandnum;
		bandnum=pdoc->m_nbands;
		CString strText1="";
		CString strText2="";
		strText1.Format("Ó°Ïñ²¨¶ÎÊýÎª: %d",bandnum);
		SetDlgItemText(IDC_STATIC_BAND_NUM,strText1);
		for (int i=0;i<bandnum;i++)
		{
			strText2.Format("²¨¶Î%d",i+1);
			m_combo_grey.AddString(strText2);
		}
		m_combo_grey.SetCurSel(0);
	}
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGreyDlg::OnSelchangeComboGrey() 
{
	// TODO: Add your control notification handler code here
	if (pdoc!=NULL)
	{
		int CurSel=m_combo_grey.GetCurSel();
		switch (CurSel)
		{
		case 0:
			m_sel_band=0;
			break;
		case 1:
			m_sel_band=1;
			break;
		case 2:
			m_sel_band=2;
			break;
		default:
			m_sel_band=0;
			break;
		}
		for (int i=0;i<pdoc->m_nbands;i++)
		{
		    pdoc->pBandBlock[i]=(BYTE*)CPLMalloc(sizeof(BYTE)*pdoc->m_nHeigth*pdoc->m_nWidth);
			pdoc->pbands[m_sel_band]->RasterIO(GF_Read,0,0,pdoc->m_nWidth,pdoc->m_nHeigth,
			pdoc->pBandBlock[i],pdoc->m_nWidth,pdoc->m_nHeigth,pdoc->pbands[m_sel_band]->GetRasterDataType(),0,0);
		}
		pdoc->UpdateAllViews(NULL);
	}
}

