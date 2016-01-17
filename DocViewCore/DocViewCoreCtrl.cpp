// DocViewCoreCtrl.cpp : CDocViewCoreCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "DocViewCore.h"
#include "DocViewCoreCtrl.h"
#include "DocViewCorePropPage.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CString GetDocTitle(CString strDoc);

IMPLEMENT_DYNCREATE(CDocViewCoreCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CDocViewCoreCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(WM_DOC_OPENED,OnDocOpened)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CDocViewCoreCtrl, COleControl)
	DISP_FUNCTION_ID(CDocViewCoreCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CDocViewCoreCtrl, "OpenDocFile", dispidOpenDocFile, OpenDocFile, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION_ID(CDocViewCoreCtrl, "SelectFile", dispidSelectFile, SelectFile, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CDocViewCoreCtrl, "DocumentOpened", dispidDocumentOpened, DocumentOpened, VT_EMPTY, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CDocViewCoreCtrl, COleControl)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CDocViewCoreCtrl, 1)
	PROPPAGEID(CDocViewCorePropPage::guid)
END_PROPPAGEIDS(CDocViewCoreCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CDocViewCoreCtrl, "DOCVIEWCORE.DocViewCoreCtrl.1",
	0x2f519fba, 0x91e5, 0x439e, 0x8c, 0x46, 0x93, 0x18, 0xec, 0x43, 0xa7, 0xed)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CDocViewCoreCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID IID_DDocViewCore = { 0xB890C406, 0xEAD1, 0x44FF, { 0xAF, 0x3B, 0x4F, 0xD9, 0x69, 0xC7, 0x67, 0xF0 } };
const IID IID_DDocViewCoreEvents = { 0x13A00996, 0xF84A, 0x4DD7, { 0x9E, 0x74, 0x2, 0xF4, 0x2E, 0x17, 0x69, 0xC5 } };


// 控件类型信息

static const DWORD _dwDocViewCoreOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CDocViewCoreCtrl, IDS_DOCVIEWCORE, _dwDocViewCoreOleMisc)



// CDocViewCoreCtrl::CDocViewCoreCtrlFactory::UpdateRegistry -
// 添加或移除 CDocViewCoreCtrl 的系统注册表项

BOOL CDocViewCoreCtrl::CDocViewCoreCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_DOCVIEWCORE,
			IDB_DOCVIEWCORE,
			afxRegApartmentThreading,
			_dwDocViewCoreOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CDocViewCoreCtrl::CDocViewCoreCtrl - 构造函数

CDocViewCoreCtrl::CDocViewCoreCtrl()
{
	InitializeIIDs(&IID_DDocViewCore, &IID_DDocViewCoreEvents);
	// TODO: 在此初始化控件的实例数据。
}



// CDocViewCoreCtrl::~CDocViewCoreCtrl - 析构函数

CDocViewCoreCtrl::~CDocViewCoreCtrl()
{
	// TODO: 在此清理控件的实例数据。
}



// CDocViewCoreCtrl::OnDraw - 绘图函数

void CDocViewCoreCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(LTGRAY_BRUSH)));
	//pdc->Ellipse(rcBounds);
}



// CDocViewCoreCtrl::DoPropExchange - 持久性支持

void CDocViewCoreCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CDocViewCoreCtrl::OnResetState - 将控件重置为默认状态

void CDocViewCoreCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CDocViewCoreCtrl::AboutBox - 向用户显示“关于”框

void CDocViewCoreCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_DOCVIEWCORE);
	dlgAbout.DoModal();
}



// CDocViewCoreCtrl 消息处理程序


void CDocViewCoreCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_ViewDlg.m_hWnd)
	{
		m_ViewDlg.MoveWindow(1,1,cx-1,cy-1);
	}
	if (m_PreviewDlg.m_hWnd)
	{
		m_PreviewDlg.MoveWindow(1,1,cx-1,cy-1);
	}
	DragAcceptFiles(TRUE); //允许拖拽文件到视图
}


int CDocViewCoreCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnActivateInPlace (TRUE, NULL); // == UI-Activate the control
	// TODO:  在此添加您专用的创建代码
	if (m_ViewDlg.m_hWnd == NULL)
	{
		m_ViewDlg.Create(m_ViewDlg.IDD,this);
		m_ViewDlg.ShowWindow(SW_SHOW);
	}
	if (m_PreviewDlg.m_hWnd == NULL)
	{
		m_PreviewDlg.Create(m_PreviewDlg.IDD,this);
		m_PreviewDlg.ShowWindow(SW_SHOW);
	}
	return 0;
}


