// MeYeDlg.cpp : implementation file
//
#include <objbase.h> 
#include "stdafx.h"
#include "MeYe.h"
#include "MeYeDlg.h"
#include "InfoWnd.h"
#include <shlwapi.h>
#include "MD5.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#define		TIMER_SHOW_INFO		233



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeYeDlg dialog

CMeYeDlg::CMeYeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeYeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeYeDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_brushBkrect = ::CreateSolidBrush(RGB(38,59,90));
    m_brushBkrectMid = ::CreateSolidBrush(RGB(221,228,228));

//	 m_pRealRightDlg=NULL;
//	 m_pRecoderDlg = NULL;
	 m_pSConfigDlg = NULL;
	 m_pCConfigDlg = NULL;
	 m_pVideoCtrlDlg = NULL;
	 m_pLeftDlg = NULL;
	 
	 m_CurSel = 0;

	m_strscroll = m_strloginStatus = m_strStatus = m_strVersion = _T("");
    m_scrollLen=0;
	m_scrollpos=0;

	m_nCMNET = TRUE;
	m_bloging_falg = FALSE;
	m_bloging_index = -1;
	m_cam_id = -1;

	m_strTimeInfo  = _T("");
}

void CMeYeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeYeDlg)
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_BtnSearch);
	DDX_Control(pDX, IDC_BUTTON_MAIN_S_EXIT, m_ExitBtn);
	DDX_Control(pDX, IDC_BUTTON_MAIN_S_CONFIG, m_MainSConfigBtn);
	DDX_Control(pDX, IDC_BUTTON_MAIN_RAL, m_MainRealBtn);
	DDX_Control(pDX, IDC_BUTTON_MAIN_HISTORY, m_MainHistoryBtn);
	DDX_Control(pDX, IDC_BUTTON_MAIN_C_CONFIG, m_MainCConfogBtn);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_CloseBtn);
	DDX_Control(pDX, IDC_BUTTON_MIN, m_MinBtn);
	DDX_Control(pDX, IDC_BUTTON_CONF, m_ConfBtn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMeYeDlg, CDialog)
	//{{AFX_MSG_MAP(CMeYeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_MIN, OnButtonMin)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_RAL, OnButtonMainRal)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_HISTORY, OnButtonMainHistory)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_C_CONFIG, OnButtonMainCConfig)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_S_CONFIG, OnButtonMainSConfig)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_S_EXIT, OnButtonMainSExit)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_ITEM_DEV_SELECT, OnItemChange)
	ON_BN_CLICKED(IDC_BUTTON_CONF, OnButtonConf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




LRESULT CMeYeDlg::OnItemChange(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
// CMeYeDlg message handlers

CString  CMeYeDlg::GetCurrentApPath()
{
	TCHAR DBFileName[MAX_PATH] ;
	memset(DBFileName,0x00,sizeof(DBFileName));
	GetModuleFileName(AfxGetInstanceHandle(), DBFileName, MAX_PATH);	
	CString strPath_temp;
	strPath_temp = DBFileName;
	strPath_temp = strPath_temp.Mid(0, strPath_temp.ReverseFind('\\'));
	return strPath_temp;
}


BOOL CMeYeDlg::DownXmlVer(void )
{
    
	return FALSE;
}

BOOL CMeYeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	



	// TODO: Add extra initialization here
	m_ucSel=0;
	m_ucNum=0;
	m_ucUser=0;
	m_ucChan=0;


	m_strVersion = "主程序版本 :";
	m_strscroll  = "测试版本,主程序版本,initialization here";

	CInternetSession mySession(NULL,0);
	CHttpFile* myHttpFile=NULL;

	CString myData,SiteInfo,strRet;
	long nPos1;

	BOOL bOpenFile(FALSE);
	try
	{
		myHttpFile=(CHttpFile*)mySession.OpenURL("http://wgz360.com/wbhfeng/initconfig.php");
		bOpenFile = TRUE;
	}
	catch(CInternetException* pEx)
	{
       myHttpFile->Close();
	   myHttpFile=NULL;//如果发生运行时错误，给file赋空值，程序将继续运行
       
	   OutputDebugString(_T("Error IE\r\n"));
	   Sleep(500);
	   bOpenFile = FALSE;
	   
	}

	if(bOpenFile)
	{

		while(myHttpFile->ReadString(myData))
		{
			SiteInfo +=  myData;
		}

		myHttpFile->Close();
		myHttpFile=NULL;
		mySession.Close();

		nPos1 = SiteInfo.Find("<version>");
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<version>"));
		nPos1 = SiteInfo.Find("</version>");

		strRet = SiteInfo.Left(nPos1);
		//写入到ini文件
		m_strVersion += strRet;


		nPos1 = SiteInfo.Find("<update>");
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<update>"));
		nPos1 = SiteInfo.Find("</update>");

		strRet = SiteInfo.Left(nPos1);//强制升级

		if(strRet.Compare("1")==0)
		{
			AfxMessageBox("因新浪开放API相关法则,网购者,已经不支持旧版本了，说情查看!");
			ShellExecute(NULL,NULL,_T("http://www.wgz360.com/"),NULL,NULL,SW_SHOWNORMAL);
			return 0;
		}

		if(strRet.Compare("2")==0)
		{
			AfxMessageBox("因新浪开放API相关法则,本软件已经不能使用了,说情看详细规说明!");
			ShellExecute(NULL,NULL,_T("http://www.wgz360.com/"),NULL,NULL,SW_SHOWNORMAL);
			return 0;
		}


		nPos1 = SiteInfo.Find("<xmlver>");//价格临控的版本，可支持的自定义数量
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<xmlver>"));
		nPos1 = SiteInfo.Find("</xmlver>");

		strRet = SiteInfo.Left(nPos1);

		CIniFile szIniFile;
		CString strtempIniVer = szIniFile.GetIniFileString(_T("wbidstar"),_T("procfgver"));

		if(strtempIniVer.Compare(strRet)!=0)
		{//版本不一样，去下载XML文件
            DownXmlVer();
			szIniFile.WriteIniFile(_T("wbidstar"),_T("procfgver"),strRet);
		}

		//image//procfg.ini


		nPos1 = SiteInfo.Find("<num1>");//笑话数量
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<num1>"));
		nPos1 = SiteInfo.Find("</num1>");

		strRet = SiteInfo.Left(nPos1);


		nPos1 = SiteInfo.Find("<num2>");//微言大义数量
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<num2>"));
		nPos1 = SiteInfo.Find("</num2>");

		strRet = SiteInfo.Left(nPos1);


		nPos1 = SiteInfo.Find("<num3>");//搞笑配图
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<num3>"));
		nPos1 = SiteInfo.Find("</num3>");

		strRet = SiteInfo.Left(nPos1);


		nPos1 = SiteInfo.Find("<num4>");//经典微语录
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<num4>"));
		nPos1 = SiteInfo.Find("</num4>");

		strRet = SiteInfo.Left(nPos1);


		nPos1 = SiteInfo.Find("<num5>");//英语经典
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<num5>"));
		nPos1 = SiteInfo.Find("</num5>");

		strRet = SiteInfo.Left(nPos1);


		nPos1 = SiteInfo.Find("<add1>");//新增说明1
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<add1>"));
		nPos1 = SiteInfo.Find("</add1>");

		strRet = SiteInfo.Left(nPos1);


		nPos1 = SiteInfo.Find("<addurl1>");//新增说明1
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<addurl1>"));
		nPos1 = SiteInfo.Find("</addurl1>");

		strRet = SiteInfo.Left(nPos1);



		nPos1 = SiteInfo.Find("<add2>");//新增说明2
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<add2>"));
		nPos1 = SiteInfo.Find("</add2>");

		strRet = SiteInfo.Left(nPos1);


		nPos1 = SiteInfo.Find("<addurl2>");//新增说明2
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<addurl2>"));
		nPos1 = SiteInfo.Find("</addurl2>");

		strRet = SiteInfo.Left(nPos1);


		nPos1 = SiteInfo.Find("<add3>");//新增说明3
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<add3>"));
		nPos1 = SiteInfo.Find("</add3>");

		strRet = SiteInfo.Left(nPos1);


		nPos1 = SiteInfo.Find("<addurl3>");//新增说明2
		SiteInfo= SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strlen("<addurl3>"));
		nPos1 = SiteInfo.Find("</addurl3>");

		strRet = SiteInfo.Left(nPos1);
	}




	//以下是界面信息
	long i = 0;
	long j = 0;


	this->MoveWindow(0,0,850,550);

	CRect rcClient;
	GetClientRect(&rcClient);

	SetBackImage(0,_T("分图\\顶部背景.bmp"));
	SetBackImage(1,_T("分图\\状态.bmp"));
	SetBackImage(2,_T("image\\button_begin.BMP"));


	CPoint szpoint;
	m_MainRealBtn.SetImage(_T("分图\\菜单_1_2.bmp"),_T("分图\\菜单_1_1.bmp"),_T("分图\\菜单_1_0.bmp"),_T("分图\\菜单_1_0.bmp"));
	m_MainRealBtn.SetWindowText("");




	m_MainHistoryBtn.SetImage(_T("分图\\菜单_2_2.bmp"),_T("分图\\菜单_2_1.bmp"),_T("分图\\菜单_2_0.bmp"),_T("分图\\菜单_2_0.bmp"));
	m_MainHistoryBtn.SetWindowText("");



	m_MainCConfogBtn.SetImage(_T("分图\\菜单_3_2.bmp"),_T("分图\\菜单_3_1.bmp"),_T("分图\\菜单_3_0.bmp"),_T("分图\\菜单_3_0.bmp"));
	m_MainCConfogBtn.SetWindowText("");


	m_MainSConfigBtn.SetImage(_T("分图\\菜单_4_2.bmp"),_T("分图\\菜单_4_1.bmp"),_T("分图\\菜单_4_0.bmp"),_T("分图\\菜单_4_0.bmp"));
	m_MainSConfigBtn.SetWindowText("");


	m_ExitBtn.SetImage(_T("分图\\菜单_5_2.bmp"),_T("分图\\菜单_5_1.bmp"),_T("分图\\菜单_5_0.bmp"),_T("分图\\菜单_5_0.bmp"));
	m_ExitBtn.SetWindowText("");

	m_BtnSearch.SetImage(_T("分图\\按钮.bmp"),_T("分图\\按钮.bmp"),_T("分图\\按钮.bmp"),_T("分图\\按钮.bmp"));
 

	CSkinBtn* pBtnall[5]={&m_MainRealBtn,&m_MainHistoryBtn,&m_MainCConfogBtn,&m_MainSConfigBtn,&m_ExitBtn};

	CRect rcBtn;
	memset(rcBtn,0x00,sizeof(rcBtn));
	for( i = 0; i< 5; i++)
	{
		rcBtn.left  = rcClient.left + i*79 + (i + 1)*10;
		rcBtn.right =   rcBtn.left + 79;
		rcBtn.top    = 25;
		rcBtn.bottom    = rcBtn.top + 85;
		pBtnall[i]->MoveWindow(rcBtn,FALSE);
	}
	m_MainRealBtn.SetCurrentShow(1);


	// 右上角的三个按妞
	m_ConfBtn.SetImage(_T("分图\\隐藏.bmp"),_T("分图\\隐藏_1.bmp"),_T("分图\\隐藏_2.bmp"),_T("分图\\隐藏.bmp"));
	CRect tempUp = rcClient;
	tempUp.left = rcClient.right - (34 + 34 +34  + 16);//21
	tempUp.right = tempUp.left + 34 ;//21
	tempUp.top = rcClient.top + 5;
	tempUp.bottom = rcClient.top + 24 ;
	m_ConfBtn.SetWindowText(_T(""));
	m_ConfBtn.MoveWindow(tempUp,true);

	m_MinBtn.SetImage(_T("分图\\最小化.bmp"),_T("分图\\最小化_1.bmp"),_T("分图\\最小化_2.bmp"),_T("分图\\最小化.bmp"));
	tempUp.left = rcClient.right - (34 + 34 + 16);//21
	tempUp.right = tempUp.left  +  34 ;//21
	tempUp.top = rcClient.top + 5;
	tempUp.bottom = rcClient.top + 24 ;
	m_MinBtn.SetWindowText(_T(""));
	m_MinBtn.MoveWindow(tempUp,true);
	

	m_CloseBtn.SetImage(_T("分图\\关闭.bmp"),_T("分图\\关闭_1.bmp"),_T("分图\\关闭_2.bmp"),_T("分图\\关闭.bmp"));
	tempUp.left = rcClient.right - (34 + 16);//21
	tempUp.right = tempUp.left  +  34 ;//21
	tempUp.top = rcClient.top + 5;
	tempUp.bottom = rcClient.top + 24 ;
	m_CloseBtn.SetWindowText(_T(""));
	m_CloseBtn.MoveWindow(tempUp,true);

//	SetLayeredWindowTrans(240);




     m_pVideoCtrlDlg=new CVideoCtrlDlg;
	 m_pVideoCtrlDlg->Create(IDD_DIALOG_VIDEO_CTRL,this);
	 m_pVideoCtrlDlg->MoveWindow(rcClient.left + 190,112 , 430+222, 408);
	 m_pVideoCtrlDlg->ShowWindow(SW_SHOW);
	 m_pVideoCtrlDlg->SetMainCall(this);


	 m_pLeftDlg=new CLeftClientDlg;
	 m_pLeftDlg->Create(IDD_DIALOG_LEFT_CLIENT,this);
	 m_pLeftDlg->MoveWindow(rcClient.left+2, 112, 190, 408);
//	 m_pLeftDlg->SetCurDlgSta(0);
	 m_pLeftDlg->ShowWindow(SW_SHOW);

	 //设置关联IE窗口
	 m_pLeftDlg->SetIeHwnd(m_pVideoCtrlDlg);


	// CPriceUpDlg*      m_pLeftPriceDlg;
     

	 //价格监控左窗口
	 m_pLeftPriceDlg=new CPriceUpDlg;
	 m_pLeftPriceDlg->Create(IDD_DIALOG_PRICEUP,this);
	 m_pLeftPriceDlg->MoveWindow(rcClient.left+2, 112, 430+222 + 190, 408);
//	 m_pLeftDlg->SetCurDlgSta(0);
	 m_pLeftPriceDlg->ShowWindow(SW_HIDE);

	 //IE比价右窗口
//	 m_pBjCtrlDlg=new CVideoCtrlDlg;
//	 m_pBjCtrlDlg->Create(IDD_DIALOG_VIDEO_CTRL,this);
//	 m_pBjCtrlDlg->MoveWindow(rcClient.left + 190,112 , 430+222, 408);
//	 m_pBjCtrlDlg->ShowWindow(SW_SHOW);
//	 m_pBjCtrlDlg->SetMainCall(this);


	 //ini读设备信息值
	GetModuleFileName(NULL,m_FilePath,255); 
	*(strstr(m_FilePath,".exe"))= 0;
	strcat(m_FilePath, ".ini");
	*(m_FilePath+strlen(m_FilePath)+1) = 0;
	CString tmp;
	char szTemp[255];



	memset(szTemp,0x00,sizeof(szTemp));
	GetPrivateProfileString("myyeset","SERVER_DEV_DNS_COUNT","",szTemp,255,m_FilePath);

	/*

	long nCountDev = atoi(szTemp);

	Productors szProductors;	
	for( i = 0; i < nCountDev;i++)
	{
		//终端控制设备名称
		memset(szTemp,0x00,sizeof(szTemp));
		memset(szValue,0x00,sizeof(szValue));
		sprintf(szValue,_T("SERVER_DEV_DNS_%d"),i);
		GetPrivateProfileString("myyeset",szValue,"",szTemp,255,m_FilePath);
		sprintf(szProductors.ipadd,_T("%s"),szTemp);

		//终端控制别名
		memset(szTemp,0x00,sizeof(szTemp));
		memset(szValue,0x00,sizeof(szValue));
		sprintf(szValue,_T("SERVER_DEV_DNSNAME_%d"),i);
		GetPrivateProfileString("myyeset",szValue,"",szTemp,255,m_FilePath);
		sprintf(szProductors.itemName,_T("%s"),szTemp);

		//终端password
		memset(szTemp,0x00,sizeof(szTemp));
		memset(szValue,0x00,sizeof(szValue));
		sprintf(szValue,_T("SERVER_DEV_LOGIN_PWD_%d"),i);
		GetPrivateProfileString("myyeset",szValue,"",szTemp,255,m_FilePath);
    	sprintf(szProductors.pwd,_T("%s"),szTemp);


		szProductors.nId = i;


	}
*/

	//读用户名,密码
	GetPrivateProfileString("myyeset","login_user","",login_user,sizeof(login_user),m_FilePath);
	

	/* m_pRealRightDlg=new CRealVideoDlg;
	 m_pRealRightDlg->Create(IDD_DIALOG_VIDEO_REAL,this);
	 m_pRealRightDlg->MoveWindow(rcClient.right - 230, 112, 228, 408);
	 m_pRealRightDlg->ShowWindow(SW_SHOW);
	  m_pRealRightDlg->SetMainCall(this);

	 
	 m_pRecoderDlg=new CRecoderDlg;
	 m_pRecoderDlg->Create(IDD_DIALOG_RECODER,this);
	 m_pRecoderDlg->MoveWindow(rcClient.right - 230, 112, 228, 408);
	 m_pRecoderDlg->ShowWindow(SW_HIDE);
	 m_pRecoderDlg->SetMainCall(this);*/

	 m_pSConfigDlg=new CSConfigDlg;
	 m_pSConfigDlg->Create(IDD_DIALOG_S_CONF_DLG,this);
	 m_pSConfigDlg->MoveWindow(rcClient.left + 2,112 , rcClient.right - rcClient.left  -4, 408);
	 m_pSConfigDlg->ShowWindow(SW_HIDE);
	 

	 
	 m_pCConfigDlg=new CClientConfigDlg;
	 m_pCConfigDlg->Create(IDD_DIALOG_C_CONF_DLG,this);
	 m_pCConfigDlg->MoveWindow(rcClient.left + 190 ,112 , rcClient.right - rcClient.left  -192, 408);
	 m_pCConfigDlg->ShowWindow(SW_HIDE);

	 

	
	 
	 
	 
	 
	    
	//左窗口
/*	CInfoWnd* pWnd=new CInfoWnd;
	pWnd->Create(this->m_hWnd);
	pWnd->SetWndCaption(_T("终端控制"));

	pWnd->ModifyStyle(WS_BORDER,   0);


	



	pWnd->SetCloseTime(-1);

	if(!pWnd) return FALSE;
	//设定窗口位置和大小
	pWnd->MoveWindow(8, 112, 182, 408);

	pWnd->m_lPARAM.nFlag = 1;

	CRect rcNew;
	pWnd->GetWindowRect(&rcNew);
	pWnd->m_lPARAM.rcPos = rcNew;
	pWnd->m_lPARAM.bMove = FALSE;
	
	pWnd->ShowWindow(SW_SHOW);
    */
	
	CenterWindow();

	







	
	SetWindowText("MyYe-视频监控");

	CoInitializeEx(NULL, COINIT_MULTITHREADED);


	SetTimer(TIMER_SHOW_INFO,300,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMeYeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMeYeDlg::OnPaint() 
{

	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);


	//内存画图//////////////////////////
	CDC szMemDC;
	szMemDC.CreateCompatibleDC(&dc);

	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	szMemDC.SelectObject(&btScreen);
	btScreen.DeleteObject();


	HGDIOBJ oldBrush=szMemDC.SelectObject(m_brushBkrect);
	szMemDC.Rectangle(rcClient);
	szMemDC.SelectObject(oldBrush);


	CRect rcMid = rcClient;
	rcMid.left = rcClient.left + 10;
	rcMid.top = rcClient.top + 140;
	rcMid.bottom = rcClient.bottom -25;
	rcMid.right = rcClient.right - 10;
	

	oldBrush=szMemDC.SelectObject(m_brushBkrectMid);
	szMemDC.Rectangle(rcMid);
	szMemDC.SelectObject(oldBrush);


    //头部主图背景
	CRect rcTop = rcClient;
	rcTop.bottom =   100;
	DrawRangeImage(&m_BitmapBack[0],&szMemDC,rcTop);

	//底部主图背景
	CRect rcBottom = rcClient;
	rcBottom.top =   rcClient.bottom - 30;
	DrawRangeImage(&m_BitmapBack[1],&szMemDC,rcBottom);
    szMemDC.SetTextColor(RGB(225,225,225));
	szMemDC.SetBkMode(TRANSPARENT);
	szMemDC.TextOut(10,rcClient.bottom-23,m_strVersion);
    

	szMemDC.TextOut(300,rcClient.bottom-23,m_strscroll);
	

	//当前登陆状态提示
	if(m_strloginStatus.GetLength() > 0)
	{
	  CSize   cs=szMemDC.GetTextExtent(m_strloginStatus);//获得字符串长度 

	   szMemDC.TextOut(rcClient.right - cs.cx-10,rcClient.bottom-23,m_strloginStatus);
	}
    
	// 画按妞

	szMemDC.SetTextColor(RGB(38,59,90));
	szMemDC.SetBkMode(TRANSPARENT);
	szMemDC.TextOut(10,5,_T("网购者 - (自动发微博)"));
	//创建不规则窗体
	ChangeWindowRgn(&szMemDC);

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
    


	szMemDC.DeleteDC();
}


BOOL CMeYeDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(_icur==0)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CMeYeDlg::OnNcHitTest(CPoint point)
{
	
	ScreenToClient(&point);

	RECT rtWindow;
	GetClientRect(&rtWindow);

    long wndHeight = rtWindow.bottom - rtWindow.top;
	long wndWidth = rtWindow.right - rtWindow.left;

	


	_icur = 0;
	RECT rcW = {0,0,wndWidth,100};
	if(::PtInRect(&rcW,point))
	{  
		return HTCAPTION;  // 在拖动范围内
	}

	return CWnd::OnNcHitTest(point);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMeYeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



//载入资源位图
void CMeYeDlg::SetBackImage(int index,CString strName)
{
	SetImage(m_BitmapBack[index], strName);
	SetImageSize(&m_BitmapBack[index], m_szBitmapBack[index]);
}

//载入硬盘文件位图
void CMeYeDlg::SetImage(CBitmap &bitmap, CString strName)
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

//保存位图尺寸
void CMeYeDlg::SetImageSize(CBitmap *pBitmap, CSize &sz)
{
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	sz = CSize(bm.bmWidth, bm.bmHeight);
}

void CMeYeDlg::SetLayeredWindowTrans(int nTrans)
{
   SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,   
   GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);   
   HINSTANCE   hInst   =   LoadLibrary("User32.DLL");     
   if(hInst)     
   {     
      typedef   BOOL   (WINAPI   *MYFUNC)(HWND,COLORREF,BYTE,DWORD);     
      MYFUNC   fun   =   NULL;   
      //取得SetLayeredWindowAttributes函数指针     
      fun=(MYFUNC)GetProcAddress(hInst,   "SetLayeredWindowAttributes");   
      if(fun)fun(this->GetSafeHwnd(),0,nTrans,2);     
      FreeLibrary(hInst);     
   }   
}

void CMeYeDlg::ChangeWindowRgn(CDC *pDC)
{
	COLORREF col = RGB(255,0,255);
	CRect rcClient;
	GetClientRect (rcClient);
	CRgn rgn;
	rgn.CreateRoundRectRgn (1, 1, rcClient.Width(), rcClient.Height(),6,6);

	SetWindowRgn (rgn, TRUE);
}

void CMeYeDlg::OnClose() 
{
	KillTimer(TIMER_SHOW_INFO);

	// TODO: Add your message handler code here and/or call default
	DeleteObject(m_brushBkrect);
	DeleteObject(m_brushBkrectMid);
	/*if(m_pRealRightDlg)
	{
		delete m_pRealRightDlg;
		m_pRealRightDlg = NULL;
	}*/
/*	if(m_pRecoderDlg)
	{
		delete m_pRecoderDlg;
		m_pRecoderDlg = NULL;
	}*/

	if(m_pSConfigDlg)
	{
		delete m_pSConfigDlg;
		m_pSConfigDlg = NULL;
	}
	if(m_pCConfigDlg)
	{
		delete m_pCConfigDlg;
		m_pCConfigDlg = NULL;
	}

	if(m_pLeftDlg)
	{
		
		delete m_pLeftDlg;
		m_pLeftDlg = NULL;
	}

	if(m_pVideoCtrlDlg)
	{
		
		delete m_pVideoCtrlDlg;
		m_pVideoCtrlDlg = NULL;
	}






	CDialog::OnOK();
}

//平铺画位图
void CMeYeDlg::DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc)
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

