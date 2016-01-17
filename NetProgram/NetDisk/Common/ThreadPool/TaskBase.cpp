#include "stdafx.h"
#include "TaskBase.h"

CTaskBase::CTaskBase()
{
	m_bExit = FALSE;
	m_dProgress = 0.0;
	m_strOperation.Empty();
	m_strErrMsg.Empty();
	m_bFlag = TRUE;
	m_Status = task_wait;
}
CTaskBase::~CTaskBase()
{

}
void CTaskBase::SetExit()
{
	m_bExit = TRUE;
}
void CTaskBase::SetTaskStatus(TaskStatus status)
{
	m_Status = status;
}
CTaskBase::TaskStatus CTaskBase::GetTaskStatus()
{
	return m_Status;
}