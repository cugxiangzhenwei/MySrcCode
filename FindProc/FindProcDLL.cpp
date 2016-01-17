#include "stdafx.h"
#include "FindPro.h"

using namespace std;

HINSTANCE g_hInstance;
HWND g_hwndParent;

BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
    g_hInstance=(struct HINSTANCE__ *)hInst;
    return TRUE;
}

int FindProcByName(const char *szToFind)
// Created: 12/29/2000  (RK)
// Last modified: 6/16/2003  (RK)
// Please report any problems or bugs to kochhar@physiology.wisc.edu
// The latest version of this routine can be found at:
//     http://www.neurophys.wisc.edu/ravi/software/killproc/
// Check whether the process "szToFind" is currently running in memory
// This works for Win/95/98/ME and also Win/NT/2000/XP
// The process name is case-insensitive, i.e. "notepad.exe" and "NOTEPAD.EXE"
// will both work (for szToFind)
// Return codes are as follows:
//   0   = Process was not found
//   1   = Process was found
//   605 = Unable to search for process
//   606 = Unable to identify system type
//   607 = Unsupported OS
//   632 = Process name is invalid
// Change history:
//  3/10/2002   - Fixed memory leak in some cases (hSnapShot and
//                and hSnapShotm were not being closed sometimes)
//  6/13/2003   - Removed iFound (was not being used, as pointed out
//                by John Emmas)
{
    BOOL bResult,bResultm;
    DWORD aiPID[1000],iCb=1000,iNumProc,iV2000=0;
    DWORD iCbneeded,i;
    char szName[MAX_PATH],szToTermUpper[MAX_PATH];
    HANDLE hProc,hSnapShot,hSnapShotm;
    OSVERSIONINFO osvi;
    HINSTANCE hInstLib;
    int iLen,iLenP,indx;
    HMODULE hMod;
    PROCESSENTRY32 procentry;
    MODULEENTRY32 modentry;

     // PSAPI Function Pointers.
     BOOL (WINAPI *lpfEnumProcesses)( DWORD *, DWORD cb, DWORD * );
     BOOL (WINAPI *lpfEnumProcessModules)( HANDLE, HMODULE *,
        DWORD, LPDWORD );
     DWORD (WINAPI *lpfGetModuleBaseName)( HANDLE, HMODULE,
        LPTSTR, DWORD );

      // ToolHelp Function Pointers.
      HANDLE (WINAPI *lpfCreateToolhelp32Snapshot)(DWORD,DWORD) ;
      BOOL (WINAPI *lpfProcess32First)(HANDLE,LPPROCESSENTRY32) ;
      BOOL (WINAPI *lpfProcess32Next)(HANDLE,LPPROCESSENTRY32) ;
      BOOL (WINAPI *lpfModule32First)(HANDLE,LPMODULEENTRY32) ;
      BOOL (WINAPI *lpfModule32Next)(HANDLE,LPMODULEENTRY32) ;

    // Transfer Process name into "szToTermUpper" and
    // convert it to upper case
    iLenP=strlen(szToFind);
    if(iLenP<1 || iLenP>MAX_PATH) return 632;
    for(indx=0;indx<iLenP;indx++)
        szToTermUpper[indx]=toupper(szToFind[indx]);
    szToTermUpper[iLenP]=0;

    // First check what version of Windows we're in
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    bResult=GetVersionEx(&osvi);
    if(!bResult)     // Unable to identify system version
        return 606;

    // At Present we only support Win/NT/2000/XP or Win/9x/ME
    if((osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) &&
        (osvi.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS))
        return 607;

    if(osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
    {
        // Win/NT or 2000 or XP

         // Load library and get the procedures explicitly. We do
         // this so that we don't have to worry about modules using
         // this code failing to load under Windows 9x, because
         // it can't resolve references to the PSAPI.DLL.
         hInstLib = LoadLibraryA("PSAPI.DLL");
         if(hInstLib == NULL)
            return 605;

         // Get procedure addresses.
         lpfEnumProcesses = (BOOL(WINAPI *)(DWORD *,DWORD,DWORD*))
            GetProcAddress( hInstLib, "EnumProcesses" ) ;
         lpfEnumProcessModules = (BOOL(WINAPI *)(HANDLE, HMODULE *,
            DWORD, LPDWORD)) GetProcAddress( hInstLib,
            "EnumProcessModules" ) ;
         lpfGetModuleBaseName =(DWORD (WINAPI *)(HANDLE, HMODULE,
            LPTSTR, DWORD )) GetProcAddress( hInstLib,
            "GetModuleBaseNameA" ) ;

         if(lpfEnumProcesses == NULL ||
            lpfEnumProcessModules == NULL ||
            lpfGetModuleBaseName == NULL)
            {
               FreeLibrary(hInstLib);
               return 605;
            }

        bResult=lpfEnumProcesses(aiPID,iCb,&iCbneeded);
        if(!bResult)
        {
            // Unable to get process list, EnumProcesses failed
            FreeLibrary(hInstLib);
            return 605;
        }

        // How many processes are there?
        iNumProc=iCbneeded/sizeof(DWORD);

        // Get and match the name of each process
        for(i=0;i<iNumProc;i++)
        {
            // Get the (module) name for this process

            strcpy(szName,"Unknown");
            // First, get a handle to the process
            hProc=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,
                aiPID[i]);
            // Now, get the process name
            if(hProc)
            {
               if(lpfEnumProcessModules(hProc,&hMod,sizeof(hMod),&iCbneeded) )
               {
                  iLen=lpfGetModuleBaseName(hProc,hMod,szName,MAX_PATH);
               }
            }
            CloseHandle(hProc);
            // We will match regardless of lower or upper case
#ifdef BORLANDC
            if(strcmp(strupr(szName),szToTermUpper)==0)
#else
            if(strcmp(_strupr(szName),szToTermUpper)==0)
#endif
            {
                // Process found
                FreeLibrary(hInstLib);
                return 1;
            }
        }
    }

    if(osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
    {
        // Win/95 or 98 or ME

        hInstLib = LoadLibraryA("Kernel32.DLL");
        if( hInstLib == NULL )
            return 605;

        // Get procedure addresses.
        // We are linking to these functions of Kernel32
        // explicitly, because otherwise a module using
        // this code would fail to load under Windows NT,
        // which does not have the Toolhelp32
        // functions in the Kernel 32.
        lpfCreateToolhelp32Snapshot=
            (HANDLE(WINAPI *)(DWORD,DWORD))
            GetProcAddress( hInstLib,
            "CreateToolhelp32Snapshot" ) ;
        lpfProcess32First=
            (BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))
            GetProcAddress( hInstLib, "Process32First" ) ;
        lpfProcess32Next=
            (BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))
            GetProcAddress( hInstLib, "Process32Next" ) ;
        lpfModule32First=
            (BOOL(WINAPI *)(HANDLE,LPMODULEENTRY32))
            GetProcAddress( hInstLib, "Module32First" ) ;
        lpfModule32Next=
            (BOOL(WINAPI *)(HANDLE,LPMODULEENTRY32))
            GetProcAddress( hInstLib, "Module32Next" ) ;
        if( lpfProcess32Next == NULL ||
            lpfProcess32First == NULL ||
            lpfModule32Next == NULL ||
            lpfModule32First == NULL ||
            lpfCreateToolhelp32Snapshot == NULL )
        {
            FreeLibrary(hInstLib);
            return 605;
        }

        // The Process32.. and Module32.. routines return names in all uppercase

        // Get a handle to a Toolhelp snapshot of all the systems processes.

        hSnapShot = lpfCreateToolhelp32Snapshot(
            TH32CS_SNAPPROCESS, 0 ) ;
        if( hSnapShot == INVALID_HANDLE_VALUE )
        {
            FreeLibrary(hInstLib);
            return 605;
        }

        // Get the first process' information.
        procentry.dwSize = sizeof(PROCESSENTRY32);
        bResult=lpfProcess32First(hSnapShot,&procentry);

        // While there are processes, keep looping and checking.
        while(bResult)
        {
            // Get a handle to a Toolhelp snapshot of this process.
            hSnapShotm = lpfCreateToolhelp32Snapshot(
                TH32CS_SNAPMODULE, procentry.th32ProcessID) ;
            if( hSnapShotm == INVALID_HANDLE_VALUE )
            {
                CloseHandle(hSnapShot);
                FreeLibrary(hInstLib);
                return 605;
            }
            // Get the module list for this process
            modentry.dwSize=sizeof(MODULEENTRY32);
            bResultm=lpfModule32First(hSnapShotm,&modentry);

            // While there are modules, keep looping and checking
            while(bResultm)
            {
                if(strcmp(modentry.szModule,szToTermUpper)==0)
                {
                    // Process found
					          CloseHandle(hSnapShotm);
					          CloseHandle(hSnapShot);
                    FreeLibrary(hInstLib);
                    return 1;
                }
                else
                {  // Look for next modules for this process
                    modentry.dwSize=sizeof(MODULEENTRY32);
                    bResultm=lpfModule32Next(hSnapShotm,&modentry);
                }
            }

            //Keep looking
            CloseHandle(hSnapShotm);
            procentry.dwSize = sizeof(PROCESSENTRY32);
            bResult = lpfProcess32Next(hSnapShot,&procentry);
        }
        CloseHandle(hSnapShot);
    }
    FreeLibrary(hInstLib);
    return 0;
}


//
// This is the only exported function, FindProc. It receives the name
// of a process through the NSIS stack. The return-value from the
// FindProcByName function is stored in the $R0 variable, so push
// it before calling FindProc function if you don't want to lose the
// data that R0 could contain.
//
// You can call this function in NSIS like this:
//
// FindProcDLL::FindProc "process_name.exe"
//
// example:
//    FindProcDLL::FindProc "msnmsgr.exe"
//  would return 1 in R0 if running
//  if it's not running, it would return 0.
//
//  ---------------------------      ----     ---    --   -

extern "C" __declspec(dllexport) void FindProc(HWND hwndParent, int string_size,
                                      char *variables, stack_t **stacktop)
{
    char parameter[200];
    int value;
    g_hwndParent=hwndParent;
  EXDLL_INIT();
  {
        popstring(parameter);
        value=FindProcByName(parameter);
        wsprintf(parameter,"%d",value);
        setuservariable(INST_R0, parameter);
    }
}