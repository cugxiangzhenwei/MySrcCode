// FileManager.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CFileManagerApp:
// See FileManager.cpp for the implementation of this class
//

class CFileManagerApp : public CWinApp
{
public:
	CFileManagerApp();

// Overrides
	public:
	virtual BOOL InitInstance();
	void ProcessCmd();
private:
	BOOL m_bRunAtBack;

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFileManagerApp theApp;