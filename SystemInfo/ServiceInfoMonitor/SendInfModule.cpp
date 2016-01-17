#include <Windows.h>
#include <WinIoCtl.h>
#include <WinSock.h>
#include <iptypes.h>
#include <stdio.h>
#include "../commonheader/SysInfoClass.h"

//For[2]获取DNS的IP
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib,"Ws2_32.lib")

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES			3
#define MALLOC(x)			HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x)				HeapFree(GetProcessHeap(), 0, (x))


//获取机器信息
bool CollectHostInf(CClientInf *pCClientInf)
{
	DWORD dwSize;
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)	return false;

	//[3]获取系统信息
	HKEY  hKey;
	LONG  ReturnValue;
	DWORD type;
	WCHAR strBuf[100];
	//打开注册表
	ReturnValue = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		0,
		KEY_ALL_ACCESS,
		&hKey);
	if(ReturnValue != ERROR_SUCCESS)	return false;
	//获取操作系统名称
	LONG lReturn = RegQueryValueExW(hKey, L"ProductName", NULL, \
		&type, (BYTE *)strBuf, &dwSize);
	if(lReturn != ERROR_SUCCESS)		return false;
	wcscpy(pCClientInf->wsSystemInf, strBuf);
	//获取CSDVersion
	dwSize = 100;
	lReturn = RegQueryValueExW(hKey, L"CSDVersion", NULL,
		&type, (BYTE *)strBuf, &dwSize);
	if(lReturn != ERROR_SUCCESS)		return false;
	wcscat(pCClientInf->wsSystemInf, L"_");
	wcscat(pCClientInf->wsSystemInf, strBuf);
	//获取CurrentVersion
	dwSize = 100;
	lReturn = RegQueryValueExW(hKey, L"CurrentVersion", NULL,
		&type, (BYTE *)strBuf, &dwSize);
	if(lReturn != ERROR_SUCCESS)		return false;
	wcscat(pCClientInf->wsSystemInf, L"_");
	wcscat(pCClientInf->wsSystemInf, strBuf);

	//[1]获取主机名、IP
	char host_name[255];
	if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR) {
		return false;
	}
	struct hostent *phe = gethostbyname(host_name);
	if (phe == 0) {
		return false;
	}
	struct in_addr addr;
	memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
	strcpy(pCClientInf->hostName, host_name);
	pCClientInf->nClientIP = addr.S_un.S_addr;
	

	//[5]获取CPU主频
	DWORD dwMHz;
	lReturn = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
						   "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
						   0,
						   KEY_READ,
						   &hKey);
	if(lReturn != ERROR_SUCCESS)		return false;
	lReturn = RegQueryValueExW(hKey, L"~MHz", NULL, NULL, (LPBYTE)&dwMHz, &dwSize);
	if(lReturn != ERROR_SUCCESS)		return false;
	pCClientInf->nCPUFrequency = dwMHz;
	RegCloseKey(hKey);

	//[6]获取内存大小
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	pCClientInf->nMemorySize =  (ULONG)((statex.ullTotalPhys)/(1024*1024));  //总内存大小，换算成MB
	pCClientInf->nFreeMemSize = (ULONG)((statex.ullAvailPhys)/(1024*1024));  // 剩余内存大小
	pCClientInf->nUsedMemSize = (ULONG)((statex.ullTotalPhys - statex.ullAvailPhys)/(1024*1024));  // 已使用内存大小
	pCClientInf->nTotalVirtualMemSize = (ULONG)((statex.ullTotalVirtual)/(1024*1024));  // 虚拟内存大小
	pCClientInf->nVirtualFreeMemSize = (ULONG)((statex.ullAvailVirtual)/(1024*1024));  // 可用虚拟内存大小

	//[7]获取硬盘大小
	HANDLE hDevice;
	hDevice = CreateFile("\\\\.\\PhysicalDrive0", 
						 GENERIC_READ | GENERIC_WRITE, 
						 FILE_SHARE_READ | FILE_SHARE_WRITE, 
						 NULL, OPEN_EXISTING,
						 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	GET_LENGTH_INFORMATION nDiskSizeByBytes;
	DWORD dwReturnSize;
	DeviceIoControl(hDevice,
					IOCTL_DISK_GET_LENGTH_INFO,
					NULL,
					0,
					(LPVOID)&nDiskSizeByBytes,
					sizeof(GET_LENGTH_INFORMATION),
					&dwReturnSize,
					NULL);
	pCClientInf->nDiskSize = (ULONG)(nDiskSizeByBytes.Length.QuadPart/(1000000000));
	CloseHandle(hDevice);

	WSACleanup();
	return true;
}