// DocViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DocViewCore.h"
#include "DocViewDlg.h"

#include <atlbase.h> // CComVariant 使用
using namespace ATL; // CComVariant 使用

#include "./word/CWordApplication.h"
#include "./word/CDocuments.h"
#include "./word/CDocument0.h"
#include "comutil.h"
struct __declspec(uuid("6fb7713e-f432-42d5-be19-d32013160189")) PDFDocument;

CString GetDocTitle(CString strDoc)
{
	int iPosS = strDoc.ReverseFind('\\');
	if (iPosS < 0)
		return "";

	return strDoc.Mid(iPosS+1);
}
BOOL DeleteDir(CString strDir)
{
	CString strWildCard  = strDir;
	strWildCard +="\\*.*";
	CFileFind finder;
	BOOL bFind = FALSE;
	// 查找文件
	bFind = finder.FindFile(strWildCard);
	while(bFind)
	{
		// 查找下一个文件
		bFind = finder.FindNextFileA();
		if (finder.IsDots())
			continue;

		//找到文件的路径
		CString strPathName = finder.GetFilePath();
		if (finder.IsDirectory())
		{
			DeleteDir(strPathName);
		}
		else
		{
			if (!::DeleteFile(strPathName))
			{
				return FALSE;
			}
		}
	}
	finder.Close();
	if (!::RemoveDirectory(strDir))
		return FALSE;

	return TRUE;
}
// CDocViewDlg 对话框

IMPLEMENT_DYNCREATE(CDocViewDlg, CDHtmlDialog)

CDocViewDlg::CDocViewDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDocViewDlg::IDD,0UL/* CDocViewDlg::IDH*/, pParent)
{
	m_strLastTmpFile = "";
	m_ActiveXType = NONE_PDF;
	m_pRadaeePdfDoc = NULL;
}

CDocViewDlg::~CDocViewDlg()
{
	if (m_pRadaeePdfDoc)
	{
		delete m_pRadaeePdfDoc;
		m_pRadaeePdfDoc = NULL;
	}
	RemoveTempHtmlFile();
}

void CDocViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CDocViewDlg::OnInitDialog()
{
	SetHostFlags(DOCHOSTUIFLAG_NO3DBORDER/*|DOCHOSTUIFLAG_DIALOG*/); //加了这句话才会自动出现隐藏滚动条
	CDHtmlDialog::OnInitDialog();
	m_pBrowserApp->put_Silent(VARIANT_TRUE);//禁止脚本错误提示 
	Navigate("about:blank");
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
BOOL CDocViewDlg::IsExternalDispatchSafe()
{
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDocViewDlg, CDHtmlDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDocViewDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CDocViewDlg 消息处理程序

HRESULT CDocViewDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CDocViewDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}
BOOL CDocViewDlg::TestOpenPdf(CString strPdfFile)
{
	switch(m_ActiveXType)
	{
	case NONE_PDF:
		return FALSE;
	case Adobe_PDF:
		{
			m_drvPDF.LoadFile(strPdfFile);
			return TRUE;
		}
	case RadaeePDFDll_PDF:
		{
			if (m_pRadaeePdfDoc)
			{
				delete m_pRadaeePdfDoc;
				m_pRadaeePdfDoc = NULL;
			}
			m_pRadaeePdfDoc = new CRadaeePDFDocument();
			BOOL bOK2 = m_pRadaeePdfDoc->CreateDispatch( __uuidof(PDFDocument));
			BOOL bOK = m_pRadaeePdfDoc->open(strPdfFile,NULL);
			if (!bOK2|| !bOK)
				return FALSE;
			// 以下代码将打开的文档绑定到控件窗口
			BOOL result;
			static BYTE parms[] = VTS_DISPATCH ;
			m_wndPDF.InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, m_pRadaeePdfDoc->m_lpDispatch);
			return TRUE;
		}
	case  PdfviewOcx_PDF:
		{
			BOOL bOK = m_drvViewPDF.OpenPDF(strPdfFile,"","");
			m_drvViewPDF.ZoomFitWidth();
			return TRUE;
		}
	case CajOcx_PDF:
		{
			BOOL result;
			static BYTE parms[] = VTS_BSTR ;
			m_wndPDF.InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strPdfFile);
			return TRUE;
		}
	default:
		return FALSE;
	}

	return FALSE;
}
BOOL CDocViewDlg::OpenDocFile(CString strUrl)
{
	strUrl.MakeLower();
	if (strUrl.Right(4)==".pdf" || strUrl.Right(4)==".kdh" || strUrl.Right(4)==".caj" )
	{
		if (NULL == m_wndPDF.m_hWnd)
			return FALSE;

		m_wndPDF.ShowWindow(SW_SHOW);
		if (!TestOpenPdf(strUrl))
			return FALSE;

		DocDesc *pdoc = new DocDesc;
		pdoc->strPath = strUrl;
		pdoc->strTitle = GetDocTitle(strUrl);
		::PostMessage(GetParent()->m_hWnd,WM_DOC_OPENED,0,(LPARAM)pdoc);
		return TRUE;
	}
	else
	{
		m_wndPDF.ShowWindow(SW_HIDE);
		RemoveTempHtmlFile();
		CString strTmp = strUrl;
		if (IsWordFile(strUrl))
		{
			CString strHtml = strUrl.Left(strUrl.ReverseFind('.'))+ ".html";
			char szTmpBuf[256] = {0};
			char szFileName[256] = {0};
			::GetTempPath(256,szTmpBuf);
			CString strTmp = szTmpBuf;
			::GetTempFileName(szTmpBuf,"~htm",TRUE,szFileName);
			strHtml = szFileName;
			strHtml +=".html";
			BOOL bOk = ConvertWordToHtml(strUrl,strHtml);
			if (!bOk)
			{
				//MessageBox("无法打开该文档","提示",MB_OK|MB_ICONERROR);
				return FALSE;
			}
			m_strLastTmpFile = strHtml;
			strUrl = m_strLastTmpFile;
			//UpdateHtmlToActivexSafe(strUrl);
		}
		Navigate(strUrl);
		DocDesc *pdoc = new DocDesc;
		pdoc->strPath = strTmp;
		pdoc->strTitle = GetDocTitle(strTmp);
		::PostMessage(GetParent()->m_hWnd,WM_DOC_OPENED,0,(LPARAM)pdoc);
	}
	return TRUE;
}
BOOL CDocViewDlg::TestCreateAdobeCtrl()
{
	CRect rt;
	GetClientRect(&rt);
	// 利用CWnd::CreateControl创建PDF控件窗口
	// Acrobat (Reader) 5.x/6.x的控件类ID为"PDF.PdfCtrl.5"，7.x为"AcroPDF.PDF.1"
	BOOL bOK = m_wndPDF.CreateControl("AcroPDF.PDF.1", NULL, WS_CHILD | WS_VISIBLE, rt, this, 0);
	if (!bOK)
		return FALSE;

	// 获取IDispatch接口指针
	LPUNKNOWN lpUnknown = m_wndPDF.GetControlUnknown();
	if (lpUnknown==NULL)
		return FALSE;

	LPDISPATCH lpDispatch;
	lpUnknown->QueryInterface(IID_IDispatch, (void**)&lpDispatch);
	// 把获得的接口指针传递给组件驱动对象m_drvPDF，通过它调用控件的属性和方法
	// 当m_drvPDF销毁时，会自动释放接口指针
	m_drvPDF.AttachDispatch(lpDispatch,TRUE);
	m_ActiveXType = Adobe_PDF;
	return TRUE;
}
BOOL CDocViewDlg::TestCreateRadaeePDFDllCtrl()
{
	if (m_wndPDF.m_hWnd==NULL)
	{
		CRect rt;
		GetClientRect(&rt);
		// 利用CWnd::CreateControl创建PDF控件窗口
		static CLSID const clsid
			= { 0x3AB65BA7, 0xFCFC, 0x4656, { 0xBD, 0x1B, 0x40, 0xF4, 0x73, 0x3A, 0x9D, 0x68 } };
		BOOL bOK = m_wndPDF.CreateControl(clsid, NULL, WS_CHILD | WS_VISIBLE, rt, this, 0);
		if (!bOK)
			return FALSE;
	}
	m_ActiveXType = RadaeePDFDll_PDF;
	return TRUE;
}
BOOL CDocViewDlg::TestCreatePdfviewOcxCtrl()
{
	if (m_wndPDF.m_hWnd==NULL)
	{
		CRect rt;
		GetClientRect(&rt);
		BOOL bOK = m_wndPDF.CreateControl("{433268D7-2CD4-43E6-AA24-2188672E7252}", NULL, WS_CHILD | WS_VISIBLE, rt, this, 0);
		if (!bOK)
			return FALSE;
		// 获取IDispatch接口指针
		LPUNKNOWN lpUnknown = m_wndPDF.GetControlUnknown();
		if (lpUnknown==NULL)
			return FALSE;

		LPDISPATCH lpDispatch;
		lpUnknown->QueryInterface(IID_IDispatch, (void**)&lpDispatch);
		m_drvViewPDF.AttachDispatch(lpDispatch,TRUE);
	}
	m_ActiveXType = PdfviewOcx_PDF;
	return TRUE;
}
BOOL CDocViewDlg::TestCreateCAjCtrl()
{
	static CLSID const clsid = { 0x78E87ACB, 0x656E, 0x4257, { 0x96, 0x1D, 0x3F, 0xAD, 0xBE, 0x77, 0xA6, 0x26 } };
	CRect rt;
	GetClientRect(&rt);
	BOOL bOK = m_wndPDF.CreateControl(clsid, NULL, WS_CHILD | WS_VISIBLE, rt, this, 0);
	if (!bOK)
		return FALSE;

	m_ActiveXType = CajOcx_PDF;
	return TRUE;
}
BOOL CDocViewDlg::TestCreatePdfViewCtrl()
{
	if (TestCreateCAjCtrl())
		return TRUE;
	if (TestCreateRadaeePDFDllCtrl())
		return TRUE;
	if (TestCreatePdfviewOcxCtrl())
		return TRUE;
	if (TestCreateAdobeCtrl())
		return TRUE;
	m_ActiveXType = NONE_PDF;
	return FALSE;
}
int CDocViewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDHtmlDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!TestCreatePdfViewCtrl())
		MessageBox("创建PDF控件失败!","错误!",MB_OK|MB_ICONERROR);

	return 0;
}


void CDocViewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDHtmlDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_wndPDF.m_hWnd)
	{
		CRect rect;
		GetClientRect(&rect);
		m_wndPDF.SetWindowPos(&wndTop,rect.left,rect.top,rect.Width(),rect.Height(),NULL);
		m_wndPDF.RedrawWindow();
	}
}
BOOL CDocViewDlg::IsWordFile(const CString &strFile)
{
	CString strTmp = strFile;
	strTmp.MakeLower();
	if (strFile.Right(3)=="doc"|| strFile.Right(4)=="docx")
		return TRUE;

	return FALSE;
}
BOOL CDocViewDlg::ConvertWordToHtml(CString strWordFile,CString strHtml)
{
	CWordApplication app;
	BeginWaitCursor();
	if (!app.CreateDispatch("Word.Application",NULL))
	{
		EndWaitCursor();
		return FALSE;
	}

	CDocuments docxs = app.get_Documents();
	CComVariant FileName =  strWordFile;
	CComVariant passWd = "";
	CComVariant templateDoc = "";
	CComVariant wrietPassWdDoc = "";
	CComVariant wriePassWdtemplateDoc = "";
	CComVariant xmltrans = "";
	CComVariant vBoolTrue,vBoolFalse;
	vBoolTrue  = TRUE;
	vBoolFalse = FALSE;
	CComVariant vFormat = 0;
	CComVariant vEcode  = 0;
	CComVariant vDirection  = 0;
	try
	{
		app.put_Visible(FALSE);
		// 打开Word 文档
		CDocument0 docx = docxs.Open(&FileName,&vBoolFalse,&vBoolTrue,&vBoolTrue,
			&passWd,&templateDoc,&vBoolFalse,&wrietPassWdDoc,&wriePassWdtemplateDoc,&vFormat,&vEcode,&vBoolFalse,&vBoolTrue,&vDirection,&vBoolTrue,&xmltrans);

		// 另存为html 页面
		FileName = strHtml;
		vFormat = 8;
		docx.SaveAs(&FileName,&vFormat,&vBoolFalse,&passWd,&vBoolFalse,&wrietPassWdDoc,&vBoolFalse,&vBoolFalse,&vBoolFalse,&vBoolFalse,&vBoolFalse,&vEcode,
			&vBoolFalse,&vBoolFalse,&vBoolFalse,&vBoolFalse);
	}
	catch (...)
	{
		CComVariant vRotuneDoc = 0;
		vFormat = 0;
		app.Quit(&vBoolFalse,&vFormat,&vRotuneDoc);
		EndWaitCursor();
		return FALSE;
	}
	// 退出Word
	CComVariant vRotuneDoc = 0;
	vFormat = 0;
	app.Quit(&vBoolFalse,&vFormat,&vRotuneDoc);
	EndWaitCursor();
	return TRUE;
}
void CDocViewDlg::RemoveTempHtmlFile()
{
	if (m_strLastTmpFile.IsEmpty())
		return;

	::DeleteFile(m_strLastTmpFile);
	CString strFileDir = m_strLastTmpFile.Left(m_strLastTmpFile.ReverseFind('.')) + ".files";
	DeleteDir(strFileDir);
}
STDMETHODIMP CDocViewDlg::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
{
	return S_OK; //屏蔽右键菜单
}
BOOL CDocViewDlg::UpdateHtmlToActivexSafe(CString strHtml)
{
	FILE *pFile = NULL;
	fopen_s(&pFile,strHtml,"rb"); // 以只读方式打开文件
	if (pFile==NULL)
		return FALSE;

	fseek(pFile,0,SEEK_END);
	int iFileSize = ftell(pFile);
	char * pBuf = new char[iFileSize+1];
	memset(pBuf,0,sizeof(char)*(iFileSize+1));
	fseek(pFile,0,SEEK_SET);
	size_t iRead = fread_s(pBuf,iFileSize,sizeof(char),iFileSize,pFile);
	fclose(pFile); // 读完之后关闭文件
	pFile = NULL;
	if (iRead != iFileSize)
	{
		delete pBuf;
		return FALSE;
	}
	pBuf[iFileSize] = 0;
	CString strSourceContent = pBuf;
	delete pBuf;

	CString strFind = "<head>";
	int iPosHeader = strSourceContent.Find(strFind);
	int iPosWriteTo = iPosHeader;
	while(strSourceContent[iPosWriteTo]==' ' 
		||strSourceContent[iPosWriteTo]=='\n'
		||strSourceContent[iPosWriteTo]=='\r'
		||strSourceContent[iPosWriteTo]=='\t')
		iPosWriteTo--;

	strSourceContent.Insert(iPosWriteTo,"<!-- saved from url=(0017)http://localhost/ -->\n");
	fopen_s(&pFile,strHtml,"wb"); // 以写方式打开文件

	fseek(pFile,0,SEEK_SET);
	size_t iLength = strSourceContent.GetLength();
	size_t iWritten = fwrite(strSourceContent,1,iLength,pFile);
	if (iLength!=iWritten)
		return FALSE;

	fclose(pFile);
	pFile = NULL;
	return TRUE;
}