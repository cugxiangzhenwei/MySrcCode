#include "StdAfx.h"
#include "MonitorDiskTask.h"
#include "LogTools.h"
vector<CFileAction *> g_MonotorFileActionList;
CRITICAL_SECTION      g_FileActionListCS;

CMonitorDiskTask::CMonitorDiskTask(CString strMonitorDir)
{
	m_strMonitorDir = strMonitorDir;
	m_handleDir = NULL;
}

CMonitorDiskTask::~CMonitorDiskTask(void)
{
}
BOOL CMonitorDiskTask::Run()
{
	if (m_strMonitorDir.Right(1)!="\\")
		m_strMonitorDir.AppendChar('\\');

	m_handleDir = CreateFile( m_strMonitorDir, FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | 
		FILE_SHARE_WRITE |
		FILE_SHARE_DELETE, NULL, 
		OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | 
		FILE_FLAG_OVERLAPPED, NULL); 
	if (m_handleDir == INVALID_HANDLE_VALUE) 
		return FALSE;

	char notifyMsg[1024] = {0};
	wchar_t wstrFileName[MAX_PATH*2]={0};
	CString strFileName;
	DWORD dRetBytes = 0;
	CString strReNameOldFile;

	FILE_NOTIFY_INFORMATION *pnotify=(FILE_NOTIFY_INFORMATION *)notifyMsg; 
	FILE_NOTIFY_INFORMATION *tmp; 
	
	while (!m_bExit && m_handleDir) 
	{ 
		_strset_s(notifyMsg,0);
		if(!ReadDirectoryChangesW(m_handleDir, &notifyMsg, sizeof(notifyMsg),
			TRUE, FILE_NOTIFY_CHANGE_ATTRIBUTES|
			FILE_NOTIFY_CHANGE_FILE_NAME| 
			FILE_NOTIFY_CHANGE_DIR_NAME|
			FILE_NOTIFY_CHANGE_CREATION, 
			&dRetBytes, NULL, NULL)) 
			continue;

		tmp = pnotify; 
		if (tmp->Action >= 6)
			continue;

	
		CFileAction *pAction = NULL;
		strReNameOldFile.Empty();
		while(1) 
		{
			StrCpyNW(wstrFileName,tmp->FileName,tmp->FileNameLength/2 + 1);
			strFileName = wstrFileName;
			strFileName.MakeLower();
			if ( ( strFileName.GetLength() >11 && strFileName.Right(11)==".db-journal")
				||( strFileName.GetLength() > 4 && strFileName.Right(4)==".tmp")
				|| strFileName.Find("recycle.bin")>0)
				break;

			strFileName = m_strMonitorDir + strFileName;

			switch(tmp->Action) 
			{ 
			case FILE_ACTION_ADDED: //添加文件
				pAction = new CFileAction(CFileAction::AC_ADD,strFileName);
				break; 
			case FILE_ACTION_REMOVED: //删除文件
				pAction = new CFileAction(CFileAction::AC_DEL,strFileName);
				break; 
			case FILE_ACTION_RENAMED_OLD_NAME:  // 修改文件名字
				strReNameOldFile = strFileName;
				break; 
			case FILE_ACTION_RENAMED_NEW_NAME: // 修改文件名字
				pAction = new CFileAction(CFileAction::AC_RENAME,strReNameOldFile,strFileName);
				break; 
			default: 
				break; 
			}
			if (tmp->NextEntryOffset > 0)
			{
				tmp = (FILE_NOTIFY_INFORMATION*)((char*)tmp + tmp->NextEntryOffset);
			}
			else
				break;
		}
		if (pAction) 
		{
			EnterCriticalSection(&g_FileActionListCS);
			g_MonotorFileActionList.push_back(pAction);
			LeaveCriticalSection(&g_FileActionListCS);
		}
	}

	LogMsg("结束【%s】磁盘监控...",m_strMonitorDir);
	return TRUE;
}
CTaskBase * CMonitorDiskTask::GetCopy()
{
	CMonitorDiskTask *pTask = new CMonitorDiskTask(m_strMonitorDir);
	return pTask;
}
void CMonitorDiskTask::SetExit()
{
	m_bExit = TRUE;
	if (m_handleDir)
	{
		LogMsg("关闭【%s】磁盘监控句柄",m_strMonitorDir);
		CloseHandle(m_handleDir);
		m_handleDir = NULL;
	}
}