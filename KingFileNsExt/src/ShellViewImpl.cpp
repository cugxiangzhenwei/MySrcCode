// ShellViewImpl.cpp : Implementation of CShellViewImpl
#include "stdafx.h"
#include <windowsx.h>   // for GET_X/Y_LPARAM()
#include "KingFileNsExt.h"
#include "ShellViewImpl.h"
#pragma comment(lib,"shlwapi")  // link with shlwapi since we use StrFormatByteSize()

UINT CShellViewImpl::sm_uListID = 101;

/////////////////////////////////////////////////////////////////////////////
// CShellViewImpl

STDMETHODIMP CShellViewImpl::GetWindow ( HWND* phwnd )
{
    // Return our container window's handle to the browser.

    *phwnd = m_hWnd;
    return S_OK;
}


// CreateViewWindow() creates the container window.  Once that window is
// created, it will create the list control.
STDMETHODIMP CShellViewImpl::CreateViewWindow ( LPSHELLVIEW pPrevView, 
                                                LPCFOLDERSETTINGS lpfs,
                                                LPSHELLBROWSER psb, 
                                                LPRECT prcView, HWND* phWnd )
{
    *phWnd = NULL;

    // Init member variables.

    m_spShellBrowser = psb;
    m_FolderSettings = *lpfs;

    // Get the parent window from Explorer.
    
    m_spShellBrowser->GetWindow( &m_hwndParent );

    // Create a container window, which will be the parent of the list control.
    
    if ( NULL == Create ( m_hwndParent, *prcView ) )
       return E_FAIL;

    // Return our window handle to the browser.

    *phWnd = m_hWnd;

    return S_OK;
}


// DestroyViewWindow() is responsible for destroying our windows & cleaning
// up stuff.
STDMETHODIMP CShellViewImpl::DestroyViewWindow()
{
    // Clean up the UI.

    UIActivate ( SVUIA_DEACTIVATE );

    // Destroy our windows & other resources.

    if ( NULL != m_hMenu )
       DestroyMenu ( m_hMenu );

    m_wndList.DeleteAllItems();
    m_wndList.DestroyWindow();
    DestroyWindow();

    return S_OK;
}


// GetCurrentInfo() returns our FODLERSETTINGS to the browser.
STDMETHODIMP CShellViewImpl::GetCurrentInfo ( LPFOLDERSETTINGS lpfs )
{
    *lpfs = m_FolderSettings;
    return S_OK;
}


// Refresh() refreshes the shell view.
STDMETHODIMP CShellViewImpl::Refresh()
{
    // Repopulate the list control.

    m_wndList.DeleteAllItems();
    FillList();

    return S_OK;
}


// UIActivate() is called whenever the focus switches among the Address bar,
// the tree, and our shell view.
STDMETHODIMP CShellViewImpl::UIActivate ( UINT uState )
{
    // Nothing to do if the state hasn't changed since the last call.
    
    if ( m_uUIState == uState )
        return S_OK;
    
    // Modify the Explorer menu and status bar.

    HandleActivate ( uState );

    return S_OK;
}


// QueryStatus() is called by the browser to determine which standard commands
// our extension supports.
STDMETHODIMP CShellViewImpl::QueryStatus ( const GUID* pguidCmdGroup, ULONG cCmds,
                                           OLECMD prgCmds[], OLECMDTEXT* pCmdText )
{
    if ( NULL == prgCmds )
        return E_POINTER;

    // The only useful standard command I've figured out is "refresh".  I've put
    // some trace messages in so you can see the other commands that the
    // browser sends our way.  If you can figure out what they're all for,
    // let me know!

    if ( NULL == pguidCmdGroup )
        {
        for ( UINT u = 0; u < cCmds; u++ )
            {
            ATLTRACE(">> Query - DEFAULT: %u\n", prgCmds[u]);

            switch ( prgCmds[u].cmdID )
                {
                case OLECMDID_REFRESH:
                    prgCmds[u].cmdf = OLECMDF_SUPPORTED | OLECMDF_ENABLED;
                break;
                }
            }

        return S_OK;
        }
    else if ( CGID_Explorer == *pguidCmdGroup )
        {
        for ( UINT u = 0; u < cCmds; u++ )
            {
            ATLTRACE(">> Query - EXPLORER: %u\n", prgCmds[u]);
            }
        }
    else if ( CGID_ShellDocView == *pguidCmdGroup )
        {
        for ( UINT u = 0; u < cCmds; u++ )
            {
            ATLTRACE(">> Query - DOCVIEW: %u\n", prgCmds[u]);
            }
        }

    return OLECMDERR_E_UNKNOWNGROUP;
}