void CMeYeDlg::OnButtonClose() 
{
	
	OnClose();
}

void CMeYeDlg::OnButtonMin() 
{
	this->ShowWindow(SW_MINIMIZE);
}

void CMeYeDlg::OnChangeSelDlg(void)
{
   CSkinBtn* pBtnall[5]={&m_MainRealBtn,&m_MainHistoryBtn,&m_MainCConfogBtn,&m_MainSConfigBtn,&m_ExitBtn};
	for(long i = 0; i< 5; i++)
	{
		pBtnall[i]->SetCurrentShow(0);
		pBtnall[i]->Invalidate(FALSE);
	}
	pBtnall[m_CurSel]->SetCurrentShow(1);
	pBtnall[m_CurSel]->Invalidate(FALSE);


	if(m_CurSel == 0)
	{
     //  m_pRealRightDlg->ShowWindow(SW_SHOW);
	 //  m_pRecoderDlg->ShowWindow(SW_HIDE);
	//   m_pCConfigDlg->ShowWindow(SW_HIDE);
	   m_pSConfigDlg->ShowWindow(SW_HIDE);

	   m_pLeftDlg->ShowWindow(SW_SHOW);
//       m_pLeftDlg->SetCurDlgSta(0);

	   //视频窗口显示
//	   m_pVideoCtrlDlg->SetIEshow(SW_SHOW);
	   m_pVideoCtrlDlg->ShowWindow(SW_SHOW);
	   

	  
	   m_pLeftPriceDlg->ShowWindow(SW_HIDE);
//	   m_pBjCtrlDlg->ShowWindow(SW_HIDE);
//	   m_pBjCtrlDlg->SetIEshow(SW_HIDE);
	   

	}

	if(m_CurSel == 1)
	{
	//	m_pRealRightDlg->ShowWindow(SW_SHOW);
    //    m_pRecoderDlg->ShowWindow(SW_SHOW);
		
      // m_pRealRightDlg->ShowWindow(SW_HIDE);
       // m_pRecoderDlg->ShowWindow(SW_HIDE);
	//	m_pCConfigDlg->ShowWindow(SW_HIDE);
		m_pSConfigDlg->ShowWindow(SW_SHOW);
		m_pSConfigDlg->initDlg();

//		m_pVideoCtrlDlg->SetIEshow(SW_HIDE);
		m_pLeftDlg->ShowWindow(SW_HIDE);
		

		m_pVideoCtrlDlg->ShowWindow(SW_HIDE);

		m_pLeftPriceDlg->ShowWindow(SW_HIDE);
	//   m_pBjCtrlDlg->ShowWindow(SW_HIDE);
	//   m_pBjCtrlDlg->SetIEshow(SW_HIDE);

//        m_pLeftDlg->SetCurDlgSta(0);
	   
	}

	if(m_CurSel == 2)
	{
       // m_pRealRightDlg->ShowWindow(SW_HIDE);
       // m_pRecoderDlg->ShowWindow(SW_HIDE);
	//	m_pCConfigDlg->ShowWindow(SW_SHOW);
		m_pSConfigDlg->ShowWindow(SW_HIDE);

		m_pLeftDlg->ShowWindow(SW_HIDE);
//        m_pLeftDlg->SetCurDlgSta(1);
		//视频窗口显示
//		m_pVideoCtrlDlg->SetIEshow(SW_HIDE);
	    m_pVideoCtrlDlg->ShowWindow(SW_HIDE);
	   

	   m_pLeftPriceDlg->ShowWindow(SW_SHOW);
	 //  m_pBjCtrlDlg->ShowWindow(SW_SHOW);
	 //  m_pBjCtrlDlg->SetIEshow(SW_SHOW);

	}

	if(m_CurSel == 3)
	{
       
	  //视频窗口显示
	   m_pVideoCtrlDlg->ShowWindow(SW_HIDE);

	 //  m_pCConfigDlg->ShowWindow(SW_HIDE);
		m_pSConfigDlg->ShowWindow(SW_HIDE);

		m_pLeftDlg->ShowWindow(SW_SHOW);
//        m_pLeftDlg->SetCurDlgSta(0);

		//视频窗口显示
	  //
//		m_pVideoCtrlDlg->SetIEshow(SW_HIDE);
		m_pVideoCtrlDlg->ShowWindow(SW_SHOW);

		
	}
}

