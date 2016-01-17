
// RePrintToWndTestDlg.h : 头文件
//

#pragma once

#include <string>
#include "afxwin.h"
#include "MyListBox.h"
using namespace std;
// CRePrintToWndTestDlg 对话框

extern DWORD g_ExeProcessId;
class CRePrintToWndTestDlg : public CDialog
{
// 构造
public:
	CRePrintToWndTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REPRINTTOWNDTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	void CallExe(const char * pszCmd,bool bShowWnd = false);
private:
	string  m_strCallMsg;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSubThreadMsg(WPARAM wparam,LPARAM lparam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	CMyListBox m_ListBox;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnStart2();
	afx_msg void OnBnClickedBtnStart3();
};
