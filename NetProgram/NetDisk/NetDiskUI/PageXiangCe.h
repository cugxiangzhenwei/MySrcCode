#pragma once


// CPageXiangCe 对话框

class CPageXiangCe : public CDialogEx
{
	DECLARE_DYNAMIC(CPageXiangCe)

public:
	CPageXiangCe(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageXiangCe();

// 对话框数据
	enum { IDD = IDD_DLG_PAGE_XIANGCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
