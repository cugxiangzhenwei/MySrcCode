#pragma once
#include <map>
#include <vector>
#include <string>
using namespace std;

typedef  int(*SelectResultFunc)(void *params,int column_size,char **column_value,char **column_name);

class CAttirbuteValue
{
public:
	CAttirbuteValue(int iType,string strValue)
	{
		m_iType = iType;
		m_strValue = strValue;
	}
protected:
	CAttirbuteValue()
	{
		m_iType = 0;
		m_strValue = "";
	}
public:
	int m_iType;
	string m_strValue;
};

typedef  map<string,CAttirbuteValue> MAP_ATTRIBUTES;

class CRecordBase
{
public:
	CRecordBase(){}
	virtual ~CRecordBase(){};
	virtual size_t GetColumSize() const
	{
		return m_AttributeMap.size();
	}
	virtual const map<string,CAttirbuteValue> & GetAttributeMap() const
	{
		return m_AttributeMap;
	}
	virtual const string & GetTableName() const
	{
		return m_strTable;
	}
protected:
	MAP_ATTRIBUTES m_AttributeMap;
	string m_strTable;
};

class CSqliteBase
{
protected:
	CSqliteBase(void);
public:
	~CSqliteBase(void);
	// 打开数据库
	static CSqliteBase * OpenDataBase(const char * pszDataBaseName);

	// 开启事务，在开始批量操作前调用
	bool StartTransaction();
	// 出错时回滚
	bool Rollback();
	// 批量操作
	bool EndTransaction(); 
	// 执行SQL命令
	bool ExcuteSqlCmd(const char * pszSQL);
	// 添加记录
	bool AddRecord(const CRecordBase &);
	// 删除记录
	bool DeleteRecord(const char * pszTable,const char * pszConditonName,const CAttirbuteValue & ConditonValue);
	// 更新记录
	bool ModifyRecord(const char * pszTable,const char * pszConditonName,const CAttirbuteValue & ConditonValue,const MAP_ATTRIBUTES & newValueMap);
	// 查询记录
	bool SelectRecord(const char * pszSQL,SelectResultFunc pSelResult = NULL,void *pFunData = NULL);
	// 不使用回调的查询
	bool SelectRecord(const char * pszSQL,int &nRows,int &nColums,char ** &pszDbReslut);
	// 释放查询结果
	void FreeSelectResult(char ** & pszDbReslut);
	// 新建表操作
	bool CreateTable(const char * pszTable,bool DropIfExist = true);
	// 判断表是否存在
	bool IsTableExist(const char * pszTable);
	struct sqlite3 * GetDB(){return m_pDB;}
	static bool loadOrSaveDb(CSqliteBase *pInMemeoryBase, const char *zFilename, bool isSave);
private:
	static bool loadOrSaveDb(sqlite3 * & pInMemeory, const char *zFilename, bool isSave);
private:
	struct sqlite3 * m_pDB; 
};

