#include "DataJsDefine.h"
#include <json/reader.h>
#include <json/writer.h>

CJsonDataGetFileList::~CJsonDataGetFileList()
{
	Clear();
}
void CJsonDataGetFileList::Clear()
{
	m_strUsrId.empty();
	m_strNetDiskDir.empty();
	m_bRecursion = false;
}
bool CJsonDataGetFileList::Parse(const char * pszInJs)
{
	Clear();
	string strSrc = pszInJs == NULL ? "" : pszInJs;
	if (strSrc.empty()) return false;

	Json::Reader reader;
	Json::Value obj;

	try
	{
		if (!reader.parse(strSrc, obj, false)) return false;
		if (!obj.isObject()) return false;

		m_strUsrId = obj["User"].isString() ? obj["User"].asString() : "";
		m_strNetDiskDir = obj["NetDiskPath"].isString() ? obj["NetDiskPath"].asString() : "";
		m_bRecursion = obj["Recursion"].isBool() ? obj["Recursion"].asBool() : false;

		return true;
	}
	catch(...)
	{
		Clear();
		return false;
	}
	return false;
}
string  CJsonDataGetFileList::GetResultJson()
{
	try
	{
		string strResult;
		Json::Value obj(Json::objectValue);
		obj["User"] = Json::Value(m_strUsrId.c_str());
		obj["NetDiskPath"] = Json::Value(m_strNetDiskDir.c_str());
		obj["Recursion"] = Json::Value(m_bRecursion);

		strResult = obj.toStyledString().c_str();
		return strResult;
	}
	catch (...)
	{
		return "";
	}
}

CJsonDataSendFileList::~CJsonDataSendFileList()
{
	Clear();
}
void CJsonDataSendFileList::Clear()
{
	m_strUsrId.empty();
	m_strNetDiskDir.empty();
	m_bRecursion = false;
	for (size_t i=0;i<m_vFileList.size();i++)
	{
		delete m_vFileList[i];
		m_vFileList[i] = NULL;
	}
	m_vFileList.clear();
}
void CJsonDataSendFileList::GetSendListCopied(vector<FileListItem *> &vListOut)
{
	vListOut.clear();
	for (size_t i=0;i<m_vFileList.size();i++)
	{
		FileListItem *pCopiedInfo = m_vFileList[i]->GetCopied();
		vListOut.push_back(pCopiedInfo);
	}
}
bool CJsonDataSendFileList::Parse(const char * pszInJs)
{
	Clear();
	string strSrc = pszInJs == NULL ? "" : pszInJs;
	if (strSrc.empty()) return false;

	Json::Reader reader;
	Json::Value obj;

	try
	{
		if (!reader.parse(strSrc, obj, false)) return false;
		if (!obj.isObject()) return false;

		m_strUsrId = obj["User"].isString() ? obj["User"].asString() : "";
		m_strNetDiskDir = obj["NetDiskPath"].isString() ? obj["NetDiskPath"].asString() : "";
		m_bRecursion = obj["Recursion"].isBool() ? obj["Recursion"].asBool() : false;

		Json::Value jListFile = obj["FileList"];
		Json::Value jListDir = obj["DirList"];
		if (!jListFile.isArray() || !jListDir.isArray())
		{
			Clear();
			return false;
		}

		for (size_t i=0;i< jListDir.size();i++)
		{
			FileListItem *pInfo = new FileListItem;
			string strDirName = jListDir[i].isString() ? jListDir[i].asString():"";
			strcpy_s(pInfo->szItemName,strDirName.c_str());
			m_vFileList.push_back(pInfo);
		}

		for (size_t i=0;i< jListFile.size();i++)
		{
			FileListItem *pInfo = new FileListItem;
			string strTmp = jListFile[i]["ItemName"].isString() ? jListFile[i]["ItemName"].asString():"";
			strcpy_s(pInfo->szItemName,strTmp.c_str());

			strTmp = jListFile[i]["ItemType"].isString() ? jListFile[i]["ItemType"].asString():"";
			strcpy_s(pInfo->szItemType,strTmp.c_str());

			strTmp = jListFile[i]["FileId"].isString() ? jListFile[i]["FileId"].asString():"";
			strcpy_s(pInfo->szFileId,strTmp.c_str());


			strTmp = jListFile[i]["FileSize"].isString() ? jListFile[i]["FileSize"].asString():"";
			pInfo->iFileLength = _atoi64(strTmp.c_str());

			strTmp = jListFile[i]["FileTimeLast"].isString() ? jListFile[i]["FileTimeLast"].asString():"";
			pInfo->iLastModifyTime = _atoi64(strTmp.c_str());

			strTmp = jListFile[i]["FileMd5"].isString() ? jListFile[i]["FileMd5"].asString():"";
			strcpy_s(pInfo->szMd5,strTmp.c_str());

			m_vFileList.push_back(pInfo);
		}

		return true;
	}
	catch(...)
	{
		Clear();
		return false;
	}
	return false;
}
string  CJsonDataSendFileList::GetResultJson()
{
	try
	{
		string strResult;
		Json::Value obj(Json::objectValue);
		obj["User"] = Json::Value(m_strUsrId.c_str());
		obj["NetDiskPath"] = Json::Value(m_strNetDiskDir.c_str());
		obj["Recursion"] = Json::Value(m_bRecursion);

		Json::Value jListFile(Json::arrayValue);
		Json::Value jListDir(Json::arrayValue);

		for (size_t i=0;i<m_vFileList.size();i++)
		{
			if ( _stricmp(m_vFileList[i]->szItemType,"Dir")==0)
			{
				Json::Value jDir(m_vFileList[i]->szItemName);
				jListDir.append(jDir);
			}
			else
			{
				Json::Value jFile(Json::objectValue);
				string  strTmp = FormatString("%s",m_vFileList[i]->szItemName);
				jFile["ItemName"] = Json::Value(strTmp.c_str());

				strTmp = FormatString("%s",m_vFileList[i]->szItemType);
				jFile["ItemType"] = Json::Value(strTmp.c_str());

				strTmp = FormatString("%s",m_vFileList[i]->szFileId);
				jFile["FileId"] = Json::Value(strTmp.c_str());

				strTmp = FormatString("%lld",m_vFileList[i]->iFileLength);
				jFile["FileSize"] = Json::Value(strTmp.c_str());

				strTmp = FormatString("%lld",m_vFileList[i]->iLastModifyTime);
				jFile["FileTimeLast"] = Json::Value(strTmp.c_str());

				strTmp = FormatString("%s",m_vFileList[i]->szMd5);
				jFile["FileMd5"] = Json::Value(strTmp.c_str());

				jListFile.append(jFile);
			}
		}

		obj["FileList"] = jListFile;
		obj["DirList"]  = jListDir;
		strResult = obj.toStyledString().c_str();
		return strResult;
	}
	catch (...)
	{
		return "";
	}
}

