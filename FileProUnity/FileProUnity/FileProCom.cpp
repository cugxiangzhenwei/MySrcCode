// FileProCom.cpp : CFileProCom 的实现

#include "stdafx.h"
#include "FileProCom.h"
#include "FilePro.h"
#include "FileCopy.h"
#include <string>

using namespace std;

//JSON 头文件
#include "value.h" 
#include "reader.h"
#include "writer.h"

// CFileProCom
#include <comutil.h>
#pragma comment(lib,"comsupp.lib")

CComPtr<IDispatch> CFileProCom::m_spCallback = NULL;

BEGIN_MESSAGE_MAP(MsgWnd, CWnd)      
	ON_MESSAGE(WM_COMM, OnRecvMsg)  
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()  

LRESULT MsgWnd::OnRecvMsg(WPARAM wPar, LPARAM lParram)  
{  
	if ( NULL != m_pParent )  
	{
		m_pParent->SendProgressMsg((LPVOID)lParram);//调用Parent，也就是接口类中的函数  
	}  
	return 1;  
}

void  CFileProCom::SendProgressMsg(LPVOID pProgrssMsg)
{
	CFileCopy *param = (CFileCopy*)pProgrssMsg;
	if (param==NULL)
		return;

	CComVariant avarParams[5];  //指定回调函数的参数 
	avarParams[0] = param->m_iCurrentBytes; 
	avarParams[1] = param->m_iTotalBytes; 
	avarParams[2] = param->strSrc; 
	avarParams[3] = param->strDst;

	// 连接点形式报告进度测试
	CString str;
	str.Format("已完成：%.2f",(param->m_iCurrentBytes/1000.0)/(param->m_iTotalBytes/1000.0));
	BSTR bst = _bstr_t(LPCTSTR(str));
	Fire_ReportCopyProgress(bst);
	return ;

	DISPPARAMS params = { avarParams, NULL, 5, 0 };  
	if(m_spCallback)  
	{  
		HRESULT hr = m_spCallback->Invoke(0,   
			IID_NULL,   
			LOCALE_USER_DEFAULT,   
			DISPATCH_METHOD,   
			&params, NULL, NULL, NULL);       
	}  
}

STDMETHODIMP CFileProCom::OpenFileDlg(BSTR strInJson, BSTR* strOutJson)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	//////////////////////////////////////////////
	string strIn  = (strInJson != NULL) ? _bstr_t(strInJson) : "";
	CString strFilter = "所有文件(*.*)|*.*||";

	bool bMutilSelect = false;
	Json::Reader reader;
	Json::Value obj( Json::objectValue );
	CString str;
	try
	{
		if (!reader.parse(strIn, obj, true)) goto LABLE;
		if ( obj.isNull() || !obj.isObject() ) goto LABLE;

		if (obj["filter"].isString())
			strFilter = obj["filter"].asCString();

		if (!obj["selectmode"].empty())
		{
			if (obj["selectmode"].asString() == "multi")
				bMutilSelect = true;
			else if (obj["selectmode"].asString() == "single")
				bMutilSelect = false;
		}
	}
	catch (...)
	{
		goto LABLE;
	}

LABLE:
	CFilePro filePro;
	vector<CString> vSelectFiles = filePro.GetSelectedFile(bMutilSelect,NULL,strFilter);
	
	Json::Value objOut;
	Json::Value jsonPathItems(Json::arrayValue);
	for (size_t i=0;i<vSelectFiles.size();i++)
		jsonPathItems.append(LPCTSTR(vSelectFiles[i]));

	objOut["path"] = jsonPathItems;
	objOut["flag"] = Json::Value(vSelectFiles.empty()? "false":"true");
	objOut["msg"] = Json::Value(LPCTSTR(filePro.GetErrMsg()));
	
	CString strOut = objOut.toStyledString().c_str();
	*strOutJson = _bstr_t(strOut).GetBSTR();
	return S_OK;
}


STDMETHODIMP CFileProCom::SelectDirDlg(BSTR strJsonIn, BSTR* strJsonOut)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	CFilePro filePro;
	CString strDir = filePro.GetSelectedFolder();

	Json::Value objOut;
	objOut["folderpath"]=  Json::Value(LPCTSTR(strDir));
	objOut["flag"]=  Json::Value(strDir.IsEmpty()? "false":"true");
	objOut["msg"]=  Json::Value(LPCTSTR(filePro.GetErrMsg()));

	CString strOut = objOut.toStyledString().c_str();
	*strJsonOut = _bstr_t(strOut).GetBSTR();
	return S_OK;
}


