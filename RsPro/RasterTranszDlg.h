#pragma once
#include "afxwin.h"


// CRasterTranszDlg 对话框

class CRasterTranszDlg : public CDialog
{
	DECLARE_DYNAMIC(CRasterTranszDlg)

public:
	CRasterTranszDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRasterTranszDlg();

// 对话框数据
	enum { IDD = IDD_DLG_TRANSRASTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedOk();
	CString m_strInFile;
	CString m_strOutFile;
	CString m_strFormat;
	CEdit m_CtrEdit_Input;
	CEdit m_CtrEditOutPut;
};
