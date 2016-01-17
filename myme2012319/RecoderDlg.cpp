// RecoderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MeYe.h"
#include "RecoderDlg.h"
#include "MeYeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecoderDlg dialog


CRecoderDlg::CRecoderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecoderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecoderDlg)
	m_DataEnd = 0;
	//}}AFX_DATA_INIT
}


void CRecoderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecoderDlg)
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DataStar);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_DataEnd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecoderDlg, CDialog)
	//{{AFX_MSG_MAP(CRecoderDlg)
	ON_WM_PAINT()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecoderDlg message handlers
BOOL CRecoderDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	

	return TRUE; 
}

void CRecoderDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient,rcBtn,rcScroll,rcPic;
	GetClientRect(&rcClient);

	//�ڴ滭ͼ//////////////////////////
	CDC szMemDC;
	szMemDC.CreateCompatibleDC(&dc);

	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	szMemDC.SelectObject(&btScreen);
	btScreen.DeleteObject();
	szMemDC.Rectangle(rcClient);
//	szMemDC.Draw3dRect(&rcClient,RGB(0,0,0),RGB(221,228,238));

	szMemDC.FillSolidRect( &rcClient ,RGB(221,228,238));//RGB( 150,150,150));

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
    
	szMemDC.DeleteDC();
}

void CRecoderDlg::OnButtonSerchVideo() 
{
	
}