// Exec() is called when the user executes a command in Explorer that we
// have to deal with.
STDMETHODIMP CShellViewImpl::Exec ( const GUID* pguidCmdGroup, DWORD nCmdID,
                                    DWORD nCmdExecOpt, VARIANTARG* pvaIn,
                                    VARIANTARG* pvaOut )
{
HRESULT hrRet = OLECMDERR_E_UNKNOWNGROUP;

    // The only standard command we act on is "refresh".  I've put
    // some trace messages in so you can see the other commands that the
    // browser sends our way.  If you can figure out what they're all for,
    // let me know!

    if ( NULL == pguidCmdGroup )
        {
        ATLTRACE(">> Exec - DEFAULT: %u\n", nCmdID);

        if ( OLECMDID_REFRESH == nCmdID )
            {
            Refresh();
            hrRet = S_OK;
            }
        }
    else if ( CGID_Explorer == *pguidCmdGroup )
        {
        ATLTRACE(">> Exec - EXPLORER : %u\n", nCmdID);
        }
    else if ( CGID_ShellDocView == *pguidCmdGroup )
        {
        ATLTRACE(">> Exec - DOCVIEW: %u\n", nCmdID);
        }

    return hrRet;
}


/////////////////////////////////////////////////////////////////////////////
// Message handlers

LRESULT CShellViewImpl::OnCreate ( UINT uMsg, WPARAM wParam, 
                                   LPARAM lParam, BOOL& bHandled )
{
HWND hwndList;
DWORD dwListStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER |
                       LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS;
DWORD dwListExStyles = WS_EX_CLIENTEDGE;
DWORD dwListExtendedStyles = LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP;
m_FolderSettings.ViewMode = FVM_DETAILS;
    // Set the list view's display style (large/small/list/report) based on
    // the FOLDERSETTINGS we were given in CreateViewWindow().

    switch ( m_FolderSettings.ViewMode )
        {
        case FVM_ICON:      dwListStyles |= LVS_ICON;      break;
        case FVM_SMALLICON: dwListStyles |= LVS_SMALLICON; break;
        case FVM_LIST:      dwListStyles |= LVS_LIST;      break;
        case FVM_DETAILS:   dwListStyles |= LVS_REPORT;    break;
        DEFAULT_UNREACHABLE;
        }

    // Create the list control.  Note that m_hWnd (inherited from CWindowImpl)
    // has already been set to the container window's handle.

    hwndList = CreateWindowEx ( dwListExStyles, WC_LISTVIEW, NULL, dwListStyles,
                                0, 0, 0, 0, m_hWnd, (HMENU) sm_uListID, 
                                _Module.GetModuleInstance(), 0 );

    if ( NULL == hwndList )
        return -1;

    // Attach m_wndList to the list control and initialize styles, image
    // lists, etc.

    m_wndList.Attach ( hwndList );

    m_wndList.SetExtendedListViewStyle ( dwListExtendedStyles );

    m_wndList.SetImageList ( g_ImglistLarge, LVSIL_NORMAL );
    m_wndList.SetImageList ( g_ImglistSmall, LVSIL_SMALL );

    if ( dwListStyles & LVS_REPORT )
        {
        m_wndList.InsertColumn ( 0, L"Drive", LVCFMT_LEFT, 0, 0 );
        m_wndList.InsertColumn ( 1, L"Volume Name", LVCFMT_LEFT, 0, 1 );
        m_wndList.InsertColumn ( 2, L"Free Space", LVCFMT_LEFT, 0, 2 );
        m_wndList.InsertColumn ( 3, L"Total Space", LVCFMT_LEFT, 0, 3 );
        }

    FillList();

    return 0;
}


