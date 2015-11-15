#pragma once
#include "TaskBase.h"
class CSqliteBase;
class CFileAction;

class CWriteMonitorResult :
	public CTaskBase
{
public:
	CWriteMonitorResult(CSqliteBase * pDataBase,HWND hReceiveMsg);
	~CWriteMonitorResult(void);
	virtual BOOL Run();
	virtual CTaskBase * GetCopy();
	void AddRecord(CFileAction * pAction); // 增加文件
	void RemoveRecord(CFileAction * pAction); // 删除文件
	void ModifyRecord(CFileAction * pAction); // 重命名文件

private:
	CSqliteBase * m_pDataBase;
	HWND m_hReceiveMsg;
};

