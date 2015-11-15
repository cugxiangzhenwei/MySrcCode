// FileManagerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "VirtualList.h"
class CFileInfo
{
public:
	int m_iImageID;
	CString m_strDir;
	CString m_strName;
	CString m_strFileSize;
	CString m_strModifyTime;
	CString m_strCreateTime;
	CFileInfo * Clone()
	{
		CFileInfo * pInfo  = new CFileInfo;
		if (pInfo)
		{
			pInfo->m_iImageID = this->m_iImageID;
			pInfo->m_strCreateTime = this->m_strCreateTime;
			pInfo->m_strModifyTime = this->m_strModifyTime;
			pInfo->m_strDir = this->m_strDir;
			pInfo->m_strFileSize = this->m_strFileSize;
			pInfo->m_strName = this->m_strName;
		}
		return pInfo;
	}
};
class CFileManagerDlg;
typedef struct 
{
	CFileManagerDlg *pManagerDlg;
	CString m_strDisk;
	BOOL m_bScanFinished;
}ThreadScanDiskData;

class CSqliteBase;
// CFileManagerDlg dialog
class CFileManagerDlg : public CBCGPDialog
{
// Construction
public:
	CFileManagerDlg(CWnd* pParent = NULL);	// standard constructor
	~CFileManagerDlg();
	static BOOL GetFileInfo(const CString & strFile,CFileInfo &);
	static CString TransferFileSize(ULONGLONG fSize);
// Dialog Data
	enum { IDD = IDD_FILEMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void InsertFileItem(const CFileInfo & f);
	
	void GetUsrSettings();
	void WriteStartAtWindowsLogin();
	void RemoveStartAtWindowsLoginKey();
	void LoadDataBase();
	void CheckRefreshList(CString strFile,CString strNewFile = CString());
	void StartDiskMonitor();
	void QueryFromDataBase(const CString & strKeyWords,BOOL bMatchEnd,BOOL bMatchStart);
	static DWORD WINAPI ThreadFuncScanDisk(LPVOID pData);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void  HideMain();
	void ShowNotifyIcon(BOOL bAdd);
private:
	CImageList m_IconList;
	CListCtrlEx m_ListCtrl;
	CString   m_strDB;
	CSqliteBase * m_pDataBase;
	CStatusBarCtrl   m_StatusBar;
	HANDLE  *m_pScanThreadHandleArry;
public:
	afx_msg void OnOpenItem();
	afx_msg void OnOpenDir();
	afx_msg void OnShowAttribute();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg HRESULT OnMsgScanResult(WPARAM,LPARAM);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg HRESULT OnMsgScanFinished(WPARAM,LPARAM);
	afx_msg HRESULT OnMsgNotifyIcon(WPARAM,LPARAM);
	afx_msg HRESULT OnMsgUpdateList(WPARAM,LPARAM);
	afx_msg void OnMsgNotifyStartWithWindow();
	afx_msg void OnMsgNotifyOpen();
	afx_msg void OnExitApp();
public:
	BOOL m_bExitScanDisk;
	CEdit m_EditSelect;
	CButton m_btnSelect;
	int  m_iFinishedThreadCount;
	int     m_iHandleCount;
	BOOL m_InsetDbFinished;
	BOOL m_bRunAtBack;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnUpdateOpenItem(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOpenDir(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowAttribute(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStartWithWindow(CCmdUI *pCmdUI);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnClose();
	afx_msg void OnCopyFile();
private:
	NOTIFYICONDATA m_nid;
	BOOL m_bIsEditingSelect;
	BOOL m_bStartWithWindow;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileCust();
	afx_msg void OnUpdateFileCust(CCmdUI *pCmdUI);
	afx_msg void OnFileDelete();
	afx_msg void OnUpdateFileDelete(CCmdUI *pCmdUI);
};
