#pragma once
#include "stdafx.h"

class CTaskBase
{
public:
	typedef enum
	{
		task_wait    = 0,
		task_running = 1,
		task_stop    = 2,
		task_finish  = 3,
		task_error   = 4
	}TaskStatus;

public:
	CTaskBase();
	virtual ~CTaskBase();
	virtual BOOL Run() = 0;
	virtual CTaskBase * GetCopy()= NULL;
	virtual void SetExit();
	virtual CString GetTaskId()=NULL;
	void SetTaskStatus(TaskStatus status);
	TaskStatus GetTaskStatus();
public:
	double m_dProgress;
	CString m_strOperation;

	// 运行状态参数
	CString m_strErrMsg;
	BOOL    m_bFlag;
	TaskStatus m_Status;
protected:
	BOOL m_bExit;
};