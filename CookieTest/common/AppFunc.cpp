#include "StdAfx.h"
#include "AppFunc.h"
#include "PtrVectorHeader.h"

//查找进程主窗口的回调函数的参数
typedef struct structGetMainWindowParam
{
	struct structGetMainWindowParam()
		: dwProcessId(0)
		, hwnd(NULL)
		, pCaptionList (NULL)
	{
	}
	HWND hwnd;
	DWORD dwProcessId;
	vector<CString> * pCaptionList;
} GETMAINWINDOW_PARAM;

/*
根据进程id 获取顶层窗口的回调函数的参数结构
*/
typedef struct stGetTopModalWindowParam
{
	struct stGetTopModalWindowParam()
		: dwProcessId(0)
		, hwnd(NULL)
	{
	}
	HWND hwnd;
	DWORD dwProcessId;
} GETTOPMODALWINDOW_PARAM;

//查找进程主窗口的回调函数
static BOOL CALLBACK EnumWindowCallback_GetMainWindow( HWND hWnd, LPARAM lParam )
{
	GETMAINWINDOW_PARAM *pData = (GETMAINWINDOW_PARAM *)lParam;
	if (pData == NULL) return TRUE;

	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId); //取得当前窗口对应的进程id
	if (dwProcessId != pData->dwProcessId) return TRUE;

	if (!IsWindow(hWnd)
		|| ::GetParent(hWnd) != NULL) return TRUE;

	CString strWindowCaption("");
	LPTSTR lpsz = strWindowCaption.GetBufferSetLength(256 + 1);
	::GetWindowText(hWnd, lpsz, 256);
	CString strRealWindowCaption = strWindowCaption;

	BOOL bOk = TRUE;
	strWindowCaption.MakeLower();
	for (int i=0; i<(int)pData->pCaptionList->size(); i++)
	{
		CString str = pData->pCaptionList->at(i);
		str.MakeLower();
		if (strWindowCaption.Find(str) == -1)
		{
			bOk = FALSE;
			break;
		}
	}
	if (!bOk)
		return TRUE;

	pData->hwnd = hWnd;
	return FALSE;
}

//根据进程ID，主窗口标题（模糊匹配）取得进程主窗口句柄
HWND GetMainWindow( DWORD dwPID, vector<CString>& vCaption )
{
	GETMAINWINDOW_PARAM param;
	param.dwProcessId = dwPID;
	param.pCaptionList = &vCaption;
	EnumWindows(EnumWindowCallback_GetMainWindow, (LPARAM)&param);
	return param.hwnd;
}

int GetMainWindow( vector< HWND >& vMainHwnd, LPCTSTR lpszProcessName, vector< CString >& vCaption )
{
	vMainHwnd.clear();
	if ( lpszProcessName == NULL ) return 0;
	if ( strlen( (char*)lpszProcessName ) <= 0 ) return 0;
	vector< PROCESSENTRY32 * > vData;
	GetProcessEntry32( vData, lpszProcessName );
	if ( vData.size() == 0 ) return 0;
	for ( int i=0; i<(int)vData.size(); i++ )
	{
		HWND hWnd = GetMainWindow( vData[ i ]->th32ProcessID, vCaption );
		if ( !IsWindow( hWnd ) ) continue;
		vMainHwnd.push_back( hWnd );
	}
	
	PtrVectorClear( vData );
	return (int)vMainHwnd.size();
}

//进程Pid是否有效
BOOL IsValidProcessID( DWORD dwPID )
{
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPID);
	if (hProcess == NULL) return FALSE;

	DWORD dwExitCode = 0;
	BOOL bOk = ::GetExitCodeProcess(hProcess, &dwExitCode);
	::CloseHandle(hProcess);

	if (!bOk
		|| dwExitCode != STILL_ACTIVE)
		return FALSE;

	return TRUE;
}

//根据进程名称，遍历系统中，当前运行中的进程信息结构
int GetProcessEntry32( vector< PROCESSENTRY32 * >& vData, LPCTSTR lpszProcessName, DWORD dwParentPid )
{
	PtrVectorClear(vData);

	CString strTargetProcessName = lpszProcessName == NULL ? _T("") : lpszProcessName;
	if (strTargetProcessName.IsEmpty()) return 0;

	HANDLE hProcessSnap = NULL;
	PROCESSENTRY32 pe32 = {0};
	CString strCurProcessName(_T(""));

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	memset(&pe32, NULL, sizeof(PROCESSENTRY32));
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			if (strTargetProcessName.CompareNoCase(pe32.szExeFile) != 0) continue;
			if (dwParentPid != 0)
			{
				if (pe32.th32ParentProcessID != dwParentPid
					&& pe32.th32ProcessID != dwParentPid) continue;
			}

			PROCESSENTRY32 *pData = new PROCESSENTRY32;
			*pData = pe32;
			vData.push_back(pData);
		}
		while (Process32Next(hProcessSnap, &pe32));
	}
	CloseHandle(hProcessSnap);
	return (int)vData.size();
}

