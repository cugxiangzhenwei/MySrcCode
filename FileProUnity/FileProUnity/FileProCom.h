// FileProCom.h : CFileProCom 的声明

#pragma once
#include "resource.h"       // 主符号



#include "FileProUnity_i.h"
#include "_IFileProComEvents_CP.h"

#define WM_COMM WM_USER+200

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;

class CFileProCom;
class MsgWnd : public CWnd  
{  
public:  
	MsgWnd() {}  
	MsgWnd( CFileProCom* p ){ m_pParent = p; }
	~MsgWnd() {}
	CFileProCom * GetComPtr() {return m_pParent;}
protected:  
	afx_msg LRESULT OnRecvMsg(WPARAM wParam, LPARAM lParam);  
	DECLARE_MESSAGE_MAP()  
private:  
	MsgWnd(const MsgWnd&);  
	MsgWnd& operator=(const MsgWnd&);  
	CFileProCom* m_pParent;

public:
	afx_msg void OnNcDestroy();
};
// CFileProCom

class ATL_NO_VTABLE CFileProCom :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFileProCom, &CLSID_FileProCom>,
	public IConnectionPointContainerImpl<CFileProCom>,
	public CProxy_IFileProComEvents<CFileProCom>,
	public IObjectWithSiteImpl<CFileProCom>,
	public IDispatchImpl<IFileProCom, &IID_IFileProCom, &LIBID_FileProUnityLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IProvideClassInfo2Impl<&CLSID_FileProCom, &__uuidof(_IFileProComEvents), &LIBID_FileProUnityLib>  // add by xiangzw
{
public:
	CFileProCom()
	{
		m_pMsgWnd = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FILEPROCOM)


BEGIN_COM_MAP(CFileProCom)
	COM_INTERFACE_ENTRY(IFileProCom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IProvideClassInfo)    //add by xiangzw
	COM_INTERFACE_ENTRY(IProvideClassInfo2)   //add by xiangzw
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CFileProCom)
	CONNECTION_POINT_ENTRY(__uuidof(_IFileProComEvents))
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:



	STDMETHOD(OpenFileDlg)(BSTR strInJson, BSTR* strOutJson);
	STDMETHOD(SelectDirDlg)(BSTR strJsonIn, BSTR* strJsonOut);
	STDMETHOD(GetFileInfo)(BSTR strJSonIn, BSTR* strOutJson);
	STDMETHOD(DeleteLocalFile)(BSTR strInJson, BSTR* strOutJson);
	STDMETHOD(GetFileMd5)(BSTR strInJson, BSTR* strOutJson);
	STDMETHOD(OpenFile)(BSTR strInJson, BSTR* strOutJson);
	STDMETHOD(CopyFileCom)(BSTR strInJson, BSTR* strOutJson);
	STDMETHOD(GetCopyProgress)(BSTR strInJson, BSTR* strOutJson);
	STDMETHOD(SetJsCallbackFunc)(VARIANT scriptCallback);
	STDMETHOD(CancelCopy)(BSTR strInJson);
	STDMETHOD(GetAllNewItemTypes)(BSTR strInJson, BSTR* strOutJson);
	STDMETHOD(CreateNewFile)(BSTR strInJson, BSTR* strOutJson);
	void  SendProgressMsg(LPVOID pProgrssMsg);
	static CComPtr<IDispatch> m_spCallback;
	MsgWnd* m_pMsgWnd;
};

OBJECT_ENTRY_AUTO(__uuidof(FileProCom), CFileProCom)