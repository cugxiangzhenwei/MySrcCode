#pragma once
#include "sqlitebase.h"
class CUserRecord :public CRecordBase
{
public:
	CUserRecord(const char * pszUsrName,const char * pszAge);
	CUserRecord(const MAP_ATTRIBUTES &map);
	~CUserRecord(void);
};
class CFileRecord:public CRecordBase
{
public:
	CFileRecord(const char * pszFilePath);
	CFileRecord(const char * pszFolder,const char * pszFileNmae);
	~CFileRecord(void){}
};
