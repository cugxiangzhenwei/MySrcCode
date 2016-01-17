#pragma once

#include "SkinBtn.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "NetDiskOperation.h"
#include <vector>
using namespace std;
// CPageLogin 对话框

class CPageLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CPageLogin)

public:
	CPageLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageLogin();

// 对话框数据
	enum { IDD = IDD_DLG_PAGE_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//CComboBoxEx m_ComboUsr;
	afx_msg void OnBnClickedBtnLogin();
	CString GetPassWd(); 
public:
	CDataLoginResult m_LoginRes;
private:
	CSkinBtn m_btnLogin;
	CComboBox m_ComboUsr;
	CEdit m_EditPasswd;
	vector<CString> m_UsrHistory;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL m_bSavePassWd;
	BOOL m_bAutoLogin;
	afx_msg void OnBnClickedCheckSavePwd();
};
