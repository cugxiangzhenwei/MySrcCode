// »æÍ¼³ÌÐò.h : main header file for the »æÍ¼³ÌÐò application
//

#if !defined(AFX__H__B73E370B_090E_45D1_A223_5F1C8EDAB680__INCLUDED_)
#define AFX__H__B73E370B_090E_45D1_A223_5F1C8EDAB680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyApp:
// See »æÍ¼³ÌÐò.cpp for the implementation of this class
//

class CMyApp : public CWinApp
{
public:
	CMyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMyApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__H__B73E370B_090E_45D1_A223_5F1C8EDAB680__INCLUDED_)