void CMeYeDlg::OnButtonMainRal() 
{
	// TODO: Add your control notification handler code here
	m_CurSel = 0;
	OnChangeSelDlg();

}

void CMeYeDlg::OnButtonMainHistory() 
{
	
	// TODO: Add your control notification handler code here
	m_CurSel = 1;
	OnChangeSelDlg();
}

void CMeYeDlg::OnButtonMainCConfig() 
{
	// TODO: Add your control notification handler code here
	m_CurSel = 2;
	OnChangeSelDlg();
}

void CMeYeDlg::OnButtonMainSConfig() 
{
	ShellExecute(NULL,NULL,_T("http://www.wgz360.com/eSerbjprolist.php?type=2&q=5233"),NULL,NULL,SW_SHOWNORMAL);
	
}

void CMeYeDlg::OnButtonMainSExit() 
{

	ShellExecute(NULL,NULL,_T("http://www.wgz360.com/etyhq/index.php?type=15"),NULL,NULL,SW_SHOWNORMAL);

	//CoUninitialize();
	//OleUninitialize(); 

	//OnClose();
}

BOOL CMeYeDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
//	CLient
//	
    
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CMeYeDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnMouseMove(nFlags, point);
}

void CMeYeDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	
	CDialog::OnLButtonDown(nFlags, point);
}








