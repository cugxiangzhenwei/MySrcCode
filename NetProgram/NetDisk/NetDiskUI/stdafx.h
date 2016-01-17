
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#define  CAPTION_HEIGHT 130  // 标题栏高度 
#define  STATUS_HEIGHT 30  // 主界面状态栏高度
#define  BORDER_WIDTH  1  // 边框宽度
#define  TOOLBAR_HEIGHT_NAVAGITE 35 // 网盘页面导航栏高度
#define  CAPTION_BTN_GAP       25 // 标题栏大图标之间的间隔
#define  CAPTON_BTN_START_X    25 // 标题栏按钮起始位置x坐标
#define  CAPTON_BTN_START_Y    25 // 标题栏按钮起始位置y坐标
#define  CAPTON_BTN_WIDTH      75 // 标题栏大按钮宽度
#define  CAPTON_BTN_HEIGHT     85 // 标题栏大按钮高度

typedef enum
{
	WM_LOGIN_SUCCESS = WM_USER + 1,
	WM_RESITER_SUCCESS,
	WM_DWONLOAD_FINISH,
	WM_STATUS_TEXT_CHANGE
};

typedef enum
{
	WM_TMIMER_QUER_STATUS = 4000,
	WM_TMIMER_LOAD_HIST
};