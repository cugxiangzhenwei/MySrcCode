
// PeraSqlToolDlg.h : 头文件
//

#pragma once
#include "MineListBox.h"
#include <vector>
#include "afxcmn.h"
using namespace std;

// CPeraSqlToolDlg 对话框
class CPeraSqlToolDlg : public CDialogEx
{
// 构造
public:
	CPeraSqlToolDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CPeraSqlToolDlg();

// 对话框数据
	enum { IDD = IDD_PERASQLTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	bool IsItemOk(const CString & strItemDir);
	BOOL CheckFolder();
	void ShowProgressMsg(CString strMsg);
	BOOL CopyDir(const char * pSrc,const char *pDes,int iLevelFolder);
	BOOL RemoveDir(const CString & strDir);
	BOOL UpdateSqlBatFile(const CString & strBatFile);
	BOOL UpateSqlFile(const CString &strSqlFile);
	void ExcuteBatImport();
	void ExcuteOneItem(const CString & strItemDir);
	void UpdateUI(BOOL bEnable);
	static DWORD WINAPI ThreadImportPro(LPVOID); 
// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonExcute();
	afx_msg void OnBnClickedButtonSelSql();
	afx_msg LRESULT OnNewCmdMsg(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	// 用户名
	CString m_StrUser;
	// 用户密码
	CString m_StrPassWd;
	// 实例名
	CString m_strCaseName;
	// SQL目录
	CString m_StrSqlDir;

	FILE  *m_pCmdOutFile;
	vector<CString> m_vStrDirItem;
	CString m_strRunDir;
	BOOL  m_bImportFinished;
	CMineListBox m_List;
	CProgressCtrl m_ProgressCtrl;
};
