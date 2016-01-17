// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E896B6DF_4E69_4FAB_9A11_A686A55B06D1__INCLUDED_)
#define AFX_STDAFX_H__E896B6DF_4E69_4FAB_9A11_A686A55B06D1__INCLUDED_

#if _MSC_VER > 1500
#pragma once
#endif // _MSC_VER > 1500

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <gdal_priv.h>
#pragma comment(lib,"gdal_i.lib")
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include "RasterTrans.h"
#include "ProgressWnd.h"
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

//WINVER not defined. Defaulting to 0x0600 (Windows Vista)
#define WINVER 0x0400  //∂®“Âwindows∞Ê±æ∫≈
#endif // !defined(AFX_STDAFX_H__E896B6DF_4E69_4FAB_9A11_A686A55B06D1__INCLUDED_)
