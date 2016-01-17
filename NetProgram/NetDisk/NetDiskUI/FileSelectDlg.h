#pragma once

extern const int SELECT_MAX_FILE_NUMBER; 
// CFileSelectDlg 对话框

class CFileSelectDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CFileSelectDlg)

public:
	CFileSelectDlg(BOOL bOpenFileDialog,
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL,
		DWORD dwSize = 0,
		BOOL bVistaStyle = FALSE);   // 标准构造函数
	virtual ~CFileSelectDlg();

	static LRESULT CALLBACK WindowProcNew(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam);
	CStringArray & GetSelectList();
// 对话框数据
//	enum { IDD = IDD_DIALOG_MINE_OPENFILE };

protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitDone();
	virtual void OnFolderChange();
	virtual void OnFileNameChange();
	CString GetDisplayNameOf(LPCITEMIDLIST pidl, DWORD flags);
	virtual BOOL OnNotify(WPARAM wp, LPARAM lp, LRESULT* pRes);
	CListCtrl* GetListCtrl();
	int GetSelectCount();
	DECLARE_MESSAGE_MAP()
	
	static WNDPROC m_wndProc;
public:
	static CStringArray m_SelectedItemList;
	static CString m_strCurrendDirectory;
private:
	CStringArray m_strSelectListTemp;
	CStringArray m_strSelectListNow;
};

CString GetSelectedFolder(HWND hParent = NULL);