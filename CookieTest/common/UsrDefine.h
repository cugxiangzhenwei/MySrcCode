#pragma once

enum
{
	WM_CD_SETCOOKIE  = (WM_USER +1),
	WM_CD_GETCOOKIE
};

typedef struct
{
	CStringW m_strHost;
	CStringW m_strValue;
}CookiePair;

extern CRITICAL_SECTION g_CookieListCS;

#define MAX_COOKIE_SETS_COUNT    150