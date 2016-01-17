#pragma once
#include "stdafx.h"
#include <list>
#include <map>
#include <vector>
using namespace std;

class CTaskBase;
class CPoolThread;
class CThreadPoolManager
{
	friend CTaskBase;
public:
	CThreadPoolManager(int iMaxThread = 5);
	~CThreadPoolManager();
	bool AddTask(CTaskBase & pTask,BOOL bHighLevel = FALSE);
	void ExitAll();
	static DWORD WINAPI _ControlPoolThread(LPVOID);
	CTaskBase * GetTaskFromHistory(const CString & strTaskId);// 用于具体的任务获取任务指针，然后更新任务的状态
	void RemoveFinishedTask(const CString & strTaskId);
	void GetFrontMatchTask(const CString & strTaskIdFront,vector<CTaskBase *> & vOut);
	BOOL HasFrontMatchTask(const CString & strTaskIdFront);
	void ClearAllFinishedTask();
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
	map<CString,CTaskBase*> m_TaskHistoryMap;
};
extern CThreadPoolManager thePoolManager;