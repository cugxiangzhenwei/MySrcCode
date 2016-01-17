// ClientConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MeYe.h"
#include "ClientConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientConfigDlg dialog


CClientConfigDlg::CClientConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientConfigDlg)
	//}}AFX_DATA_INIT


   m_CurrentTabSel=0;
   m_CurrentDev = 0;
}


void CClientConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientConfigDlg)
	DDX_Control(pDX, IDC_BUTTON_C_SAVE, m_CBtnSave);
	DDX_Control(pDX, IDC_BUTTON_C_CANCE, m_CBtnCancle);
	DDX_Control(pDX, IDC_STATIC_C_23, m_StaticC23);
	DDX_Control(pDX, IDC_STATIC_C_22, m_StaticC22);
	DDX_Control(pDX, IDC_STATIC_C_21, m_StaticC21);
	DDX_Control(pDX, IDC_EDIT_C_23, m_EditC23);
	DDX_Control(pDX, IDC_EDIT_C_22, m_EditC22);
	DDX_Control(pDX, IDC_EDIT_C_21, m_EditC21);
	DDX_Control(pDX, IDC_STATIC_C_31, m_StaticC31);
	DDX_Control(pDX, IDC_STATIC_C_3, m_StaticC3);
	DDX_Control(pDX, IDC_STATIC_C_2, m_StaticC2);
	DDX_Control(pDX, IDC_STATIC_C_1, m_StaticC1);
	DDX_Control(pDX, IDC_EDIT_C_31, m_EditC31);
	DDX_Control(pDX, IDC_EDIT_C_3, m_EditC3);
	DDX_Control(pDX, IDC_EDIT_C_2, m_EditC2);
	DDX_Control(pDX, IDC_EDIT_C_1, m_EditC1);
	DDX_Control(pDX, IDC_STATIC_TAB_C_CONFIG, m_TabConfig);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClientConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CClientConfigDlg)
	ON_WM_PAINT()
	ON_MESSAGE(WM_TAB_SELECT, OnTabChange)
	ON_BN_CLICKED(IDC_BUTTON_C_SAVE, OnButtonCSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientConfigDlg message handlers

BOOL CClientConfigDlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();
	
	CRect rcClient;
	GetClientRect(&rcClient);
	
	m_rcHeadTab[0].top = rcClient.top + 150;
	m_rcHeadTab[0].left = rcClient.left + 50;
	
	
	
	
	m_TabConfig.MoveWindow(30,30,197*3,54);
	int tabcount = 3;
	m_TabConfig.SetCount(tabcount);
	for(int i=0;i<tabcount;i++)
	{
		m_TabConfig.SetRect(i,CRect(197*i,0,197*(i+1),54));
		m_TabConfig.SetSkin(i,_T("image\\barback_normal.bmp"),_T("image\\barback_over.bmp"),_T("image\\barback_disabled.bmp"));
	}
    m_TabConfig.SetTextFile("流播参数",0);
	m_TabConfig.SetTextFile("修改密码",1);
	m_TabConfig.SetTextFile("别名设置",2);
	m_TabConfig.SetSelTab();
	
	
	
	
	m_EditC1.ShowWindow(SW_SHOW);
	m_EditC2.ShowWindow(SW_SHOW);
	m_EditC3.ShowWindow(SW_SHOW);
	m_StaticC1.ShowWindow(SW_SHOW);
	m_StaticC2.ShowWindow(SW_SHOW);
	m_StaticC3.ShowWindow(SW_SHOW);
	
	m_StaticC23.ShowWindow(SW_HIDE);
	m_StaticC22.ShowWindow(SW_HIDE);
	m_StaticC21.ShowWindow(SW_HIDE);
	m_EditC23.ShowWindow(SW_HIDE);
	m_EditC22.ShowWindow(SW_HIDE);
	m_EditC21.ShowWindow(SW_HIDE);


	m_StaticC31.ShowWindow(SW_HIDE);
	m_EditC31.ShowWindow(SW_HIDE);
	

    m_CBtnSave.SetImage(_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"));
	m_CBtnSave.SetWindowText("保存");
	CPoint szPoint;
	szPoint.x = 20;
	szPoint.y = 5;
	m_CBtnSave.SetTextPos(szPoint);

	m_CBtnCancle.SetImage(_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"),_T("分图\\按钮_4.bmp"));
	m_CBtnCancle.SetWindowText("取消");
	m_CBtnCancle.SetTextPos(szPoint);

	return TRUE;
}

void CClientConfigDlg::OnPaint() 
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

	CRect rcRect = rcClient;
    rcRect.top = 30 + 54;
	rcRect.left = 30;
	rcRect.right = rcRect.left + 590;
	rcRect.bottom = rcRect.top + 288;

	szMemDC.MoveTo(rcRect.left,rcRect.top);
	szMemDC.LineTo(rcRect.right,rcRect.top);
	szMemDC.LineTo(rcRect.right,rcRect.bottom);
	szMemDC.LineTo(rcRect.left,rcRect.bottom);
	szMemDC.LineTo(rcRect.left,rcRect.top);

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
    
	szMemDC.DeleteDC();
}

LRESULT CClientConfigDlg::OnTabChange(WPARAM wParam, LPARAM lParam)
{
	CIniFile szIniFile;
	//流媒体设备端口
	CString strValue;


	if(lParam < 4)
		m_CurrentTabSel = lParam;
	switch(m_CurrentTabSel)
	{
	case 0:
		{
			m_EditC1.ShowWindow(SW_SHOW);
			m_EditC2.ShowWindow(SW_SHOW);
			m_EditC3.ShowWindow(SW_SHOW);
			m_StaticC1.ShowWindow(SW_SHOW);
			m_StaticC2.ShowWindow(SW_SHOW);
			m_StaticC3.ShowWindow(SW_SHOW);
			
			m_StaticC23.ShowWindow(SW_HIDE);
			m_StaticC22.ShowWindow(SW_HIDE);
			m_StaticC21.ShowWindow(SW_HIDE);
			m_EditC23.ShowWindow(SW_HIDE);
			m_EditC22.ShowWindow(SW_HIDE);
			m_EditC21.ShowWindow(SW_HIDE);
			
			m_StaticC31.ShowWindow(SW_HIDE);
			m_EditC31.ShowWindow(SW_HIDE);


			TCHAR strKey[64]={0};
			CString strValue="";
			sprintf(strKey,_T("C_CONFIG_Stream_DEV_PORT_%d"),m_CurrentDev);
			strValue = szIniFile.GetIniFileString(_T("myyeset"),strKey);
			m_EditC1.SetWindowText(strValue);


			//服务端口
			sprintf(strKey,_T("C_CONFIG_Stream_SERVER_PORT_%d"),m_CurrentDev);
			strValue = szIniFile.GetIniFileString(_T("myyeset"),strKey);
			m_EditC2.SetWindowText(strValue);
				
			//播放缓冲
			sprintf(strKey,_T("C_CONFIG_Stream_PLAY_CASH_%d"),m_CurrentDev);
			strValue = szIniFile.GetIniFileString(_T("myyeset"),strKey);
			m_EditC3.SetWindowText(strValue);
		}
		break;
	case 1:
		{
			m_EditC1.ShowWindow(SW_HIDE);
			m_EditC2.ShowWindow(SW_HIDE);
			m_EditC3.ShowWindow(SW_HIDE);
			m_StaticC1.ShowWindow(SW_HIDE);
			m_StaticC2.ShowWindow(SW_HIDE);
			m_StaticC3.ShowWindow(SW_HIDE);
			
			m_StaticC23.ShowWindow(SW_SHOW);
			m_StaticC22.ShowWindow(SW_SHOW);
			m_StaticC21.ShowWindow(SW_SHOW);
			m_EditC23.ShowWindow(SW_SHOW);
			m_EditC22.ShowWindow(SW_SHOW);
			m_EditC21.ShowWindow(SW_SHOW);
			
			m_StaticC31.ShowWindow(SW_HIDE);
			m_EditC31.ShowWindow(SW_HIDE);
		}
		break;
	case 2:
		{
			m_EditC1.ShowWindow(SW_HIDE);
			m_EditC2.ShowWindow(SW_HIDE);
			m_EditC3.ShowWindow(SW_HIDE);
			m_StaticC1.ShowWindow(SW_HIDE);
			m_StaticC2.ShowWindow(SW_HIDE);
			m_StaticC3.ShowWindow(SW_HIDE);
			
			m_StaticC23.ShowWindow(SW_HIDE);
			m_StaticC22.ShowWindow(SW_HIDE);
			m_StaticC21.ShowWindow(SW_HIDE);
			m_EditC23.ShowWindow(SW_HIDE);
			m_EditC22.ShowWindow(SW_HIDE);
			m_EditC21.ShowWindow(SW_HIDE);
			
			m_StaticC31.ShowWindow(SW_SHOW);
			m_EditC31.ShowWindow(SW_SHOW);

			//别名设置
			if(m_CurrentDev >= 0)
			{
				TCHAR strKey[64]={0};
				sprintf(strKey,_T("SERVER_DEV_DNSNAME_%d"),m_CurrentDev);
				strValue = szIniFile.GetIniFileString(_T("myyeset"),strKey);
				m_EditC31.SetWindowText(strValue);
			}
		}
		break;
	case 3:
		//	  m_dlgFace->ShowWindow(SW_HIDE);
		break;
	default:
		//	  m_dlgFace->ShowWindow(SW_HIDE);
		break;
	}

	return S_OK;
}

void CClientConfigDlg::SetDevIndex(long nIndex)
{
	if(nIndex >=1)
	{
		m_CurrentDev = nIndex-1;//从0开始

		if(m_CurrentTabSel == 2)
		{
			CIniFile szIniFile;
			//别名设置
			TCHAR strKey[64]={0};
			sprintf(strKey,_T("SERVER_DEV_DNSNAME_%d"),m_CurrentDev);
			CString strValue = szIniFile.GetIniFileString(_T("myyeset"),strKey);
			m_EditC31.SetWindowText(strValue);
		}
		if(m_CurrentTabSel == 0)
		{
			CIniFile szIniFile;
			//流媒体设备端口
			TCHAR strKey[64]={0};
			CString strValue="";
			sprintf(strKey,_T("C_CONFIG_Stream_DEV_PORT_%d"),m_CurrentDev);
			strValue = szIniFile.GetIniFileString(_T("myyeset"),strKey);
			m_EditC1.SetWindowText(strValue);


			//服务端口
			sprintf(strKey,_T("C_CONFIG_Stream_SERVER_PORT_%d"),m_CurrentDev);
			strValue = szIniFile.GetIniFileString(_T("myyeset"),strKey);
			m_EditC2.SetWindowText(strValue);
				
			//播放缓冲
			sprintf(strKey,_T("C_CONFIG_Stream_PLAY_CASH_%d"),m_CurrentDev);
			strValue = szIniFile.GetIniFileString(_T("myyeset"),strKey);
			m_EditC3.SetWindowText(strValue);
		}
	}
}

void CClientConfigDlg::OnButtonCSave() 
{
	CIniFile szIniFile;
    CString strValue ;
	TCHAR strKey[64]={0};

	if(m_CurrentTabSel == 0)
	{
		//流媒体设备端口
		m_EditC1.GetWindowText(strValue);
		sprintf(strKey,_T("C_CONFIG_Stream_DEV_PORT_%d"),m_CurrentDev);
		szIniFile.WriteIniFile(_T("myyeset"),strKey,strValue);
		

		//服务端口
		m_EditC2.GetWindowText(strValue);
		sprintf(strKey,_T("C_CONFIG_Stream_SERVER_PORT_%d"),m_CurrentDev);
		szIniFile.WriteIniFile(_T("myyeset"),strKey,strValue);
		
			
		//播放缓冲
		m_EditC3.GetWindowText(strValue);
		sprintf(strKey,_T("C_CONFIG_Stream_PLAY_CASH_%d"),m_CurrentDev);
		szIniFile.WriteIniFile(_T("myyeset"),strKey,strValue);
		
		
	}

	if(m_CurrentTabSel == 1)
	{
		//密码保存
		CString strPWd1,strPwd2;
		m_EditC22.GetWindowText(strPWd1);
		m_EditC23.GetWindowText(strPwd2);

		if(strPWd1.Compare(strPwd2)!=0)
		{
			AfxMessageBox(_T("两次输入,新密码不一致!"));
			m_EditC22.SetFocus();
			return;
		}

		m_EditC21.GetWindowText(strValue);

		//别名设置
		TCHAR strKey[64]={0};
		TCHAR strSerchkey[64]={0};

		sprintf(strSerchkey,_T("SERVER_DEV_LOGIN_PWD_%d"),m_CurrentDev);

		CString strValuepwd = szIniFile.GetIniFileString(_T("myyeset"),strSerchkey);
		if(strValue.Compare(strValuepwd)!=0)
		{
            AfxMessageBox(_T("当前密码错误，请确认!"));
			m_EditC21.SetFocus();
			return;
		}

		szIniFile.WriteIniFile(_T("myyeset"),_T("login_pwd"),strPWd1);
		AfxMessageBox(_T("新密码设置成功,请牢记!"));

		//设置密码命令
    	//?activeiHomectrlSetPWD(char *username, char *passwd, char *szNewPass);

		return;

	}

    if(m_CurrentTabSel == 2)
	{
		if(m_CurrentDev >=0)
		{
				//别名设置
			sprintf(strKey,_T("SERVER_DEV_DNSNAME_%d"),m_CurrentDev);
			m_EditC31.GetWindowText(strValue);
			szIniFile.WriteIniFile(_T("myyeset"),strKey,strValue);
		}
	}	

	//发送给主窗口说明当前配置消息已经改变
	GetParent()->PostMessage(WM_ITEM_DEV_ON_CHANGE,m_CurrentTabSel,m_CurrentDev+1);
	 
}
