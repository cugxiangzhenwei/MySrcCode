// Game2048.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CGame2048App:
// See Game2048.cpp for the implementation of this class
//

class CGame2048App : public CWinApp
{
public:
	CGame2048App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGame2048App theApp;