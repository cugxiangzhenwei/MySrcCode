// MyOverlayIcon.cpp : CMyOverlayIcon 的实现

#include "stdafx.h"
#include "MyOverlayIcon.h"


// CMyOverlayIcon

// IShellIconOverlayIdentifier::GetOverlayInfo
// returns The Overlay Icon Location to the system
STDMETHODIMP CMyOverlayIcon::GetOverlayInfo(
	LPWSTR pwszIconFile,
	int cchMax,
	int* pIndex,
	DWORD* pdwFlags)
{
	// Get our module's full path
	GetModuleFileNameW(_AtlBaseModule.GetModuleInstance(), pwszIconFile, cchMax);

	// Use first icon in the resource
	*pIndex=2; 

	*pdwFlags = ISIOI_ICONFILE | ISIOI_ICONINDEX;
	return S_OK;
}

// IShellIconOverlayIdentifier::GetPriority
// returns the priority of this overlay 0 being the highest. 
STDMETHODIMP CMyOverlayIcon::GetPriority(int* pPriority)
{
	// we want highest priority 
	*pPriority=0;
	return S_OK;
}

// IShellIconOverlayIdentifier::IsMemberOf
// Returns whether the object should have this overlay or not 
STDMETHODIMP CMyOverlayIcon::IsMemberOf(LPCWSTR pwszPath, DWORD dwAttrib)
{
	wchar_t *s = _wcsdup(pwszPath);
	HRESULT r = S_FALSE;

	_wcslwr(s);

	// Criteria
	if (wcsstr(s, L"文档") != 0) // 查看是否包含"文档"两个字,如果包含则显示图标
		r = S_OK;

	free(s);

	return r;
}