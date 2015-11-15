#pragma once
#include "TaskBase.h"
#include <vector>
using namespace std;
class CFileAction
{
public:
	typedef enum
	{
		AC_ADD		= 0,
		AC_DEL		= 1,
		AC_RENAME	= 2
	}ActionType;

	ActionType m_Type;
	CString m_strFile;
	CString m_strNewFile;

	CFileAction(ActionType iType,const CString & strFileName,const CString & strNewName = CString())
	{
		m_Type = iType;
		m_strFile = strFileName;
		m_strNewFile = strNewName;
	}
};
class CMonitorDiskTask :
	public CTaskBase
{
public:
	CMonitorDiskTask(CString strMonitorDir);
	~CMonitorDiskTask(void);
	virtual BOOL Run();
	virtual CTaskBase * GetCopy();
	virtual void SetExit();
public:
	CString m_strMonitorDir;
private:
	HANDLE m_handleDir;
};

extern vector<CFileAction *> g_MonotorFileActionList;
extern CRITICAL_SECTION      g_FileActionListCS;