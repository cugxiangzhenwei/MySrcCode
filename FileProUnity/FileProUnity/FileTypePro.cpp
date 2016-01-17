#include "stdafx.h"
#include "FilePro.h"
#include <Windows.h>
#include<io.h>
#include <direct.h>
#include <ShellAPI.h>

/************************************************************************/
/* 在此字符串数组常量中添加过滤类型即可实现过滤                     */
/************************************************************************/
static const char *g_TypeFilter[] = {
				".contact", //联系人
				".library-ms",//库
				"Briefcase",//公文包
				"Folder",  //文件夹
				".lnk",   //快捷方式
				".bfc"    //xp下的公文包
			};

static bool IsUsefullType(const char *pszType)
{
	static int iSize = sizeof(g_TypeFilter)/sizeof(const char*);
	int i = 0;
	while(i<iSize)
	{
		if(stricmp(g_TypeFilter[i],pszType)==0)
			return false; //被过滤掉
		i++;
	}
	return true;
}

std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}
string GetWindowVesion()
{
	OSVERSIONINFO   osversioninfo;
	osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osversioninfo);
	if( (osversioninfo.dwMajorVersion == 5) && (osversioninfo.dwMinorVersion == 1)) 
		return "xp";

	if( (osversioninfo.dwMajorVersion == 6) && (osversioninfo.dwMinorVersion == 1))
		return "win7";

	if( (osversioninfo.dwMajorVersion == 6) && (osversioninfo.dwMinorVersion == 2))
		return "win8";

	return "xp";
}
void GetXpTypeItems(vector<NewType> &vAllItems)
{

	HKEY   hFirstKey,keyFinal;
	if(ERROR_SUCCESS!=RegOpenKey(HKEY_CURRENT_USER,NULL,&hFirstKey))  
		return;

	string strPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Discardable\\PostSetup\\ShellNew";
	if(ERROR_SUCCESS!=RegOpenKey(hFirstKey,strPath.c_str(),&keyFinal))  
		return;


	DWORD dwType;
	DWORD dwSize = 1000;
	WCHAR  szString[1000] = {0};

	DWORD   Item=0,dwLen=MAX_PATH; 
	char   ValNameStr[MAX_PATH];
	while(ERROR_SUCCESS==RegEnumValue(keyFinal,Item++,ValNameStr,&dwLen,NULL,&dwType,(BYTE *)szString,&dwSize))
	{
		dwLen = MAX_PATH;
		if (dwType!=REG_BINARY || stricmp("~reserved~",ValNameStr)==0)
			continue;

		NewType nt;
		nt.m_strDesrciption = ValNameStr;
		wstring wstr = szString + 71;
		nt.m_strExt = ws2s(wstr);
		if (IsUsefullType(nt.m_strExt.c_str()))
			vAllItems.push_back(nt);

		memset(szString,0,sizeof(1000));
	}
}
static bool IsRightNewItem(HKEY keyShellNew,string & strKeyPath)
{
	//遍历ShellNew子键
	DWORD   Item=0,dwLen=MAX_PATH; 
	char   SubKeyStr[MAX_PATH];
	string strTmpPath = strKeyPath;
	while(ERROR_SUCCESS==RegEnumKeyEx(keyShellNew,Item++,SubKeyStr,&dwLen,NULL,NULL,NULL,NULL))   
	{
		strTmpPath = strKeyPath + string("\\") + SubKeyStr;

		dwLen = MAX_PATH; //每次调用RegEnumKeyEx前必须重新将KeySize的值设为KeyMaxLen缓冲区的大小，否则遍历失败
		if (stricmp(SubKeyStr,"ShellNew")==0) //找到SHellNew子键
		{
			int a = 0;
			strKeyPath = strTmpPath;
			return true;
		}
		else //未找到SHellNew子键，则遍历该键下的子键
		{
			HKEY keySub;
			if (RegOpenKey(keyShellNew,SubKeyStr,&keySub)!=ERROR_SUCCESS)
				continue;

			if (IsRightNewItem(keySub,strTmpPath))
			{
				strKeyPath = strTmpPath;
				return true;
			}
			else
				continue;
		}

	}

	return false;
}

static bool GetTypeName(string strFile,string & strTypeName)
{
	SHFILEINFOA info;

	if(SHGetFileInfoA(strFile.c_str(),FILE_ATTRIBUTE_NORMAL,
		&info,sizeof(info),SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES))
	{
		strTypeName = info.szTypeName;
		return true;
	}

	return false;
}

