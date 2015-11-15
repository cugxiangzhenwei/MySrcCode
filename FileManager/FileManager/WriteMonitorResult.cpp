#include "StdAfx.h"
#include "WriteMonitorResult.h"
#include "SqliteBase.h"
#include "MonitorDiskTask.h"
#include "LogTools.h"
#include "sqlite3.h"
CWriteMonitorResult::CWriteMonitorResult(CSqliteBase * pDataBase,HWND hReceiveMsg)
{
	m_pDataBase = pDataBase;
	m_hReceiveMsg = hReceiveMsg;
}


CWriteMonitorResult::~CWriteMonitorResult(void)
{
}

BOOL CWriteMonitorResult::Run()
{
	vector<CFileAction *>::iterator iter;
	vector<CFileAction *> ListCopy;
	while(!m_bExit)
	{
		EnterCriticalSection(&g_FileActionListCS);
		ListCopy.assign(g_MonotorFileActionList.begin(),g_MonotorFileActionList.end());
		g_MonotorFileActionList.clear();
		LeaveCriticalSection(&g_FileActionListCS);

		if (ListCopy.empty())
		{
			Sleep(1000);
			continue;
		}
		
		iter = ListCopy.begin(); 
		for (iter;iter!=ListCopy.end();)
		{
			if (m_bExit)
				break;

			if ((*iter)->m_Type == CFileAction::AC_ADD)
				AddRecord(*iter);

			else if ((*iter)->m_Type == CFileAction::AC_DEL)
				RemoveRecord(*iter);

			else if ((*iter)->m_Type == CFileAction::AC_RENAME)
				ModifyRecord(*iter);

			iter = ListCopy.erase(iter);
		}

		for (size_t i=0;i< ListCopy.size();i++)
			delete ListCopy[i];

		ListCopy.clear();
	}
	
	return TRUE;
}

CTaskBase *  CWriteMonitorResult::GetCopy()
{
	return new CWriteMonitorResult(*this);
}
void CWriteMonitorResult::AddRecord(CFileAction * pAction)
{
	char * zSQL = sqlite3_mprintf("insert into FILE(path,type) values('%q',%d);",pAction->m_strFile,0);
	CString strSql = zSQL;
	sqlite3_free(zSQL);
	m_pDataBase->ExcuteSqlCmd(strSql);
	LogMsg("新建文件【%s】\n",pAction->m_strFile);
	if (m_hReceiveMsg)
		::PostMessage(m_hReceiveMsg,WM_UPDATE_SEARCH,0,(LPARAM)pAction);
	else
		delete pAction;
}
void CWriteMonitorResult::RemoveRecord(CFileAction * pAction)
{
	char * zSQL = sqlite3_mprintf("delete from FILE where path = '%q';",pAction->m_strFile);
	CString strSql = zSQL;
	sqlite3_free(zSQL);
	m_pDataBase->ExcuteSqlCmd(strSql);
	LogMsg("删除文件【%s】\n",pAction->m_strFile);
	if (m_hReceiveMsg)
		::PostMessage(m_hReceiveMsg,WM_UPDATE_SEARCH,0,(LPARAM)pAction);
	else
		delete pAction;
}
void CWriteMonitorResult::ModifyRecord(CFileAction * pAction)
{
	char * zSQL = sqlite3_mprintf("delete from FILE where path = '%q';",pAction->m_strFile);
	CString strSql = zSQL;
	sqlite3_free(zSQL);
	m_pDataBase->ExcuteSqlCmd(strSql);

	zSQL = sqlite3_mprintf("insert into FILE(path,type) values('%q',%d);",pAction->m_strNewFile,0);
	strSql = zSQL;
	sqlite3_free(zSQL);
	m_pDataBase->ExcuteSqlCmd(strSql);

	LogMsg("重命名文件【%s】到【%s】\n",pAction->m_strFile,pAction->m_strNewFile);
	if (m_hReceiveMsg)
		::PostMessage(m_hReceiveMsg,WM_UPDATE_SEARCH,0,(LPARAM)pAction);
	else
		delete pAction;
}