//登陆失败
void CMeYeDlg::LoginExit(void)
{
	m_bloging_index=-1;
	m_bloging_falg = FALSE;
}

//登陆成功,自动查看视频
void CMeYeDlg::LoginViewVideo(void)
{

}


//得到当前摄像头
void CMeYeDlg::SetVideoInput(int sel,int count)
{//只有一个摄像头

}

void CMeYeDlg::loginagain(void)
{

}

void CMeYeDlg::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == TIMER_SHOW_INFO )
	{
		m_strscroll  = "测试版本,主程序版本,initialization here";
        m_scrollLen = m_strscroll.GetLength();
	    if(m_scrollpos <= m_scrollLen)
		{
			m_scrollpos++;
		}
		else
		{
			m_scrollpos = 0;
		}
		m_strscroll = m_strscroll.Right(m_scrollLen-m_scrollpos);


		CRect rcClient;
	    GetClientRect(&rcClient);


		rcClient.top = rcClient.bottom - 35;
		rcClient.left = rcClient.right - 550;

		::InvalidateRect(this->m_hWnd,&rcClient,FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}




void CMeYeDlg::DCickVideoGridListCtrl(CString url)
{


	
}




CString CMeYeDlg::TimeToString(__int64 llTime)
{
	CString strTime;
	if(llTime<0)
	{
		strTime = _T("-");
	}
	else if(!llTime)
	{
		strTime = _T("0:00:00");
	}
	else
	{
		llTime /= 10000;
		int nHour,nMinute,nSecond,nMillisecond;
		nHour = nMinute = nSecond = nMillisecond = 0;
		nMillisecond = (int)(llTime % 1000);
		llTime /= 1000;
		nSecond = (int)(llTime % 60);
		llTime /= 60;
		nMinute = (int)(llTime % 60);
		llTime /= 60;
		nHour = (int)(llTime);
		TCHAR buf[128];
		/*wsprintf(buf,_T("%d:%02d:%02d:%03d"),nHour,nMinute,nSecond,nMillisecond);*/
		wsprintf(buf,_T("%d:%02d:%02d"),nHour,nMinute,nSecond);
		strTime = buf;
	}
	return strTime;
}

void CMeYeDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CMeYeDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnRButtonUp(nFlags, point);
}

