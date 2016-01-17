#pragma once


// CDlgResLibImage 对话框

class CDlgResLibImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgResLibImage)

public:
	CDlgResLibImage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgResLibImage();

// 对话框数据
	enum { IDD = IDD_JIANMO_COMPONENT_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
