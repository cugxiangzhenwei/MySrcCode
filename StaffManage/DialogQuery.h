#pragma once


// CDialogQuery 对话框

class CDialogQuery : public CDialog
{
	DECLARE_DYNAMIC(CDialogQuery)

public:
	CDialogQuery(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogQuery();

// 对话框数据
	enum { IDD = IDD_DIALOG_QUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_query;
};
