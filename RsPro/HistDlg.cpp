// HistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RsProc.h"
#include "HistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistDlg dialog


CHistDlg::CHistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pdoc=NULL;
	m_sel_band=0;
}


void CHistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistDlg, CDialog)
	//{{AFX_MSG_MAP(CHistDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_R, OnRadioR)
	ON_BN_CLICKED(IDC_RADIO_G, OnRadioG)
	ON_BN_CLICKED(IDC_RADIO_B, OnRadioB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistDlg message handlers

void CHistDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (pdoc==NULL)
	{
		return;
	}
	CPen *pBlackPen=new CPen;
	CPen *pRGBPen=new CPen;
	CPen *pOldPen;
	pBlackPen->CreatePen(PS_SOLID,1,RGB(0,0,0));
	pOldPen=dc.SelectObject(pBlackPen);
	CRect rect;
	GetClientRect(&rect);
	//绘制X轴
	dc.MoveTo(rect.left+30,rect.Height()-50);
	dc.LineTo(rect.Width()-20,rect.Height()-50);
	//绘制Y轴
	dc.MoveTo(rect.left+30,rect.Height()-50);
	dc.LineTo(rect.left+30,60);
	//绘制X轴箭头
	dc.MoveTo(rect.Width()-26,rect.Height()-54);
    dc.LineTo(rect.Width()-20,rect.Height()-50);
	dc.MoveTo(rect.Width()-26,rect.Height()-46);
    dc.LineTo(rect.Width()-20,rect.Height()-50);
	//绘制Y轴箭头
	dc.MoveTo(rect.left+26,66);
	dc.LineTo(rect.left+30,60);
	dc.MoveTo(rect.left+34,66);
	dc.LineTo(rect.left+30,60);
	//绘制x轴下方的刻度
	CString strText="";
	for (int i=0;i<6;i++)
	{
		strText.Format("%d",50*i);
		int gap=(rect.Width()-50)/6;
		dc.TextOut(rect.left+30+i*gap,rect.Height()-30,strText);
	}
	dc.SelectObject(pOldPen);
	//绘制直方图
	switch (Select())
	{
	case 0:
		pRGBPen->CreatePen(PS_SOLID,2,RGB(255,0,0));
		break;
	case 1:
		pRGBPen->CreatePen(PS_SOLID,2,RGB(0,255,0));
		break;
	case 2:
		pRGBPen->CreatePen(PS_SOLID,2,RGB(0,0,255));
		break;
	case -1:
		pRGBPen->CreatePen(PS_SOLID,2,RGB(0,0,0));
		break;
	default:
		break;
	}
	pOldPen=dc.SelectObject(pRGBPen);
	for (int i=0;i<256;i++)
	{
		//绘制高度
		int h=(double)(rect.Height()-120)/(double)pdoc->max[Select()]*(double)pdoc->count[Select()][i];
		int x=rect.left+30+i*(double)(rect.Width()-60)/256;
		dc.MoveTo(x,rect.Height()-50);
		dc.LineTo(x,rect.Height()-50-h);
	}
	UpdateWindow();
}

void CHistDlg::OnRadioR() 
{
	// TODO: Add your control notification handler code here
	Invalidate();
	OnPaint();
}

void CHistDlg::OnRadioG() 
{
	// TODO: Add your control notification handler code here
	Invalidate();
	OnPaint();
}

void CHistDlg::OnRadioB() 
{
	// TODO: Add your control notification handler code here
	Invalidate();
	OnPaint();
}
int CHistDlg::Select()
{
	if (pdoc->m_nbands==1)
	{
		GetDlgItem(IDC_RADIO_R)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_G)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_B)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_BAND_SEL)->ShowWindow(FALSE);
	
		m_sel_band=0;
	}
	if (IDC_RADIO_R==GetCheckedRadioButton(IDC_RADIO_R,IDC_RADIO_B))
	{
		m_sel_band=0;
	}
	if (IDC_RADIO_G==GetCheckedRadioButton(IDC_RADIO_R,IDC_RADIO_B))
	{
		m_sel_band=1;
	}
	if (IDC_RADIO_B==GetCheckedRadioButton(IDC_RADIO_R,IDC_RADIO_B))
	{
		m_sel_band=2;
	}
	return m_sel_band;
}
BOOL CHistDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CMDIFrameWnd *pMainFrame=(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	if (pMainFrame)
	{
		CMDIChildWnd*pChild=(CMDIChildWnd*)pMainFrame->GetActiveFrame();
		if (pChild)
		{
			pdoc=(CRsProcDoc*)pChild->GetActiveDocument();
// 			if (pdoc)
// 			{
// 		    	pdoc->Calculate();
// 			}
// 			else
// 			{
// 				AfxMessageBox("读取文档指针失败!");
// 				return FALSE;
// 			}
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
