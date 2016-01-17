// PuzzleGame.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PuzzleGame.h"
#include "PuzzleGameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPuzzleGameApp

BEGIN_MESSAGE_MAP(CPuzzleGameApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPuzzleGameApp construction

CPuzzleGameApp::CPuzzleGameApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPuzzleGameApp object

CPuzzleGameApp theApp;


// CPuzzleGameApp initialization

BOOL CPuzzleGameApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	globalData.SetDPIAware ();

	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	CPuzzleGameDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	BCGCBProCleanUp ();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
BOOL CPuzzleGameApp::ExitInstance()
{
	GdiplusShutdown(m_gdiplusToken);
	return CWinApp::ExitInstance();
}