VARIANT_BOOL CDocViewCoreCtrl::OpenDocFile(LPCTSTR strDocFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: 在此添加调度处理程序代码
	//BOOL
	CRect rc;
	m_PreviewDlg.GetClientRect(&rc);
	rc.DeflateRect(1,1,1,1);
	BOOL bOK = m_PreviewDlg.PreviewDocWithHandle(strDocFile,m_PreviewDlg.GetSafeHwnd(),rc);
	if (bOK)
	{
		m_ViewDlg.ShowWindow(SW_HIDE);
		m_PreviewDlg.ShowWindow(SW_SHOW);
		Invalidate(TRUE);
		DocDesc *pdoc = new DocDesc;
		pdoc->strPath = strDocFile;
		pdoc->strTitle = GetDocTitle(strDocFile);
		PostMessage(WM_DOC_OPENED,0,(LPARAM)pdoc);
		return VARIANT_TRUE;
	}

	 bOK = m_ViewDlg.OpenDocFile(strDocFile);
	 if (bOK)
	 {
		 m_PreviewDlg.ShowWindow(SW_HIDE);
		 m_ViewDlg.ShowWindow(SW_SHOW);
		 Invalidate(TRUE);
		 return VARIANT_TRUE;
	 }

	CString strErrMsg;
	strErrMsg.Format("打开文件[%s]失败!",strDocFile);
	MessageBox(strErrMsg,"错误",MB_OK|MB_ICONERROR);
	return VARIANT_FALSE;
}


BSTR CDocViewCoreCtrl::SelectFile(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码
	CFileDialog dlg(TRUE,".docx",NULL,6UL,"所有格式(*.*)|*.*|\
										  支持的格式|*.doc;*.docx;*.pdf;*.kdh;*.caj;*.htm;*.html;*.xml;*.txt;*.bmp;*.jpg;*.png|\
										  Word 文档(*.doc;*.docx)|*.doc;*.docx|\
										  PDF文档(*.pdf)|*.pdf|\
										  网页文件(*.html;*.html)|*.html;*.html|\
										  图片文件(*.jpg;*.bmp)|*.jpg;*.bmp|\
										  文本文件(*.txt)|*.txt|\
										  XML文件(*.xml)|*.xml|\
										  论文格式(*.kdh;*.caj)|*.kdh;*.caj||",this);

	if (dlg.DoModal()==IDOK)
	   strResult = dlg.GetPathName();

	return strResult.AllocSysString();
}


void CDocViewCoreCtrl::DocumentOpened(LPCTSTR strDocPath, LPCTSTR strDocTitle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	FireEvent(dispidDocumentOpened,EVENT_PARAM(VTS_BSTR VTS_BSTR),strDocPath,strDocTitle);
}
HRESULT CDocViewCoreCtrl::OnDocOpened(WPARAM,LPARAM lData)
{
	DocDesc *pDesc = (DocDesc*)lData;
	DocumentOpened(pDesc->strPath,pDesc->strTitle);
	delete pDesc;
	return S_OK;
}

int CDocViewCoreCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bUIActive)
		OnActivateInPlace (TRUE, NULL); // == UI-Activate the control
	return COleControl::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


BOOL CDocViewCoreCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_ViewDlg.m_hWnd)
	{
		::SendMessage(m_ViewDlg.m_hWnd,pMsg->message,pMsg->wParam,pMsg->lParam);
		return TRUE;
	}
	
	//switch (pMsg->message)
	//{
	//case WM_KEYDOWN:
	//case WM_KEYUP:
	//	switch (pMsg->wParam)
	//	{
	//	case VK_UP:
	//	case VK_DOWN:
	//	case VK_LEFT:
	//	case VK_RIGHT:
	//	case VK_HOME:
	//	case VK_END:
	//		SendMessage (pMsg->message, pMsg->wParam, pMsg->lParam);
	//		// Windowless controls won't be able to call SendMessage.
	//		// Instead, just respond to the message here.
	//		return TRUE;
	//	}
	//	break;
	//}
	return COleControl::PreTranslateMessage(pMsg);
}


void CDocViewCoreCtrl::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int num = DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0);
	if (num <=0)
		return;
	char szPath[MAX_PATH];
	bool bOK = false;
	for (int i=0;i<num;i++)
	{
		DragQueryFile(hDropInfo,i,szPath,MAX_PATH);
		CString strFile = szPath;
		if (m_ViewDlg.OpenDocFile(strFile))
		{
			bOK = true;
			break;
		}
	}
	if (!bOK)
		MessageBox(_T("指定的文件不能打开！"),_T("提示"),MB_OK|MB_ICONINFORMATION);
	DragFinish(hDropInfo);
	COleControl::OnDropFiles(hDropInfo);
}
