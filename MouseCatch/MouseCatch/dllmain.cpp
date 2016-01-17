// dllmain.cpp : 定义 DLL 应用程序的入口点。

#define _WIN32_WINNT  0x0500				// 设置系统版本, 确保可以使用底层键盘钩子
#include "stdafx.h"
#include "MouseCatch.h"
#include <Windows.h>
static HINSTANCE	g_hInstance = NULL;				// 模块实例句柄
static HHOOK		g_hHook = NULL;					// 钩子句柄

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	// 保存模块实例句柄
	g_hInstance = (HINSTANCE)hModule;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		if (g_hHook != NULL) UnhookWindowsHookEx(g_hHook);
		break;
	}
	return TRUE;
}
/// 底层键盘钩子函数
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode==HC_ACTION)
	{
		if (wParam==WM_LBUTTONDOWN)
		{
			OutputDebugString("鼠标左键按下\n");
		}
	}
	
	// 传给系统中的下一个钩子
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

BOOL WINAPI StartMouseHook()
{
	g_hHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, g_hInstance, NULL);
	if (g_hHook == NULL) 
		return FALSE;

	return TRUE;
}
void WINAPI StopMouseHook()
{
	if (g_hHook)
	{
		UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
	}
}