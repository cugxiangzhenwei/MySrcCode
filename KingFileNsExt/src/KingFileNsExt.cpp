// SimpleNsExt.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f SimpleNsExtps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "KingFileNsExt.h"

#include "KingFileNsExt_i.c"
#include "ShellFolderImpl.h"
#include "ShellViewImpl.h"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_ShellFolderImpl, CShellFolderImpl)
OBJECT_ENTRY_NON_CREATEABLE(CShellViewImpl)
END_OBJECT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Globals

ATLControls::CImageList g_ImglistSmall, g_ImglistLarge;


/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
        {
        _Module.Init(ObjectMap, hInstance, &LIBID_SIMPLENSEXTLib);
        DisableThreadLibraryCalls(hInstance);

        // Load imagelists.
        g_ImglistSmall.Create ( IDB_IMGLIST_SMALL, 16, 1, RGB(255,0,0) );
        g_ImglistLarge.Create ( IDB_IMGLIST_LARGE, 32, 1, RGB(255,0,0) );
        }
    else if (dwReason == DLL_PROCESS_DETACH)
        {
        // Destroy imagelists.
        g_ImglistSmall.Destroy();
        g_ImglistLarge.Destroy();

        _Module.Term();
        }
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}

