// PageLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "NetDiskUI.h"
#include "PageLogin.h"
#include "afxdialogex.h"

// CPageLogin 对话框

IMPLEMENT_DYNAMIC(CPageLogin, CDialogEx)

CPageLogin::CPageLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageLogin::IDD, pParent)
	, m_bSavePassWd(TRUE)
	, m_bAutoLogin(TRUE)
{

}

CPageLogin::~CPageLogin()
{
}

void CPageLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_LOGIN, m_btnLogin);
	DDX_Control(pDX, IDC_COMBOBOXEX_USR, m_ComboUsr);
	DDX_Control(pDX, IDC_EDIT_PWD_LOGIN, m_EditPasswd);
	DDX_Check(pDX, IDC_CHECK_SAVE_PWD, m_bSavePassWd);
	DDX_Check(pDX, IDC_CHECK_AUTO_LOGIN, m_bAutoLogin);
}


BEGIN_MESSAGE_MAP(CPageLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CPageLogin::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_CHECK_SAVE_PWD, &CPageLogin::OnBnClickedCheckSavePwd)
END_MESSAGE_MAP()


// CPageLogin 消息处理程序


BOOL CPageLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strLastUsrId = "";
	char szDir[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(),szDir,MAX_PATH);
	CString strConfigINI = szDir;
	strConfigINI.MakeLower();
	strConfigINI = strConfigINI.Left(strConfigINI.ReverseFind('.') + 1) + "ini";
	GetPrivateProfileString("setting","UsrIdHistory","",strLastUsrId.GetBuffer(512),512,strConfigINI);
	strLastUsrId.ReleaseBuffer();
	if (!strLastUsrId.IsEmpty())
	{
		int iStart = 0;
		while(1)
		{
			CString strUsrId = strLastUsrId.Tokenize(";",iStart);
			if (strUsrId.IsEmpty())
				break;

			m_UsrHistory.push_back(strUsrId);
			m_ComboUsr.AddString(strUsrId);
		}
		m_ComboUsr.SetCurSel(0);
	}

	GetPrivateProfileStruct("setting","SavePassWord",&m_bSavePassWd,sizeof(m_bSavePassWd),strConfigINI);
	GetPrivateProfileStruct("setting","AutoLogin",&m_bAutoLogin,sizeof(m_bAutoLogin),strConfigINI);
	if (FALSE == m_bSavePassWd)
		m_bAutoLogin = FALSE;

	if (m_bSavePassWd)
	{
		CDataLogin LogInData;
		CString strLastUsrId;
		m_ComboUsr.GetWindowText(strLastUsrId);
		if (!strLastUsrId.IsEmpty())
		{
			strLastUsrId = "UsrInfo:"+ strLastUsrId;
			GetPrivateProfileStruct("setting",strLastUsrId,&LogInData,sizeof(LogInData),strConfigINI);
			LogInData.Decode();
			m_EditPasswd.SetWindowText(LogInData.m_szPassWd);

			if (strcmp(LogInData.m_szPassWd,"")==0)
				m_bAutoLogin = FALSE;
		}
		else
		{
			m_bSavePassWd = FALSE;
			m_bAutoLogin = FALSE;
		}
	}
	else
		m_bAutoLogin = FALSE;

	char szFileDir[MAX_PATH] = {0};
	GetModuleFileName(NULL,szFileDir,MAX_PATH);
	CString strDir = szFileDir;
	strDir = strDir.Left(strDir.ReverseFind('\\')+1);
	CString strLoginBtnImg = strDir + "Images\\login_btn_normal.bmp";
	CString strLoginBtnImgHover = strDir + "Images\\login_btn_Hover.bmp";
	m_btnLogin.SetImage(strLoginBtnImg,strLoginBtnImgHover,strLoginBtnImgHover,strLoginBtnImg);
	m_btnLogin.SetWindowText("");
	CRect rc;
	m_btnLogin.GetClientRect(rc);
	m_btnLogin.MapWindowPoints(this,rc);
	m_btnLogin.MoveWindow(rc.left,rc.top,238,35);
	UpdateData(FALSE);

	if (m_bAutoLogin)
		m_btnLogin.PostMessage(BM_CLICK,0,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPageLogin::OnBnClickedBtnLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSel = m_ComboUsr.GetCurSel();
	CString strUsrId;
	if(iSel==-1)
	{
		m_ComboUsr.GetWindowText(strUsrId);
	}
	else
	{
		m_ComboUsr.GetLBText(iSel,strUsrId);
	}
	
	

	CString strPwd = "";
	m_EditPasswd.GetWindowText(strPwd);

	SOCKET sockClent = GetConnect(theApp.m_strServerIp);
	if (sockClent == 0)
	{
		MessageBox("连接服务器失败!","提示",MB_ICONERROR|MB_OK);
		return;
	}

	bool bok = Login(sockClent,strUsrId,strPwd,m_LoginRes);
	if (!bok)
	{
		MessageBox(m_LoginRes.m_szErrMsg,"错误",MB_ICONERROR|MB_OK);
		return;
	}

	char szDir[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(),szDir,MAX_PATH);
	CString strConfigINI = szDir;
	strConfigINI.MakeLower();
	strConfigINI = strConfigINI.Left(strConfigINI.ReverseFind('.') + 1) + "ini";
	

	vector<CString>::iterator iter = m_UsrHistory.begin();
	for (;iter!=m_UsrHistory.end();iter++)
	{
		if (iter->Compare(strUsrId)==0)
		{
			m_UsrHistory.erase(iter); // 先去掉之前的登录历史，把它移动到最前端
			break;
		}
	}

	CString strHistoryList = strUsrId + ";"; // 这次登录历史放在最前端
	for (size_t i=0;i< m_UsrHistory.size();i++)
	{
		strHistoryList += m_UsrHistory[i];
		strHistoryList +=";";
	}

	WritePrivateProfileString("setting","UsrIdHistory",strHistoryList,strConfigINI);
	::PostMessage(GetParent()->GetSafeHwnd(),WM_LOGIN_SUCCESS,0,NULL);

	UpdateData(TRUE);
	if (FALSE == m_bSavePassWd)
		m_bAutoLogin = FALSE;
	WritePrivateProfileStruct("setting","SavePassWord",&m_bSavePassWd,sizeof(m_bSavePassWd),strConfigINI);
	WritePrivateProfileStruct("setting","AutoLogin",&m_bAutoLogin,sizeof(m_bAutoLogin),strConfigINI);

	if (m_bSavePassWd)
	{
		CDataLogin LogInData;
		strcpy_s(LogInData.m_szUsrId,100,strUsrId);
		strcpy_s(LogInData.m_szPassWd,50,strPwd);
		CString strKey = "UsrInfo:"+ strUsrId;
		LogInData.Code();
		WritePrivateProfileStruct("setting",strKey,&LogInData,sizeof(LogInData),strConfigINI);
	}
}


BOOL CPageLogin::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			 OnBnClickedBtnLogin();
			 return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPageLogin::OnBnClickedCheckSavePwd()
{
	// TODO: 在此添加控件通知处理程序代码
}
CString CPageLogin::GetPassWd()
{
	CString str;
	m_EditPasswd.GetWindowText(str);
	return str;
}