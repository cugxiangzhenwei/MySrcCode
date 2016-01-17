
// GridViewTestDlg.h : 头文件
//

#pragma once
#include "calendar1.h"
#include "msflexgrid1.h"
#include"MyEdit.h"


// CGridViewTestDlg 对话框
class CGridViewTestDlg : public CDialog
{
// 构造
public:
	CGridViewTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GRIDVIEWTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LONG OnGridDataChanged(UINT wParam,LONG lParam);
	DECLARE_MESSAGE_MAP()
public:
	CCalendar1 m_calendar;
	CMsflexgrid1 m_flexGrid;
	DECLARE_EVENTSINK_MAP()
	void EnterCellMsflexgrid1();
private:
	void CreateGridEditBox();
	CMyEdit m_Edit;
	long int m_iSelRow;
	long int m_iSelCol;
	long int m_iOldSelRow;
	long int m_iOldSelCol;
public:
	afx_msg void OnBnClickedButtonExportgrid();
	afx_msg void OnBnClickedButtonImportcsv();
};
