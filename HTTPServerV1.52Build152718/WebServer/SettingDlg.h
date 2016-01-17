#pragma once
#include "afxwin.h"


// CSettingDlg dialog

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	BOOL m_bAutoLaunch;
	BOOL m_bAutoRun;
	CString m_strRoot;
	int m_nPort;
	BOOL m_bListFile;
	int m_nMaxConn;
	int m_nSessionTimeout;
	int m_nDeadConnectionTimeout;
	int m_nMaxClientConn;
	float m_fMaxSpeed;
	BOOL m_bDisableScreenLog;
	BOOL m_bDisableFileLog;
	afx_msg void OnBnClickedCheck3();
	CString m_strDefName;
	CButton m_chkDisableLog;
	CButton m_chkDisableScreenLog;
	CButton m_chkListDir;
	CEdit m_edtDftFileName;
	afx_msg void OnBnClickedCheck5();
};
