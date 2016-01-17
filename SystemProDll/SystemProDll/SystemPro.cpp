#include "stdafx.h"
#include"SystemPro.h"

void AdjustPrivileges(bool bSlient )
{
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
	{
		if(!bSlient)
			AfxMessageBox(_T("OpenProcessToken Error."));
	}
	//获取LUID
	TOKEN_PRIVILEGES tkp;
	LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// 调整令牌权限
	AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);
	if (GetLastError()!=ERROR_SUCCESS)
	{
		if(!bSlient)
			AfxMessageBox(_T("AdjustTokenPrivileges Error."));
	}
}
int ReStartSystem(bool bSlient )
{
	if(!bSlient)
	{
		if( IDNO== MessageBox(NULL,_T("是否重启系统？"),_T("提示"),MB_YESNO) )
			return 0;
	}

	AdjustPrivileges(bSlient);

	if (!ExitWindowsEx(EWX_REBOOT,0))
	{
		AfxMessageBox(_T("ExitWindowEx Error."));
		return -1;
	}

	return 0;
}
int ShutDownSystem(bool bSlient )
{
	if(!bSlient)
	{
		if( IDNO== MessageBox(NULL,_T("是否关闭系统？"),_T("提示"),MB_YESNO) )
			return 0;
	}

	AdjustPrivileges(bSlient);

	if (!ExitWindowsEx(EWX_SHUTDOWN,0))
	{
		AfxMessageBox(_T("ExitWindowEx Error."));
		return -1;
	}
	return 0;
}
int SystemLogOff(bool bSlient)
{
	if(!bSlient)
	{
		if( IDNO== MessageBox(NULL,_T("是否注销系统？"),_T("提示"),MB_YESNO) )
			return 0;
	}

	AdjustPrivileges(bSlient);

	if (!ExitWindowsEx(EWX_LOGOFF,0))
	{
		AfxMessageBox(_T("ExitWindowEx Error."));
		return -1;
	}
	return 0;
}