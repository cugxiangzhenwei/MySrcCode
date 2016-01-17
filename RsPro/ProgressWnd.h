#ifndef _INCLUDE_PROGRESSWND_H  
#define _INCLUDE_PROGRESSWND_H  
#include "StdAfx.h"
#include "afxcmn.h"
/////////////////////////////////////////////////////////////////////////////  
// CProgressWnd window  

class  CProgressWnd : public CWnd  
{  
	// Construction/Destruction  
public:  
	CProgressWnd();  
	CProgressWnd(CWnd* pParent, LPCTSTR pszTitle, BOOL bSmooth = FALSE);  
	virtual ~CProgressWnd();  

	BOOL Create(CWnd* pParent, LPCTSTR pszTitle, BOOL bSmooth = FALSE);  
	BOOL GoModal(LPCTSTR pszTitle =_T("Progress"), BOOL bSmooth = FALSE);  

protected:  

	void CommonConstruct();  

	// Operations  
public:  
	static int  __stdcall GDALCallBack(double dfComplete, const char *pszMessage, void *pData);  
	static void SetCurrentWnd(CProgressWnd *pWnd);  
	static int SagaCallback(int MessageID, long Param_1, long Param_2);  

	static CProgressWnd *   m_pCurProWnd;  

	CString       m_strTitle;  
	BOOL m_bCancelled;  


	void MakebCancelFalse();  
	void SetRange(int nLower, int nUpper, int nStep = 1);  
	void        OnCancelOk()  ;                                          // Set range and step size  
	int OffsetPos(int nPos);                        // Same as CProgressCtrl  
	int StepIt();                                   //    "  
	int SetStep(int nStep);                         //    "  
	int SetPos(int nPos);                           //    "  

	void SetText(LPCTSTR fmt, ...);                 // Set text in text area  

	void Clear();                                   // Clear text, reset bar  
	void Hide();                                    // Hide window  
	void Show();                                    // Show window  

	BOOL Cancelled() { return m_bCancelled; }       // Was "Cancel" hit?  

	void SetWindowSize(int nNumTextLines, int nWindowWidth = 390);  

	void PeekAndPump(BOOL bCancelOnESCkey = TRUE);  // Message pumping for modal operation     

	// Implementation  
protected:  
	void GetPreviousSettings();  
	void SaveCurrentSettings();  
	BOOL m_bModal;  
	BOOL m_bPersistantPosition;  
	int  m_nPrevPos, m_nPrevPercent;  
	int  m_nStep;  
	int  m_nMaxValue, m_nMinValue;  
	int  m_nNumTextLines;         
	CStatic       m_Text;  
	CProgressCtrl m_wndProgress;  
	CButton       m_CancelButton;  
	CString       m_strCancelLabel;  
	CFont         m_font;      




	// Overrides  
	// ClassWizard generated virtual function overrides  
	//{{AFX_VIRTUAL(CProgressWnd)  
public:  
	virtual BOOL DestroyWindow();  
	//}}AFX_VIRTUAL  

	// Generated message map functions  
protected:  
	//{{AFX_MSG(CProgressWnd)  
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);  
	//}}AFX_MSG  
	afx_msg void OnCancel();  
	DECLARE_MESSAGE_MAP()  
};  


#endif  
