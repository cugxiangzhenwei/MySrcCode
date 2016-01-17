#pragma once


// CPreviewMainDlg 对话框

class CPreviewMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPreviewMainDlg)

public:
	CPreviewMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPreviewMainDlg();
	BOOL PreviewDocWithHandle(const CString & strDocFile,HWND hwnd,const CRect & rect);

// 对话框数据
	enum { IDD = IDD_DIALOG_PREVIEW_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 使用Preview Handle 预览文档
	IPreviewHandler *m_pIP ;
	IInitializeWithFile *m_pIFile ;
	IInitializeWithStream *m_pIStream ;
	HGLOBAL m_hGlobal;
	IStream * m_pStream;	
	CString GetClsidFromExtn( char * szExtn );
};