BOOL SendCopyData( HWND hWnd, DWORD dwId, LPBYTE pData, DWORD dwDataSize )
{
	if ( !IsWindow( hWnd ) ) return FALSE;
	if ( pData == NULL || dwDataSize == 0 ) return FALSE;

	LPBYTE pBuf = new BYTE[ dwDataSize + 1 ];
	memcpy( pBuf, pData, dwDataSize );
	pBuf[ dwDataSize ] = NULL;

	COPYDATASTRUCT cds;
	memset( &cds, NULL, sizeof(cds) );
	cds.cbData = dwDataSize + 1;
	cds.dwData = dwId;
	cds.lpData = pBuf;
	::SendMessage( hWnd, WM_COPYDATA, NULL, (LPARAM)&cds );

	delete[] pBuf; pBuf = NULL;
	return TRUE;
}


//查找弹出顶层窗口的回调函数
static BOOL CALLBACK EnumWindowCallback_GetTopModalWindow( HWND hWnd, LPARAM lParam )
{
	GETTOPMODALWINDOW_PARAM *pData = (GETTOPMODALWINDOW_PARAM *)lParam;
	if (pData == NULL) return TRUE;

	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId); //取得当前窗口对应的进程id
	if (dwProcessId != pData->dwProcessId) return TRUE;

	if ( !IsWindow(hWnd) ) return TRUE;

	DWORD lStyle = GetWindowLong( hWnd, GWL_STYLE );
	DWORD lExStyle = GetWindowLong( hWnd, GWL_EXSTYLE );
	if ( ( lStyle & WS_POPUP && lStyle & WS_VISIBLE ) 
		&& ( lExStyle & WS_EX_LAYERED || ( lStyle & DS_MODALFRAME && lExStyle & WS_EX_DLGMODALFRAME ) || lStyle & WS_THICKFRAME ) )
	{
		//POPUP 并且 VISIBLE  并且 WS_EX_LAYERED 或者 DS_MODALFRAME 和 WS_EX_DLGMODALFRAME
		pData->hwnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

/*
获取窗口弹出的顶层窗口句柄
dwPid： 查找的窗口进程id
找到返回句柄，未找到返回NULL
*/
HWND GetTopModalWindow( DWORD dwPid )
{
	GETTOPMODALWINDOW_PARAM param;
	param.dwProcessId = dwPid;
	EnumWindows(EnumWindowCallback_GetTopModalWindow, (LPARAM)&param);
	return param.hwnd;
}

#pragma comment(lib, "version.lib")
CString GetIEVersion()
{
	char szFileName[500] ={0};
	SHGetSpecialFolderPath(NULL,szFileName,CSIDL_PROGRAM_FILES,0);
	CString strIeFile = szFileName + CString("\\Internet Explorer\\iexplore.exe");
	DWORD dwSize = GetFileVersionInfoSize(strIeFile,NULL);
	if (dwSize==0)
		return "";

	char *pVersionData = new char[dwSize];
	if (!GetFileVersionInfo(strIeFile,0,dwSize,pVersionData))
	{
		delete pVersionData;
		return "";
	}

	UINT nQuerySize;
	DWORD* pTransTable;
	if (!::VerQueryValue(pVersionData, _T("\\VarFileInfo\\Translation"),(void **)&pTransTable, &nQuerySize) )
	{
		delete pVersionData;
		return "";
	}

	DWORD dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));

	LPVOID lpData;
	CString strValue, strBlockName;
	strBlockName.Format(_T("\\StringFileInfo\\%08lx\\%s"),dwLangCharset, _T("ProductVersion"));

	if ( ::VerQueryValue((void **)pVersionData, strBlockName.GetBuffer(0),&lpData, &nQuerySize))
		strValue = (LPCTSTR)lpData;

	strBlockName.ReleaseBuffer();

	delete pVersionData;
	return strValue;
}