// ShellViewImpl.h : Declaration of the CShellViewImpl

#ifndef __SHELLVIEWIMPL_H_
#define __SHELLVIEWIMPL_H_

#include "resource.h"       // main symbols

#include "ShellFolderImpl.h"
#include "PidlMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CShellViewImpl

class ATL_NO_VTABLE CShellViewImpl : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CShellViewImpl, &CLSID_ShellViewImpl>,
    public IShellView,
    public IOleCommandTarget,
    public CWindowImpl<CShellViewImpl>
{
public:
    CShellViewImpl() : m_uUIState(SVUIA_DEACTIVATE), m_nSortedColumn(0), 
                       m_bForwardSort(true), m_hwndParent(NULL),
                       m_hMenu(NULL), m_psfContainingFolder(NULL), 
                       m_wndList(this, 1)
    {
    }

    ~CShellViewImpl()
    {
        if ( NULL != m_psfContainingFolder )
            m_psfContainingFolder->Release();
    }

DECLARE_NO_REGISTRY()
DECLARE_PROTECT_FINAL_CONSTRUCT()

DECLARE_WND_CLASS(NULL)

BEGIN_COM_MAP(CShellViewImpl)
    COM_INTERFACE_ENTRY(IShellView)
    COM_INTERFACE_ENTRY(IOleWindow)
    COM_INTERFACE_ENTRY(IOleCommandTarget)
END_COM_MAP()

BEGIN_MSG_MAP(CShellViewImpl)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
    MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)

    NOTIFY_CODE_HANDLER(LVN_DELETEITEM, OnListDeleteitem)
    NOTIFY_CODE_HANDLER(HDN_ITEMCLICK, OnHeaderItemclick)
    NOTIFY_CODE_HANDLER(NM_SETFOCUS, OnListSetfocus)
	NOTIFY_CODE_HANDLER(NM_DBLCLK, OnListDoubelClick)

    COMMAND_ID_HANDLER(IDC_SYS_PROPERTIES, OnSystemProperties)
    COMMAND_ID_HANDLER(IDC_EXPLORE_DRIVE, OnExploreDrive)
    COMMAND_ID_HANDLER(IDC_ABOUT_SIMPLENS, OnAbout)
    MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup)
    MESSAGE_HANDLER(WM_MENUSELECT, OnMenuSelect)
END_MSG_MAP()

public:
    // IOleWindow
    STDMETHOD(GetWindow)(HWND* phwnd);

    STDMETHOD(ContextSensitiveHelp)(BOOL)
        { return E_NOTIMPL; }
    
    // IShellView methods
    STDMETHOD(CreateViewWindow)(LPSHELLVIEW, LPCFOLDERSETTINGS, LPSHELLBROWSER, LPRECT, HWND*);
    STDMETHOD(DestroyViewWindow)();
    STDMETHOD(GetCurrentInfo)(LPFOLDERSETTINGS);
    STDMETHOD(Refresh)();
    STDMETHOD(UIActivate)(UINT);

    STDMETHOD(AddPropertySheetPages)(DWORD, LPFNADDPROPSHEETPAGE, LPARAM)
        { return E_NOTIMPL; }
    STDMETHOD(EnableModeless)(BOOL)
        { return E_NOTIMPL; }
    STDMETHOD(GetItemObject)(UINT, REFIID, void**)
        { return E_NOTIMPL; }
    STDMETHOD(SaveViewState)()
        { return E_NOTIMPL; }
    STDMETHOD(SelectItem)(LPCITEMIDLIST, UINT)
        { return E_NOTIMPL; }
    STDMETHOD(TranslateAccelerator)(LPMSG)
        { return E_NOTIMPL; }
    
    // IOleCommandTarget methods
    STDMETHOD(QueryStatus)(const GUID*, ULONG, OLECMD prgCmds[], OLECMDTEXT*);
    STDMETHOD(Exec)(const GUID*, DWORD, DWORD, VARIANTARG*, VARIANTARG*);

    // Message handlers
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMenuSelect(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // List control message handlers
    LRESULT OnListDeleteitem(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
    LRESULT OnHeaderItemclick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
    LRESULT OnListSetfocus(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnListDoubelClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

    // Command handlers
    LRESULT OnSystemProperties(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnExploreDrive(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
        
    // Other stuff
    HRESULT _init ( CShellFolderImpl* pContainingFolder )
    {
        m_psfContainingFolder = pContainingFolder;

        if ( NULL != m_psfContainingFolder )
            m_psfContainingFolder->AddRef();

        return S_OK;
    }

    static UINT sm_uListID;

private:
    CPidlMgr     m_PidlMgr;
    UINT         m_uUIState;
    int          m_nSortedColumn;
    bool         m_bForwardSort;
    FOLDERSETTINGS m_FolderSettings;

    HWND         m_hwndParent;
    HMENU        m_hMenu;
    CContainedWindowT<ATLControls::CListViewCtrl> m_wndList;

    CShellFolderImpl*      m_psfContainingFolder;
    CComPtr<IShellBrowser> m_spShellBrowser;

    void FillList();
    static int CALLBACK CompareItems ( LPARAM l1, LPARAM l2, LPARAM lData );
    void HandleActivate(UINT uState);
    void HandleDeactivate();
};

#endif //__SHELLVIEWIMPL_H_