CJsonDataRegisterUser::~CJsonDataRegisterUser()
{
	Clear();
}
void CJsonDataRegisterUser::Clear()
{
	m_strUserName.empty();
	m_strUserId.empty();
	m_strPassWord.empty();
}
bool CJsonDataRegisterUser::Parse(const char * pszInJs)
{
	Clear();
	string strSrc = pszInJs == NULL ? "" : pszInJs;
	if (strSrc.empty()) return false;

	Json::Reader reader;
	Json::Value obj;

	try
	{
		if (!reader.parse(strSrc, obj, false)) return false;
		if (!obj.isObject()) return false;

		m_strUserId = obj["UserId"].isString() ? obj["UserId"].asString() : "";
		m_strUserName = obj["UserName"].isString() ? obj["UserName"].asString() : "";
		m_strPassWord = obj["PassWord"].isString() ? obj["PassWord"].asString() : "";

		return true;
	}
	catch(...)
	{
		Clear();
		return false;
	}
	return false;
}
string  CJsonDataRegisterUser::GetResultJson()
{
	try
	{
		string strResult;
		Json::Value obj(Json::objectValue);
		obj["UserId"] = Json::Value(m_strUserId.c_str());
		obj["UserName"] = Json::Value(m_strUserName.c_str());
		obj["PassWord"] = Json::Value(m_strPassWord.c_str());

		strResult = obj.toStyledString().c_str();
		return strResult;
	}
	catch (...)
	{
		return "";
	}
}

void CJsonDataDelteFile::Clear()
{
	m_strUserId.clear();
	m_vDelFileList.clear();
}
bool CJsonDataDelteFile::Parse(const char * pszInJs)
{
	Clear();
	string strSrc = pszInJs == NULL ? "" : pszInJs;
	if (strSrc.empty()) return false;

	Json::Reader reader;
	Json::Value obj;

	try
	{
		if (!reader.parse(strSrc, obj, false)) return false;
		if (!obj.isObject()) return false;

		m_strUserId = obj["UserId"].isString() ? obj["UserId"].asString() : "";
		Json::Value &  Filelist = obj["FileList"];

		if (!Filelist.isArray())
		{
			Clear();
			return false;
		}

		for (size_t i=0;i<Filelist.size();i++)
		{
			CDataDeleteFile DFile;
			DFile.m_strNetDiskDir = Filelist[i]["Path"].isString() ? Filelist[i]["Path"].asString():"";
			DFile.m_strItemName = Filelist[i]["Name"].isString() ? Filelist[i]["Name"].asString():"";
			DFile.m_strType =  Filelist[i]["Type"].isString()? Filelist[i]["Type"].asString():"";
			m_vDelFileList.push_back(DFile);
		}

		return true;
	}
	catch(...)
	{
		Clear();
		return false;
	}
	return false;
}
string CJsonDataDelteFile::GetResultJson()
{
	try
	{
		string strResult;
		Json::Value obj(Json::objectValue);
		obj["UserId"] = Json::Value(m_strUserId.c_str());

		Json::Value vFileArry(Json::arrayValue);
		for (size_t i=0;i<m_vDelFileList.size();i++)
		{
			Json::Value file(Json::objectValue);
			file["Path"] = Json::Value(m_vDelFileList[i].m_strNetDiskDir.c_str());
			file["Name"] = Json::Value(m_vDelFileList[i].m_strItemName.c_str());
			file["Type"] = Json::Value(m_vDelFileList[i].m_strType.c_str());
			vFileArry.append(file);
		}

		obj["FileList"] = vFileArry;
		strResult = obj.toStyledString().c_str();
		return strResult;
	}
	catch (...)
	{
		return "";
	}
}