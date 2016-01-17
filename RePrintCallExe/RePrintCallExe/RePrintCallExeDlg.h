
// RePrintCallExeDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <string>
#include "MyListBox.h"
using namespace std;

extern DWORD  g_ExeProcessID;
// CRePrintCallExeDlg 对话框
class CRePrintCallExeDlg : public CDialog
{
// 构造
public:
	CRePrintCallExeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REPRINTCALLEXE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	void CallExe(const char *pszCmd,bool bShowWnd = false);
// 实现
protected:
	HICON m_hIcon;
	string m_strLast; // 用来记录接收消息最后一条不完整的消息，保存，以用于下次处理


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 显示调用exe输出消息的控件变量
	CMyListBox m_ListBox;
	afx_msg void OnBnClickedBtnCallExe();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
