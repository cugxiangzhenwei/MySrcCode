
// TestFileUnityComDlg.h : 头文件
//

#pragma once


// CTestFileUnityComDlg 对话框
class CTestFileUnityComDlg : public CDialogEx
{
// 构造
public:
	CTestFileUnityComDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTFILEUNITYCOM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	afx_msg void OnBnClickedOpenFileDlg();
	afx_msg void OnBtnSelectDir();
	afx_msg void OnBnClickedBtnFileInfo();
	afx_msg void OnBnClickedBtnDeletefile();
	afx_msg void OnBnClickedBtnGetMd5();
	afx_msg void OnBnClickedBtnOpenFile();
	afx_msg void OnBnClickedBtnCopyfile();
	CString m_StrCopyInfo;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
