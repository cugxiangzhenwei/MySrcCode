#include "stdafx.h"
#include "TaskBase.h"
#include "PoolThread.h"

CPoolThread::CPoolThread()
{
	m_pTask = NULL;
	m_hThread = CreateThread(NULL,0,CPoolThread::WorkThread,this,CREATE_SUSPENDED,&m_dWThreadID);
	ResumeThread();
}
CPoolThread::~CPoolThread()
{
	TerminateThread(m_hThread,0);
	CloseHandle(m_hThread);
	m_hThread = NULL;
}
BOOL CPoolThread::AttchTask(CTaskBase * pTask)
{
	m_pTask = pTask;
	return TRUE;
}
BOOL CPoolThread::IsBusy()
{
	return m_pTask!=NULL;
}
void CPoolThread::ResumeThread()
{
	if (m_hThread)
	{
		::ResumeThread(m_hThread);
	}
}
void CPoolThread::SuspendThread()
{
	if (m_hThread)
	{
		::SuspendThread(m_hThread);
	}
}
DWORD WINAPI CPoolThread::WorkThread(LPVOID pData)
{
	CPoolThread * pThread = (CPoolThread*)pData;
	while(1)
	{
		if (pThread->m_pTask)
		{
			pThread->m_pTask->Run();
			delete pThread->m_pTask;
			pThread->m_pTask = NULL;
		}
		else
			Sleep(100);

	}
	return 0;
}