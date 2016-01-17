#pragma once
#include "DocViewDlg.h"
#include "PreviewMainDlg.h"
// DocViewCoreCtrl.h : CDocViewCoreCtrl ActiveX 控件类的声明。


// CDocViewCoreCtrl : 有关实现的信息，请参阅 DocViewCoreCtrl.cpp。

class CDocViewCoreCtrl : public COleControl
{
	DECLARE_DYNCREATE(CDocViewCoreCtrl)
// 构造函数
public:
	CDocViewCoreCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CDocViewCoreCtrl();

	DECLARE_OLECREATE_EX(CDocViewCoreCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CDocViewCoreCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CDocViewCoreCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CDocViewCoreCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		dispidDocumentOpened = 1L,
		dispidSelectFile = 2L,
		dispidOpenDocFile = 1L
	};
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HRESULT OnDocOpened(WPARAM,LPARAM);

	CDocViewDlg m_ViewDlg;
	CPreviewMainDlg m_PreviewDlg;
protected: // 调度接口
	VARIANT_BOOL OpenDocFile(LPCTSTR strDocFile);
	BSTR SelectFile(void);
protected: // 通知事件
	void DocumentOpened(LPCTSTR strDocPath, LPCTSTR strDocTitle);
public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

