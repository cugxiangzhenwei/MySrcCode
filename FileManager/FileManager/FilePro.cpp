#include "stdafx.h"
#include "FilePro.h"
#include "SqliteBase.h"
#include <windows.h>
#include <winioctl.h>
#include "FileManager.h"
static const char * g_pszFilterList[] = 
{
	"windows",
	"MSOCache",
	"ProgramData"
};
namespace FileProTool
{
	CSqliteBase * g_pDataBase = NULL;
	vector<CString> m_gFileList;
	CRITICAL_SECTION g_cs;

	BOOL  GetAllDisk(CStringArray & diskList,BOOL bIncludeRemote,BOOL bInCludeCD,BOOL bIncludeUSB,BOOL bIncludeRamDisk)
	{
		char  szDrives[512];
		char* pDrive;

		if ( !GetLogicalDriveStrings( sizeof(szDrives), szDrives ) )
			return FALSE;

		pDrive = szDrives;
		while( *pDrive )
		{
			CString strDrive = pDrive;
			UINT iType =  GetDriveType(strDrive); //DRIVE_FIXED 硬盘
			if(iType== DRIVE_CDROM && !bInCludeCD)// 光驱
			{
				pDrive += strlen( pDrive ) + 1;
				continue;
			}
			else if (iType== DRIVE_REMOVABLE && !bIncludeUSB) //可移动式磁盘
			{
				pDrive += strlen( pDrive ) + 1;
				continue;
			}
			else if (iType== DRIVE_REMOTE && !bIncludeRemote) //网络磁盘
			{
				pDrive += strlen( pDrive ) + 1;
				continue;
			}
			else if (iType== DRIVE_RAMDISK && !bIncludeRemote) //虚拟RAM磁盘
			{
				pDrive += strlen( pDrive ) + 1;
				continue;
			}
			else if (iType== DRIVE_UNKNOWN) // 未知设备
			{
				pDrive += strlen( pDrive ) + 1;
				continue;
			}

			diskList.Add(strDrive);
			pDrive += strlen( pDrive ) + 1;
		}
		return TRUE;
	}
	BOOL GetAllItems(const CString & strPath,const BOOL & bExitScan)
	{
		CString strWildCard  = strPath;
		strWildCard +="\\*.*";
		CFileFind finder;
		BOOL bFind = FALSE;
		// 查找文件
		BOOL bErrOccured = FALSE;
		bFind = finder.FindFile(strWildCard);
		while(bFind)
		{
			if (bExitScan)
				break;

			// 查找下一个文件
			bFind = finder.FindNextFile();
			if (finder.IsDots())
				continue;
			//找到文件的路径

			CString strPathName = finder.GetFilePath();
			CString strName = finder.GetFileName();
			int iCount = sizeof(g_pszFilterList)/sizeof(const char *);
			BOOL bContinue = FALSE;
			for (int i=0;i<iCount;i++)
			{
				if (strName.CompareNoCase(g_pszFilterList[i])==0)
				{
					bContinue = TRUE;
					break;
				}
			}
			if (bContinue)
				continue;

			if (finder.IsDirectory())
			{
				EnterCriticalSection(&g_cs);
				m_gFileList.push_back(strPathName);
				LeaveCriticalSection(&g_cs);
				GetAllItems(strPathName,bExitScan);
			}
			else
			{
				EnterCriticalSection(&g_cs);
				m_gFileList.push_back(strPathName);
				LeaveCriticalSection(&g_cs);
			}
		}
		finder.Close();
		return TRUE;
	}
	BOOL GetAllItemsUseUSN(CString strRootDir,vector<CString> & m_vStrItems,const BOOL & bExitScan)
	{
		if (strRootDir.Right(1)!="\\")
			strRootDir.AppendChar('\\');

		char sysNameBuf[MAX_PATH] = {0};
		if (!GetVolumeInformation(strRootDir,NULL,0,NULL,NULL,NULL,sysNameBuf,MAX_PATH))
		{
			LogMsg("GetVolumeInformation失败！\n",strRootDir);
			return FALSE;
		}
		if(strcmp(sysNameBuf, "NTFS")!=0)
		{
			LogMsg("该驱动盘非NTFS格式,采用FindFile进行查找\n",strRootDir);
			return GetAllItems(strRootDir,bExitScan);
		}

		// 传入的文件名必须为\\.\C:的形式
		char fileName[MAX_PATH];
		strcpy_s(fileName, "\\\\.\\");
		strcat_s(fileName, CString(strRootDir[0]));
		strcat_s(fileName,":");
		LogMsg("驱动盘地址: %s\n", fileName);

		// 调用该函数需要管理员权限
		HANDLE hVol = CreateFileA(fileName,
			GENERIC_READ | GENERIC_WRITE, // 可以为0
			FILE_SHARE_READ | FILE_SHARE_WRITE, // 必须包含有FILE_SHARE_WRITE
			NULL, // 这里不需要
			OPEN_EXISTING, // 必须包含OPEN_EXISTING, CREATE_ALWAYS可能会导致错误
			FILE_ATTRIBUTE_READONLY, // FILE_ATTRIBUTE_NORMAL可能会导致错误
			NULL); // 这里不需要

		if(INVALID_HANDLE_VALUE ==hVol || hVol == NULL)
		{
			LogMsg("获取驱动盘句柄失败 —— handle:%x error:%d\n", hVol, GetLastError());
			return FALSE;
		}

		/**
		* step 03. 初始化USN日志文件
		*/
		DWORD br;
		CREATE_USN_JOURNAL_DATA cujd;
		cujd.MaximumSize = 0; // 0表示使用默认值
		cujd.AllocationDelta = 0; // 0表示使用默认值
		if (!DeviceIoControl(hVol,FSCTL_CREATE_USN_JOURNAL,&cujd,sizeof(cujd),NULL,0,&br,NULL))
		{
			LogMsg("初始化USN日志文件失败 —— error:%d\n",GetLastError());
			CloseHandle(hVol);
			return FALSE;
		}

		/**
		* step 04. 获取USN日志基本信息(用于后续操作)
		*/
		USN_JOURNAL_DATA UsnInfo; // 用于储存USN日志的基本信息
		if (!DeviceIoControl(hVol,FSCTL_QUERY_USN_JOURNAL,NULL,
			0,&UsnInfo,sizeof(USN_JOURNAL_DATA),&br,NULL))
		{
			LogMsg("获取USN日志基本信息失败 —— error:%d\n",GetLastError());
			CloseHandle(hVol);
			return FALSE;
		}

		LogMsg("UsnJournalID: %lld\n", UsnInfo.UsnJournalID);
		LogMsg("lowUsn: %lld\n", UsnInfo.FirstUsn);
		LogMsg("highUsn: %lld\n", UsnInfo.NextUsn);

		/**
		* step 05. 枚举USN日志文件中的所有记录
		*/
		MFT_ENUM_DATA med;
		med.StartFileReferenceNumber = 0;
		med.LowUsn = 0;//UsnInfo.FirstUsn; 这里经测试发现，如果用FirstUsn有时候不正确，导致获取到不完整的数据，还是直接写0好.
		med.HighUsn = UsnInfo.NextUsn;

		CHAR buffer[4096]; // 用于储存记录的缓冲,尽量足够地大
		DWORD usnDataSize;
		PUSN_RECORD UsnRecord;

		map<DWORDLONG,USN_FIEL_ITEM> mapUSN;
		USN_FIEL_ITEM rootUSN;
		rootUSN.pfn = 0;
		rootUSN.fn = 1407374883553285;
		rootUSN.strFullPath = strRootDir.Left(2);
		mapUSN.insert(make_pair(1407374883553285,rootUSN));

		while(0!=DeviceIoControl(hVol, FSCTL_ENUM_USN_DATA, &med,sizeof(med),
			buffer, 4096, &usnDataSize, NULL))
		{
			if (bExitScan)
				break;

			DWORD dwRetBytes = usnDataSize - sizeof(USN);

			// 找到第一个USN记录
			// return a USN followed by zero or more change journal records, each in a USN_RECORD structure. 
			UsnRecord = (PUSN_RECORD)(((PCHAR)buffer)+sizeof(USN));

			while(dwRetBytes>0)
			{
				if (bExitScan)
					break;

				// 打印获取到的信息
				const int strLen = UsnRecord->FileNameLength;
				int iMBSize = WideCharToMultiByte(CP_OEMCP,NULL,UsnRecord->FileName,strLen/2,NULL,0,NULL,FALSE);
				char * fileName = new char[iMBSize + 1];
				memset(fileName, 0, sizeof(char)*(iMBSize + 1));
				WideCharToMultiByte(CP_OEMCP,NULL,UsnRecord->FileName,strLen/2,fileName,iMBSize,NULL,FALSE);

				USN_FIEL_ITEM usnFile;
				usnFile.strName = fileName;
				usnFile.fn = UsnRecord->FileReferenceNumber;
				usnFile.pfn = UsnRecord->ParentFileReferenceNumber;
				mapUSN.insert(make_pair(UsnRecord->FileReferenceNumber,usnFile));
				delete []fileName;
				// 获取下一个记录
				DWORD recordLen = UsnRecord->RecordLength;
				dwRetBytes -= recordLen;
				UsnRecord = (PUSN_RECORD)(((PCHAR)UsnRecord)+recordLen);
			}

			//获取下一页数据，MTF大概是分多页来储存的吧？
			// The USN returned as the first item in the output buffer is the USN of the next record number to be retrieved. 
			// Use this value to continue reading records from the end boundary forward.
			med.StartFileReferenceNumber = *(USN *)&buffer;
		}

		
			/**
	     * step 06. 删除USN日志文件(当然也可以不删除)
	     * msdn:http://msdn.microsoft.com/en-us/library/aa364561%28v=VS.85%29.aspx
	     */
		DELETE_USN_JOURNAL_DATA dujd;
		dujd.UsnJournalID = UsnInfo.UsnJournalID;
		dujd.DeleteFlags = USN_DELETE_FLAG_DELETE;
		if (!DeviceIoControl(hVol,FSCTL_DELETE_USN_JOURNAL,&dujd,sizeof(dujd),NULL,0,&br,NULL))
			LogMsg("删除USN文件失败!");

		CloseHandle(hVol);

		BOOL bOK = TRUE;
		map<DWORDLONG,USN_FIEL_ITEM>::iterator iter = mapUSN.begin();
		int iIndex = 0;
		for (iter; iter!=mapUSN.end(); iter++)
		{
			if (bExitScan)
				break;

			CString str = GetItemPath(iter,mapUSN);
			if (str.GetLength()<2|| str[1]!= ':')
				continue; // 废弃的项
			
			m_vStrItems.push_back(str);
		}

		return TRUE;
	}

