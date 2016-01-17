#include "StdAfx.h"
#include "DownloadDirTask.h"
#include "CommonFun.h"
#include "NetDiskUI.h"
#include "ThreadPoolManager.h"
#include "NetDiskOperation.h"
#include <imagehlp.h>
#include "LogTools.h"
#include "DownloadTask.h"

CDownloadDirTask::CDownloadDirTask(LPCTSTR strNetDiskDir,const CFileInfo & info,LPCTSTR strLocalDir)
{
	m_bFlag = TRUE;
	m_strErrMsg.Empty();
	m_strOperation = "downloadDir";

	m_strNetDiskDir = strNetDiskDir==NULL ? "":strNetDiskDir;
	m_strLocalDir   = strLocalDir==NULL ? "":strLocalDir;
	if ( m_strNetDiskDir.IsEmpty()
		|| m_strLocalDir.IsEmpty()
		|| info.m_strName.IsEmpty())
	{
		m_bFlag = FALSE;
		m_strErrMsg = "出现空参数！";
	}
	m_strNetDiskDir = strNetDiskDir;
	m_strLocalDir = strLocalDir;
	m_FileInfo = info;
}


CDownloadDirTask::~CDownloadDirTask(void)
{
}
CDownloadDirTask::CDownloadDirTask(const CDownloadDirTask & other)
{
	m_strNetDiskDir = other.m_strNetDiskDir;
	m_strLocalDir   = other.m_strLocalDir;

	m_bFlag = other.m_bFlag;
	m_strErrMsg = other.m_strErrMsg;
	m_strOperation = other.m_strOperation;
	m_dProgress = other.m_dProgress;
	m_Status = other.m_Status;
	m_FileInfo = other.m_FileInfo;
}
BOOL CDownloadDirTask::Run()
{
	CTaskBase *pTask = thePoolManager.GetTaskFromHistory(GetTaskId());
	CDownloadDirTask * pUploadTaskHistory = NULL;
	if (pTask && pTask->m_strOperation=="downloadDir")
		pUploadTaskHistory = dynamic_cast<CDownloadDirTask*>(pTask);

	ASSERT(pUploadTaskHistory!=NULL);
	if (!m_bFlag)
	{
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	BOOL bok = FALSE;
	pUploadTaskHistory->SetTaskStatus(CTaskBase::task_running);
	bok = DownloadDirRecusion(m_strLocalDir,m_FileInfo.m_strName,GetTaskId());
	pUploadTaskHistory->SetTaskStatus(CTaskBase::task_finish);
	return bok;
}
CTaskBase * CDownloadDirTask::GetCopy()
{
	CTaskBase *pCopied = new CDownloadDirTask(*this);
	return pCopied;
}
CString CDownloadDirTask::GetTaskId()
{
	return m_strNetDiskDir;
}
BOOL CDownloadDirTask::DownloadDirRecusion(CString strDestDir,const CString & strDirItemName,CString strNetDiskDir)
{
	CString strDir = strDestDir + "\\" + strDirItemName + "\\";
	strDir.Replace("\\\\","\\");

	if (!MakeSureDirectoryPathExists((LPCTSTR)strDir))
	{
		m_bFlag = FALSE;
		m_strErrMsg.Format("【%s】不是文件夹,且不能创建！",m_strLocalDir);
		return FALSE;
	}
	
	SOCKET sockConn = GetConnect(theApp.m_strServerIp);
	if (sockConn==0)
	{
		m_strErrMsg = "连接服务器失败！";
		m_bFlag = FALSE;
		return FALSE;
	}

	vector<FileListItem *> vList;
	GetNetDiskFileList(sockConn,theApp.m_strUsrId,strNetDiskDir,false,vList);
	for (size_t i=0; i<vList.size();i++)
	{
		CFileInfo info;
		if (_stricmp(vList[i]->szItemType,"File")==0)
		{
			info.m_iImageID = info.m_iImageID;
			info.m_strName = vList[i]->szItemName;
			info.m_strModifyTime = TransferFileTime(vList[i]->iLastModifyTime);
			info.m_strFileSize = TransferFileSize(vList[i]->iFileLength);
			info.m_strFileId = vList[i]->szFileId;
			GetFileInfo(vList[i]->szItemName,info,FALSE);
			CDownloadTask downloadTask(strNetDiskDir,info,strDir);
			if(!thePoolManager.AddTask(downloadTask))
			{
				CString strMsg;
				strMsg.Format("文件\"%s\"前一操作尚未完成，请稍后重试！",info.m_strName);
				MessageBox(theApp.m_pMainWnd->GetSafeHwnd(),strMsg,"提示",MB_OK|MB_ICONINFORMATION);
				delete vList[i];
				continue; // 添加任务失败的，不加入查询进度列表里
			}
			theApp.m_pPageNetDisk->AddRunningDownloadTaskIdList(downloadTask.GetTaskId());
		}
		else
		{
			CString strNetDiskDirSub = strNetDiskDir + CString(vList[i]->szItemName) + "/";
			DownloadDirRecusion(strDir,vList[i]->szItemName,strNetDiskDirSub);
		}
		delete vList[i];
	}
	vList.clear();
	return TRUE;
}