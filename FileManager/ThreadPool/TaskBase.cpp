#include "stdafx.h"
#include "TaskBase.h"

CTaskBase::CTaskBase()
{
	m_RunStatus = FALSE;
	m_bExit = FALSE;
}
CTaskBase::~CTaskBase()
{

}
void CTaskBase::SetExit()
{
	m_bExit = TRUE;
}