/*
BOOL CMeYeDlg::PreTranslateMessage(MSG *pMsg)
{ 
    if(pMsg-> message   ==WM_KEYDOWN) 
        { 
                if(pMsg-> wParam   ==VK_RETURN   ) 
                { 
                     return FALSE;
                } 
         } 
    return     CDialog::PreTranslateMessage   (pMsg); 

}
*/
void CMeYeDlg::OnOK() 
{ 
// TODO: Add extra validation here 
// CDialog::OnOK(); 
} 

void CMeYeDlg::OnButtonSearch() 
{
	char szChar[512] = {0};
	GetDlgItemText(IDC_EDIT_URL_SEARCH,szChar,sizeof(szChar));
	CString keyword = GetGbkToUrlEncode(szChar);
	CString strUrlTo;
    strUrlTo.Format(_T("http://www.wgz360.com/eSerbjprolist.php?q=5233"));
	ShellExecute(NULL,NULL,strUrlTo,NULL,NULL,SW_SHOWNORMAL);

}
CString CMeYeDlg::GetGbkToUrlEncode(CString keyWord)
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

void CMeYeDlg::OnButtonConf() 
{
	ShellExecute(NULL,NULL,_T("http://q.weibo.com/905125"),NULL,NULL,SW_SHOWNORMAL);
}
