
// TestClientDlg.h : 头文件
//

#pragma once


// CTestClientDlg 对话框
class CTestClientDlg : public CDialogEx
{
// 构造
public:
	CTestClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	CString m_strLicenseData;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCheckLicense();
	afx_msg void OnBnClickedButtonSaveKey();
	afx_msg void OnBnClickedButtonGetPcCode();
	afx_msg void OnBnClickedButtonGetLicense();
};
