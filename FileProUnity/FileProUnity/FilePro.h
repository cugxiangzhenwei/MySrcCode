#pragma once
#include <vector>
#include <string>
using  namespace std;

typedef struct 
{
	string m_strExt; // 后缀名
	string m_strDesrciption; //描述符
}NewType;

class CFilePro
{
public:
	typedef struct 
	{
		long long iFileSize;
		CString m_strFileName;
		CString m_strFullPath;
		CString m_strCreateTime;
		CString m_strLastModifyTime;
		CString m_strLastAccessTime;
	}SFileInfo;

	CFilePro(void);
	~CFilePro(void);
	CString GetErrMsg(){return m_strErrMsg;}
	void ClearErrMsg(){m_strErrMsg = _T("");}

	/*
	选择文件对话框
	*/
	vector<CString> GetSelectedFile(bool bMutilSelect = true, const char * pszDefExt = NULL,const char *pszFilter = NULL);
	/*
	选择目录对话框
	*/
	CString GetSelectedFolder();
	/*
	打开指定的文件
	*/
	bool OpenFile(CString strFile);

	/*
	获取文件的MD5值
	*/
	bool GetFileMD5(CString strInFile,CString & strOutMd5);

	/*
	获取文件的信息
	*/
	bool GetFileInfo(CString strInFile,SFileInfo &fileInfo,const char *pszTimeFormat = "%Y-%m-%d %H:%M:%S");

	/*
	删除本地文件
	*/
	bool DeleteLocalFile(CString strFile);
	
	/* 
	获取所有右键菜单新建文件类型
	*/
	void GetAllDesktopRightMenuNewItems(vector<NewType> &vAllItems);
	/*
	在指定目录下创建指定类型的新文件
	*/
	bool CreateNewItem(string strItemType,string strDir);
private:
	CString m_strErrMsg;
};
