// PuzzleGame.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CPuzzleGameApp:
// See PuzzleGame.cpp for the implementation of this class
//

class CPuzzleGameApp : public CWinApp
{
public:
	CPuzzleGameApp();

// Overrides
	public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();
private:
	ULONG_PTR m_gdiplusToken;    // ULONG PTR Îªint64 ÀàÐÍ
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPuzzleGameApp theApp;