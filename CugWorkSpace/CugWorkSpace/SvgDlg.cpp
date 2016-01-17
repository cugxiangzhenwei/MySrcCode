// SvgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CugWorkSpace.h"
#include "SvgDlg.h"


// CSvgDlg 对话框

IMPLEMENT_DYNCREATE(CSvgDlg, CDHtmlDialog)

CSvgDlg::CSvgDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CSvgDlg::IDD, 0, pParent)
{

}

CSvgDlg::~CSvgDlg()
{
}

void CSvgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CSvgDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	OpenDefaultPage();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CSvgDlg::OpenJianMoHuanJing()
{
	Navigate("F:/安世亚太/WorkPlatForm/Debug/PeraWorkspace/modelEditor/html/modelEditor.html");
}
void CSvgDlg::OpenDefaultPage()
{
	Navigate("F:/安世亚太/WorkPlatForm/Debug/PeraWorkSpace/Res/SvgDefaultPage/code.html");
}
void CSvgDlg::OpenWorkHistPage()
{
	Navigate("J:/program/Programing Tools/Adobe_photoshop_cs5_sdk_win/connectionsdk/samples/ios/firetutorial/resources/1.html");
}
BEGIN_MESSAGE_MAP(CSvgDlg, CDHtmlDialog)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CSvgDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CSvgDlg 消息处理程序

HRESULT CSvgDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	//OnOK();
	return S_OK;
}

HRESULT CSvgDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	//OnCancel();
	return S_OK;
}


void CSvgDlg::OnNcDestroy()
{
	CDHtmlDialog::OnNcDestroy();

	// TODO: 在此处添加消息处理程序代码
	delete this;
}
