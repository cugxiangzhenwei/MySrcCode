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
	LeaveCriticalSection(&m_TaskCs);

	DeleteCriticalSection(&m_TaskCs);
}
bool CThreadPoolManager::AddTask(CTaskBase & pTask,BOOL bHighLevel)
{
	EnterCriticalSection(&m_TaskCs);
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
	while (1)
	{
		BOOL bHaveBusy = FALSE;
		EnterCriticalSection(&m_TaskCs);
		list<CPoolThread *>::iterator iter = m_PoolThreadList.begin();
		for (;iter!=m_PoolThreadList.end();iter++)
		{
			if ((*iter)->IsBusy())
			{
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
		}
	}
	m_bRunContrl = FALSE;
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
	
	// 给线程分配任务
	while(pThis->m_bRunContrl)
	{
		EnterCriticalSection(&pThis->m_TaskCs);
		CTaskBase *pTask = pThis->PeekOutOneTask();
		if (pTask == NULL)
		{
			LeaveCriticalSection(&pThis->m_TaskCs);
			Sleep(100);
			continue;
		}

		list<CPoolThread *>::iterator iter = pThis->m_PoolThreadList.begin();
		for (;iter!=pThis->m_PoolThreadList.end();iter++)
		{
			if (!(*iter)->IsBusy())
			{
				(*iter)->AttchTask(pTask);
				break;
			}
		}
		LeaveCriticalSection(&pThis->m_TaskCs);
	}
	return 0;
}