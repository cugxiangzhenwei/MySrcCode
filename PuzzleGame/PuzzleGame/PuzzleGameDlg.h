// PuzzleGameDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "GameCtrler.h"

// CPuzzleGameDlg dialog
class CPuzzleGameDlg : public CBCGPDialog
{
// Construction
public:
	CPuzzleGameDlg(CWnd* pParent = NULL);	// standard constructor
	~CPuzzleGameDlg();
// Dialog Data
	enum { IDD = IDD_PUZZLEGAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


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
	CStatic m_PictureWnd;
	CGameCtrler m_GameCtrler;
	int m_iSwapCount;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnPre();
	afx_msg void OnBnClickedBtnRestart();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnCbnSelchangeComboGridCount();
private:
	void LoadGameImage();
	BOOL IntilizeGameUseRecord();
	void IntilizeGame(CString strImage);
	void IntilizeGameUseRecord(const CString & strImg,vector<int> & vIDs);
	void UpdateBtnEnable();
	vector<CString> m_strBackImgFileList;
	int m_iCurBackImageIndex;
	int m_iGridCount;
	CComboBox m_ComboboxGridCount;
	BOOL m_bShowGridLine;
public:
	afx_msg void OnBnClickedCheckShowGrid();
	CButton m_Check;
	afx_msg void OnDestroy();
};