LRESULT CShellViewImpl::OnSize ( UINT uMsg, WPARAM wParam, 
                                 LPARAM lParam, BOOL& bHandled )
{
    // Resize the list control to the same size as the container window.

    if ( m_wndList.IsWindow() )
       m_wndList.MoveWindow ( 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE );

    return 0;
}


LRESULT CShellViewImpl::OnSetFocus ( UINT uMsg, WPARAM wParam,
                                     LPARAM lParam, BOOL& bHandled )
{
    // This handler is called when the list container window gets the focus, 
    // usually because the user tabbed to it.  Immediately set the focus to
    // the list control.
   
    m_wndList.SetFocus();
    return 0;
}


LRESULT CShellViewImpl::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
int   nSelItem = m_wndList.GetNextItem ( -1, LVIS_SELECTED );
int   x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
HMENU hMenu, hPopup;
int   nMenuSelection;

    if ( -1 == nSelItem )
        return 0;

    hMenu = LoadMenu ( _Module.GetResourceInstance(), 
                       MAKEINTRESOURCE(IDR_LIST_CTXMENU) );

    if ( NULL == hMenu )
        return 0;

    hPopup = GetSubMenu ( hMenu, 0 );
    ATLASSERT(NULL != hPopup);

    // If the user invoked the context menu with the keyboard, x and y will be
    // -1.  In that case, find the center of the selected item's icon and place
    // the menu there.

    if ( -1 == x  ||  -1 == y )
        {
        RECT rcIcon;

        m_wndList.GetItemRect ( nSelItem, &rcIcon, LVIR_ICON );

        m_wndList.ClientToScreen ( &rcIcon );

        x = (rcIcon.right + rcIcon.left) / 2;
        y = (rcIcon.bottom + rcIcon.top) / 2;
        }

    // Show the popup menu.

    nMenuSelection = (int) TrackPopupMenu ( hPopup, TPM_LEFTBUTTON | TPM_RETURNCMD,
                                            x, y, 0, m_hWnd, NULL );

    if ( IDC_EXPLORE_DRIVE == nMenuSelection )
        {
        PostMessage ( WM_COMMAND, IDC_EXPLORE_DRIVE );
        }

    DestroyMenu ( hMenu );

    return 0;
}


LRESULT CShellViewImpl::OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if ( !IsWindow() || !m_wndList.IsWindow() || NULL == m_hMenu )
        return DefWindowProc();

    // The "explore drive" menu item must be disabled if no drive is selected
    // in the list.

bool bEnable = ( m_wndList.GetSelectedCount() > 0 );
UINT uState = bEnable ? MF_BYCOMMAND : MF_BYCOMMAND | MF_GRAYED;

    EnableMenuItem ( m_hMenu, IDC_EXPLORE_DRIVE, uState );

    return 0;
}


LRESULT CShellViewImpl::OnMenuSelect(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
WORD wMenuID = LOWORD(wParam);
WORD wFlags = HIWORD(wParam);

    // If the selected menu item is one of ours, show a flyby help string
    // in the Explorer status bar.

    if ( !(wFlags & MF_POPUP) )
        {
        switch ( wMenuID )
            {
            case IDC_EXPLORE_DRIVE:
            case IDC_SYS_PROPERTIES:
            case IDC_ABOUT_SIMPLENS:
                {
                CComBSTR bsHelpText;

                if ( bsHelpText.LoadString ( wMenuID ))
                    {
                    m_spShellBrowser->SetStatusTextSB ( bsHelpText.m_str );
                    }

                return 0;
                }
            break;
            }
        }

    // Otherwise, pass the message to the default handler.
    return DefWindowProc();
}


/////////////////////////////////////////////////////////////////////////////
// List control message handlers

