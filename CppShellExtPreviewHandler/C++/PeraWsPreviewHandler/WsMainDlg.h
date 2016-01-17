#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "mapcorectrl1.h"

// CWsMainDlg 对话框

class CWsMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWsMainDlg)

public:
	CWsMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWsMainDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WS_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_StaticTitle;
	CStatic m_StaticZoomNum;
	CStatic m_WsContainer;
	CSliderCtrl m_SliderZoom;
	virtual BOOL OnInitDialog();
	CMapcorectrl1 m_MapCore;
};
