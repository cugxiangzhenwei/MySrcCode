#include "StdAfx.h"
#include "FilePro.h"
#include<Windows.h>
#include <shellapi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<stdio.h>
#include <io.h>

#include <sstream>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "md5.h"
#include "filters.h"
#include "channels.h"
#include "files.h"
#include "hex.h"
#ifdef _DEBUG
#pragma comment(lib, "cryptlibd.lib")
#else
#pragma comment(lib, "cryptlib.lib")
#endif
static std::wstring s2ws(const std::string& s)
{
	setlocale(LC_ALL, "chs"); 
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	std::wstring result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

static CTime ConvertTime_tToCtime( const time_t * t)
{
	int nYear, nMonth, nDate, nHour, nMin, nSec;
	struct tm *local = localtime(t);
	nYear = local->tm_year + 1900;
	nMonth = local->tm_mon + 1;
	nDate = local->tm_mday;
	nHour = local->tm_hour;
	nMin = local->tm_min;
	nSec = local->tm_sec;
	CTime ptime(nYear, nMonth, nDate, nHour, nMin, nSec);
	return ptime;
}
CFilePro::CFilePro(void)
	:m_strErrMsg(_T(""))
{
}


CFilePro::~CFilePro(void)
{
}

vector<CString>  CFilePro::GetSelectedFile(bool bMutilSelect,const char * pszDefExt,const char *pszFilter)
{
	DWORD flags = 6UL;
	if (bMutilSelect)
		flags = OFN_ALLOWMULTISELECT|OFN_PATHMUSTEXIST;

	vector<CString> vOutSelect;
	CFileDialog dlg(TRUE,pszDefExt,NULL,flags,pszFilter);
	if (dlg.DoModal() != IDOK)
	{
		m_strErrMsg = "用户取消";
		return vOutSelect;
	}

	POSITION   pos   =   dlg.GetStartPosition();  
	while(pos   !=   NULL)  
	{  
		CString	filepath   =   dlg.GetNextPathName(pos);
		vOutSelect.push_back(filepath);
	}

	return vOutSelect;
}
CString CFilePro::GetSelectedFolder()
{
	BROWSEINFO bi;  
	ZeroMemory(&bi,sizeof(BROWSEINFO));  
	LPMALLOC pMalloc;  
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  

	if(!pidl )
	{
		m_strErrMsg = "用户取消";
		return "";
	}

	CString strFolderOut = "";
	TCHAR * path = new TCHAR[MAX_PATH];   

	SHGetPathFromIDList(pidl,path);  
	if(SUCCEEDED(SHGetMalloc(&pMalloc))) 
	{  
		pMalloc->Free(pidl);  
		pMalloc->Release();  
	}

	strFolderOut = path;
	delete [] path;
	return strFolderOut;
}

bool CFilePro::OpenFile(CString strFile)
{
	ShellExecuteA(0,"open",strFile,NULL,NULL,SW_SHOWNORMAL);
	return true;
}

bool CFilePro::GetFileMD5(CString strInFile,CString & strOutMd5)
{
	if (access(strInFile,0)!=0)
	{
		m_strErrMsg.Format(_T("文件【%s】无法访问!"),strInFile);
		return false;
	}
	CryptoPP::Weak::MD5 md5;
	CryptoPP::HashFilter filter(md5);

	std::auto_ptr<CryptoPP::ChannelSwitch> channelSwitch(new CryptoPP::ChannelSwitch);
	channelSwitch->AddDefaultRoute(filter);
	std::string s = strInFile.GetBuffer(0);
	std::wstring wstr = s2ws(s);
	CryptoPP::FileSource(wstr.c_str(), true, channelSwitch.release());

	std::stringstream result;
	CryptoPP::HexEncoder encoder(new CryptoPP::FileSink(result), false);
	filter.TransferTo(encoder);
	s = result.str();
	strOutMd5 = s.c_str();
	return true;
}
bool CFilePro::GetFileInfo(CString strInFile,SFileInfo &fileInfo,const char *pszTimeFormat)
{
	struct stat buf;  
	int result;  
	result = stat (strInFile, &buf); 
	if(result!=0)
	{
		m_strErrMsg = _T("获取文件信息失败!");
		return false;
	}

	fileInfo.iFileSize =  buf.st_size;

	CTime m_t = ConvertTime_tToCtime(&buf.st_mtime);
	fileInfo.m_strLastModifyTime = m_t.Format(pszTimeFormat);

	CTime c_t = ConvertTime_tToCtime(&buf.st_ctime);
	fileInfo.m_strCreateTime = c_t.Format(pszTimeFormat);

	CTime a_t = ConvertTime_tToCtime(&buf.st_atime);
	fileInfo.m_strLastAccessTime = a_t.Format(pszTimeFormat);

	fileInfo.m_strFullPath = strInFile;

	char szDriver[3],dir[256],fileName[100],ext[10];
	_splitpath(strInFile,szDriver,dir,fileName,ext);
	fileInfo.m_strFileName = fileName;
	fileInfo.m_strFileName +=ext;

	return true;
}
bool CFilePro::DeleteLocalFile(CString strFile)
{
	if (access(strFile,0)!=0)
	{
		m_strErrMsg.Format( _T("文件【%s】不存在"),strFile);
		return false;
	}

	if (remove(strFile)==0)
		return true;

	return false;
}