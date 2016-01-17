// ComBase.cpp : CComBase 的实现

#include "stdafx.h"
#include "ComBase.h"


// CComBase


STDMETHODIMP CComBase::PrintMsg(CHAR* msg)
{
	// TODO: 在此添加实现代码
	printf("%s",msg);

	return S_OK;
}

STDMETHODIMP CComBase::Sum(LONG iStart, LONG iEnd)
{
	// TODO: 在此添加实现代码

	LONG sum = 0;
	for (LONG i= iStart;i<=iEnd;i++)
	{
		sum += i;
	}
	printf("%d",sum);
	return S_OK;
}
