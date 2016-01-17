// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile()
{

}

CIniFile::~CIniFile()
{

}

void CIniFile::WriteIniFile(const TCHAR* lpSection,const TCHAR* lpKey,const TCHAR* lpValue)
{
	static TCHAR szPath[MAX_PATH];
	memset(szPath,0x00,sizeof(szPath));
	sprintf(szPath,TEXT("%s%s"),GetCurrentApPath(),INIFILELPATH);
	WritePrivateProfileString(lpSection,lpKey,lpValue,szPath);
}

CString CIniFile::GetIniFileString(const TCHAR* lpSection,const TCHAR* lpKey)
{
	static TCHAR szPath[MAX_PATH];
	memset(szPath,0x00,sizeof(szPath));
	sprintf(szPath,TEXT("%s%s"),GetCurrentApPath(),INIFILELPATH);

	CString szValue;
	DWORD dwSize = 1000;
	GetPrivateProfileString(lpSection,lpKey,TEXT(""),
		szValue.GetBuffer(dwSize),dwSize,szPath);
	
	szValue.ReleaseBuffer();
	szValue.Replace(TEXT("\\n"),TEXT("\n"));	
	return szValue;
}



int CIniFile::GetIniFileInt(const TCHAR* lpSection,const TCHAR* lpKey)
{
	static TCHAR szPath[MAX_PATH];
	memset(szPath,0x00,sizeof(szPath));
	sprintf(szPath,TEXT("%s%s"),GetCurrentApPath(),INIFILELPATH);
	CString str = szPath;
	return GetPrivateProfileInt(lpSection,lpKey,0,szPath);
}




CString  CIniFile::GetCurrentApPath()
{
	TCHAR DBFileName[MAX_PATH] ;
	memset(DBFileName,0x00,sizeof(DBFileName));
	GetModuleFileName(AfxGetInstanceHandle(), DBFileName, MAX_PATH);	
	CString strPath_temp;
	strPath_temp = DBFileName;
	strPath_temp = strPath_temp.Mid(0, strPath_temp.ReverseFind('\\'));
	return strPath_temp;
}

/*CString CIniFile::GetProxyIPAddress()
{
	TCHAR szSection[32];
	TCHAR szKey[32];
	
	memset(szSection,0x00,sizeof(szSection));
	memset(szKey,0x00,sizeof(szKey));
	
	wcscpy(szSection,TEXT("Setting"));
	wcscpy(szKey,TEXT("ProxyIPAddress"));
	return GetIniFileString(szSection,szKey);
}*/

/*BOOL CIniFile::GetDeviceAutoConnect(const int nDevice)
{
	TCHAR szSection[32];
	TCHAR szKey[32];
 
	memset(szSection,0x00,sizeof(szSection));
	swprintf(szSection,TEXT("Device%dSetting"),nDevice+1);

	wcscpy(szKey,TEXT("DeviceConnect"));
	
	if ( SYMONITOR_ON == CIniFile::GetIniFileInt(szSection,szKey))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL CIniFile::GetDeviceConnectLocal(const int nDevice)
{
	TCHAR szSection[32];
	TCHAR szKey[32];
 
	memset(szSection,0x00,sizeof(szSection));
	swprintf(szSection,TEXT("Device%dSetting"),nDevice+1);

	wcscpy(szKey,TEXT("LogInMode"));
	
	if ( CONNECTION_LOCALNETWORK == CIniFile::GetIniFileInt(szSection,szKey))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
*/

/*CString CIniFile::GetLanguageString(CString strCodeID)
{
	static TCHAR szPath[MAX_PATH];
	memset(szPath,0x00,sizeof(szPath));
	swprintf(szPath,TEXT("%s%s"),GetCurrentApPath(),INIFILELPATH);


	TCHAR szSection[32];
	TCHAR szKey[32];

	memset(szSection,0x00,sizeof(szSection));
	memset(szKey,0x00,sizeof(szKey));

	wcscpy(szSection,TEXT("Setting"));
	wcscpy(szKey,TEXT("LanguageID"));

	switch ( GetIniFileInt(szSection,szKey))
	{
	case 0:
		wcscpy(szSection,TEXT("ChLanguage"));
		break;
	case 1:
		wcscpy(szSection,TEXT("EnLanguage"));
		break;
	default:
		wcscpy(szSection,TEXT("EnLanguage"));
		break;
	}
	
	wcscpy(szKey,strCodeID);

	CString szValue;
	DWORD dwSize = 1000;
	GetPrivateProfileString(szSection,szKey,TEXT(""),
		szValue.GetBuffer(dwSize),dwSize,szPath);
	
	szValue.ReleaseBuffer();
	szValue.Replace(TEXT("\\n"),TEXT("\n"));	

	return szValue;
}*/