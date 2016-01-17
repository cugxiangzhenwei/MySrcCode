#if !defined(AFX_LEFTCLIENTDLG_H__25A63F6E_A371_434E_83EC_F9E84DCEDB3E__INCLUDED_)
#define AFX_LEFTCLIENTDLG_H__25A63F6E_A371_434E_83EC_F9E84DCEDB3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftClientDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeftClientDlg dialog
#include "MyStatic.h"
#include "SkinBtn.h"
#include "VideoCtrlDlg.h"

typedef	struct	tagBaiduItem	//产品结构定义
{

	char	tempURL[256];			//分类样板网址,需要替代
    char	itemname[256];			//分类名字
	long    UpLoadFlag;				//子类

}BaiduItems,*pBaiduItems;


class CLeftClientDlg : public CDialog
{
// Construction
public:
	CLeftClientDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLeftClientDlg)
	enum { IDD = IDD_DIALOG_LEFT_CLIENT };
	CSkinBtn	m_BtnFanLi;
	CMyStatic	m_DetailText;
	CSkinBtn	m_QunBtn2;
	CSkinBtn    m_QunBtn3;
	CSkinBtn	m_QunBtn1;
	CSkinBtn	m_HfBtn;
	CSkinBtn	m_questionBtn;
	CSkinBtn	m_wbEndBtn;
	CSkinBtn	m_wbStarBtn;
	CSkinBtn	m_wblogBtn;
	CSkinBtn	m_BtnImportDev;
	CSkinBtn	m_BtnAddDev;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLeftClientDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnButtonWeiboLogon();
	afx_msg void OnButtonStar();
	afx_msg void OnButtonEnd();
	afx_msg void OnButtonHf();
	afx_msg void OnButtonQun1();
	afx_msg void OnButtonQun2();
	afx_msg void OnButtonQun3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//		virtual   BOOL PreTranslateMessage(MSG *pMsg) ;

public:

   

   
	void SetCurDlgSta(long nStat)
	{
	/*	long nLast = m_DlgShowBtn;
		m_DlgShowBtn = nStat;//当前对话框显示状态，及当客户端配置时才让显示按妞
		

		if(m_DlgShowBtn!=nLast)
		{
			this->Invalidate(FALSE);
		}*/
	}


	void SetIeHwnd(CVideoCtrlDlg* lpwnd)
	{
		if(lpwnd)
		{
			m_iectrlcur = lpwnd;
		}
	}


private:
	void SetBackImage(int index,CString strName);
    long DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc);
	void SetImageSize(CBitmap *pBitmap, CSize &sz);
	void SetImage(CBitmap &bitmap, CString strName);

private:
    void MouseSelect( const CPoint & point);
	BOOL MouseLDown( const CPoint & point );
	
	

private:

    int    m_DlgShowBtn;  // m_DlgShowBtn = 1时显示按妞

	CBitmap m_ItemBack[9];//前三项是单项的三个背景,第四个是线路选择背景.第5,6,7项是SCROL的顶，中,底的背景,8个是SCROLL按妞的背景
    CSize   m_szItemArea[9];






	

	//以下是自动发weibo的
    int m_idStar0;//笑话
	int m_idStar1;//图文
	int m_idStar2;//微大
	int m_idStar3;//经典微语
	int m_idStar4;//经典英语
	int m_wbbindflag;
	CVideoCtrlDlg*  m_iectrlcur;
	HANDLE  m_hBjThreadRun;
	static DWORD WINAPI StarCovertDbToBjData(LPVOID lpParameter);

	char *m_pbuf ; // 4M
    wchar_t* m_wszString ;

	CString GetGbkToUrlEncode(CString keyWord);
	void getfrombaidu(void);
    void getbddetail(CString url,CString itemkey);

	pBaiduItems m_szBaiduItems;

	 void sendweiboall(int nType);

	 void UTF_8ToGB2312(CString &pOut, char *pText, int pLen);
	void UTF_8ToUnicode(wchar_t* pOut,char *pText);
    void UnicodeToUTF_8(char* pOut,wchar_t* pText);
	void UnicodeToGB2312(char* pOut,wchar_t uData);

	int bindweibotowgz(void);
	int getweibostat(void);

	CString m_weibonum,m_weiboname,m_fllowed,m_fllowing,m_wbuserid;
	long i_m_fllowed,i_m_fllowing;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTCLIENTDLG_H__25A63F6E_A371_434E_83EC_F9E84DCEDB3E__INCLUDED_)
