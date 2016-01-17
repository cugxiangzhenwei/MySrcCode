#pragma once
#include "TaskBase.h"
class CUploadDirRecusion :
	public CTaskBase
{
public:
	CUploadDirRecusion(LPCTSTR strNetDiskDir,LPCTSTR strLocalDir);
	~CUploadDirRecusion(void);
	CUploadDirRecusion(const CUploadDirRecusion & other);
	BOOL Run();
	CTaskBase * GetCopy();
	CString GetTaskId();
	BOOL UploadDirRecusion(CString strDir,CString strNetDiskDir);
public:
	//  ‰»Î≤Œ ˝
	CString m_strNetDiskDir;
	CString m_strLocalDir;
};

