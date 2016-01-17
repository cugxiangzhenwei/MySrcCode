// RealVideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MeYe.h"
#include "MeYeDlg.h"
#include "RealVideoDlg.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealVideoDlg dialog


CRealVideoDlg::CRealVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRealVideoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRealVideoDlg)
	//}}AFX_DATA_INIT
	m_pMainDlg = NULL;
}


void CRealVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealVideoDlg)
	DDX_Control(pDX, IDC_BUTTON_VIDEO_CAP, m_Btn_Video_Cap);
	DDX_Control(pDX, IDC_BUTTON_UP_DOWN, m_Btn_Up_Down);
	DDX_Control(pDX, IDC_BUTTON_REAL_UP, m_Btn_Up);
	DDX_Control(pDX, IDC_BUTTON_REAL_4LINE_BTN, m_Btn_4Line);
	DDX_Control(pDX, IDC_BUTTON_LEFT_RIGHT, m_Btn_Left_Right);
	DDX_Control(pDX, IDC_BUTTON_REAL_RIGHT, m_Btn_Right);
	DDX_Control(pDX, IDC_BUTTON_REAL_LEFT, m_Btn_Left);
	DDX_Control(pDX, IDC_BUTTON_DOWN, m_Btn_Down);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealVideoDlg, CDialog)
	//{{AFX_MSG_MAP(CRealVideoDlg)
	ON_BN_CLICKED(IDC_BUTTON_REAL_4LINE_BTN, OnButtonReal4lineBtn)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_VIDEO_CAP, OnButtonVideoCap)
	ON_BN_CLICKED(IDC_BUTTON_REAL_UP, OnButtonRealUp)
	ON_BN_CLICKED(IDC_BUTTON_REAL_LEFT, OnButtonRealLeft)
	ON_BN_CLICKED(IDC_BUTTON_REAL_RIGHT, OnButtonRealRight)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_RIGHT, OnButtonLeftRight)
	ON_COMMAND(ID_MENU_LINE_1,OnMenuLine1)
	ON_COMMAND(ID_MENU_LINE_2,OnMenuLine2)
	ON_COMMAND(ID_MENU_LINE_3,OnMenuLine3)
	ON_COMMAND(ID_MENU_LINE_4,OnMenuLine4)
	ON_BN_CLICKED(IDC_BUTTON_UP_DOWN, OnButtonUpDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealVideoDlg message handlers

void CRealVideoDlg::OnButtonReal4lineBtn() 
{
	COfficeXPMenu m_popmenu;
	m_popmenu.LoadMenu(IDR_MENU_LINE);	
	m_popmenu.SetType(TYPE_XP);
	CMenu *psub = (CMenu *)m_popmenu.GetSubMenu(0);
//	psub->DrawItem()
	CRect rect;
	GetWindowRect(rect);
	DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON),
		rect.left+52,rect.top+366, this); 	
}

void CRealVideoDlg::OnPaint() 
{
	  
    CPaintDC dc(this); // device context for painting
	CRect rcClient,rcBtn,rcScroll,rcPic;
	GetClientRect(&rcClient);

	//内存画图//////////////////////////
	CDC szMemDC;
	szMemDC.CreateCompatibleDC(&dc);

	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	szMemDC.SelectObject(&btScreen);
	btScreen.DeleteObject();
	szMemDC.Rectangle(rcClient);
//	szMemDC.Draw3dRect(&rcClient,RGB(0,0,0),RGB(221,228,238));

	szMemDC.FillSolidRect( &rcClient ,RGB(221,228,238));//RGB( 150,150,150));

	CRect szRc = rcClient;
	szRc.top = rcClient.top + 25;
	szRc.left = rcClient.left + 20;
	szRc.right = szRc.left + 180;
	szRc.bottom  = szRc.top + 255;
	DrawRangeImage(&m_BitmapBack[0],&szMemDC,szRc);

	szRc.top = rcClient.top + 255 + 25 + 15;
	szRc.left = rcClient.left;
	szRc.right = szRc.left + 209;
	szRc.bottom  = szRc.top + 40;
	DrawRangeImage(&m_BitmapBack[1],&szMemDC,szRc);

	szRc.top = rcClient.top + 255 + 25 + 15 + 40;
	szRc.left = rcClient.left;
	szRc.right = szRc.left + 209;
	szRc.bottom  = szRc.top + 45;
	DrawRangeImage(&m_BitmapBack[2],&szMemDC,szRc);


	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
    
	szMemDC.DeleteDC();
}


