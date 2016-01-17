#include "StdAfx.h"
#include "UploadDirRecusion.h"
#include "CommonFun.h"
#include "NetDiskUI.h"
#include "ThreadPoolManager.h"
#include "NetDiskOperation.h"
#include "UploadTask.h"
#include "LogTools.h"
#include "TaskSerialization.h"
CUploadDirRecusion::CUploadDirRecusion(LPCTSTR strNetDiskDir,LPCTSTR strLocalDir)
{
	m_bFlag = TRUE;
	m_strErrMsg.Empty();
	m_strOperation = "uploadDir";

	m_strNetDiskDir = strNetDiskDir==NULL ? "":strNetDiskDir;
	m_strLocalDir   = strLocalDir==NULL ? "":strLocalDir;
	if ( m_strNetDiskDir.IsEmpty()
		|| m_strLocalDir.IsEmpty())
	{
		m_bFlag = FALSE;
		m_strErrMsg = "出现空参数！";
	}
	if (!IsDir(m_strLocalDir))
	{
		m_bFlag = FALSE;
		m_strErrMsg.Format("【%s】不是目录！",m_strLocalDir);
	}

	m_strNetDiskDir = strNetDiskDir;
	m_strLocalDir = strLocalDir;
}


CUploadDirRecusion::~CUploadDirRecusion(void)
{

}
CUploadDirRecusion::CUploadDirRecusion(const CUploadDirRecusion & other)
{
	m_strNetDiskDir = other.m_strNetDiskDir;
	m_strLocalDir   = other.m_strLocalDir;

	m_bFlag = other.m_bFlag;
	m_strErrMsg = other.m_strErrMsg;
	m_strOperation = other.m_strOperation;
	m_dProgress = other.m_dProgress;
	m_Status = other.m_Status;
}
BOOL CUploadDirRecusion::Run()
{
	CTaskBase *pTask = thePoolManager.GetTaskFromHistory(GetTaskId());
	CUploadDirRecusion * pUploadTaskHistory = NULL;
	if (pTask && pTask->m_strOperation=="uploadDir")
		pUploadTaskHistory = dynamic_cast<CUploadDirRecusion*>(pTask);

	ASSERT(pUploadTaskHistory!=NULL);
	if (!m_bFlag)
	{
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	BOOL bok = FALSE;
	pUploadTaskHistory->SetTaskStatus(CTaskBase::task_running);
	bok = UploadDirRecusion(m_strLocalDir,m_strNetDiskDir);
	pUploadTaskHistory->SetTaskStatus(CTaskBase::task_finish);
	return bok;
}
CTaskBase * CUploadDirRecusion::GetCopy()
{
	CTaskBase *pCopied = new CUploadDirRecusion(*this);
	return pCopied;
}
CString CUploadDirRecusion::GetTaskId()
{
	return m_strNetDiskDir;
}
BOOL CUploadDirRecusion::UploadDirRecusion(CString strDir,CString strNetDiskDir)
{
	CString strWildCard  = strDir;
	strWildCard +="\\*.*";
	CFileFind finder;
	BOOL bFind = FALSE;
	// 查找文件
	BOOL bErrOccured = FALSE;
	bFind = finder.FindFile(strWildCard);
	while(bFind)
	{
		// 查找下一个文件
		bFind = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		//找到文件的路径
		CString strPathName = finder.GetFilePath();
		CString strItemName = finder.GetFileName();
		if (finder.IsDirectory())
		{
			SOCKET sockClent = GetConnect(theApp.m_strServerIp);
			if (sockClent == 0)
			{
				LogMsg("连接服务器失败!");
				continue;
			}
			string strErrMsg;
			if (!CreateNetDiskDir(sockClent,theApp.m_strUsrId,strItemName,strNetDiskDir,strErrMsg))
			{
				LogMsg("上传目录【%s】失败!",strItemName);
				continue;
			}
			UploadDirRecusion(strPathName,strNetDiskDir + strItemName + "/");
		}
		else
		{
			CFileInfo info;
			if (!GetFileInfo(strPathName,info,TRUE))
			{
				LogMsg("获取文件基本信息失败,调过上传!",strPathName);
				continue;
			}

			info.m_strLocalPath   = strPathName;
			info.m_strNetDiskPath = strNetDiskDir + info.m_strName;

			CUploadTask uploadTask(strNetDiskDir,info,strPathName);
			if (!thePoolManager.AddTask(uploadTask))
			{
				CString strMsg;
				strMsg.Format("文件\"%s\"前一操作尚未完成，调过上传！",strPathName);
				LogMsg(strMsg);
				continue;
			}
			SerializationManager.AddUploadTaskHistory(theApp.m_strUsrId,info);
			theApp.m_pPageNetDisk->AddRunningUploadTaskIdList(uploadTask.GetTaskId());
		}
	}
	finder.Close();
	return TRUE;
}