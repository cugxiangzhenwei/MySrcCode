// PriceUpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "meye.h"
#include "PriceUpDlg.h"
#include "Markup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPriceUpDlg dialog


CPriceUpDlg::CPriceUpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPriceUpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPriceUpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iePricestat = 0;
    m_CheckTbWbFlag = FALSE;
	m_pPriceUpstag = NULL;
	m_pPriceUpstagNumber = 0;
}


void CPriceUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPriceUpDlg)
	DDX_Control(pDX, IDC_BUTTON_LET, m_BtnImportPrice);
	DDX_Control(pDX, IDC_BUTTON_FLOOWWGZ, m_BtnFloowWgz);
	DDX_Control(pDX, IDC_BUTTON_ADD_PROID, m_BtnAddPro);
	DDX_Control(pDX, IDC_EXPLORER_PRICEUP, m_IePriceCtrl);
	DDX_Control(pDX, IDC_STATIC_1, m_Static1);
	DDX_Control(pDX, IDC_STATIC_2, m_Static2);
	DDX_Control(pDX, IDC_STATIC_3, m_Static3);
	DDX_Control(pDX, IDC_STATIC_4, m_Static4);
	DDX_Control(pDX, IDC_STATIC_5, m_Static5);
	DDX_Control(pDX, IDC_STATIC_6, m_Static6);
	DDX_Control(pDX, IDC_STATIC_7, m_Static7);
	//}}AFX_DATA_MAP

	memset(&m_GetPriceUps,0x00,sizeof( GetPriceUps));
}


BEGIN_MESSAGE_MAP(CPriceUpDlg, CDialog)
	//{{AFX_MSG_MAP(CPriceUpDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_ADD_PROID, OnButtonAddProid)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_BUTTON_LET, OnButtonLet)
	ON_BN_CLICKED(IDC_BUTTON_FLOOWWGZ, OnButtonFloowwgz)
	ON_BN_CLICKED(IDC_CHECK_TB_WB, OnCheckTbWb)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPriceUpDlg message handlers

void CPriceUpDlg::UTF_8ToUnicode(wchar_t* pOut,char *pText)
{
    char* uchar = (char *)pOut;
    uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
    uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
    return;
}


void CPriceUpDlg::UnicodeToUTF_8(char* pOut,wchar_t* pText)
{
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
    char* pchar = (char *)pText;
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
    return;
}


void CPriceUpDlg::UTF_8ToGB2312(CString &pOut, char *pText, int pLen)
{

    char * newBuf = new char[pLen*2];
	memset(newBuf,0x00,sizeof(char)*pLen*2);
    char Ctemp[4];
    memset(Ctemp,0,4);
    int i =0;
    int j = 0;
    
    while(i < pLen)
    {
        if(pText[i] > 0)
        {
                newBuf[j++] = pText[i++];                       
        }
        else                 
        {
                WCHAR Wtemp;
                UTF_8ToUnicode(&Wtemp,pText + i);
        
                UnicodeToGB2312(Ctemp,Wtemp);
            
                newBuf[j] = Ctemp[0];
                newBuf[j + 1] = Ctemp[1];
                i += 3;    
                j += 2;   
        }
    }
	newBuf[j] = ' ';
    
    pOut = newBuf;

    
    return; 
}  

void CPriceUpDlg::UnicodeToGB2312(char* pOut,wchar_t uData)
{
    WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);
    return;
}    

void CPriceUpDlg::OnPaint() 
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
//	szMemDC.Draw3dRect(&rcClient,RGB(0,0,0),RGB(255,255,255));

//	szMemDC.FillSolidRect( &rcClient ,RGB(255,255,255));//RGB( 150,150,150));


    szMemDC.FillSolidRect( &rcClient ,RGB(221,228,238));//RGB( 150,150,150));

	//画边框
	szMemDC.MoveTo(rcClient.right-1,rcClient.top);
	szMemDC.LineTo(rcClient.right-1,rcClient.bottom);

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
    
	szMemDC.DeleteDC();
}







void CPriceUpDlg::OnButtonAddProid() 
{//http://mvd.360buy.com/20049856.html
	// TODO: Add your control notification handler code here

	 
}