STDMETHODIMP CFileProCom::GetFileInfo(BSTR strJSonIn, BSTR* strOutJson)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码

	string strIn  = (strJSonIn!=NULL)? _bstr_t(strJSonIn):"";
	CString strInFile = "";

	Json::Reader reader;
	Json::Value obj( Json::objectValue );
	CString str;
	try
	{
		if (!reader.parse(strIn, obj, true)) return S_FALSE;
		if ( obj.isNull() || !obj.isObject() ) return S_FALSE;

		strInFile = ( obj["file"].isString()) ? obj["file"].asCString() : _T("");
	}
	catch (...)
	{
		return S_FALSE;
	}

	CFilePro filePro;
	CFilePro::SFileInfo info;
	
	bool bOk = filePro.GetFileInfo(strInFile,info);

	// 将获取的文件信息以json格式字符串保存输出
	Json::Value objOut;
	objOut["fullPath"] = Json::Value( (LPCTSTR)info.m_strFullPath );
	objOut["filename"] = Json::Value( (LPCTSTR)info.m_strFileName );
	objOut["createtime"] = Json::Value( (LPCTSTR)info.m_strCreateTime);
	objOut["modifytime"] = Json::Value( (LPCTSTR)info.m_strLastModifyTime);
	objOut["accesstime"] = Json::Value( (LPCTSTR)info.m_strLastAccessTime);

	CString strFileSize;
	strFileSize.Format("%d",info.iFileSize);

	objOut["filesize"] = Json::Value( (LPCTSTR)strFileSize );
	objOut["flag"] = Json::Value("true");
	objOut["msg"] = Json::Value( (LPCTSTR)filePro.GetErrMsg());

	CString strOut = objOut.toStyledString().c_str();
	*strOutJson = _bstr_t(strOut).GetBSTR();

	return S_OK;
}


STDMETHODIMP CFileProCom::DeleteLocalFile(BSTR strInJson, BSTR* strOutJson)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	string strIn  = (strInJson!=NULL)? _bstr_t(strInJson):"";
	CString strInFile = "";

	Json::Reader reader;
	Json::Value obj( Json::objectValue );
	CString str;
	try
	{
		if (!reader.parse(strIn, obj, true)) return S_FALSE;
		if ( obj.isNull() || !obj.isObject() ) return S_FALSE;

		strInFile = ( obj["file"].isString()) ? obj["file"].asCString() : _T("");
	}
	catch (...)
	{
		return S_FALSE;
	}

	CFilePro filePro;
	bool bOk = filePro.DeleteLocalFile(strInFile);

	CString strMsg;
	if (bOk)
	{
		strMsg.Format("文件【%s】删除成功!",strInFile);
		MessageBox(NULL,strMsg,"提示",MB_OK);
	}
	else
	{
		strMsg.Format("文件【%s】删除失败!",strInFile);
		MessageBox(NULL,strMsg,"提示",MB_OK);
	}

	Json::Value objOut;
	objOut["flag"]=  Json::Value(bOk ? "true":"false");
	objOut["msg"] =  Json::Value(LPCTSTR(filePro.GetErrMsg()));

	CString strOut = objOut.toStyledString().c_str();
	*strOutJson = _bstr_t(strOut).GetBSTR();
	return S_OK;
}


STDMETHODIMP CFileProCom::GetFileMd5(BSTR strInJson, BSTR* strOutJson)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	string strIn  = (strInJson!=NULL)? _bstr_t(strInJson):"";
	CString strInFile = "";

	Json::Reader reader;
	Json::Value obj( Json::objectValue );
	CString str;
	try
	{
		if (!reader.parse(strIn, obj, true)) return S_FALSE;
		if ( obj.isNull() || !obj.isObject() ) return S_FALSE;

		strInFile = ( obj["file"].isString()) ? obj["file"].asCString() : _T("");
	}
	catch (...)
	{
		return S_FALSE;
	}

	CFilePro filePro;

	CString strOutMD5;
	bool bOk = filePro.GetFileMD5(strInFile,strOutMD5);

	Json::Value objOut;
	objOut["MD5"]=  Json::Value(LPCTSTR(strOutMD5));
	objOut["flag"]=  Json::Value(bOk ? "true":"false");
	objOut["msg"] =  Json::Value(LPCTSTR(filePro.GetErrMsg()));

	CString strOut = objOut.toStyledString().c_str();
	*strOutJson = _bstr_t(strOut).GetBSTR();
	return S_OK;
}


