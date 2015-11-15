#pragma once
#include "stdafx.h"
#include <list>
using namespace std;

class CTaskBase;
class CPoolThread;
class CThreadPoolManager
{
	friend CTaskBase;
public:
	CThreadPoolManager(int iMaxThread = 10);
	~CThreadPoolManager();
	bool AddTask(CTaskBase & pTask,BOOL bHighLevel = FALSE);
	void ExitAll();
	static DWORD WINAPI _ControlPoolThread(LPVOID);
private:
	void WaitFinish();
	void WaitPoolManagerExit();
	CTaskBase * PeekOutOneTask(); //负责从任务队列取出一个在排队的任务
private:
	size_t m_iMaxThreadCount;
	CRITICAL_SECTION m_TaskCs;
	list<CTaskBase * > m_TaskList;
	list<CPoolThread *> m_PoolThreadList;
	BOOL m_bRunContrl;
	HANDLE m_hManagerThread;
};
extern CThreadPoolManager thePoolManager;