// SConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MeYe.h"
#include "SConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSConfigDlg dialog


CSConfigDlg::CSConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSConfigDlg)
	//}}AFX_DATA_INIT
}


void CSConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSConfigDlg)
	DDX_Control(pDX, IDC_CHECK16, m_BtnCheck16);
	DDX_Control(pDX, IDC_CHECK17, m_BtnCheck17);
	DDX_Control(pDX, IDC_CHECK15, m_BtnCheck15);
	DDX_Control(pDX, IDC_CHECK14, m_BtnCheck14);
	DDX_Control(pDX, IDC_CHECK13, m_BtnCheck13);
	DDX_Control(pDX, IDC_CHECK12, m_BtnCheck12);
	DDX_Control(pDX, IDC_CHECK11, m_BtnCheck11);
	DDX_Control(pDX, IDC_CHECK10, m_BtnCheck10);
	DDX_Control(pDX, IDC_BUTTON_BIND, m_BtnBindwb);
	DDX_Control(pDX, IDC_BUTTON_S_SAVE, m_BtnSSave);
	DDX_Control(pDX, IDC_BUTTON_S_CANCLE, m_BtnSCancle);
	DDX_Control(pDX, IDC_STATIC_30, m_Static30);
	DDX_Control(pDX, IDC_STATIC_31, m_Static31);
	DDX_Control(pDX, IDC_STATIC_32, m_Static32);
	DDX_Control(pDX, IDC_STATIC_33, m_Static33);
	DDX_Control(pDX, IDC_STATIC_37, m_Static37);
	DDX_Control(pDX, IDC_STATIC_38, m_Static38);
	DDX_Control(pDX, IDC_STATIC_50, m_Static43);
	DDX_Control(pDX, IDC_STATIC_51, m_Static44);
	DDX_Control(pDX, IDC_STATIC_42, m_Static45);
	DDX_Control(pDX, IDC_STATIC_43, m_Static46);
	DDX_Control(pDX, IDC_STATIC_44, m_Static47);
	DDX_Control(pDX, IDC_STATIC_45, m_Static48);
	DDX_Control(pDX, IDC_STATIC_46, m_Static49);
	DDX_Control(pDX, IDC_STATIC_47, m_Static50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CSConfigDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_S_SAVE, OnButtonSSave)
	ON_BN_CLICKED(IDC_BUTTON_BIND, OnButtonBind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSConfigDlg message handlers

void CSConfigDlg::OnPaint() 
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

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
    
	szMemDC.DeleteDC();
}

