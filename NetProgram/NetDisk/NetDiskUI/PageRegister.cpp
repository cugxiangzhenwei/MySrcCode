// PageRegister.cpp : 实现文件
//

#include "stdafx.h"
#include "NetDiskUI.h"
#include "PageRegister.h"
#include "afxdialogex.h"
#include "NetDiskOperation.h"
// CPageRegister 对话框

IMPLEMENT_DYNAMIC(CPageRegister, CDialogEx)

CPageRegister::CPageRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageRegister::IDD, pParent)
	, m_strValUsrID(_T(""))
	, m_strValUsrName(_T(""))
	, m_strValPassWd(_T(""))
	, m_strValPassWd2(_T(""))
{

}

CPageRegister::~CPageRegister()
{
}

void CPageRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_REGISTER, m_btnRegister);
	DDX_Control(pDX, IDC_EDIT_REG_USR_ID, m_EditUsrID);
	DDX_Control(pDX, IDC_EDIT_REG_USR_NAME, m_EditUsrName);
	DDX_Control(pDX, IDC_EDIT_REG_USR_PWD, m_EditPassWd);
	DDX_Control(pDX, IDC_EDIT_REG_USR_PWD2, m_EditPassWd2);

	DDX_Text(pDX, IDC_EDIT_REG_USR_ID, m_strValUsrID);
	DDX_Text(pDX, IDC_EDIT_REG_USR_NAME, m_strValUsrName);
	DDX_Text(pDX, IDC_EDIT_REG_USR_PWD, m_strValPassWd);
	DDX_Text(pDX, IDC_EDIT_REG_USR_PWD2, m_strValPassWd2);

	DDV_MaxChars(pDX, m_strValUsrID, 50);
	DDV_MaxChars(pDX, m_strValUsrName, 50);
	DDV_MaxChars(pDX, m_strValPassWd, 20);
	DDV_MaxChars(pDX, m_strValPassWd2, 20);
}


BEGIN_MESSAGE_MAP(CPageRegister, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_REGISTER, &CPageRegister::OnBnClickedBtnRegister)
END_MESSAGE_MAP()


// CPageRegister 消息处理程序


BOOL CPageRegister::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	char szFileDir[MAX_PATH] = {0};
	GetModuleFileName(NULL,szFileDir,MAX_PATH);
	CString strDir = szFileDir;
	strDir = strDir.Left(strDir.ReverseFind('\\')+1);
	CString strLoginBtnImg = strDir + "Images\\register_btn_normal.bmp";
	CString strLoginBtnImgHover = strDir + "Images\\register_btn_hover.bmp";
	m_btnRegister.SetImage(strLoginBtnImg,strLoginBtnImgHover,strLoginBtnImgHover,strLoginBtnImg);
	m_btnRegister.SetWindowText("");
	CRect rc;
	m_btnRegister.GetClientRect(rc);
	m_btnRegister.MapWindowPoints(this,rc);
	m_btnRegister.MoveWindow(rc.left,rc.top,238,35);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPageRegister::OnBnClickedBtnRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strValUsrID.IsEmpty()
		||m_strValUsrName.IsEmpty()
		||m_strValPassWd.IsEmpty()
		||m_strValPassWd2.IsEmpty())
	{
		MessageBox("请填写完整的用户信息，再执行注册!","提示",MB_ICONINFORMATION|MB_OK);
		return;
	}
	if (m_strValPassWd.Compare(m_strValPassWd2)!=0)
	{
		MessageBox("两次输入的密码不一致!","提示",MB_ICONERROR|MB_OK);
		m_EditPassWd2.SetFocus();
		return;
	}

	SOCKET sockClent = GetConnect(theApp.m_strServerIp);
	if (sockClent == 0)
	{
		MessageBox("连接服务器失败!","提示",MB_ICONERROR|MB_OK);
		return;
	}

	if (!RegisterUser(sockClent,m_strValUsrID,m_strValUsrName,m_strValPassWd))
	{
		MessageBox("注册失败!","提示",MB_ICONERROR|MB_OK);
		return;
	}
	else
	{
		MessageBox("注册成功!","提示",MB_ICONINFORMATION|MB_OK);
		return;
	}
}
BOOL CPageRegister::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedBtnRegister();
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
