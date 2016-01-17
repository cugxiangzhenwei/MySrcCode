#include "stdafx.h"
#include "TaskBase.h"
#include "PoolThread.h"

CPoolThread::CPoolThread()
{
	m_pTask = NULL;
	m_bExit = FALSE;
	m_hThread = CreateThread(NULL,0,CPoolThread::WorkThread,this,CREATE_SUSPENDED,&m_dWThreadID);
	ResumeThread();
}
CPoolThread::~CPoolThread()
{
	if (m_pTask)
	{
		delete m_pTask;
		m_pTask = NULL;
	}
	CloseHandle(m_hThread);
	m_hThread = NULL;
}
BOOL CPoolThread::AttchTask(CTaskBase * pTask)
{
	if (m_bExit)
		return FALSE;

	m_pTask = pTask;
	return TRUE;
}
BOOL CPoolThread::IsBusy()
{
	return m_pTask!=NULL;
}
BOOL CPoolThread::ExitThread()
{
	return m_bExit;
}
void CPoolThread::SendExit()
{
	m_bExit = TRUE;
	if (m_pTask)
	{
		m_pTask->SetExit();
	}
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
	while(!pThread->ExitThread())
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

	if (pThread->m_pTask)
	{
		delete pThread->m_pTask;
		pThread->m_pTask = NULL;
	}

	return 0;
}