BOOL CSConfigDlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();
	
	/*
	CIniFile szIniFile;
    //默认域名
	CString strValue = szIniFile.GetIniFileString(_T("myyeset"),_T("DNS_DEFAULT"));
	SetDlgItemText(IDC_EDIT_S_URL,strValue);

	BOOL bCheck1 = szIniFile.GetIniFileInt(_T("myyeset"),_T("DNS_SET_MY_FLAG"));

	if(bCheck1)
	{
		m_Check1Btn.SetCheck(bCheck1);

		strValue = szIniFile.GetIniFileString(_T("myyeset"),_T("DNS_SET_MY_FLAG"));
		//指定域名
		SetDlgItemText(IDC_EDIT_S_IP,strValue);
	}
	else
	{
		m_Check1Btn.SetCheck(0);
		SetDlgItemText(IDC_EDIT_S_IP,_T(""));
	}

	BOOL bCheck2 = szIniFile.GetIniFileInt(_T("myyeset"),_T("PROXY_SET_MY_FLAG"));
	if(bCheck2)
	{
		m_Check2Btn.SetCheck(bCheck2);
		//指定代理
		strValue = szIniFile.GetIniFileString(_T("myyeset"),_T("PROXY_SET_MY_IP"));
		SetDlgItemText(IDC_EDIT_S_IPADDER,strValue);

		//代理端口
		strValue = szIniFile.GetIniFileString(_T("myyeset"),_T("PROXY_SET_MY_IP_PORT"));
		SetDlgItemText(IDC_EDIT_S_IPADDER_PORT,strValue);
	}
	else
	{
		m_Check2Btn.SetCheck(FALSE);
		SetDlgItemText(IDC_EDIT_S_IPADDER,_T(""));
		SetDlgItemText(IDC_EDIT_S_IPADDER_PORT,_T(""));

	}

	//录像地址
	strValue = szIniFile.GetIniFileString(_T("myyeset"),_T("VIDEO_SET_IP_ADDRESS"));
	SetDlgItemText(IDC_EDIT_S_RECODER_IP,strValue);
*/

	CIniFile szIniFile;
    //默认域名
	CString strValue;

	m_BtnSSave.SetImage(_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"));
	m_BtnSSave.SetWindowText("保存");
	CPoint szPoint;
	szPoint.x = 20;
	szPoint.y = 5;
	m_BtnSSave.SetTextPos(szPoint);

	m_BtnSCancle.SetImage(_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"));
	m_BtnSCancle.SetWindowText("取消");
	m_BtnSCancle.SetTextPos(szPoint);

	m_BtnBindwb.SetImage(_T("分图\\按钮.bmp"),_T("分图\\按钮.bmp"),_T("分图\\按钮.bmp"),_T("分图\\按钮.bmp"));
	m_BtnBindwb.SetWindowText("绑定帐号");
	m_BtnBindwb.SetTextPos(szPoint);


	//绑定
	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("wbbindflag"));
	m_wbbindflag = atoi(strValue);
	if(m_wbbindflag == 1)
	{
		m_BtnBindwb.SetWindowText("取消绑定");

		strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("user"));
		SetDlgItemText(IDC_EDIT_WBUSER,strValue);

		strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("pwd"));
		SetDlgItemText(IDC_EDIT_WBPWD,strValue);

	}


	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag10"));
	if(strValue.Compare("1")==0)
	{
		m_BtnCheck10.SetCheck(TRUE);
	}
	else
	{
		m_BtnCheck10.SetCheck(FALSE);
	}

	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag11"));
	if(strValue.Compare("1")==0)
	{
		m_BtnCheck11.SetCheck(TRUE);
	}
	else
	{
		m_BtnCheck11.SetCheck(FALSE);
	}

	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag12"));
	if(strValue.Compare("1")==0)
	{
		m_BtnCheck12.SetCheck(TRUE);
	}
	else
	{
		m_BtnCheck12.SetCheck(FALSE);
	}
	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag13"));
	if(strValue.Compare("1")==0)
	{
		m_BtnCheck13.SetCheck(TRUE);
	}
	else
	{
		m_BtnCheck13.SetCheck(FALSE);
	}
	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag14"));
	if(strValue.Compare("1")==0)
	{
		m_BtnCheck14.SetCheck(TRUE);
	}
	else
	{
		m_BtnCheck14.SetCheck(FALSE);
	}
	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag15"));
	if(strValue.Compare("1")==0)
	{
		m_BtnCheck15.SetCheck(TRUE);
	}
	else
	{
		m_BtnCheck15.SetCheck(FALSE);
	}

	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag16"));
	if(strValue.Compare("1")==0)
	{
		m_BtnCheck16.SetCheck(TRUE);
	}
	else
	{
		m_BtnCheck16.SetCheck(FALSE);
	}

	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag17"));
	if(strValue.Compare("1")==0)
	{
		m_BtnCheck17.SetCheck(TRUE);
	}
	else
	{
		m_BtnCheck17.SetCheck(FALSE);
	}



	return TRUE;
}

