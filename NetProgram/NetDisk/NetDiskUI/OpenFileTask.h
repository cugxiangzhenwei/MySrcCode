#pragma once
#include "TaskBase.h"
#include "NetDiskOperation.h"
class COpenFileTask :public CTaskBase
{
public:
	COpenFileTask(const CFileInfo & info,LPCTSTR strLocalFile);
	~COpenFileTask(void);
	COpenFileTask(const COpenFileTask & other);

	BOOL Run();
	CTaskBase * GetCopy();
	CString GetTaskId();
public:
	CFileInfo m_FileInfo;
	CString   m_strLocalFile;
private:
	CString m_strOpenType; // "local","netdisk"
	CString m_strOpenTaskId;
};

