/****************************** Module Header ******************************\
Module Name:  dllmain.cpp
Project:      CppShellExtContextMenuHandler
Copyright (c) Microsoft Corporation.

The file implements DllMain, and the DllGetClassObject, DllCanUnloadNow, 
DllRegisterServer, DllUnregisterServer functions that are necessary for a COM 
DLL. 

DllGetClassObject invokes the class factory defined in ClassFactory.h/cpp and 
queries to the specific interface.

DllCanUnloadNow checks if we can unload the component from the memory.

DllRegisterServer registers the COM server and the preview handler in the 
registry by invoking the helper functions defined in Reg.h/cpp. The preview 
handler is associated with the .recipe file class.

DllUnregisterServer unregisters the COM server and the preview handler. 

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/
#include "stdafx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// global data

// The following symbol used to force inclusion of this module for _USRDLL
#ifdef _X86_
extern "C" { int _afxForceUSRDLL; }
#else
extern "C" { int __afxForceUSRDLL; }
#endif

#include <windows.h>
#include <Guiddef.h>
#include "ClassFactory.h"           // For the class factory
#include "Reg.h"

// {DE47995C-5A99-47E1-B624-2651CAEE0DA4}
// When you write your own handler, you must create a new CLSID by using the 
// "Create GUID" tool in the Tools menu, and specify the CLSID value here.
const CLSID CLSID_WsPreviewHandler = 
{ 0xde47995c, 0x5a99, 0x47e1, { 0xb6, 0x24, 0x26, 0x51, 0xca, 0xee, 0xd, 0xa4 } };

// {021C6987-4ADC-4E20-B931-734C0170E918}
// When you write your own handler, you must create a new CLSID by using the 
// "Create GUID" tool in the Tools menu, and specify the CLSID value here.
const GUID APPID_WsPreviewHandler = 
{ 0x21c6987, 0x4adc, 0x4e20, { 0xb9, 0x31, 0x73, 0x4c, 0x1, 0x70, 0xe9, 0x18 } };



HINSTANCE   g_hInst     = NULL;
long        g_cDllRef   = 0;


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
        // Hold the instance of this DLL module, we will use it to get the 
        // path of the DLL to register the component.
        g_hInst = hModule;
		if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0))
		{
			return -1;
		}
		 AfxEnableControlContainer(); // 加上这句才可以在该dll中创建ocx控件
        DisableThreadLibraryCalls(hModule);
        break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


//
//   FUNCTION: DllGetClassObject
//
//   PURPOSE: Create the class factory and query to the specific interface.
//
//   PARAMETERS:
//   * rclsid - The CLSID that will associate the correct data and code.
//   * riid - A reference to the identifier of the interface that the caller 
//     is to use to communicate with the class object.
//   * ppv - The address of a pointer variable that receives the interface 
//     pointer requested in riid. Upon successful return, *ppv contains the 
//     requested interface pointer. If an error occurs, the interface pointer 
//     is NULL. 
//
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

    if (IsEqualCLSID(CLSID_WsPreviewHandler, rclsid))
    {
        hr = E_OUTOFMEMORY;

        ClassFactory *pClassFactory = new ClassFactory();
        if (pClassFactory)
        {
            hr = pClassFactory->QueryInterface(riid, ppv);
            pClassFactory->Release();
        }
    }

    return hr;
}


//
//   FUNCTION: DllCanUnloadNow
//
//   PURPOSE: Check if we can unload the component from the memory.
//
//   NOTE: The component can be unloaded from the memory when its reference 
//   count is zero (i.e. nobody is still using the component).
// 
STDAPI DllCanUnloadNow(void)
{
    return g_cDllRef > 0 ? S_FALSE : S_OK;
}


//
//   FUNCTION: DllRegisterServer
//
//   PURPOSE: Register the COM server and the context menu handler.
// 
STDAPI DllRegisterServer(void)
{
    HRESULT hr;

    wchar_t szModule[MAX_PATH];
    if (GetModuleFileName(g_hInst, szModule, ARRAYSIZE(szModule)) == 0)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

    // Register the component.
    hr = RegisterInprocServer(szModule, CLSID_WsPreviewHandler, 
        L"PeraWsPreviewHandler.WsPreviewHandler Class", 
        L"Apartment", APPID_WsPreviewHandler);
    if (SUCCEEDED(hr))
    {
        // Register the preview handler. The preview handler is associated
        // with the .recipe file class.
        hr = RegisterShellExtPreviewHandler(L".ws", CLSID_WsPreviewHandler, 
            L"WsPreviewHandler");
    }

    return hr;
}


//
//   FUNCTION: DllUnregisterServer
//
//   PURPOSE: Unregister the COM server and the context menu handler.
// 
STDAPI DllUnregisterServer(void)
{
    HRESULT hr = S_OK;

    wchar_t szModule[MAX_PATH];
    if (GetModuleFileName(g_hInst, szModule, ARRAYSIZE(szModule)) == 0)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

    // Unregister the component.
    hr = UnregisterInprocServer(CLSID_WsPreviewHandler, 
        APPID_WsPreviewHandler);
    if (SUCCEEDED(hr))
    {
        // Unregister the context menu handler.
        hr = UnregisterShellExtPreviewHandler(L".ws", 
            CLSID_WsPreviewHandler);
    }

    return hr;
}