void CSConfigDlg::initDlg(void)
{//读取INI文件
	/*
	CIniFile szIniFile;
//	TCHAR szChar[256];

	//默认域名
	CString strtemp = szIniFile.GetIniFileString(_T("myyeset"),_T("DNS_DEFAULT"));
	SetDlgItemText(IDC_EDIT_S_URL,strtemp);

    long flag = szIniFile.GetIniFileInt(_T("myyeset"),_T("DNS_SET_MY_FLAG"));
	if(flag == 1)
	{
		//指定域名
		m_Check1Btn.SetCheck(TRUE);
		strtemp = szIniFile.GetIniFileString(_T("myyeset"),_T("DNS_SET_MY_IP"));
	
		SetDlgItemText(IDC_EDIT_S_IP,strtemp);
	}
	else
	{
		m_Check1Btn.SetCheck(FALSE);
		SetDlgItemText(IDC_EDIT_S_IP,_T(""));
	}

	//指定代理服务器IP

	flag = szIniFile.GetIniFileInt(_T("myyeset"),_T("PROXY_SET_MY_FLAG"));
	if(flag == 1)
	{
		//指定代理名
		m_Check2Btn.SetCheck(TRUE);
		strtemp = szIniFile.GetIniFileString(_T("myyeset"),_T("PROXY_SET_MY_IP"));
		SetDlgItemText(IDC_EDIT_S_IPADDER,strtemp);

		//代理端口
		strtemp = szIniFile.GetIniFileString(_T("myyeset"),_T("PROXY_SET_MY_IP_PORT"));
		SetDlgItemText(IDC_EDIT_S_IPADDER_PORT,strtemp);


	}
	else
	{
		m_Check2Btn.SetCheck(FALSE);
		SetDlgItemText(IDC_EDIT_S_IPADDER,_T(""));
		SetDlgItemText(IDC_EDIT_S_IPADDER_PORT,_T(""));
	}

	

	//录像地址
	strtemp = szIniFile.GetIniFileString(_T("myyeset"),_T("VIDEO_SET_IP_ADDRESS"));
	SetDlgItemText(IDC_EDIT_S_RECODER_IP,strtemp);

*/
}

void CSConfigDlg::OnButtonSSave() 
{
	CIniFile szIniFile;
//	TCHAR szChar[256];
//	GetDlgItemText(IDC_EDIT_S_URL,szChar,sizeof(szChar));

	if(m_BtnCheck10.GetCheck()==TRUE)
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag10"),"1");
	}
	else
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag10"),"0");
	}

	if(m_BtnCheck11.GetCheck()==TRUE)
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag11"),"1");
	}
	else
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag11"),"0");
	}

	if(m_BtnCheck12.GetCheck()==TRUE)
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag12"),"1");
	}
	else
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag12"),"0");
	}

	if(m_BtnCheck13.GetCheck()==TRUE)
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag13"),"1");
	}
	else
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag13"),"0");
	}

	if(m_BtnCheck14.GetCheck()==TRUE)
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag14"),"1");
	}
	else
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag14"),"0");
	}

	if(m_BtnCheck15.GetCheck()==TRUE)
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag15"),"1");
	}
	else
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag15"),"0");
	}
    
	if(m_BtnCheck16.GetCheck()==TRUE)
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag16"),"1");
	}
	else
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag16"),"0");
	}

	if(m_BtnCheck17.GetCheck()==TRUE)
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag17"),"1");
	}
	else
	{
		szIniFile.WriteIniFile(_T("wbidstar"),_T("setflag17"),"0");
	}
	
}

void CSConfigDlg::OnButtonBind() 
{
	// TODO: Add your control notification handler code here

	CIniFile szIniFile;
	TCHAR szChar[256];
	if(m_wbbindflag==1)
	{
		if(MessageBox("取消绑定如果将不能自动发微博","自动发微博", MB_OKCANCEL )==IDOK)
		{
			m_wbbindflag = 0;
			szIniFile.WriteIniFile(_T("wbidstar"),_T("user"),"");
			szIniFile.WriteIniFile(_T("wbidstar"),_T("pwd"),"");
			m_BtnBindwb.SetWindowText("绑定帐号");


            szIniFile.WriteIniFile(_T("wbidstar"),_T("wbbindflag"),_T("0"));
		}
		
	}
	else
	{
		GetDlgItemText(IDC_EDIT_WBUSER,szChar,sizeof(szChar));

		//user
		szIniFile.WriteIniFile(_T("wbidstar"),_T("user"),szChar);
        if(strlen(szChar) == 0)
		{
			 AfxMessageBox("请输入用户名！");
			 return ;
		}

		GetDlgItemText(IDC_EDIT_WBPWD,szChar,sizeof(szChar));
		szIniFile.WriteIniFile(_T("wbidstar"),_T("pwd"),szChar);
		if(strlen(szChar) == 0)
		{
			 AfxMessageBox("请输入密码！");
			 return ;
		}

		m_wbbindflag = 1;
		m_BtnBindwb.SetWindowText("取消绑定");
		szIniFile.WriteIniFile(_T("wbidstar"),_T("wbbindflag"),_T("1"));
	}

	
	

}