CString CPriceUpDlg::GetGbkToUrlEncode(CString keyWord)
{


	CString strSql,str;
	char *p1 =new char[1024*1024*4]; // 4M
	memset(p1,0x00,sizeof(char)*1024*1024*4);
	wchar_t* wszString1 = new wchar_t[1024*1024*16 + 1];
	memset(wszString1,0x00,sizeof(char)*1024*1024*8 + 1);

	

	strSql = keyWord;

	CString strurldecode = _T("");
	{

		sprintf(p1,"%s",strSql);
		

		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, p1, strlen(p1), NULL, 0);  
		MultiByteToWideChar(CP_ACP, NULL, p1, strlen(p1), wszString1, wcsLen);
		wszString1[wcsLen] = '\0';  

		memset(p1,0x00,strSql.GetLength()*sizeof(char));
		//UNICODE再转换成UTF-8 （放在buf里）
		int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString1, wcslen(wszString1), NULL, 0, NULL, NULL);  
		WideCharToMultiByte(CP_UTF8, NULL, wszString1, wcslen(wszString1), p1, u8Len, NULL, NULL);  
		p1[u8Len] = '\0';  

		long nData[500] = {0};
		long nLen = strlen(p1);
		
		for(long i = 0 ;i < nLen;i++)
		{
			if(p1[i] == 32)
			{//空格
			    strurldecode +=_T("%20");
			}
			else if(p1[i] > 0)
			{
				strurldecode += p1[i];
			}
			else
			{
				nData[i] = 256 + (int)p1[i];
	

				str.Format(_T("%x"), nData[i]);
				str = _T("%") + str;
                strurldecode+=str;

				//转16进制字符
			}
			
		}

	}

	if(p1)
	{
		delete [] p1;
		p1 = NULL;
	}
	if(wszString1)
	{
		delete [] wszString1;
		wszString1 = NULL;
	}

	return strurldecode;
}

void CPriceUpDlg::SetXmlData(int nShopid,int nType,CString strRet)
{
	
}


BOOL CPriceUpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CIniFile szIniFile;
	CString useremail = szIniFile.GetIniFileString(_T("wbidstar"),_T("user"));
		CString UrlTo;//?http://www.wgz360.com/
		UrlTo.Format(_T("http://www.wgz360.com/wbhfeng/pricelook.php"),useremail);

	m_IePriceCtrl.Navigate(UrlTo, NULL, NULL, NULL, NULL );

	CButton*   pBtn3   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO3)); //父窗口指针
	pBtn3->SetCheck(TRUE);


	 CString tbwbflag = szIniFile.GetIniFileString(_T("wbidstar"),_T("tbwbflag"));
	 
	 CButton*   tbpBtn3   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_CHECK_TB_WB)); //父窗口指针
    if(tbwbflag.Compare(_T("1"))==1)
	{
		m_CheckTbWbFlag = TRUE;
		
		tbpBtn3->SetCheck(TRUE);
	}
	else
	{
		m_CheckTbWbFlag = FALSE;
		tbpBtn3->SetCheck(FALSE);
	}



    CPoint szPoint;
	szPoint.x = 1;
	szPoint.y = 5;
	m_BtnImportPrice.SetTextPos(szPoint);

	m_BtnImportPrice.SetImage(_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"));
	m_BtnImportPrice.SetWindowText("已支持商城");
	m_BtnImportPrice.SetTextPos(szPoint);

	m_BtnFloowWgz.SetTextPos(szPoint);

	m_BtnFloowWgz.SetImage(_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"));
	m_BtnFloowWgz.SetWindowText("关注网购者");
	m_BtnFloowWgz.SetTextPos(szPoint);


	szPoint.x = 8;
	szPoint.y = 5;
	m_BtnAddPro.SetTextPos(szPoint);
	m_BtnAddPro.SetImage(_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"));
	m_BtnAddPro.SetWindowText("添加商品");
	m_BtnAddPro.SetTextPos(szPoint);



	LoadConfigXml();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CPriceUpDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CPriceUpDlg)
	ON_EVENT(CPriceUpDlg, IDC_EXPLORER_PRICEUP, 259 /* DocumentComplete */, OnDocumentCompleteExplorerPriceup, VTS_DISPATCH VTS_PVARIANT)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


CString  CPriceUpDlg::GetCurrentApPath()
{
	TCHAR DBFileName[MAX_PATH] ;
	memset(DBFileName,0x00,sizeof(DBFileName));
	GetModuleFileName(AfxGetInstanceHandle(), DBFileName, MAX_PATH);	
	CString strPath_temp;
	strPath_temp = DBFileName;
	strPath_temp = strPath_temp.Mid(0, strPath_temp.ReverseFind('\\'));
	return strPath_temp;
}

void CPriceUpDlg::LoadConfigXml(void)
{

		
}


void CPriceUpDlg::OnDocumentCompleteExplorerPriceup(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	// TODO: Add your control notification handler code here
//	m_iestat = 1;

   
	if(m_iePricestat==1)
	{//添加商品后才处理

		m_iePricestat = 0;
		IHTMLDocument2 *pDoc = (IHTMLDocument2 *)m_IePriceCtrl.GetDocument(); 
		if ( !pDoc ) 
		{
		//	m_iestat = 2;
			return ;
		}

		//return m_IePriceCtrl.GetStatusText();

		IHTMLElement *pElement, *pElement1;
		HRESULT hr = pDoc->get_body( &pElement );
		if ( hr != S_OK ) 
		{
			pDoc->Release();
			
		//	m_iestat = 2;
			return ;
		
		}

		if(!pElement)
		{
			pDoc->Release();
		//	m_iestat = 2;
			return ;
		}
		hr = pElement->get_parentElement( &pElement1 );
		if ( hr != S_OK ) 
		{
			pElement->Release();
			pDoc->Release();
		//	m_iestat = 2;
			return ;
		}

		if(!pElement1)
		{
			pElement->Release();
			pDoc->Release();
		//	m_iestat = 2;
			return ;
		}

		BSTR bs;
		hr = pElement1->get_outerHTML( &bs );
		if ( hr != S_OK )
		{
			pElement->Release();
			pElement1->Release();
			pDoc->Release();
		//	m_iestat = 2;
			return ;
		}
		
		CString sRes = bs;	
		SysFreeString( bs );


		pElement->Release();
		pElement1->Release();
		pDoc->Release();

	
		int nFindpos = sRes.Find(_T("<BODY>"));

		//处理状态
		if(nFindpos > -1)
		{
			sRes = sRes.Right(sRes.GetLength() - nFindpos - strlen("<BODY>"));

			nFindpos = sRes.Find(_T("</BODY>"));
			if(nFindpos > -1)
			{
				sRes = sRes.Left(nFindpos);
                if(sRes.Compare("1")==0)
				{
					AfxMessageBox("添加商品监控成功!");
				}
				else if(sRes.Compare("10")==0)
				{
					AfxMessageBox("你已经添加了该商品!");
				}
				else if(sRes.Compare("101")==0)
				{
					AfxMessageBox("提交商品审核成功!");
				}
				else if(sRes.Compare("110")==0)
				{
					AfxMessageBox("你已经提交了该商品,该商品正在审核,请不要重复提交，谢谢!");
				}
				else
				{
					AfxMessageBox("你提交的商品暂不支持,如有需要请直接联系作者QQ:280888954@qq.com!");
				}
			}

			//转向商品列表
			SetTimer(10001,500,NULL);
		}

	}	
}

void CPriceUpDlg::OnRadio1() 
{
	CButton*   pBtn1   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO1)); //父窗口指针
	CButton*   pBtn2   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO2)); //父窗口指针
	CButton*   pBtn3   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO3)); //父窗口指针
	CButton*   pBtn4   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO4)); //父窗口指针


	pBtn1->SetCheck(TRUE);
    pBtn2->SetCheck(FALSE);
	pBtn3->SetCheck(FALSE);
	pBtn4->SetCheck(FALSE);

}

