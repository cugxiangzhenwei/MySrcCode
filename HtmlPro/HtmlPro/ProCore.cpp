#include "stdafx.h"
#include "afxinet.h"
#include "imagehlp.h"
#include "HtmlPro.h"
#pragma comment(lib,"imagehlp.lib")

BOOL GetImgLink(vector<CString> &vImgLinks,const CString & strHtmlConment,const CString & strMainUrl)
{
	int iPos    = 1;
	int iPosEnd = 0;
	CString strTmp = strHtmlConment;
	CString strTmpMain = strMainUrl;

	strTmpMain.MakeLower();
	if (strTmpMain.Right(4)== ".htm" || strTmpMain.Right(5)== ".html")
	{
		strTmpMain.Replace("\\","/");
		strTmpMain = strTmpMain.Left(strTmpMain.ReverseFind('/'));
	}

	strTmp.MakeLower();
	while(iPos>0)
	{
		iPos = strTmp.Find("<img src=\"",iPos);
		if (iPos<0)
			break;

		iPos +=10;
		iPosEnd = strTmp.Find("\"",iPos);
		CString strImgLink = strTmp.Mid(iPos,iPosEnd-iPos);
		iPos = iPosEnd;
		if (strImgLink.Find("http")!=0)
		{
			strImgLink  = strTmpMain + strImgLink;
		}
		vImgLinks.push_back(strImgLink);
	}
	return TRUE;
}
CString GetHtmlChCode(CString strHtmlComment)
{
	int iMetaPos = strHtmlComment.Find("<meta ");
	iMetaPos = strHtmlComment.Find("charset=",iMetaPos) + strlen("charset=");
	int iEnd = strHtmlComment.Find("\"",iMetaPos);
	CString strCode = strHtmlComment.Mid(iMetaPos,iEnd-iMetaPos);
	strCode.MakeLower();
	return strCode;
}
//UTF8编码转为多字节编码 
CString UTF8ToMB(const char* pu8 )
{
	// convert an UTF8 string to widechar   
	int nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, -1, NULL, 0);  

	WCHAR* lpszW = NULL;  
	try  
	{  
		lpszW = new WCHAR[nLen];  
	}  
	catch(std::bad_alloc &/*memExp*/)  
	{  
		return false;  
	}  

	int nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, -1, lpszW, nLen);  

	if(nRtn != nLen)  
	{  
		delete[] lpszW;  
		return false;  
	}  

	// convert an widechar string to Multibyte   
	int MBLen = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
	if (MBLen <=0)  
	{  
		return false;  
	}	
	char *pmb = new char[MBLen+1];
	nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, pmb, MBLen, NULL, NULL);
	delete[] lpszW;  

	if(nRtn != MBLen)  
	{		
		delete pmb;
		return "";  
	}  

	CString str = pmb;
	delete pmb;
	return str;  
}

CString GetDownloadFileName(CHttpFile *pHttpFile )
{
	if (NULL==pHttpFile)
		return "";

	CString sContentDisposition;
	DWORD dwIndex = 0;
	BOOL bSuccess = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_DISPOSITION, sContentDisposition, &dwIndex);
	int iPosName = sContentDisposition.Find("filename=\"") + strlen("filename=\"");
	CString strName = sContentDisposition.Mid(iPosName); 
	int iPosEnd = strName.Find("\"");
	strName = strName.Left(iPosEnd);
	if (!strName.IsEmpty())
		printf("获取下载文件名成功:%s\n",strName);
	return strName;
}


BOOL DownloadFile(CString strLocalDir,CString strUrl)
{
	CInternetSession session;  
	CHttpFile *pHttpFile = (CHttpFile *)session.OpenURL(strUrl);  

	DWORD dwStatusCode;  
	pHttpFile->QueryInfoStatusCode(dwStatusCode);  

	if(dwStatusCode != HTTP_STATUS_OK)
	{
		pHttpFile->Close();  
		session.Close(); 
		printf("【%s】地址无效...\n",strUrl);
		return FALSE;
	}
	printf("下载地址：%s\n",strUrl);
	
	 CString strFileLen;
	 pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, strFileLen);
	 ULONGLONG iFileLen = _ttol(strFileLen);
	 printf("下载文件大小为:%s\n",strFileLen);

	 CString strFileName = GetDownloadFileName(pHttpFile);
	 if (strFileName.IsEmpty())
	 {
		 strUrl.Replace("\\","/");
		 strFileName = strUrl.Mid(strUrl.ReverseFind('/')+1);
	 }
	 strLocalDir.Replace("/","\\");
	 if (strLocalDir.Right(1)!="\\")
	 {
		 strLocalDir.AppendChar('\\');
	 }
	 CString strDest = strLocalDir + strFileName;
	 printf("目标文件存储为:%s\n",strDest);

	 MakeSureDirectoryPathExists(strDest);
	CStdioFile imgFile;  
	imgFile.Open(strDest, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	int nReadBlockSize = 50*1024;
	char* pBuffer = new char[nReadBlockSize];

	ULONGLONG iSizeGet = 0;
	int size=0;  
	do {  
			size = pHttpFile->Read(pBuffer,nReadBlockSize);    // 读取图片 
			imgFile.Write(pBuffer,size);  
			iSizeGet +=size;
			printf("下载进度为%2d%%\r",iSizeGet*100/iFileLen);
	}while(iSizeGet < iFileLen);  

	delete pBuffer;
	pHttpFile->Close();  
	session.Close();
	printf("\n下载完成\n");
	return TRUE;
}
BOOL DownloadHtml(CString & strDestHtml,CString strUrl,BOOL bIsFile)
{
	CInternetSession session;  
	CHttpFile *pHttpFile = (CHttpFile *)session.OpenURL(strUrl);  

	DWORD dwStatusCode;  
	pHttpFile->QueryInfoStatusCode(dwStatusCode);  

	if(dwStatusCode != HTTP_STATUS_OK)
	{
		pHttpFile->Close();  
		session.Close();   
		return FALSE;
	}
	CString strHtmlConment;
	CString strLine;  
		while(pHttpFile->ReadString(strLine) != NULL)
			strHtmlConment += strLine;

	session.Close();  
	pHttpFile->Close();

	if (GetHtmlChCode(strHtmlConment)=="utf-8")
	{
		if(bIsFile)
		{
			MakeSureDirectoryPathExists(strDestHtml);
			CStdioFile fileHtml;
			fileHtml.Open(strDestHtml, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			fileHtml.WriteString(strHtmlConment);
			fileHtml.Close();
		}
		strHtmlConment = UTF8ToMB(strHtmlConment);
	}

	//printf("网页源码为：\n%s\n",strHtmlConment);
	printf("请求链接【%s】完毕...\n",strUrl);
	if (!bIsFile)
		strDestHtml = strHtmlConment;

	return TRUE;
}