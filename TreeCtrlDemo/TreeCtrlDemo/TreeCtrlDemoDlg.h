
// TreeCtrlDemoDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTreeCtrlDemoDlg 对话框
class CTreeCtrlDemoDlg : public CDialog
{
// 构造
public:
	CTreeCtrlDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TREECTRLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void InitTree();
	void SearchTree(HTREEITEM hParent,int iStrOffset);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDelSel();
	afx_msg void OnBnClickedButtonDelAll();
	afx_msg void OnBnClickedButtonResetTree();
	afx_msg void OnBnClickedButtonForeach();
	CTreeCtrl m_TreeCtrl;
	CListBox m_ListBox;
};
