#pragma once
#include "skinbtn.h"
#include "afxwin.h"
#include "afxmenubutton.h"
#include "afxcmn.h"
#include "VirtualList.h"
// CPageNetDisk 对话框
class CPageNetDisk : public CDialogEx
{
	DECLARE_DYNAMIC(CPageNetDisk)
public:
	CPageNetDisk(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageNetDisk();

// 对话框数据
	enum { IDD = IDD_DLG_PAGE_NETDISK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnMain();
	afx_msg void OnBnClickedBtnBack();
	afx_msg void OnBnClickedBtnForward();
	virtual BOOL OnInitDialog();
	void UpdateFileList(const CString & strNetDiskDir);
	BOOL HasSameNameItem(const CString & strItemName);
	void LoadLocalRunningTask(const CString & strNetDiskDir);
	// 设置按钮图片
	void SetBtnImage();
	afx_msg void OnPaint();
	void AddRunningUploadTaskIdList( LPCTSTR  strTaskId);
	void AddRunningDownloadTaskIdList( LPCTSTR  strTaskId);
	void DeleteRunningTaskId(LPCTSTR  strTaskId);
	void TellParentDirProgressChanged( LPCTSTR strChildTaskId);
	CString GetStatusText();
private:
	HBRUSH m_brushBkrectMid;
	HBRUSH m_brushBkrect;
	BOOL   m_bQueryIsRunning;
	vector<CString> RunningUploadTaskIdList;
	vector<CString> RunningDownloadTaskIdList;
	vector<CString> m_DownloadDestDirs;
	CRITICAL_SECTION m_csUploadTaskList;
private:
	void AdjustNavagiateBarLayout();
	void AdjustUploadBarLayout();
	void StartQueryTaskStatusTimer();
	void StopQueryTaskStatusTimer();
	void ProcessUploadProgress();
	void ProcessDownloadProgress();
	void RefreshUserInfo();
	void ProcessProgress(CString strOperationType);
	void LoadUnFinishedTask();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnColumnclickListNetdisk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMenuBtnSort();
	afx_msg void OnBnClickedMenuBtnShowType();
	afx_msg void OnBnClickedBtnUpload();
	afx_msg void OnBnClickedBtnDownload();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnDownloadFinishMsg(WPARAM ,LPARAM);
	afx_msg void OnNMRClickListNetdisk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListDownToDesktop();
	afx_msg void OnListDelete();
	afx_msg void OnBtnOpen();
	afx_msg void OnListProp();
	afx_msg void OnListRename();
	afx_msg void OnListCut();
	afx_msg void OnListCopy();
	afx_msg void OnUpdateRightPopMenu(CCmdUI *pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnNMDblclkListNetdisk(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void DownLoadToDir(const CString & strDir);
	void SetMenuImage(CMenu *pMenu,CBitmap & bmp,const CString & strImage,UINT iCMd);
	void SetMenuBtnShowTypeIcon(const CString & strImage);
	void UploadDirRecusion(const CString & strDir,const CString & strNetDiskDir);

	// 导航栏按钮
	CSkinBtn m_btnMain;
	CSkinBtn m_btnGoBack;
	CSkinBtn m_btnGoForward;
	CMFCMenuButton m_menuBtnShowFileType;
	CMenu *m_pMenuShowFileType;
	CBitmap m_MenuBitMapAllType;
	CBitmap m_MenuBitMapDocumentType;
	CBitmap m_MenuBitMapPictureType;
	CBitmap m_MenuBitMapMusicType;
	CBitmap m_MenuBitMapVideoType;
	CBitmap m_MenuBitMapRecycleBinType;



	CMFCMenuButton m_MenuBtnPath;
	CMenu *m_pMenuPath;

	CSkinBtn m_btnRefresh;
	CEdit m_EditSearch;
	CButton m_btnSearch;

	// 上传下载栏按钮

	CSkinBtn m_btnUpload;
	CSkinBtn m_MenuBtnNew;
	CSkinBtn m_btnDownLoad;
	CSkinBtn m_btnShare;
	CSkinBtn m_btnOpen;
	CMFCMenuButton m_MenuBtnChangeView;
	CMenu *m_pMenuSortType;
	CMFCMenuButton m_MenubtnSort;

	// 列表框
	CListCtrlEx m_ListCtrl;
	CImageList m_IconList;
	BOOL       m_SortUp[4];

	// 前进后退维护的数据
	vector<CString> m_vListNetDiskDirHistory;
	int   m_iPosition;
	void AddDirChangeHistory(const CString & strDir);
public:
	afx_msg void OnEnKillfocusEditRename();
	CEdit m_editRename;
	CString m_strNewName;
	CString m_strReNameFileNetDiskPath;
	int     m_iReNameFileRowId;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
