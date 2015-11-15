// Game2048.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Game2048.h"
#include "Game2048Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGame2048App

BEGIN_MESSAGE_MAP(CGame2048App, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGame2048App construction

CGame2048App::CGame2048App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CGame2048App object

CGame2048App theApp;


// CGame2048App initialization

BOOL CGame2048App::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	globalData.SetDPIAware ();


	CGame2048Dlg dlg;
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
