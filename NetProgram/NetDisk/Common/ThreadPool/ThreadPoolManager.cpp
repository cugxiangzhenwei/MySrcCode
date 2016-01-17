#include "stdafx.h"
#include "TaskBase.h"
#include "PoolThread.h"
#include "ThreadPoolManager.h"
CThreadPoolManager thePoolManager;

CThreadPoolManager::CThreadPoolManager(int iMaxThread /* = 10 */)
{
	m_PoolThreadList.clear();
	m_bRunContrl = TRUE;
	if (iMaxThread < 1 || iMaxThread > 20)
		iMaxThread = 5;

	m_iMaxThreadCount = iMaxThread;
	InitializeCriticalSection(&m_TaskCs);
	m_hManagerThread = CreateThread(NULL,0,CThreadPoolManager::_ControlPoolThread,this,CREATE_SUSPENDED,NULL);
	ResumeThread(m_hManagerThread);
}
CThreadPoolManager::~CThreadPoolManager()
{
	EnterCriticalSection(&m_TaskCs);
	list<CPoolThread*>::iterator iter = m_PoolThreadList.begin();
	for (; iter!=m_PoolThreadList.end();)
	{
		delete (*iter);
		iter = m_PoolThreadList.erase(iter);
	}

	map<CString,CTaskBase*>::iterator iterTaskHistory = m_TaskHistoryMap.begin();
	for (;iterTaskHistory!=m_TaskHistoryMap.end();)
	{
		delete iterTaskHistory->second;
		iterTaskHistory = m_TaskHistoryMap.erase(iterTaskHistory);
	}

	CTaskBase *pTask = PeekOutOneTask();
	while(pTask)
	{
		delete pTask;
		pTask = PeekOutOneTask();
	}

	LeaveCriticalSection(&m_TaskCs);

	DeleteCriticalSection(&m_TaskCs);
}
bool CThreadPoolManager::AddTask(CTaskBase & pTask,BOOL bHighLevel)
{
	EnterCriticalSection(&m_TaskCs);
	CTaskBase *pCopied = pTask.GetCopy();
	CString strTaskId = pCopied->GetTaskId();
	map<CString,CTaskBase*>::iterator iterFind = m_TaskHistoryMap.find(strTaskId);
	if (iterFind != m_TaskHistoryMap.end())
	{
		if (iterFind->second->m_Status == CTaskBase::task_finish)
		{
			delete iterFind->second;
			m_TaskHistoryMap.erase(iterFind); // 该文件有其他已经完成的操作,需先清空记录
		}
		else
		{
			delete pCopied;
			LeaveCriticalSection(&m_TaskCs); // 该文件有其他操作尚未完成
			return false;
		}
	}

	m_TaskHistoryMap.insert(make_pair(strTaskId,pCopied));
	if (bHighLevel)
		m_TaskList.push_front(pTask.GetCopy());
	else
		m_TaskList.push_back(pTask.GetCopy());

	LeaveCriticalSection(&m_TaskCs);
	return true;
}
CTaskBase * CThreadPoolManager::PeekOutOneTask()
{
	if (m_TaskList.empty())
		return NULL;
	CTaskBase *pTask = m_TaskList.front();
	m_TaskList.erase(m_TaskList.begin());
	return pTask;
}
void CThreadPoolManager::ExitAll()
{
	EnterCriticalSection(&m_TaskCs);
	list<CPoolThread*>::iterator iter = m_PoolThreadList.begin();
	for (; iter!=m_PoolThreadList.end();iter++)
	{
		(*iter)->SendExit();
	}
	LeaveCriticalSection(&m_TaskCs);
	WaitFinish();
}
void CThreadPoolManager::WaitPoolManagerExit()
{
	if (m_hManagerThread)
	{
		WaitForSingleObject(m_hManagerThread,INFINITE);
		CloseHandle(m_hManagerThread);
		m_hManagerThread = NULL;
	}
}
void CThreadPoolManager::WaitFinish()
{
	m_bRunContrl = FALSE;
	int iWaitTime = 0;
	while (1)
	{
		BOOL bHaveBusy = FALSE;
		EnterCriticalSection(&m_TaskCs);
		list<CPoolThread *>::iterator iter = m_PoolThreadList.begin();
		for (;iter!=m_PoolThreadList.end();iter++)
		{
			if ((*iter)->IsBusy())
			{
				if (iWaitTime > 30000)
				{
					TerminateThread((*iter)->m_hThread,0);
					delete (*iter)->m_pTask;
					(*iter)->m_pTask = NULL;
					continue;
				}
				bHaveBusy = TRUE;
				break;
			}
		}
		LeaveCriticalSection(&m_TaskCs);
		if (!bHaveBusy)
		{
			break;
		}
		else
		{
			Sleep(50);
			iWaitTime += 50;
		}
	}
	WaitPoolManagerExit();
}
DWORD WINAPI CThreadPoolManager::_ControlPoolThread(LPVOID pParam)
{
	CThreadPoolManager *pThis = (CThreadPoolManager*)pParam;
	// 启动等待接受任务的线程
	while(pThis->m_PoolThreadList.size() < pThis->m_iMaxThreadCount)
	{
		CPoolThread *pThread = new CPoolThread;
		pThis->m_PoolThreadList.push_back(pThread);
	}
	
	CTaskBase *pTask = NULL;
	// 给线程分配任务
	while(pThis->m_bRunContrl)
	{
		EnterCriticalSection(&pThis->m_TaskCs);
		if (pTask==NULL) // 从队列中取出的任务已经分配完毕
		{
			pTask = pThis->PeekOutOneTask();
			if (pTask == NULL)
			{
				LeaveCriticalSection(&pThis->m_TaskCs);
				Sleep(100);
				continue;
			}
		}

		list<CPoolThread *>::iterator iter = pThis->m_PoolThreadList.begin();
		for (;iter!=pThis->m_PoolThreadList.end();iter++)
		{
			if (!(*iter)->IsBusy())
			{
				if ((*iter)->AttchTask(pTask))
				{
					pTask = NULL;
					break;
				}
			}
		}
		LeaveCriticalSection(&pThis->m_TaskCs);
		if (pTask)
		{
			if (pThis->m_bRunContrl)
				Sleep(100); // 任务没有分配成功，等一会儿再检查分配
			else
			{
				delete pTask;
				pTask = NULL;
			}
		}
	}

	if (pTask)
	{
		delete pTask;
		pTask = NULL;
	}
	return 0;
}
CTaskBase * CThreadPoolManager::GetTaskFromHistory(const CString & strTaskId)
{
	EnterCriticalSection(&m_TaskCs);
	map<CString,CTaskBase*>::iterator iterFind = m_TaskHistoryMap.find(strTaskId);
	if (iterFind== m_TaskHistoryMap.end())
	{
		LeaveCriticalSection(&m_TaskCs);
		return NULL;
	}
	CTaskBase *pFind = iterFind->second;
	LeaveCriticalSection(&m_TaskCs);
	return pFind;
}
void CThreadPoolManager::RemoveFinishedTask(const CString & strTaskId)
{
	EnterCriticalSection(&m_TaskCs);
	map<CString,CTaskBase*>::iterator iterFind = m_TaskHistoryMap.find(strTaskId);
	if (iterFind== m_TaskHistoryMap.end())
	{
		LeaveCriticalSection(&m_TaskCs);
		return;
	}
	delete iterFind->second;
	iterFind = m_TaskHistoryMap.erase(iterFind);
	LeaveCriticalSection(&m_TaskCs);
}
void CThreadPoolManager::GetFrontMatchTask(const CString & strTaskIdFront,vector<CTaskBase *> & vTaskRuning)
{
	vTaskRuning.clear();
	EnterCriticalSection(&m_TaskCs);
	map<CString,CTaskBase*>::iterator iterFind = m_TaskHistoryMap.begin();
	for (;iterFind!=m_TaskHistoryMap.end();iterFind++)
	{
		if (iterFind->first.Find(strTaskIdFront)==0)
			vTaskRuning.push_back(iterFind->second);
	}
	LeaveCriticalSection(&m_TaskCs);
}
BOOL CThreadPoolManager::HasFrontMatchTask(const CString & strTaskIdFront)
{
	EnterCriticalSection(&m_TaskCs);
	map<CString,CTaskBase*>::iterator iterFind = m_TaskHistoryMap.begin();
	for (;iterFind!=m_TaskHistoryMap.end();iterFind++)
	{
		if (iterFind->first.Find(strTaskIdFront)==0 && iterFind->first.Compare(strTaskIdFront)!=0)
		{
			LeaveCriticalSection(&m_TaskCs);
			return TRUE;
		}
	}
	LeaveCriticalSection(&m_TaskCs);
	return FALSE;
}
void CThreadPoolManager::ClearAllFinishedTask()
{
	EnterCriticalSection(&m_TaskCs);
	map<CString,CTaskBase*>::iterator iterFind = m_TaskHistoryMap.begin();
	for (;iterFind!=m_TaskHistoryMap.end();)
	{
		if (iterFind->second->m_Status == CTaskBase::task_finish)
		{
			iterFind = m_TaskHistoryMap.erase(iterFind);
			continue;
		}
		iterFind++;
	}
	LeaveCriticalSection(&m_TaskCs);
}