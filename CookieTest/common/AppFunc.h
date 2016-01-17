#pragma once
#include "StlHeader.h"
#include <tlhelp32.h>
#include <atlstr.h>

extern int GetProcessEntry32( vector< PROCESSENTRY32 * >& vData
	, LPCTSTR lpszProcessName, DWORD dwParentPid = 0 );

extern BOOL IsValidProcessID( DWORD dwPID );

extern HWND GetMainWindow( DWORD dwPID, vector< CString >& vCaption );

extern int GetMainWindow( vector< HWND >& vMainHwnd, LPCTSTR lpszProcessName, vector< CString >& vCaption );

extern BOOL SendCopyData( HWND hWnd, DWORD dwId, LPBYTE pData, DWORD dwDataSize );

extern HWND GetTopModalWindow( DWORD dwPid );
extern CString GetIEVersion();