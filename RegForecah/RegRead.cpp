#include "RegRead.h"
#include <Windows.h>
#include<io.h>
#include <direct.h>
#include <ShellAPI.h>

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

bool GetTypeName(string strFile,string & strTypeName)
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
static void GetXpTypeItems(vector<NewType> &vAllItems);
bool IsRightNewItem(HKEY keyShellNew,string & strKeyPath)
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
bool GetAllNewFileTypeReg(vector<SNewFileTypeReg> & vNewType)
{
	//打开HKEY_CLASSES_ROOT
	HKEY   hFirstKey,keySub;
	if(ERROR_SUCCESS!=RegOpenKey(HKEY_CLASSES_ROOT,NULL,&hFirstKey))  
		return false;

	char   ExtKey[MAX_PATH];
	DWORD   Item=0,dwLen=MAX_PATH;   

	//遍历 HKEY_CLASSES_ROOT下的所有子项
	while(ERROR_SUCCESS==RegEnumKeyEx(hFirstKey,Item++,
		ExtKey,
		&dwLen,
		NULL,
		NULL,
		NULL,
		NULL))   
	{
		dwLen = MAX_PATH;
		if(ERROR_SUCCESS!=RegOpenKey(hFirstKey,ExtKey,&keySub))  
			continue;

		string strShellNewPath = ExtKey;
		if (!IsRightNewItem(keySub,strShellNewPath)) 
			continue;

		SNewFileTypeReg reg;
		reg.m_strExt = ExtKey;
		reg.m_strKeyPath = strShellNewPath;
		vNewType.push_back(reg);
	}

	return true;
}

void GetAllDesktopRightMenuNewItems(vector<NewType> &vAllItems)
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
bool CreateNewItem(string strItemType,string strDir)
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


	string strNewFileName = strDir  + string("\\") + strTypeComment + strItemType;
	if(ERROR_SUCCESS == lReturn) // 有模板的文件类型
	{
		string strSource = szString;
		if(access(szString,0)!=0)
			strSource = string("C:\\Windows\\ShellNew\\") + szString;

		if(CopyFile(strSource.c_str(),strNewFileName.c_str(),TRUE))
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

		strNewFileName = strDir + "\\新建文件夹";
		mkdir(strNewFileName.c_str());
		return true;
	}
	else
		return false;

	return true;
}
string GetWindowVesion()
{
	OSVERSIONINFO   osversioninfo;
	osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osversioninfo);
	if( (osversioninfo.dwMajorVersion == 5) && (osversioninfo.dwMinorVersion == 1)) 
		return "xp";

	if( (osversioninfo.dwMajorVersion >= 6) && (osversioninfo.dwMinorVersion == 1))
		return "win7";

	return "xp";
}
void GetXpTypeItems(vector<NewType> &vAllItems)
{

	HKEY   hFirstKey,keyFinal;
	if(ERROR_SUCCESS!=RegOpenKey(HKEY_CURRENT_USER,NULL,&hFirstKey))  
		return;

	string strPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Discardable\\PostSetup\\ShellNewXp";
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
		vAllItems.push_back(nt);
		memset(szString,0,sizeof(1000));
	}
}