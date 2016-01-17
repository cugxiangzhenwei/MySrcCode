
// PhoneBookDlg.h : 头文件
//

#pragma once

#define CPPCONN_LIB_BUILD
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


using namespace std;
// CPhoneBookDlg 对话框
class CPhoneBookDlg : public CDialogEx
{
// 构造
public:
	CPhoneBookDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PHONEBOOK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnBnClickedButtonPre();
	afx_msg void OnBnClickedButtonNext();
private:
	sql::Driver *m_pdriver;
	sql::Connection *m_pCon;
	sql::ResultSet *m_pres;
	BOOL m_bIsUpdateEditing;
	BOOL m_bIsAddEditing;
	BOOL m_bDrawSelectPhoto;
	BOOL m_bDrawBlank;

	void ReloadData();
	void UpdatePreNextBtnState();
	void EnableEdit(BOOL bEnable);
	void DrawPhoto(CDC & dc);
	void DrawSelectPhoto(CDC & dc);
	void SelectPhoto();
	void ClearAddState();
	void DrawBlank();
	void ReDrawPhotoArea();
public:
	CString m_strName;
	CString m_strSex;
	CString m_strSelectPhoto;
	// 年龄
	int m_iAge;
	CString m_strPhoneNumber;
	CString m_strLocation;
	CString m_strRelation;
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
