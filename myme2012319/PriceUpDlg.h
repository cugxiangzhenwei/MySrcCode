//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX_PRICEUPDLG_H__8BF6C250_31F6_4C12_A27E_DCC804DD306F__INCLUDED_)
#define AFX_PRICEUPDLG_H__8BF6C250_31F6_4C12_A27E_DCC804DD306F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PriceUpDlg.h : header file
//
#include"SkinBtn.h"
#include "MyStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CPriceUpDlg dialog


class CPriceUpDlg : public CDialog
{
// Construction
public:
	CPriceUpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPriceUpDlg)
	enum { IDD = IDD_DIALOG_PRICEUP };
	CSkinBtn	m_BtnImportPrice;
	CSkinBtn	m_BtnFloowWgz;
	CSkinBtn	m_BtnAddPro;
	CWebBrowser2	m_IePriceCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPriceUpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPriceUpDlg)
	afx_msg void OnPaint();
	afx_msg void OnButtonAddProid();
	virtual BOOL OnInitDialog();
	afx_msg void OnDocumentCompleteExplorerPriceup(LPDISPATCH pDisp, VARIANT FAR* URL);
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnButtonLet();
	afx_msg void OnButtonFloowwgz();
	afx_msg void OnCheckTbWb();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	_tagGetPriceUps m_GetPriceUps;

	void SetXmlData(int nShopid,int nType,CString strRet);

	void UTF_8ToGB2312(CString &pOut, char *pText, int pLen);
	void UTF_8ToUnicode(wchar_t* pOut,char *pText);
    void UnicodeToUTF_8(char* pOut,wchar_t* pText);
	void UnicodeToGB2312(char* pOut,wchar_t uData);

	CString GetGbkToUrlEncode(CString keyWord);

	int m_iePricestat;
	BOOL m_CheckTbWbFlag;//同步weibo标志


	CMyStatic	m_Static1;
	CMyStatic	m_Static2;
	CMyStatic	m_Static3;
	CMyStatic	m_Static4;
	CMyStatic	m_Static5;
	CMyStatic	m_Static6;
	CMyStatic	m_Static7;



	CString  GetCurrentApPath();
	void LoadConfigXml(void);
	long m_pPriceUpstagNumber;
	_tagPriceUps * m_pPriceUpstag;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRICEUPDLG_H__8BF6C250_31F6_4C12_A27E_DCC804DD306F__INCLUDED_)
