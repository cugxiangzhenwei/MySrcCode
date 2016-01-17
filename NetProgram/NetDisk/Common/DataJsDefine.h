#include "CommonFun.h"

class CJsonDataGetFileList
{
public:
	~CJsonDataGetFileList();
	void Clear();
	bool Parse(const char * pszInJs);
	string  GetResultJson();
public:
	string m_strUsrId;
	string m_strNetDiskDir;
	bool   m_bRecursion;
};

class CJsonDataSendFileList
{
public:
	~CJsonDataSendFileList();
	void Clear();
	bool Parse(const char * pszInJs);
	string  GetResultJson();
	void GetSendListCopied(vector<FileListItem *> &vListOut);
public:
	vector<FileListItem *> m_vFileList;
	string m_strUsrId;
	string m_strNetDiskDir;
	bool   m_bRecursion;
};


class CJsonDataRegisterUser
{
public:
	~CJsonDataRegisterUser();
	void Clear();
	bool Parse(const char * pszInJs);
	string  GetResultJson();
public:
	string m_strUserId;
	string m_strUserName;
	string m_strPassWord;
};
class CDataDeleteFile
{
	public:
	string m_strNetDiskDir;
	string m_strItemName;
	string m_strType;
};
class CJsonDataDelteFile
{
public:
	void Clear();
	bool Parse(const char * pszInJs);
	string  GetResultJson();
public:
	string m_strUserId;
	vector<CDataDeleteFile> m_vDelFileList;
};

typedef CDataNewDirResult CDataDelFileResult;