void CPriceUpDlg::OnRadio2() 
{
	CButton*   pBtn1   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO1)); //父窗口指针
	CButton*   pBtn2   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO2)); //父窗口指针
	CButton*   pBtn3   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO3)); //父窗口指针
	CButton*   pBtn4   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO4)); //父窗口指针



	pBtn1->SetCheck(FALSE);
    pBtn2->SetCheck(TRUE);
	pBtn3->SetCheck(FALSE);
	pBtn4->SetCheck(FALSE);
}

void CPriceUpDlg::OnRadio3() 
{
	CButton*   pBtn1   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO1)); //父窗口指针
	CButton*   pBtn2   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO2)); //父窗口指针
	CButton*   pBtn3   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO3)); //父窗口指针
	CButton*   pBtn4   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO4)); //父窗口指针


	pBtn1->SetCheck(FALSE);
    pBtn2->SetCheck(FALSE);
	pBtn3->SetCheck(TRUE);
	pBtn4->SetCheck(FALSE);
}

void CPriceUpDlg::OnRadio4() 
{
	CButton*   pBtn1   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO1)); //父窗口指针
	CButton*   pBtn2   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO2)); //父窗口指针
	CButton*   pBtn3   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO3)); //父窗口指针
	CButton*   pBtn4   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_RADIO4)); //父窗口指针

	pBtn1->SetCheck(FALSE);
    pBtn2->SetCheck(FALSE);
	pBtn3->SetCheck(FALSE);
	pBtn4->SetCheck(TRUE);
}

void CPriceUpDlg::OnButtonLet() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,NULL,_T("http://www.wgz360.com/letshop_list.php"),NULL,NULL,SW_SHOWNORMAL);
}

void CPriceUpDlg::OnButtonFloowwgz() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,NULL,_T("http://weibo.com/u/1594081585"),NULL,NULL,SW_SHOWNORMAL);
	
}

void CPriceUpDlg::OnCheckTbWb() 
{
	// TODO: Add your control notification handler code here
	CButton*   tbpBtn3   =   (CButton*)CWnd::FromHandle(::GetDlgItem(m_hWnd,IDC_CHECK_TB_WB)); //父窗口指针
	m_CheckTbWbFlag ^= TRUE;
	tbpBtn3->SetCheck(m_CheckTbWbFlag);
}

void CPriceUpDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==10001)
	{
		CIniFile szIniFile;
		CString useremail = szIniFile.GetIniFileString(_T("wbidstar"),_T("user"));
		CString UrlTo;
		UrlTo.Format(_T("http://www.wgz360.com/"),useremail);

		m_IePriceCtrl.Navigate(UrlTo, NULL, NULL, NULL, NULL );


		m_iePricestat = 0;
		KillTimer(10001);
	}
	CDialog::OnTimer(nIDEvent);
}
