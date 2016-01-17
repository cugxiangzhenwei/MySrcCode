/*
*根据进程ID杀掉进程
*/
BOOL KillProcess(DWORD nProcessID)
{
	HANDLE hProcessHandle; 
	hProcessHandle =::OpenProcess( PROCESS_TERMINATE,FALSE, nProcessID );
	if (hProcessHandle == NULL)
		return FALSE;

	return ::TerminateProcess( hProcessHandle, 4 );
}

/*
*根据进程显示的标题名称杀掉进程
*/
BOOL KillProcess(LPCSTR pszWindowTitle)
{
	HWND TheWindow;

	TheWindow = ::FindWindow( NULL, pszWindowTitle );
	if (TheWindow == NULL)
		return FALSE;

	DWORD nProcessID;
	::GetWindowThreadProcessId( TheWindow, &nProcessID );

	return KillProcess(nProcessID);
}