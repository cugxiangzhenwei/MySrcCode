#pragma once

#include "resource.h"
#include <vector>
using namespace  std;

BOOL DownloadFile(CString strLocalDir,CString strUrl);
BOOL DownloadHtml(CString & strDestHtml,CString strUrl,BOOL bIsFile = FALSE);
BOOL GetImgLink(vector<CString> &vImgLinks,const CString & strHtmlConment,const CString & strMainUrl);