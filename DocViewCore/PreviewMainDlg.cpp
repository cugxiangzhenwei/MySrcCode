// PreviewMainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DocViewCore.h"
#include "PreviewMainDlg.h"
#include "afxdialogex.h"

#define SAFERELEASE( X ) { if( X ) { X->Release(); X = NULL; } }
// CPreviewMainDlg 对话框

IMPLEMENT_DYNAMIC(CPreviewMainDlg, CDialogEx)

CPreviewMainDlg::CPreviewMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPreviewMainDlg::IDD, pParent)
{
	m_pIP = NULL;
	m_pIFile = NULL;
	m_pIStream = NULL;
	m_pStream = NULL;
	m_hGlobal = NULL;
}

CPreviewMainDlg::~CPreviewMainDlg()
{
	if( NULL != m_hGlobal)
	{
		GlobalFree( m_hGlobal);
	}
	SAFERELEASE( m_pIFile )

	if( NULL != m_pIP )
	{
		m_pIP->Unload();		
		SAFERELEASE( m_pIP );
	}
	SAFERELEASE( m_pIStream )
	SAFERELEASE( m_pStream )
}

void CPreviewMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreviewMainDlg, CDialogEx)
END_MESSAGE_MAP()


// CPreviewMainDlg 消息处理程序
HRESULT GetHKCRRegistryKeyAndValue(PCSTR pszSubKey, PCSTR pszValueName, 
	PSTR pszData, DWORD cbData)
{
	HRESULT hr;
	HKEY hKey = NULL;

	// Try to open the specified registry key. 
	hr = HRESULT_FROM_WIN32(RegOpenKeyExA(HKEY_CLASSES_ROOT, pszSubKey, 0, 
		KEY_READ, &hKey));

	if (SUCCEEDED(hr))
	{
		// Get the data for the specified value name.
		hr = HRESULT_FROM_WIN32(RegQueryValueExA(hKey, pszValueName, NULL, 
			NULL, reinterpret_cast<LPBYTE>(pszData), &cbData));

		RegCloseKey(hKey);
	}

	return hr;
}
CString CPreviewMainDlg::GetClsidFromExtn( char * szExtn )
{
	CString cs = "";
	CString strRet = "";
	CHAR wcData[MAX_PATH];	
	LONG cData = sizeof( wcData ) ;
	if( NULL == szExtn )
		return "";

	cs.Format("%s\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}",szExtn );
	if( ERROR_SUCCESS == RegQueryValue(HKEY_CLASSES_ROOT,cs,wcData, &cData) )
	{
		strRet = wcData ;
		return strRet;
	}

	char szDefaultVal[260];
	HRESULT hr;
	hr = GetHKCRRegistryKeyAndValue(szExtn, NULL, szDefaultVal, 
		sizeof(szDefaultVal));

	// If the key exists and its default value is not empty, use the 
	// ProgID as the file type.
	if (SUCCEEDED(hr) && szDefaultVal[0] != L'\0')
	{
		cs.Format("%s\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}",szDefaultVal );
		if( ERROR_SUCCESS == RegQueryValue(HKEY_CLASSES_ROOT,cs,wcData, &cData) )
		{
			strRet = wcData ;
			return strRet;
		}
	}

	char PerceivedType[260];
	hr = GetHKCRRegistryKeyAndValue(szExtn, "PerceivedType", PerceivedType, 
		sizeof(PerceivedType));

	if (SUCCEEDED(hr) && PerceivedType[0] != L'\0')
	{
		cs.Format("SystemFileAssociations\\%s\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}",PerceivedType );
		if( ERROR_SUCCESS == RegQueryValue(HKEY_CLASSES_ROOT,cs,wcData, &cData) )
		{
			strRet = wcData ;
			return strRet;
		}
	}
	return strRet;
}

BOOL CPreviewMainDlg::PreviewDocWithHandle(const CString & strDocFile,HWND hwnd,const CRect & rect)
{
	int iPosDot = strDocFile.ReverseFind('.');
	if (iPosDot <=0) return FALSE;

	CString strExt = strDocFile.Mid(iPosDot);
	CString csClsid = GetClsidFromExtn((char*)(LPCTSTR)strExt );

	if (csClsid.IsEmpty())
		return FALSE;

	if( NULL != m_hGlobal)
	{
		GlobalFree( m_hGlobal);
		m_hGlobal= NULL;
	}

	
	if( NULL != m_pIP )
	{
		m_pIP->Unload();
		SAFERELEASE( m_pIP );					
	}
	SAFERELEASE( m_pIFile )
	SAFERELEASE( m_pIStream )
	SAFERELEASE( m_pStream )

	HRESULT hr = E_FAIL;
	CLSID cls;

	CStringW srtWClSID = (wchar_t *)_bstr_t((LPCTSTR)csClsid);
	CLSIDFromString(srtWClSID,&cls);
	if( S_OK == CoCreateInstance(cls,NULL,CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,IID_IPreviewHandler,(LPVOID*)&m_pIP) )
	{
		m_pIP->QueryInterface(IID_IInitializeWithFile,(LPVOID*)&m_pIFile );
		m_pIP->QueryInterface(IID_IInitializeWithStream,(LPVOID*)&m_pIStream );

	}
	if( m_pIFile )
	{
		CStringW strFileW = (wchar_t*)_bstr_t(strDocFile);
		hr = m_pIFile->Initialize( strFileW,STGM_READ);
	}
	else if( m_pIStream )
	{
		HANDLE hFile = CreateFile(strDocFile,FILE_READ_DATA,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL );
		if( INVALID_HANDLE_VALUE != hFile )
		{
			DWORD dwSize = GetFileSize( hFile,NULL );
			m_hGlobal= GlobalAlloc(GPTR, dwSize );
			BYTE * pByte = (BYTE *)GlobalLock(m_hGlobal);

			if( pByte )
			{
				ReadFile(hFile,pByte,dwSize,&dwSize,NULL);	
				GlobalUnlock(m_hGlobal);

				CreateStreamOnHGlobal(m_hGlobal, TRUE, &m_pStream);	
				hr = m_pIStream->Initialize( m_pStream,STGM_READ);
				m_pStream->AddRef();
			}

			CloseHandle( hFile );
		}
	}

	if( m_pIP == NULL)
		return FALSE;

	hr = m_pIP->SetWindow(hwnd, &rect);
	if (hr != S_OK)
		return FALSE;

	hr = m_pIP->SetRect(&rect);
	if (hr != S_OK)
		return FALSE;

	if (S_OK != m_pIP->DoPreview())
		return FALSE;

	::InvalidateRect(hwnd,NULL,TRUE);
	return TRUE;
}