LRESULT CShellViewImpl::OnListDeleteitem ( int idCtrl, LPNMHDR pnmh, BOOL& bHandled )
{
NMLISTVIEW* pNMLV = (NMLISTVIEW*) pnmh;
LPITEMIDLIST pidl;

    // Each list item's LPARAM data holds a pointer to that item's PIDL.
    // Free up that memory as the item's being deleted.

    pidl = (LPITEMIDLIST) m_wndList.GetItemData ( pNMLV->iItem );
    m_PidlMgr.Delete ( pidl );

    return 0;
}


LRESULT CShellViewImpl::OnListSetfocus ( int idCtrl, LPNMHDR pnmh, BOOL& bHandled )
{
   // Tell the browser that we have the focus.

   m_spShellBrowser->OnViewWindowActive ( this );

   HandleActivate ( SVUIA_ACTIVATE_FOCUS );

   return 0;
}
LRESULT CShellViewImpl::OnListDoubelClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pnmh);
	int i = pNMItemActivate->iItem;
	LPCITEMIDLIST pidlSelected;
	int           nSelItem;
	TCHAR         chDrive;
	TCHAR         szPath[] = _T("?:\\");

	nSelItem = m_wndList.GetNextItem ( -1, LVIS_SELECTED );
	ATLASSERT(i==nSelItem);
	if (nSelItem==-1) return 0;
	

	pidlSelected = (LPCITEMIDLIST) m_wndList.GetItemData ( nSelItem );
	chDrive = m_PidlMgr.GetData ( pidlSelected );

	ATLASSERT(chDrive >= 'A' && chDrive <= 'Z');
	*szPath = chDrive;

	ShellExecute ( NULL, _T("explore"), szPath, NULL, NULL, SW_SHOWNORMAL );
	return 0;
}


