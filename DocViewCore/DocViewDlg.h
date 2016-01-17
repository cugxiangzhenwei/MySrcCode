#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CDocViewDlg 对话框
#include "pdf\CAcroAXDocShim.h"
#include "pdf\CRadaeePDFDocument.h"
#include "pdf\CDPdfview.h"

class CDocViewDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDocViewDlg)
public:
	typedef enum
	{
		NONE_PDF = 0,
		Adobe_PDF = 1,
		RadaeePDFDll_PDF = 2,
		PdfviewOcx_PDF = 3,
		CajOcx_PDF = 4
	}PDFActiveXType; // pdf 插件类型

public:
	CDocViewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDocViewDlg();
	BOOL OpenDocFile(CString strUrl);
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

	BOOL IsWordFile(const CString &strFile);
	BOOL ConvertWordToHtml(CString strWordFile,CString strHtml);
	void RemoveTempHtmlFile();
	BOOL UpdateHtmlToActivexSafe(CString strHtml);
	// 取消右键菜单
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);

// 对话框数据
	enum { IDD = IDD_DOCVIEWDLG};
protected:
	CAcroAXDocShim m_drvPDF;
	CRadaeePDFDocument *m_pRadaeePdfDoc;
	CDPdfview m_drvViewPDF;
	CWnd           m_wndPDF;
	CString m_strLastTmpFile;
	PDFActiveXType m_ActiveXType;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL IsExternalDispatchSafe();
	BOOL TestCreatePdfViewCtrl();
	BOOL TestCreateAdobeCtrl();
	BOOL TestCreateRadaeePDFDllCtrl();
	BOOL TestCreatePdfviewOcxCtrl();
	BOOL TestCreateCAjCtrl();
	BOOL TestOpenPdf(CString strPdfFile);

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
