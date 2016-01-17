#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CSvgDlg 对话框

class CSvgDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CSvgDlg)

public:
	CSvgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSvgDlg();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);
	void OpenJianMoHuanJing();
	void OpenDefaultPage();
	void OpenWorkHistPage();

// 对话框数据
	enum { IDD = IDD_SVG_DLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnNcDestroy();
};
