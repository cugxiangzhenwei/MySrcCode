#include "Record.h"

CUserRecord::CUserRecord(const char * pszUsrName,const char * pszAge)
{
	m_strTable = "user";
	m_AttributeMap.insert(make_pair("name",CAttirbuteValue(1,pszUsrName)));
	m_AttributeMap.insert(make_pair("age",CAttirbuteValue(0,pszAge)));
}
CUserRecord::CUserRecord(const MAP_ATTRIBUTES &AttMap)
{
	m_AttributeMap = AttMap;
	m_strTable = "user";
}

CUserRecord::~CUserRecord(void)
{
}

CFileRecord::CFileRecord(const char * pszFilePath)
{
	m_strTable = "file";
	string strFileName = pszFilePath;
	string strLocation = pszFilePath;
	strFileName = strFileName.substr(strFileName.rfind('\\')+1);
	strLocation = strLocation.substr(0,strLocation.rfind('\\')+1);

	m_AttributeMap.insert(make_pair("location",CAttirbuteValue(1,strLocation)));
	m_AttributeMap.insert(make_pair("filename",CAttirbuteValue(1,strFileName)));
}
CFileRecord::CFileRecord(const char * pszFolder,const char * pszFileNmae)
{
	m_strTable = "file";
	m_AttributeMap.insert(make_pair("location",CAttirbuteValue(1,pszFolder)));
	m_AttributeMap.insert(make_pair("filename",CAttirbuteValue(1,pszFileNmae)));
}