LRESULT CShellViewImpl::OnHeaderItemclick ( int idCtrl, LPNMHDR pnmh, BOOL& bHandled )
{
NMHEADER* pNMH = (NMHEADER*) pnmh;
int nClickedItem = pNMH->iItem;

    // Set the sorted column to the column that was just clicked.  If we're
    // already sorting on that column, reverse the sort order.

    if ( nClickedItem == m_nSortedColumn )
        { 
        m_bForwardSort = !m_bForwardSort;
        }
    else
        {
        m_bForwardSort = true;
        }

    m_nSortedColumn = nClickedItem;

    // Set up a CListSortInfo for the sort function to use.

const ESortedField aFields[] = { SIMPNS_SORT_DRIVELETTER, SIMPNS_SORT_VOLUMENAME,
                                 SIMPNS_SORT_FREESPACE, SIMPNS_SORT_TOTALSPACE };
CListSortInfo sort = { m_psfContainingFolder, aFields[m_nSortedColumn], m_bForwardSort };

    m_wndList.SortItems ( CompareItems, (LPARAM) &sort );

    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Command handlers

LRESULT CShellViewImpl::OnSystemProperties(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
TCHAR szParams[2*MAX_PATH], szCplPath[MAX_PATH];

    // Run the system control panel using the Control_RunDLL function in
    // shell32.dll

    GetSystemDirectory ( szCplPath, countof(szCplPath) );
    PathAppend ( szCplPath, _T("sysdm.cpl") );

    wsprintf ( szParams, _T("shell32.dll,Control_RunDLL %s"), szCplPath );

    ShellExecute ( NULL, _T("open"), _T("rundll32.exe"), szParams, NULL, SW_SHOWNORMAL );
    return 0;
}


LRESULT CShellViewImpl::OnExploreDrive(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
LPCITEMIDLIST pidlSelected;
int           nSelItem;
TCHAR         chDrive;
TCHAR         szPath[] = _T("?:\\");

    nSelItem = m_wndList.GetNextItem ( -1, LVIS_SELECTED );
    ATLASSERT ( -1 != nSelItem );

    pidlSelected = (LPCITEMIDLIST) m_wndList.GetItemData ( nSelItem );
    chDrive = m_PidlMgr.GetData ( pidlSelected );

    ATLASSERT(chDrive >= 'A' && chDrive <= 'Z');
    *szPath = chDrive;

    ShellExecute ( NULL, _T("explore"), szPath, NULL, NULL, SW_SHOWNORMAL );

    return 0;
}


LRESULT CShellViewImpl::OnAbout ( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
CSimpleDialog<IDD_ABOUTDLG> dlg;

    dlg.DoModal();
    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Other stuff

// FillList() populates the list control.
void CShellViewImpl::FillList()
{
CComPtr<IEnumIDList> pEnum;
LPITEMIDLIST pidl = NULL;
HRESULT hr;
    
    // Get an enumerator object for the folder's contents.  Since this simple
    // extension doesn't deal with subfolders, we request only non-folder
    // objects.

    hr = m_psfContainingFolder->EnumObjects ( m_hWnd, SHCONTF_NONFOLDERS,
                                              &pEnum );
    
    if ( FAILED(hr) )
        return;
        
    // Stop redrawing to avoid flickering

    m_wndList.SetRedraw ( FALSE );
        
    // Add items.
DWORD dwFetched;

    while ( pEnum->Next(1, &pidl, &dwFetched) == S_OK )
        {
        LVITEM lvi = {0};
        TCHAR szText[MAX_PATH];

        ATLASSERT(1 == dwFetched);  // sanity check

        lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
        lvi.iItem = m_wndList.GetItemCount();
        lvi.iImage = 0;
        
        // Store PIDL to the drive letter, using the lParam member for each item

        TCHAR chDrive = m_PidlMgr.GetData ( pidl );
        lvi.lParam = (LPARAM) m_PidlMgr.Create ( chDrive );
        
        // Column 1: Drive letter

        m_PidlMgr.GetPidlDescription ( pidl, szText );
        lvi.pszText = szText;
        
        m_wndList.InsertItem ( &lvi );

        if ( m_wndList.GetStyle() & LVS_REPORT )
            {
            // Column 2: volume label
            TCHAR szDriveRoot[] = _T("?:\\");
            ULARGE_INTEGER uliFreeSpace, uliTotalSpace;

            *szDriveRoot = chDrive;

            if ( GetVolumeInformation ( szDriveRoot, szText, countof(szText),
                                        NULL, NULL, NULL, NULL, 0 ))
                {
                m_wndList.SetItemText ( lvi.iItem, 1, szText );
                }

            // Columns 3/4: free/total space

            if ( GetDiskFreeSpaceEx ( szDriveRoot, &uliFreeSpace, &uliTotalSpace, NULL ))
                {
                WCHAR szSize[MAX_PATH];
                USES_CONVERSION;

                // Note we use the Unicode API here, since the ANSI one only
                // uses a 32-bit number (max 4 GB) for the size.
                // If you don't mind a dependence on IE 5, use 
                // StrFormatByteSize64() instead.

                StrFormatByteSizeW ( uliFreeSpace.QuadPart, szSize, countof(szSize) );
                m_wndList.SetItemText ( lvi.iItem, 2, W2CT(szSize) );

                StrFormatByteSizeW ( uliTotalSpace.QuadPart, szSize, countof(szSize) );
                m_wndList.SetItemText ( lvi.iItem, 3, W2CT(szSize) );

                /*
                If you don't want the dependency on shlwapi.dll, use the 
                following code, which just shows the numbers unformatted.
                Also take out the '#pragma comment' at the top of this file.

                _stprintf ( szText, _T("%I64u"), uliFreeSpace.QuadPart );
                m_wndList.SetItemText ( lvi.iItem, 2, szText);

                _stprintf ( szText, _T("%I64u"), uliTotalSpace.QuadPart );
                m_wndList.SetItemText ( lvi.iItem, 3, szText );
                */
                }
            }
        }
    
    // Sort the items by drive letter initially.

CListSortInfo sort = { m_psfContainingFolder, SIMPNS_SORT_DRIVELETTER, true };

    m_wndList.SortItems ( CompareItems, (LPARAM) &sort );
    
    if ( m_wndList.GetStyle() & LVS_REPORT )
        {
        m_wndList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
        m_wndList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
        m_wndList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
        m_wndList.SetColumnWidth ( 3, LVSCW_AUTOSIZE_USEHEADER );
        }

    // Redraw the list view.

    m_wndList.SetRedraw ( TRUE );
    m_wndList.Invalidate();
    m_wndList.UpdateWindow();
}


// CompareItems() is called when sorting the list control.  It calls the view's
// parent folder to compare the PIDLs of the items being compared.
int CALLBACK CShellViewImpl::CompareItems ( LPARAM l1, LPARAM l2, LPARAM lData )
{
CListSortInfo* pSort = (CListSortInfo*) lData;

    ATLASSERT(NULL != pSort);

    return (int) pSort->pShellFolder->CompareIDs ( lData, (LPITEMIDLIST) l1,
                                                   (LPITEMIDLIST) l2 );
}


void CShellViewImpl::HandleActivate ( UINT uState )
{
    // Undo our previous changes to the menu.

    HandleDeactivate();

    // If we are being activated, add our stuff to Explorer's menu.

    if ( SVUIA_DEACTIVATE != uState )
        {
        // First, create a new menu.
        ATLASSERT(NULL == m_hMenu);

        m_hMenu = CreateMenu();

        if ( NULL != m_hMenu )
            {
            // Let the browser insert its standard items first.
            OLEMENUGROUPWIDTHS omw = { 0, 0, 0, 0, 0, 0 };

            m_spShellBrowser->InsertMenusSB ( m_hMenu, &omw );
            
            // Insert our SimpleExt menu before the Explorer Help menu.

            HMENU hmenuSimpleNS;

            hmenuSimpleNS = LoadMenu ( _Module.GetResourceInstance(),
                                       uState == SVUIA_ACTIVATE_FOCUS ?
                                           MAKEINTRESOURCE(IDR_FOCUS) :
                                           MAKEINTRESOURCE(IDR_NONFOCUS) );

            if ( NULL != hmenuSimpleNS )
                {
                InsertMenu ( m_hMenu, FCIDM_MENU_HELP, MF_BYCOMMAND | MF_POPUP,
                             (UINT_PTR) GetSubMenu ( hmenuSimpleNS, 0 ),
                             _T("&SimpleNSExt") );
                }

            // Add an about item to Explorer's help menu.  We first need to get
            // the handle to the Help submenu.  We could do this by position,
            // using GetSubMenu(), since we can calculate its offset using the
            // numbers in the OLEMENUGROUPWIDTHS struct, but since we know the
            // command ID, that's less error-prone to use.

            MENUITEMINFO mii = { sizeof(MENUITEMINFO), MIIM_SUBMENU };

            if ( GetMenuItemInfo ( m_hMenu, FCIDM_MENU_HELP, FALSE, &mii ))
                {
                InsertMenu ( mii.hSubMenu, -1, MF_BYPOSITION,
                             IDC_ABOUT_SIMPLENS, _T("About &SimpleNSExt") );
                }

            // If our UI state is active with the focus, we have a bit more
            // work to do.

            if ( SVUIA_ACTIVATE_FOCUS == uState )
                {
                // The Edit menu created by Explorer is empty, so we can 
                // nuke it.

                DeleteMenu ( m_hMenu, FCIDM_MENU_EDIT, MF_BYCOMMAND );
                }

            // Set the new menu.

            m_spShellBrowser->SetMenuSB ( m_hMenu, NULL, m_hWnd );
            }   // if (NULL != m_hMenu)

        // Modify the status bar.
        m_spShellBrowser->SetStatusTextSB ( L"Simple Namespace Extension" );
        }
    
    // Save the current state
    m_uUIState = uState;
}


void CShellViewImpl::HandleDeactivate()
{
    if ( SVUIA_DEACTIVATE != m_uUIState )
        {
        if ( NULL != m_hMenu )
            {
            m_spShellBrowser->SetMenuSB ( NULL, NULL, NULL );
            m_spShellBrowser->RemoveMenusSB ( m_hMenu );
            
            DestroyMenu ( m_hMenu );    // also destroys the SimpleNSExt submenu
            m_hMenu = NULL;
            }

        m_uUIState = SVUIA_DEACTIVATE;
        }
}

