
//定义一个类，存放主机信息
class CClientInf	//记录信息
{
public:
	CHAR			hostName[255];	//主机名
	ULONG			nClientIP;
	USHORT			nClientPort;
	ULONG			nClientDNS;
	WCHAR			wsSystemInf[250];
	ULONG			nCPUFrequency;
	ULONG			nMemorySize;
	ULONG			nDiskSize;
	ULONG			nFreeMemSize;
	ULONG			nUsedMemSize;
	ULONG           nTotalVirtualMemSize;
	ULONG           nVirtualFreeMemSize;
};