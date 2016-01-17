// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A4A8C089_0E83_4EA4_A694_CC86361DF43F__INCLUDED_)
#define AFX_STDAFX_H__A4A8C089_0E83_4EA4_A694_CC86361DF43F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

///////////////////////////////////////////////////////////////////////////////////////////////////
#include "OfficeXPMenu.h"
#include <math.h>
#include <wingdi.h>
#include <vector>
using namespace std;
#include "IniFile.h"


struct _tagGetPriceUps
{
	char         price[512];//价格
	char         picurl[512];//图片
	char         title[512];//说明

} GetPriceUps,*pGetPriceUps;



struct _tagPriceUps
{
	int          shopid;  // 店ID
	int          utfflag;  // UTF标志
	char         shopname[64];//商店名
	char         shopurl[128];//店地址
	char         head[64];//去头
	int          flag1;//标志1
	char         left1[64];//左关键字
	char         right1[64];//右关键字
	int          flag2;//标志2
	char         left2[64];//左关键字
	char         right2[64];//右关键字
	int          flag3;//标志2
	char         left3[64];//左关键字
	char         right3[64];//右关键字
} PriceUps,*pPriceUps;



typedef	struct	tagProductors	//产品结构定义
{
	long nId;
	char  itemName[64]; // 设备别名
	char  picpath[256]; // 图片
	char  ipadd[256];   // 设备地址
	char  pwd[64];      // 密码

}Productors,*pProductors;

typedef struct _tagItemProductors
{
	CBitmap         *pImage;  // 图像内存
	Productors      productor;// 数据
	CRect           rcPos;     //窗口图片的显示的位置
	CRect           rcPosMain;     //外窗口的显示的位置
}ListItemProductors, *pListItemProductors;


struct _tagLpWnd
{
	LPARAM          lParam;  // 消息管理窗口
	WPARAM          hWnd;//定位窗口
	CRect           rcPos;//未移动前窗口原来的位置
	int             nFlag;
	BOOL            bMove;
};



#include <mshtml.h>
#include <afxinet.h>      //加入CIneternetSession的引用




#define WM_ITEM_DEV_SELECT WM_USER+108
#define WM_ITEM_DEV_ON_CHANGE WM_USER+109
#define OPEN_MSG_ID_URL   WM_USER+112 // do other 



#ifndef	COLORREF2RGB
	#define COLORREF2RGB(Color) (Color&0x0000FF00)|((Color>>16)&0x000000FF)|((Color<<16)&0x00FF0000)
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A4A8C089_0E83_4EA4_A694_CC86361DF43F__INCLUDED_)
