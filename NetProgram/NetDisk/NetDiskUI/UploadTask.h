#pragma once
#include "TaskBase.h"
#include "NetDiskOperation.h"
class CUploadTask : public CTaskBase
{
public:
	CUploadTask(LPCTSTR strNetDiskDir,const CFileInfo & info ,LPCTSTR strLocalFile);
	~CUploadTask(void);
	CUploadTask(const CUploadTask & other);
	 BOOL Run();
	 CTaskBase * GetCopy();
	 CString GetTaskId();
public:
	// 输入参数
	CString m_strNetDiskDir;
	CString m_strLocalFile;

	// 上传进度参数
	long long m_iTotalSize;
	long long m_iFinishedSize;
	CString   m_strFileId;

	// 文件基本信息参数
	CFileInfo m_FileInfo;
	CString   m_strMd5;
};