//平铺画位图
void CRealVideoDlg::DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc)
{
	CDC MemDC;
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	int li_Width = bm.bmWidth;
	int li_Height = bm.bmHeight;


	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = MemDC.SelectObject(pBitmap);

	int x=rc.left;
	int y=rc.top;
	while (y < (rc.Height()+rc.top)) 
 	{
 		while(x < (rc.Width()+rc.left)) 
 		{
			pDC->BitBlt(x, y, li_Width, li_Height, &MemDC, 0, 0, SRCCOPY);
 			x += li_Width;
 		}
		x = rc.left;
 		y += li_Height;
 	}

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}


//保存位图尺寸
void CRealVideoDlg::SetImageSize(CBitmap *pBitmap, CSize &sz)
{
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	sz = CSize(bm.bmWidth, bm.bmHeight);
}

//载入资源位图
void CRealVideoDlg::SetBackImage(int index,CString strName)
{
	SetImage(m_BitmapBack[index], strName);
	SetImageSize(&m_BitmapBack[index], m_szBitmapBack[index]);
}

//载入硬盘文件位图
void CRealVideoDlg::SetImage(CBitmap &bitmap, CString strName)
{
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)::LoadImage(NULL, strName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	//ASSERT(hBitmap);

	if(hBitmap == NULL)
	{
		CString str;
		AfxMessageBox(_T("Not Open SetImage Bitmap"));
		PostQuitMessage(0);
	}

	if (bitmap.m_hObject)
		bitmap.Detach();

	bitmap.Attach(hBitmap);
}