STDMETHODIMP CFileProCom::OpenFile(BSTR strInJson, BSTR* strOutJson)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	string strIn  = (strInJson!=NULL)? _bstr_t(strInJson):"";
	CString strInFile = "";

	Json::Reader reader;
	Json::Value obj( Json::objectValue );
	CString str;
	try
	{
		if (!reader.parse(strIn, obj, true)) return S_FALSE;
		if ( obj.isNull() || !obj.isObject() ) return S_FALSE;

		strInFile = ( obj["file"].isString()) ? obj["file"].asCString() : _T("");
	}
	catch (...)
	{
		return S_FALSE;
	}

	CFilePro filePro;
	bool bOk = filePro.OpenFile(strInFile);

	Json::Value objOut;
	objOut["flag"]=  Json::Value(bOk ? "true":"false");
	objOut["msg"] =  Json::Value(LPCTSTR(filePro.GetErrMsg()));

	CString strOut = objOut.toStyledString().c_str();
	*strOutJson = _bstr_t(strOut).GetBSTR();
	return S_OK;
}


STDMETHODIMP CFileProCom::CopyFileCom(BSTR strInJson, BSTR* strOutJson)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码

	string strIn  = (strInJson!=NULL)? _bstr_t(strInJson):"";
	CString strSrcFile,strDstFile;

	Json::Reader reader;
	Json::Value obj( Json::objectValue );
	CString str;
	try
	{
		if (!reader.parse(strIn, obj, true))
			{
				CString strErr;
				strErr.Format("%s格式不正确",strIn.c_str());
				MessageBox(NULL,strErr,"提示",MB_ICONERROR);
				return S_FALSE;
			}
		if ( obj.isNull() || !obj.isObject() ) return S_FALSE;

		strSrcFile = ( obj["Source"].isString()) ? obj["Source"].asCString() : _T("");
		strDstFile = ( obj["Dest"].isString()) ? obj["Dest"].asCString() : _T("");
	}
	catch (...)
	{
		return S_FALSE;
	}

	if ( NULL == m_pMsgWnd )  
	{  
		m_pMsgWnd = new MsgWnd( this );  
		m_pMsgWnd->CreateEx(WS_EX_CLIENTEDGE,_T("Message"),  
			_T("Message"),WS_OVERLAPPED,0,0,0,0,HWND_MESSAGE,NULL);  
	}

	CFileCopy *cp = new CFileCopy(strSrcFile,strDstFile);
	bool bOk = cp->ExcuteCopy(false,m_pMsgWnd);

	Json::Value objOut;
	objOut["flag"]=  Json::Value(bOk ? "true":"false");
	objOut["msg"] =  Json::Value(LPCTSTR(cp->GetErrMsg()));

	CString strOut = objOut.toStyledString().c_str();
	*strOutJson = _bstr_t(strOut).GetBSTR();
	return S_OK;
}


STDMETHODIMP CFileProCom::GetCopyProgress(BSTR strInJson, BSTR* strOutJson)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	string strIn  = (strInJson!=NULL)? _bstr_t(strInJson):"";
	CString strDstFile;

	Json::Reader reader;
	Json::Value obj( Json::objectValue );
	
	try
	{
		if (!reader.parse(strIn, obj, true)) return S_FALSE;
		if ( obj.isNull() || !obj.isObject() ) return S_FALSE;

		strDstFile = ( obj["Dest"].isString()) ? obj["Dest"].asCString() : _T("");
	}
	catch (...)
	{
		return S_FALSE;
	}

	CFileCopy *CopyParam =CCopyManager::Instance()->GetCopyProgressInfo(strDstFile);
	Json::Value objOut;
	CString str;

	objOut["flag"]=  Json::Value(CopyParam ? "true":"false");
	objOut["msg"] =  Json::Value(CopyParam ? CopyParam->GetErrMsg():"未找到该复制过程");

	if (CopyParam)
	{
		str.Format("%d",CopyParam->m_iTotalBytes);
		objOut["total bytes"] = Json::Value(LPCTSTR(str));

		str.Format("%d",CopyParam->m_iCurrentBytes);
		objOut["current bytes"] =Json::Value(LPCTSTR(str));

		str.Format("%s",CopyParam->strSrc);
		objOut["source file"] = Json::Value(LPCTSTR(str));

		str.Format("%s",CopyParam->strDst);
		objOut["dest file"] = Json::Value(LPCTSTR(str));
	}

	CString strOut = objOut.toStyledString().c_str();
	*strOutJson = _bstr_t(strOut).GetBSTR();
	return S_OK;
}