	CString GetItemPath(MAP_USN_TYPE::iterator iter,MAP_USN_TYPE & mapUSN)
	{
		CString strPath = iter->second.strName;
		if (!iter->second.strFullPath.IsEmpty())
			return iter->second.strFullPath;

		MAP_USN_TYPE::iterator iterParent = mapUSN.find(iter->second.pfn);
		if(iterParent== mapUSN.end())
			return "";

		strPath = GetItemPath(iterParent,mapUSN) + "\\" + strPath;
		iter->second.strFullPath = strPath;
		return strPath;
	}
	BOOL GetAllItemsUseUSN(const CString & strPath,const BOOL & bExitScan)
	{
		vector<CString> vFiles;
		BOOL bOK = GetAllItemsUseUSN(strPath,vFiles,bExitScan);
		if (!vFiles.empty())
		{
			LogMsg("%c盘扫描完成,共【%ld】条记录,正在插入数据库...\n",strPath[0],vFiles.size());
			CString *pMsgStr = new CString;
			pMsgStr->Format("%c:盘扫描完成!",strPath[0]);
			theApp.m_pMainWnd->PostMessage(WM_SCAN_RESULT,vFiles.size(),(LPARAM)pMsgStr);
			EnterCriticalSection(&g_cs);
			m_gFileList.assign(vFiles.begin(),vFiles.end());
			LeaveCriticalSection(&g_cs);
			vFiles.clear();
		}
		return bOK;
	}
};
