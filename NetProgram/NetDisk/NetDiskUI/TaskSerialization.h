#pragma once
#include "NetDiskOperation.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include <map>
using namespace rapidxml;
class CTaskSerialization
{
public:
	typedef map<CString,xml_node<>*> map_tasks_type;

	CTaskSerialization();
	~CTaskSerialization(void);
	void InitConfigXml();
	void AddUploadTaskHistory(const char * pszUserId,const CFileInfo & fInfo);
	BOOL DeleteUploadTaskHistory(const char * pszUserId,const CFileInfo & fInfo);
	BOOL LoadNotFinishedTask(const char * pszUserId,vector<CFileInfo> & vList);
	void SaveXmlFile();
private:
	CRITICAL_SECTION m_csConfigXml;
	xml_document<>   m_Xmldoc; 
	char *           m_pszXmlData;
	CString          m_strXmlPath;
	BOOL             m_bXmlLoaded;
	map_tasks_type m_mapUsrTasks;
};

extern CTaskSerialization   SerializationManager;