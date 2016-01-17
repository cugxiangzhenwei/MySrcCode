#include "UsrDefine.h"
#include <string.h>
#include "DesUtil.h"
const char * g_strServerLocalDir = NULL;
FileInfo::FileInfo()
{
	iFileLength = 0;
	iLastModifyTime = 0;
	memset(szFileName,0,512*sizeof(char));
	memset(szMd5,0,128*sizeof(char));
	memset(szUserId,0,30*sizeof(char));
	memset(szDir,0,512*sizeof(char));
}
FileInfo * FileInfo::GetCopied()
{
	FileInfo * pCopiedInfo = new FileInfo;
	if (pCopiedInfo==NULL)
		return NULL;

	pCopiedInfo->iFileLength = this->iFileLength;
	pCopiedInfo->iLastModifyTime = this->iLastModifyTime;
	strcpy_s(pCopiedInfo->szFileName,this->szFileName);
	strcpy_s(pCopiedInfo->szMd5,this->szMd5);
	strcpy_s(pCopiedInfo->szUserId,this->szUserId);
	strcpy_s(pCopiedInfo->szDir,this->szDir);
	return pCopiedInfo;
}
FileListItem::FileListItem()
{
	memset(szItemName,0,512*sizeof(char));
	memset(szItemType,0,10*sizeof(char));
	memset(szFileId,0,128*sizeof(char));
	memset(szMd5,0,128*sizeof(char));
	iFileLength = -1;
	iLastModifyTime = -1;
}
FileListItem * FileListItem::GetCopied()
{
	FileListItem * pCopiedInfo = new FileListItem;
	if (pCopiedInfo==NULL)
		return NULL;

	strcpy_s(pCopiedInfo->szItemType,this->szItemType);
	strcpy_s(pCopiedInfo->szItemName,this->szItemName);
	strcpy_s(pCopiedInfo->szFileId,this->szFileId);
	strcpy_s(pCopiedInfo->szMd5,this->szMd5);

	pCopiedInfo->iFileLength = this->iFileLength;
	pCopiedInfo->iLastModifyTime = this->iLastModifyTime;
	return pCopiedInfo;
}
UploadStartInfo::UploadStartInfo()
{
	m_iFinishedSize = 0;
	memset(m_szCmdMsg,0,MSG_CMD_LEN*sizeof(char));
}
CDataLogin::CDataLogin()
{
	memset(m_szUsrId,0,100*sizeof(char));
	memset(m_szPassWd,0,50*sizeof(char));
}
void CDataLogin::Code()
{
	DesUtil Des("&a@*~/");
	std::string str = Des.Encrypt(m_szUsrId);
	strcpy_s(m_szUsrId,str.c_str());

	str = Des.Encrypt(m_szPassWd);
	strcpy_s(m_szPassWd,str.c_str());
}
void CDataLogin::Decode()
{
	DesUtil Des("&a@*~/");
	std::string str = Des.Decrypt(m_szUsrId);
	strcpy_s(m_szUsrId,str.c_str());

	str = Des.Decrypt(m_szPassWd);
	strcpy_s(m_szPassWd,str.c_str());
}
CDataLoginResult::CDataLoginResult()
{
	Clear();
}
void CDataLoginResult::Clear()
{
	memset(m_szUsrId,0,50*sizeof(char));
	memset(m_szUserName,0,50*sizeof(char));
	memset(m_szErrMsg,0,50*sizeof(char));
	m_iUsedSpace = -1;
	m_iAllSpace  = -1;
}
CDataNewDir::CDataNewDir()
{
	Clear();
}
void CDataNewDir::Clear()
{
	memset(m_szDirName,0,sizeof(char)*MAX_PATH*2);
	memset(m_szParentDir,0,sizeof(char)*MAX_PATH*2);
	memset(m_szUsrId,0,sizeof(char)*50);
}
CDataNewDirResult::CDataNewDirResult()
{
	m_bFlag = false;
	memset(m_szErrMsg,0,sizeof(char)*500);
}
CDataReNameItem::CDataReNameItem()
{	memset(m_szUsrId,0,sizeof(char)*50);
	memset(szItemName,0,sizeof(char)*512);
	memset(szItemDir,0,sizeof(char)*512);
	memset(szNewName,0,sizeof(char)*512);
}