#include "StdAfx.h"
#include "TaskSerialization.h"
#include "CodeFunc.h"
#include "LogTools.h"
#include <iostream>

#define  MB_TO_UTF8(strMB)    CodeTools::MB_TO_UTF8(string(strMB)).c_str()
#define  UTF8_TO_MB(strUTF8)  CodeTools::UTF8_TO_MB(string(strUTF8)).c_str()

CTaskSerialization   SerializationManager;

CTaskSerialization::CTaskSerialization()
{
	CString strExePath;
	GetModuleFileName(NULL,strExePath.GetBuffer(MAX_PATH),MAX_PATH);
	strExePath.ReleaseBuffer();
	strExePath = strExePath.Left(strExePath.ReverseFind('\\')+1);
	m_strXmlPath = strExePath + "NetDiskUI_TaskList.xml";
	InitializeCriticalSection(&m_csConfigXml);
	m_bXmlLoaded = FALSE;
	m_pszXmlData = NULL;
}
void CTaskSerialization::InitConfigXml()
{
	try
	{
		if (PathFileExists(m_strXmlPath))
		{
			if (m_bXmlLoaded == FALSE)
			{
				file<> fdoc((LPCTSTR)m_strXmlPath);
				m_pszXmlData = _strdup(fdoc.data());
				m_Xmldoc.parse<0>(m_pszXmlData);
				m_bXmlLoaded = TRUE; // xml 文件只加载一次，后续都在内存操作
			}
		}
		else
		{
			xml_node<>* rot = m_Xmldoc.allocate_node(rapidxml::node_pi,m_Xmldoc.allocate_string( "xml version='1.0' encoding='UTF-8'" ));  
			m_Xmldoc.append_node(rot); 
			m_bXmlLoaded = TRUE;
		} 
	}
	catch (...)
	{
		LogMsg("初始化XML历史记录文件出现异常");
		m_bXmlLoaded = FALSE;
	}
}

CTaskSerialization::~CTaskSerialization(void)
{
	DeleteCriticalSection(&m_csConfigXml);
}
void CTaskSerialization::AddUploadTaskHistory(const char * pszUserId,const CFileInfo & fInfo)
{
	EnterCriticalSection(&m_csConfigXml);
	xml_node<>* pUploadTasks = NULL;
	map_tasks_type::iterator iter = m_mapUsrTasks.find(pszUserId);
	if (iter==m_mapUsrTasks.end())
	{
		xml_node<>* pNodeUsrId = m_Xmldoc.allocate_node(node_element,pszUserId);    
		pUploadTasks =   m_Xmldoc.allocate_node(node_element,"Upload");    
		m_Xmldoc.append_node(pNodeUsrId);  
		pNodeUsrId->append_node(pUploadTasks); 
		m_mapUsrTasks.insert(make_pair(pszUserId,pUploadTasks));
	}
	else
	{
		pUploadTasks = iter->second;
	}

	//创建节点
	xml_node<>* pNodeUploadTask = m_Xmldoc.allocate_node(node_element,"Task"); 
	rapidxml::xml_attribute<char> * attr = NULL;

	// 增加属性
	attr = m_Xmldoc.allocate_attribute("LocalFile",m_Xmldoc.allocate_string( MB_TO_UTF8(fInfo.m_strLocalPath)));
	pNodeUploadTask->append_attribute(attr);
	attr = m_Xmldoc.allocate_attribute("NetDiskPath",m_Xmldoc.allocate_string(MB_TO_UTF8(fInfo.m_strNetDiskPath)));
	pNodeUploadTask->append_attribute(attr);
	pUploadTasks->append_node(pNodeUploadTask); // 增加节点
	LeaveCriticalSection(&m_csConfigXml);
}
BOOL CTaskSerialization::DeleteUploadTaskHistory(const char * pszUserId,const CFileInfo & fInfo)
{
	
	if (m_bXmlLoaded == FALSE)
		return FALSE;

	EnterCriticalSection(&m_csConfigXml);
	xml_node<>* pUploadTasks = NULL;
	map_tasks_type::iterator iter = m_mapUsrTasks.find(pszUserId);
	if (iter==m_mapUsrTasks.end())
	{
		LeaveCriticalSection(&m_csConfigXml);
		return FALSE;
	}

	pUploadTasks = iter->second;
	ASSERT(pUploadTasks!=NULL);

	xml_node<>* pNodeTask = pUploadTasks->first_node("Task"); 
	rapidxml::xml_attribute<char> * attr = NULL;
	while(pNodeTask)
	{
		attr = pNodeTask->first_attribute("NetDiskPath");
		ASSERT(attr);

		CString strPathThis = UTF8_TO_MB(attr->value());
		if (strPathThis.CompareNoCase(fInfo.m_strNetDiskPath)==0)
			break;

		pNodeTask = pNodeTask->next_sibling();
	}
	if (pNodeTask==NULL)
	{
		LeaveCriticalSection(&m_csConfigXml);
		return FALSE;
	}

	pUploadTasks->remove_node(pNodeTask);
	LeaveCriticalSection(&m_csConfigXml);
	return TRUE;
}
BOOL CTaskSerialization::LoadNotFinishedTask(const char * pszUserId,vector<CFileInfo> & vList)
{
	vList.clear();
	if (m_bXmlLoaded == FALSE)
		return FALSE;

	EnterCriticalSection(&m_csConfigXml);
	xml_node<>* pUploadTasks = NULL;
	map_tasks_type::iterator iter = m_mapUsrTasks.find(pszUserId);
	if (iter==m_mapUsrTasks.end())
	{
		xml_node<>* pNodeUsrId = m_Xmldoc.first_node(pszUserId);
		if (pNodeUsrId==NULL) 
		{
			LeaveCriticalSection(&m_csConfigXml);
			return FALSE;
		}
		pUploadTasks = pNodeUsrId->first_node("Upload"); 
		if (pUploadTasks==NULL)
		{
			LeaveCriticalSection(&m_csConfigXml);
			return FALSE;
		}
		m_mapUsrTasks.insert(make_pair(pszUserId,pUploadTasks));
	}
	else
	{
		pUploadTasks = iter->second;
	}

	ASSERT(pUploadTasks!=NULL);

	xml_node<>* pNodeTask = pUploadTasks->first_node("Task"); 
	rapidxml::xml_attribute<char> * attr = NULL;
	try
	{
		while(pNodeTask)
		{
			CFileInfo fInfo;
			fInfo.m_strLocalPath = UTF8_TO_MB(pNodeTask->first_attribute("LocalFile")->value());
			fInfo.m_strNetDiskPath = UTF8_TO_MB(pNodeTask->first_attribute("NetDiskPath")->value());
			vList.push_back(fInfo);
			pNodeTask = pNodeTask->next_sibling();
		}
	}
	catch (...)
	{
		vList.clear();
		LogMsg("RapidXML ----LoadAllLocalTool加载全部配置记录出现异常....");
		LeaveCriticalSection(&m_csConfigXml);
		return FALSE;
	}

	LeaveCriticalSection(&m_csConfigXml);
	return TRUE;
}
void CTaskSerialization::SaveXmlFile()
{
	EnterCriticalSection(&m_csConfigXml);
	std::ofstream out(m_strXmlPath);  
	out<<m_Xmldoc; // 保存xml
	out.close();
	if (m_pszXmlData)
	{
		free(m_pszXmlData);
		m_pszXmlData = NULL;
	}
	LeaveCriticalSection(&m_csConfigXml);
}