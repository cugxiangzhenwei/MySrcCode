#pragma once
#include "TaskBase.h"
#include "NetDiskOperation.h"
class CDownloadTask :public CTaskBase
{
public:
	CDownloadTask(LPCTSTR strNetDiskDir,const CFileInfo & info,LPCTSTR strLocalDir);
	~CDownloadTask(void);
	CDownloadTask(const CDownloadTask & other);

	BOOL Run();
	CTaskBase * GetCopy();
	CString GetTaskId();
	// 输入参数
	CString m_strNetDiskDir;
	CString m_strDestDir;

	// 下载进度参数
	long long m_iTotalSize;
	long long m_iFinishedSize;

	// 文件基本信息参数，fileId必备
	CFileInfo m_FileInfo;
};

