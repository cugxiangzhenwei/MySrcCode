// MeYe.h : main header file for the MEYE application
//

#if !defined(AFX_MEYE_H__83F09A97_25E5_498F_BBB6_66366F64C30D__INCLUDED_)
#define AFX_MEYE_H__83F09A97_25E5_498F_BBB6_66366F64C30D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMeYeApp:
// See MeYe.cpp for the implementation of this class
//

class CMeYeApp : public CWinApp
{
public:
	CMeYeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeYeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMeYeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEYE_H__83F09A97_25E5_498F_BBB6_66366F64C30D__INCLUDED_)
