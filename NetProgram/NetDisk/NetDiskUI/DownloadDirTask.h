#pragma once
#include "Taskbase.h"
#include "NetDiskOperation.h"
class CDownloadDirTask :
	public CTaskBase
{
public:
	CDownloadDirTask(LPCTSTR strNetDiskDir,const CFileInfo & info,LPCTSTR strLocalDir);
	~CDownloadDirTask(void);
	CDownloadDirTask(const CDownloadDirTask & other);
	BOOL Run();
	CTaskBase * GetCopy();
	CString GetTaskId();
	BOOL DownloadDirRecusion(CString strDir,const CString & strDirItemName,CString strNetDiskDir);
public:
	// 输入参数
	CString m_strNetDiskDir;
	CString m_strLocalDir;
	// 文件基本信息参数，fileId必备
	CFileInfo m_FileInfo;
};

