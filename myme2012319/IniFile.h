// IniFile.h: interface for the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__B93960CE_A35E_4025_BAAE_6F28452C684D__INCLUDED_)
#define AFX_INIFILE_H__B93960CE_A35E_4025_BAAE_6F28452C684D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define    INIFILELPATH				TEXT("\\MeYe.ini")//配置文件路径
class CIniFile  
{
public:
	CIniFile();
	virtual ~CIniFile();
	static void WriteIniFile(const TCHAR* lpSection,const TCHAR* lpKey,const TCHAR* lpValue);	// 写INI文件
	static int  GetIniFileInt(const TCHAR* lpSection,const TCHAR* lpKey);						// 读INI文件,返回INT
	static CString GetIniFileString(const TCHAR* lpSection,const TCHAR* lpKey);					// 读INI文件
	static CString GetCurrentApPath();															// 读配置INI文件路径
//	static CString GetProxyIPAddress();															// 弄瞶狝叭竟IP
//	static BOOL GetDeviceAutoConnect(const int nDevice);
 //	static GetDeviceConnectLocal(const int nDevice);
//	static CString GetLanguageString(CString strCodeID);
};

#endif // !defined(AFX_INIFILE_H__B93960CE_A35E_4025_BAAE_6F28452C684D__INCLUDED_)