//
// CRecoderDlg message handlers
BOOL CRealVideoDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	CRect r;
	GetClientRect(&r);	
    CRect rcTemp;
	
	rcTemp.left = r.left + 93;
	rcTemp.top =  r.top + 33;
	rcTemp.right = rcTemp.left+30;
	rcTemp.bottom = rcTemp.top + 30;
    m_Btn_Up.SetImage(_T("分图\\up.bmp"),_T("分图\\up_1.bmp"),_T("分图\\up.bmp"),_T("分图\\up.bmp"));
	m_Btn_Up.SetWindowText("");
	m_Btn_Up.MoveWindow(rcTemp,TRUE);
	

	rcTemp.left = r.left + 93;
	rcTemp.top =  r.top + 102;
	rcTemp.right = rcTemp.left+30;
	rcTemp.bottom = rcTemp.top + 30;
	m_Btn_Down.SetImage(_T("分图\\down.bmp"),_T("分图\\down_1.bmp"),_T("分图\\down.bmp"),_T("分图\\down.bmp"));
	m_Btn_Down.SetWindowText("");
	m_Btn_Down.MoveWindow(rcTemp,TRUE);

	
    rcTemp.left = r.left + 25;
	rcTemp.top =  r.top + 68;
	rcTemp.right = rcTemp.left+30;
	rcTemp.bottom = rcTemp.top + 30;
	m_Btn_Left.SetImage(_T("分图\\left.bmp"),_T("分图\\left_1.bmp"),_T("分图\\left.bmp"),_T("分图\\left.bmp"));
	m_Btn_Left.SetWindowText("");
	m_Btn_Left.MoveWindow(rcTemp,TRUE);


	rcTemp.left = r.left + 163;
	rcTemp.top =  r.top + 68;
	rcTemp.right = rcTemp.left+30;
	rcTemp.bottom = rcTemp.top + 30;
	m_Btn_Right.SetImage(_T("分图\\right.bmp"),_T("分图\\right_1.bmp"),_T("分图\\right.bmp"),_T("分图\\right.bmp"));
	m_Btn_Right.SetWindowText("");
	m_Btn_Right.MoveWindow(rcTemp,TRUE);


    m_Btn_4Line.SetImage(_T("分图\\1按钮_0.bmp"),_T("分图\\1按钮_1.bmp"),_T("分图\\1按钮_2.bmp"),_T("分图\\1按钮_0.bmp"));
	m_Btn_4Line.SetWindowText("线路选择1");
	CPoint szPoint;
	szPoint.x = 20;
	szPoint.y = 3;
	m_Btn_4Line.SetTextPos(szPoint);


	rcTemp.left = r.left + 54;
	rcTemp.top =  r.top + 160;
	rcTemp.right = rcTemp.left+110;
	rcTemp.bottom = rcTemp.top + 30;
	m_Btn_Up_Down.SetImage(_T("分图\\2按钮_1_0.bmp"),_T("分图\\2按钮_1_1.bmp"),_T("分图\\2按钮_1_2.bmp"),_T("分图\\2按钮_1_0.bmp"));
	m_Btn_Up_Down.SetWindowText("");
	m_Btn_Up_Down.MoveWindow(rcTemp,TRUE);

	rcTemp.left = r.left + 54;
	rcTemp.top =  r.top + 190;
	rcTemp.right = rcTemp.left+110;
	rcTemp.bottom = rcTemp.top + 30;
	m_Btn_Left_Right.SetImage(_T("分图\\3按钮_2_0.bmp"),_T("分图\\3按钮_2_1.bmp"),_T("分图\\3按钮_2_2.bmp"),_T("分图\\3按钮_2_0.bmp"));
	m_Btn_Left_Right.SetWindowText("");
	m_Btn_Left_Right.MoveWindow(rcTemp,TRUE);

	rcTemp.left = r.left + 54;
	rcTemp.top =  r.top + 220;
	rcTemp.right = rcTemp.left+110;
	rcTemp.bottom = rcTemp.top + 30;
	m_Btn_Video_Cap.SetImage(_T("分图\\4按钮_3_0.bmp"),_T("分图\\4按钮_3_1.bmp"),_T("分图\\4按钮_3_2.bmp"),_T("分图\\4按钮_3_0.bmp"));
	m_Btn_Video_Cap.SetWindowText("");
	m_Btn_Video_Cap.MoveWindow(rcTemp,TRUE);

	SetBackImage(0,_T("分图\\控制台.bmp"));
    SetBackImage(1,_T("分图\\线路选择_1.bmp"));
	SetBackImage(2,_T("分图\\线路选择_2.bmp"));
	



	return TRUE; 
}


void CRealVideoDlg::OnMenuLine1()
{
	m_Btn_4Line.SetWindowText("线路选择1");
}

void CRealVideoDlg::OnMenuLine2()
{
	m_Btn_4Line.SetWindowText("线路选择2");
}

void CRealVideoDlg::OnMenuLine3()
{
	m_Btn_4Line.SetWindowText("线路选择3");
}

void CRealVideoDlg::OnMenuLine4()
{
	m_Btn_4Line.SetWindowText("线路选择4");
}

void CRealVideoDlg::OnButtonVideoCap() 
{
	
}

void CRealVideoDlg::OnButtonRealUp() 
{

}

void CRealVideoDlg::OnButtonRealLeft() 
{



	
}

void CRealVideoDlg::OnButtonRealRight() 
{



	
}

void CRealVideoDlg::OnButtonDown() 
{

	
}

void CRealVideoDlg::OnButtonLeftRight() 
{

}

void CRealVideoDlg::OnButtonUpDown() 
{
	

}