static bool CreateNewFolder(string strDir)
{
	string strNewFileName = strDir + "\\新建文件夹";
	int iTryCount = 1;
	while (access(strNewFileName.c_str(),0)==0)
	{
		iTryCount++;
		CString strNewTypeComment;
		strNewTypeComment.Format("新建文件夹(%d)",iTryCount);
		strNewFileName =  strDir  + string("\\") + (LPCTSTR)strNewTypeComment;
	}
	
	if (mkdir(strNewFileName.c_str())==0)
		return true;

	return false;
}
void CFilePro::GetAllDesktopRightMenuNewItems(vector<NewType> &vAllItems)
{
	if(GetWindowVesion()=="xp")
		return GetXpTypeItems(vAllItems);

	HKEY   hFirstKey,keyFinal;
	if(ERROR_SUCCESS!=RegOpenKey(HKEY_CURRENT_USER,NULL,&hFirstKey))  
		return;

	string strPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Discardable\\PostSetup\\ShellNew";
	if(ERROR_SUCCESS!=RegOpenKey(hFirstKey,strPath.c_str(),&keyFinal))  
		return;


	DWORD dwType;
	DWORD dwSize = 1000;
	char  szString[1000] = {0};

	LONG lReturn = ::RegQueryValueEx (keyFinal,"Classes", NULL,
		&dwType, (BYTE *) szString, &dwSize);


	if(lReturn != ERROR_SUCCESS)
		return;

	string strNew = "";
	for(int i=0;i<dwSize;i++)
	{
		if (szString[i]==0 && strNew.size() > 0)
		{

			if(!IsUsefullType(strNew.c_str()))
			{
				strNew = "";
				continue;
			}

			NewType newType;
			newType.m_strExt = strNew;
			GetTypeName(newType.m_strExt,newType.m_strDesrciption);
			vAllItems.push_back(newType);
			strNew = "";
		}
		else
			strNew.append(1,szString[i]);
	}
}
bool CFilePro::CreateNewItem(string strItemType,string strDir)
{
	string strTypeComment;
	if (!GetTypeName(strItemType,strTypeComment))
		strTypeComment = "新建项";

	HKEY   hFirstKey,keyExt,keyFinal;
	if(ERROR_SUCCESS!=RegOpenKey(HKEY_CLASSES_ROOT,NULL,&hFirstKey))  
		return false;

	if(ERROR_SUCCESS!=RegOpenKey(hFirstKey,strItemType.c_str(),&keyExt))  
		return false;

	string strShellNewPath = strItemType;
	if (!IsRightNewItem(keyExt,strShellNewPath)) 
		return false;

	if(ERROR_SUCCESS!=RegOpenKey(hFirstKey,strShellNewPath.c_str(),&keyFinal))  
		return false;

	DWORD dwType;
	DWORD dwSize = 1000;
	char  szString[1000] = {0};

	LONG lReturn = ::RegQueryValueEx (keyFinal,"FileName", NULL,
		&dwType, (BYTE *) szString, &dwSize);


	int iTryCount = 1;
	string strNewFileName = strDir  + string("\\") + strTypeComment + strItemType;
	while(access(strNewFileName.c_str(),0)==0) //如果文件存在，则在名字后加后缀
	{
		iTryCount++;
		CString strNewTypeComment;
		strNewTypeComment.Format("%s(%d)",strTypeComment.c_str(),iTryCount);
		strNewFileName =  strDir  + string("\\") + string((LPCTSTR)strNewTypeComment) + strItemType;
	}
	if(ERROR_SUCCESS == lReturn) // 有模板的文件类型
	{
		string strSource = szString;
		if(access(szString,0)!=0)
			strSource = string("C:\\Windows\\ShellNew\\") + szString;

		if(::CopyFile(strSource.c_str(),strNewFileName.c_str(),TRUE))
			return true;

		return false;
	}

	lReturn = ::RegQueryValueEx (keyFinal,"NullFile", NULL,
		&dwType, (BYTE *) szString, &dwSize);

	if(ERROR_SUCCESS == lReturn)  
	{
		FILE *pFile = fopen(strNewFileName.c_str(),"w");
		if (pFile==NULL)
			return false;

		fclose(pFile);
	}
	else if (stricmp(strItemType.c_str(),"Folder")==0) //创建文件夹
	{
		return CreateNewFolder(strDir);

		/*strNewFileName = strDir + "\\新建文件夹";
		mkdir(strNewFileName.c_str());
		return true;*/
	}
	else
		return false;

	return true;
}