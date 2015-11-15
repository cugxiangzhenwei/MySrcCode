#pragma once
#include <vector>
#include <map>
using namespace std;

class CSqliteBase;
class CFileItem
{
public:
	CFileItem(const CString & str,const int & iType)
	{
		m_strPath = str;
		m_iType = iType;
	}
	CString m_strPath;
	int     m_iType;
};

#define  FILE_TYPE 0
#define  DIR_TYPE  1

namespace FileProTool
{
	extern CSqliteBase * g_pDataBase;
	extern vector<CString> m_gFileList;
	extern CRITICAL_SECTION g_cs;

	typedef struct  
	{
		DWORDLONG fn;
		DWORDLONG pfn;
		CString   strName;
		CString   strFullPath;
	}USN_FIEL_ITEM;

	typedef map<DWORDLONG,USN_FIEL_ITEM> MAP_USN_TYPE;

	BOOL GetAllDisk( CStringArray &,BOOL bIncludeRemote = FALSE,BOOL bInCludeCD = FALSE,BOOL bIncludeUSB = FALSE,BOOL bIncludeRamDisk = FALSE );
	BOOL GetAllItems(const CString & strPath,const BOOL & bExitScan);
	BOOL GetAllItemsUseUSN(const CString & strPath,const BOOL & bExitScan);
	BOOL GetAllItemsUseUSN(CString strRootDir,vector<CString> & m_vStrItems,const BOOL & bExitScan);

	CString GetItemPath(MAP_USN_TYPE::iterator iter,MAP_USN_TYPE & mapUSN);
};