#pragma once
#include "stdafx.h"

class CTaskBase
{
public:
	CTaskBase();
	virtual ~CTaskBase();
	virtual BOOL Run() = 0;
	virtual CTaskBase * GetCopy()= NULL;
public:
	BOOL m_RunStatus;
};