STDMETHODIMP CFileProCom::SetJsCallbackFunc(VARIANT scriptCallback)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	if (scriptCallback.vt == VT_DISPATCH)  
	{  
		m_spCallback = scriptCallback.pdispVal;  
	}
	return S_OK;
}


STDMETHODIMP CFileProCom::CancelCopy(BSTR strInJson)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	string strIn  = (strInJson!=NULL)? _bstr_t(strInJson):"";
	CString strDstFile;

	Json::Reader reader;
	Json::Value obj( Json::objectValue );
	CString str;
	try
	{
		if (!reader.parse(strIn, obj, true))
		{
			CString strErr;
			strErr.Format("%s格式不正确",strIn.c_str());
			MessageBox(NULL,strErr,"提示",MB_ICONERROR);
			return S_FALSE;
		}
		if ( obj.isNull() || !obj.isObject() ) return S_FALSE;
		strDstFile = ( obj["Dest"].isString()) ? obj["Dest"].asCString() : _T("");
	}
	catch (...)
	{
		return S_FALSE;
	}

	CCopyManager::Instance()->CancelCopy(strDstFile);

	return S_OK;
}


STDMETHODIMP CFileProCom::GetAllNewItemTypes(BSTR strInJson, BSTR* strOutJson)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	CFilePro pro;
	vector<NewType> vItems;
	pro.GetAllDesktopRightMenuNewItems(vItems);

	Json::Value objOut;
	objOut["flag"]=  Json::Value(vItems.size()>0 ? "true":"false");
	objOut["msg"] =  Json::Value(vItems.size()>0 ? "":"获取新建文件类型错误!");

	Json::Value jsonTypeItems(Json::arrayValue);
	for (size_t i=0;i<vItems.size();i++)
	{
		Json::Value jnt;
		jnt["ext"] = Json::Value(vItems[i].m_strExt.c_str());
		jnt["disp"] = Json::Value(vItems[i].m_strDesrciption.c_str());
		jsonTypeItems.append(jnt);
	}

	objOut["types"] = jsonTypeItems;

	CString strOut = objOut.toStyledString().c_str();
	*strOutJson  = _bstr_t(strOut).GetBSTR();
	Fire_ReportCopyProgress(*strOutJson);
	return S_OK;
}


STDMETHODIMP CFileProCom::CreateNewFile(BSTR strInJson, BSTR* strOutJson)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码

	string strIn  = (strInJson!=NULL)? _bstr_t(strInJson):"";
	string strType,strDir;

	Json::Reader reader;
	Json::Value obj( Json::objectValue );

	Json::Value objOut;	
	try
	{
		if (!reader.parse(strIn, obj, true))
				return S_FALSE;

		if ( obj.isNull() || !obj.isObject() ) return S_FALSE;

		strType = ( obj["type"].isString()) ? obj["type"].asCString() : _T("");
		strDir  = ( obj["dir"].isString()) ? obj["dir"].asCString() : _T("");
	}
	catch (...)
	{
		return S_FALSE;
	}
	if (strType =="" ||strDir=="")
		return S_FALSE;

	CFilePro pro;
	bool bOK = pro.CreateNewItem(strType,strDir);
	objOut["flag"]=  Json::Value(bOK ? "true":"false");
	CString strErrMsg;
	strErrMsg.Format("在目录【%s】下创建【%s】类型文件失败!",strDir.c_str(),strType.c_str());
	objOut["msg"] =  Json::Value(bOK ? "":(LPCTSTR)strErrMsg);

	CString strOut = objOut.toStyledString().c_str();
	*strOutJson = _bstr_t(strOut).GetBSTR();

	return S_OK;
}


void MsgWnd::OnNcDestroy()
{
	CWnd::OnNcDestroy();

	// TODO: 在此处添加消息处理程序代码
	delete this;
}
