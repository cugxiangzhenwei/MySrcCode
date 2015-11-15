// Game2048Dlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "DataDefine.h"
#include "GameCtrler.h"
// CGame2048Dlg dialog
class CGame2048Dlg : public CBCGPDialog
{
// Construction
	friend CGameCtrler;
public:
	CGame2048Dlg(CWnd* pParent = NULL);	// standard constructor
	~CGame2048Dlg();

// Dialog Data
	enum { IDD = IDD_GAME2048_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void DrawBlocks(CDC & dc);
	void DrawBlock(CDC & dc,const int & iBlockID);
	void IntilizeGame();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ÓÎÏ·¿ØÖÆÇø
	CStatic m_wndCore;
	int     m_BlockWidth;
	int     m_BlockHeight;
	int     m_BlockGapX;
	int     m_BlockGapY;
	CGameNumBlock   m_RectBlocks[16];
	CGameCtrler m_GameCtrler;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnRestart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
