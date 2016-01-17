#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCompression 对话框

class CCompressionDlgPage : public CDialogEx
{
	DECLARE_DYNAMIC(CCompressionDlgPage)

public:
	CCompressionDlgPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCompressionDlgPage();

// 对话框数据
	enum { IDD = IDD_DLG_COMPRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnAddFile();
	afx_msg void OnBnClickedBtnExcuteCompress();
	afx_msg LRESULT OnMsgCompressFinish(WPARAM,LPARAM);
	CListBox m_FileList;
	CProgressCtrl m_bProgressBar;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
