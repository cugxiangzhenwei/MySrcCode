// VideoCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MeYe.h"
#include "VideoCtrlDlg.h"
#include "MeYeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVideoCtrlDlg dialog


CVideoCtrlDlg::CVideoCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVideoCtrlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


    m_iestat = 0;
	m_strIeGet = "";

	
}


void CVideoCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoCtrlDlg)
	DDX_Control(pDX, IDC_EXPLORER1, m_WebCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVideoCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CVideoCtrlDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_MESSAGE(OPEN_MSG_ID_URL, OnOpenUrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoCtrlDlg message handlers

LRESULT CVideoCtrlDlg::OnOpenUrl(WPARAM wParam, LPARAM lParam)
{
	m_iestat = 0;
	m_strIeGet = "";
	CString strurl= (char*)wParam;
	m_WebCtrl.Navigate( strurl, NULL, NULL, NULL, NULL );
	return S_OK;
}

void CVideoCtrlDlg::OnPaint() 
{
	 CDialog::OnPaint();

}

//平铺画位图
void CVideoCtrlDlg::DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc)
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
void CVideoCtrlDlg::SetImageSize(CBitmap *pBitmap, CSize &sz)
{
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	sz = CSize(bm.bmWidth, bm.bmHeight);
}


//载入资源位图
void CVideoCtrlDlg::SetBackImage(int index,CString strName)
{
	SetImage(m_BitmapBack[index], strName);
	SetImageSize(&m_BitmapBack[index], m_szBitmapBack[index]);
}


//载入硬盘文件位图
void CVideoCtrlDlg::SetImage(CBitmap &bitmap, CString strName)
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



BOOL CVideoCtrlDlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();
	
//    m_WebCtrl.SetSilent(true); //JS错误不弹出
	return TRUE;
}


/*
BOOL CVideoCtrlDlg::PreTranslateMessage(MSG *pMsg)
{ 
    if(pMsg-> message   ==WM_KEYDOWN) 
        { 
                if(pMsg-> wParam   ==VK_RETURN   ) 
                { 
					AfxMessageBox("请点网页登陆框中的，连接，不要用回车键，谢谢!");
                    return false;   
					//m_WebCtrl.PostMessage(WM_KEYDOWN, VK_RETURN, 0);  
				//	 return     CWnd::PreTranslateMessage   (pMsg); 
                } 
         } 
    return     CDialog::PreTranslateMessage   (pMsg); 

}
*/




void CVideoCtrlDlg::SetSteep(long nStep)//设置视频快进
{
}

void CVideoCtrlDlg::OnTimer(UINT nIDEvent) 
{

	

	CDialog::OnTimer(nIDEvent);
}

















void CVideoCtrlDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BEGIN_EVENTSINK_MAP(CVideoCtrlDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVideoCtrlDlg)
	ON_EVENT(CVideoCtrlDlg, IDC_EXPLORER1, 259 /* DocumentComplete */, OnDocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CVideoCtrlDlg::OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	// TODO: Add your control notification handler code here
	m_iestat = 1;



	IHTMLDocument2 *pDoc = (IHTMLDocument2 *)m_WebCtrl.GetDocument(); 
    if ( !pDoc ) 
	{
		m_iestat = 2;
		return ;
	}

	//return m_WebCtrl.GetStatusText();

	IHTMLElement *pElement, *pElement1;
	HRESULT hr = pDoc->get_body( &pElement );
	if ( hr != S_OK ) 
	{
		pDoc->Release();
		
		m_iestat = 2;
		return ;
	
	}

	if(!pElement)
	{
		pDoc->Release();
		m_iestat = 2;
		return ;
	}
	hr = pElement->get_parentElement( &pElement1 );
    if ( hr != S_OK ) 
	{
		pElement->Release();
		pDoc->Release();
		m_iestat = 2;
		return ;
	}

	if(!pElement1)
	{
		pElement->Release();
		pDoc->Release();
		m_iestat = 2;
		return ;
	}

	BSTR bs;
	hr = pElement1->get_outerHTML( &bs );
	if ( hr != S_OK )
	{
		pElement->Release();
		pElement1->Release();
		pDoc->Release();
		m_iestat = 2;
		return ;
	}
	
	CString sRes = bs;	
	SysFreeString( bs );


	pElement->Release();
	pElement1->Release();
	pDoc->Release();

	m_strIeGet = sRes;
}
