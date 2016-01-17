// ShellFolderImpl.h : Declaration of the CShellFolderImpl

#ifndef __SHELLFOLDERIMPL_H_
#define __SHELLFOLDERIMPL_H_

#include "resource.h"       // main symbols

#include "pidlmgr.h"

/////////////////////////////////////////////////////////////////////////////
// CShellFolderImpl

class ATL_NO_VTABLE CShellFolderImpl : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CShellFolderImpl, &CLSID_ShellFolderImpl>,
    public IShellFolder,
    public IPersistFolder
{
public:
    CShellFolderImpl() : m_pidl(NULL), m_pParentFolder(NULL)
    {
    HRESULT hr = SHGetMalloc ( &m_spMalloc );

        ATLASSERT( SUCCEEDED(hr) );
    }

    ~CShellFolderImpl()
    {
        if ( NULL != m_pParentFolder )
            m_pParentFolder->Release();
    }

DECLARE_REGISTRY_RESOURCEID(IDR_SHELLFOLDERIMPL)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CShellFolderImpl)
    COM_INTERFACE_ENTRY(IShellFolder)
    COM_INTERFACE_ENTRY(IPersistFolder)
    COM_INTERFACE_ENTRY(IPersist)
END_COM_MAP()

public:
    // IPersist
    STDMETHOD(GetClassID)(CLSID*);

    // IPersistFolder
    STDMETHOD(Initialize)(LPCITEMIDLIST);

    // IShellFolder
    STDMETHOD(BindToObject) (LPCITEMIDLIST, LPBC, REFIID, void**);
    STDMETHOD(CompareIDs) (LPARAM, LPCITEMIDLIST, LPCITEMIDLIST);
    STDMETHOD(CreateViewObject) (HWND, REFIID, void** );
    STDMETHOD(EnumObjects) (HWND, DWORD, LPENUMIDLIST*);
    STDMETHOD(GetAttributesOf) (UINT, LPCITEMIDLIST*, LPDWORD);
    STDMETHOD(GetUIObjectOf) (HWND, UINT, LPCITEMIDLIST*, REFIID, LPUINT, void**);

    STDMETHOD(BindToStorage) (LPCITEMIDLIST, LPBC, REFIID, void**)
        { return E_NOTIMPL; }
    STDMETHOD(GetDisplayNameOf) (LPCITEMIDLIST, DWORD, LPSTRRET)
        { return E_NOTIMPL; }
    STDMETHOD(ParseDisplayName) (HWND, LPBC, LPOLESTR, LPDWORD, LPITEMIDLIST*, LPDWORD)
        { return E_NOTIMPL; }
    STDMETHOD(SetNameOf) (HWND, LPCITEMIDLIST, LPCOLESTR, DWORD, LPITEMIDLIST*)
        { return E_NOTIMPL; }
                         
    // Init function - call right after constructing a CShellFolderImpl object.
    HRESULT _init ( CShellFolderImpl* pParentFolder, LPCITEMIDLIST pidl )
    {
        m_pParentFolder = pParentFolder;

        if ( NULL != m_pParentFolder )
            m_pParentFolder->AddRef();

        m_pidl = m_PidlMgr.Copy ( pidl );

        return S_OK;
    }

private:
    LPITEMIDLIST       m_pidl;
    CPidlMgr           m_PidlMgr;
    CShellFolderImpl*  m_pParentFolder;
    CComPtr<IMalloc>   m_spMalloc;
    std::vector<TCHAR> m_vecDriveLtrs;
};

#endif //__SHELLFOLDERIMPL_H_
