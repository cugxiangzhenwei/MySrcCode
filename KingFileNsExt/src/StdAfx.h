// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__25C94E5F_CDF8_4C88_9104_F6C47F43B42E__INCLUDED_)
#define AFX_STDAFX_H__25C94E5F_CDF8_4C88_9104_F6C47F43B42E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#define WINVER  0x0400
#define _WIN32_IE 0x0400

#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlwin.h>
#include "_atlcontrols.h"               // my version w/bug fixes

#include <shlobj.h>
#include <comdef.h>

#include <vector>

// Convenience macros
#define countof(x) (sizeof(x)/(sizeof(x)[0]))

#ifdef DEFAULT_UNREACHABLE
#undef DEFAULT_UNREACHABLE
#endif
#ifdef _DEBUG
#define DEFAULT_UNREACHABLE default: ATLASSERT(0); break
#else
#define DEFAULT_UNREACHABLE default: __assume(0); break
#endif

// Globals
extern ATLControls::CImageList g_ImglistSmall, g_ImglistLarge;

// Stuff used when sorting the list control
enum ESortedField
{
    SIMPNS_SORT_DRIVELETTER,
    SIMPNS_SORT_VOLUMENAME,
    SIMPNS_SORT_FREESPACE,
    SIMPNS_SORT_TOTALSPACE,
    SIMPNS_SORT_LAST                    // for debug checks
};

class CShellFolderImpl;                 // forward reference

struct CListSortInfo
{
    CShellFolderImpl* pShellFolder;     // parent folder of the view
    ESortedField      nSortedField;     // which field to sort by
    bool              bForwardSort;     // which direction to sort
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__25C94E5F_CDF8_4C88_9104_F6C47F43B42E__INCLUDED)
