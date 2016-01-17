// PreviewAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PreviewApp.h"
#include "shobjidl.h"
#include "windows.h"
#include "objbase.h"
#include "shlobj.h"
#include "shlwapi.h"
#include "PreviewAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT GetHKCRRegistryKeyAndValue(PCWSTR pszSubKey, PCWSTR pszValueName, 
	PWSTR pszData, DWORD cbData)
{
	HRESULT hr;
	HKEY hKey = NULL;

	// Try to open the specified registry key. 
	hr = HRESULT_FROM_WIN32(RegOpenKeyExW(HKEY_CLASSES_ROOT, pszSubKey, 0, 
		KEY_READ, &hKey));

	if (SUCCEEDED(hr))
	{
		// Get the data for the specified value name.
		hr = HRESULT_FROM_WIN32(RegQueryValueExW(hKey, pszValueName, NULL, 
			NULL, reinterpret_cast<LPBYTE>(pszData), &cbData));

		RegCloseKey(hKey);
	}

	return hr;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual VOID DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPreviewAppDlg dialog




CPreviewAppDlg::CPreviewAppDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPreviewAppDlg::IDD, pParent),
                    m_WndPreview(NULL),
                    m_pGuidFriendlyNamePair(NULL),
                    m_countHandlers(0),
                    m_pIP(NULL),
                    m_pIFile(NULL),
                    m_pIStream(NULL),
                    m_hGlobal( NULL ), 
                    m_szedtExtn(_T("")),
                    m_pStream( NULL ),
                    m_szAssociationSelectionFromCombo(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPreviewAppDlg::~CPreviewAppDlg()
{
    if( NULL != m_hGlobal)
    {
        GlobalFree( m_hGlobal);
    }
    SAFERELEASE( m_pIFile )

    if( NULL != m_pIP )
    {
        m_pIP->Unload();		
        SAFERELEASE( m_pIP );
    }
    SAFERELEASE( m_pIStream )

    SAFERELEASE( m_pStream )
    if( NULL != m_pGuidFriendlyNamePair )
    {
        delete[] m_pGuidFriendlyNamePair;
    }

}

VOID CPreviewAppDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_PREVIEWERS_INSTALLED, m_InstalledHandlers);
    DDX_Control(pDX, IDC_LIST_ASSOCIATIONS, m_AssociationsList);
    DDX_Control(pDX, IDC_BUTTON_NEXT, m_btnNext);
    DDX_Text(pDX, IDC_EDIT_EXTN, m_szedtExtn);
    DDX_Control(pDX, IDC_COMBO1, m_cbAssociatedName);
    DDX_CBString(pDX, IDC_COMBO1, m_szAssociationSelectionFromCombo);
}

BEGIN_MESSAGE_MAP(CPreviewAppDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CPreviewAppDlg::OnBnClickedBrowse)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, &CPreviewAppDlg::OnBnClickedNext)
    ON_BN_CLICKED(IDC_BUTTON_SET, &CPreviewAppDlg::OnBnClickedButtonSet)
END_MESSAGE_MAP()


// CPreviewAppDlg message handlers

BOOL CPreviewAppDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    m_WndPreview = GetDlgItem( IDC_PREVIEW );
    m_WndPreview->GetWindowRect( &m_rectPreview );
    m_rectPreview.right -= (m_rectPreview.left +10) ;
    m_rectPreview.bottom -= (m_rectPreview.top +10 );
    m_rectPreview.top = 20;
    m_rectPreview.left = 10;

    // initialize the list controls to report view
    m_InstalledHandlers.ModifyStyle(0,LVS_REPORT,0);
    ListView_SetExtendedListViewStyle( m_InstalledHandlers.m_hWnd,LVS_EX_FULLROWSELECT );	
    m_InstalledHandlers.InsertColumn(0,L"Name",0,150);
    m_InstalledHandlers.InsertColumn (1,L"CLSID",0,200);


    m_AssociationsList.ModifyStyle(0,LVS_REPORT,0);
    ListView_SetExtendedListViewStyle( m_AssociationsList.m_hWnd,LVS_EX_FULLROWSELECT );
    m_AssociationsList.InsertColumn(0,L"Extension",0,150);
    m_AssociationsList.InsertColumn (1,L"Handler",0,200);

    m_btnNext.EnableWindow( FALSE );

    ListInstalledHandlers();
    ListAssociations();
    m_cbAssociatedName.SetCurSel( 0 );

    return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CPreviewAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CPreviewAppDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPreviewAppDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// This function creates the required interfaces and previews the file
DWORD WINAPI  CPreviewAppDlg::DoPreview( HWND hWnd, RECT rectPreview, LPCTSTR szFile) 
{
    WCHAR *pwcExtn = NULL;
    CString csClsid = L"";	
    CString csPreviewer = L"";
    DWORD dwRet = ERROR_BAD_ARGUMENTS;


    if( ( NULL != hWnd )  && ( NULL != szFile ) && 
        ( NULL != StrRChr(szFile, NULL, L'.') )  )
    {
        pwcExtn = StrRChr(szFile, NULL, L'.');
        csClsid = GetClsidFromExtn( pwcExtn );
        csPreviewer.Format (L"Using Previewer:-");

        if( NULL != m_hGlobal)
        {
            GlobalFree( m_hGlobal);
            m_hGlobal= NULL;
        }


        SAFERELEASE( m_pIFile )
            SAFERELEASE( m_pIStream )

            SAFERELEASE( m_pStream )
            if( NULL != m_pIP )
            {
                m_pIP->Unload();
                SAFERELEASE( m_pIP );					
            }


            if( FALSE == csClsid.IsEmpty())
            {

                HRESULT hr = E_FAIL;
                CLSID cls;

                CLSIDFromString( (LPWSTR)(LPCTSTR)csClsid,&cls);

                if( S_OK == CoCreateInstance(cls,NULL,CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,IID_IPreviewHandler,(LPVOID*)&m_pIP) )
                {
                    m_pIP->QueryInterface(IID_IInitializeWithFile,(LPVOID*)&m_pIFile );
                    m_pIP->QueryInterface(IID_IInitializeWithStream,(LPVOID*)&m_pIStream );

                }
                if( m_pIFile )
                {
                    hr = m_pIFile->Initialize( szFile,STGM_READ);
                }
                else if( m_pIStream )
                {
                    HANDLE hFile = CreateFile(szFile,FILE_READ_DATA,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL );
                    if( INVALID_HANDLE_VALUE != hFile )
                    {
                        DWORD dwSize = GetFileSize( hFile,NULL );
                        m_hGlobal= GlobalAlloc(GPTR, dwSize );
                        BYTE * pByte = (BYTE *)GlobalLock(m_hGlobal);

                        if( pByte )
                        {
                            ReadFile(hFile,pByte,dwSize,&dwSize,NULL);	
                            GlobalUnlock(m_hGlobal);

                            CreateStreamOnHGlobal(m_hGlobal, TRUE, &m_pStream);	
                            hr = m_pIStream->Initialize( m_pStream,STGM_READ);
                        }

                        CloseHandle( hFile );
                    }
                }

                if( m_pIP )
                {
                    csPreviewer += FriendlyNameFromGuid( (WCHAR*)(LPCTSTR)csClsid ) ;			
                    hr = m_pIP->SetWindow( hWnd , &rectPreview );
                    hr = m_pIP->DoPreview( );
                    dwRet = ERROR_SUCCESS;
                }
            }
            if( ERROR_SUCCESS != dwRet )
            {
                csPreviewer += L"None" ;					
                ::InvalidateRect( this->m_hWnd , NULL, TRUE);
            }
            SetDlgItemText( IDC_STATIC_PREVIEWER_USED, csPreviewer );

    }
    return dwRet;

}

// Select the directory to preview
VOID CPreviewAppDlg::OnBnClickedBrowse()
{
    CString findpath = L"";
    HANDLE hFind = NULL;
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Pick a Directory");
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
    if ( pidl != 0 )
    {
        // get the name of the folder

        if ( SHGetPathFromIDList ( pidl, path ) )
        {
            _tprintf ( _T("Selected Folder: %s\n"), path );
        }

        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }

        findpath = path;
        findpath.Append(L"\\*.*");
        WIN32_FIND_DATA fd;
        hFind = FindFirstFile(findpath,&fd);
        SetDlgItemText(IDC_STATIC_NOW_SHOWING,L"click Next");
        m_FileList.RemoveAll();
        do
        {

            if( 0!= ( wcscmp(L".",fd.cFileName ) ) &&
                0!= ( wcscmp(L"..",fd.cFileName ) )  &&
                ( 0 == ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) )
            {
                CString stTemp;
                stTemp.Format(L"%s\\%s",path,fd.cFileName );
                m_FileList.AddTail( stTemp );

            }
        }while( FindNextFile( hFind,&fd ) );
        m_posNowShowing = m_FileList.GetHeadPosition();
    }
    if( m_FileList.GetCount() )
    {
        m_btnNext.EnableWindow( TRUE );
    }
}



VOID CPreviewAppDlg::OnBnClickedNext()
{
    if( NULL == m_posNowShowing )
    {
        m_posNowShowing = m_FileList.GetHeadPosition();
    }

    CString csShowFile;
    csShowFile = m_FileList.GetNext( m_posNowShowing );
    if( FALSE == csShowFile.IsEmpty() )
    {
        DoPreview(m_WndPreview->m_hWnd , m_rectPreview, LPCTSTR( csShowFile ) );
        SetDlgItemText(IDC_STATIC_NOW_SHOWING,LPCTSTR( csShowFile ) );	
    }
}
//Read the  //HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\PreviewHandlers 
// and list the handlers installed on the machine
VOID CPreviewAppDlg::ListInstalledHandlers()
{
    HKEY hk = 0;

    WCHAR wcValue[ MAX_PATH ];	
    WCHAR wcData[ MAX_PATH ];
    DWORD Type = 0;
    DWORD cValue = sizeof( wcValue)/ sizeof(wcValue[0]);
    DWORD cbData = sizeof( wcData);
    if( NULL != m_pGuidFriendlyNamePair )
    {
        delete[] m_pGuidFriendlyNamePair  ;
        m_pGuidFriendlyNamePair  = NULL;
    }
    m_InstalledHandlers.DeleteAllItems();
    //HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\PreviewHandlers
    if( ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PreviewHandlers",
        0,
        KEY_READ,
        &hk ) )
    {
        RegQueryInfoKey(hk,NULL,NULL,NULL,NULL,NULL,NULL,&m_countHandlers,NULL,NULL,NULL,NULL);
        m_pGuidFriendlyNamePair = new GUIDNAMEPAIR [( m_countHandlers*sizeof(GUIDNAMEPAIR) )] ;

        for( DWORD i =0; i < m_countHandlers; i ++)
        {
            DWORD cValue = sizeof( wcValue)/ sizeof(wcValue[0]);
            DWORD cbData = sizeof( wcData);
            DWORD dw = RegEnumValue( hk,
                i,
                wcValue,
                &cValue,
                NULL,
                &Type,  
                (LPBYTE)wcData,
                &cbData
                ) ;
            {

                LVITEM lvitem;
                memset( &lvitem, 0 , sizeof( lvitem ) ) ;
                lvitem.iItem = i;
                lvitem.iSubItem = 0;
                lvitem.pszText = wcData;
                lvitem.mask = LVIF_TEXT;
                m_InstalledHandlers.InsertItem( &lvitem );
                m_cbAssociatedName.AddString( wcData );
                (m_pGuidFriendlyNamePair+i)->name  = wcData ;

                memset( &lvitem, 0 , sizeof( lvitem ) ) ;
                lvitem.iItem = i;
                lvitem.iSubItem = 1;
                lvitem.pszText = wcValue;
                lvitem.mask = LVIF_TEXT;
                m_InstalledHandlers.SetItem( &lvitem );
                (m_pGuidFriendlyNamePair+i)->guid = wcValue ;

            }


        }
        RegCloseKey ( hk );
    }

}

// enumerate the file extensions listed under HKEY_CLASSES_ROOT and the 
// associated preview handlers
VOID CPreviewAppDlg::ListAssociations()
{
    HKEY hk = 0;
    DWORD dwCount = 0;
    WCHAR wcName[ MAX_PATH ];	
    WCHAR wcData[ MAX_PATH ];
    CString csExt = NULL;	

    DWORD cName = sizeof( wcName)/ sizeof(wcName[0]);
    LONG cbData = sizeof( wcData);
    DWORD countFound = 0;

    m_AssociationsList.DeleteAllItems();	
    for( DWORD i =0; ; i ++)
    {
        cName = sizeof( wcName)/ sizeof(wcName[0]);
        cbData = sizeof( wcData);
        if( ERROR_SUCCESS == RegEnumKey( HKEY_CLASSES_ROOT,
            i,
            wcName,
            cName) )			
        {
            if( L'.' != wcName[0] )
            {
                continue;
            }
            csExt = wcName ;

            LVITEM lvitem;
            memset( &lvitem, 0 , sizeof( lvitem ) ) ;
            lvitem.iItem = m_AssociationsList.GetItemCount();
            wcscat( wcName, L"\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}" );
            // found the key, now find the GUID and match it with friendly name
            if( ERROR_SUCCESS == RegQueryValue(HKEY_CLASSES_ROOT,wcName,wcData, &cbData ) )
            {
                lvitem.iSubItem = 0;
                lvitem.pszText = ( LPWSTR )( LPCTSTR )csExt ;
                lvitem.mask = LVIF_TEXT;
                m_AssociationsList.InsertItem( &lvitem );
                lvitem.iSubItem = 1;
                lvitem.pszText = ( LPWSTR )( LPCTSTR )FriendlyNameFromGuid( wcData ); // TODO find the friendly name for this
                m_AssociationsList.SetItem( &lvitem );
            }
        }
        else
        {
            break;
        }


    }

}

// Given extension, determine the  CLSID of its handler
CString CPreviewAppDlg::GetClsidFromExtn( WCHAR * szExtn )
{
    CString cs = L"";
    CString strRet = L"";
    WCHAR wcData[MAX_PATH];	
    LONG cData = sizeof( wcData ) ;
    if( NULL != szExtn )
    {
        cs.Format(L"%s\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}",szExtn );
        if( ERROR_SUCCESS == RegQueryValue(HKEY_CLASSES_ROOT,cs,wcData, &cData) )
        {
            strRet = wcData ;
        }
		else
		{
			wchar_t szDefaultVal[260];
			HRESULT hr;
			hr = GetHKCRRegistryKeyAndValue(szExtn, NULL, szDefaultVal, 
				sizeof(szDefaultVal));

			// If the key exists and its default value is not empty, use the 
			// ProgID as the file type.
			if (SUCCEEDED(hr) && szDefaultVal[0] != L'\0')
			{
				cs.Format(L"%s\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}",szDefaultVal );
				if( ERROR_SUCCESS == RegQueryValue(HKEY_CLASSES_ROOT,cs,wcData, &cData) )
				{
					strRet = wcData ;
				}
			}
		}
    }
    return strRet;
}

// Given the CLSID of handler, return the friendly name
CString CPreviewAppDlg::FriendlyNameFromGuid( WCHAR * GUID )
{
    CString csRet = L"";
    if( NULL != GUID )
    {
        for( DWORD dwIndex = 0; dwIndex < m_countHandlers; dwIndex++)
        {
            if( 0 == (m_pGuidFriendlyNamePair+dwIndex)->guid.CompareNoCase( GUID ) )
            {
                csRet = (m_pGuidFriendlyNamePair+dwIndex)->name;
            }
        }
    }

    return csRet;	
}

// Given the Friendly name, return the CLSID of the preview handler
CString CPreviewAppDlg::ClsidFromFriendlyName( WCHAR * name)
{
    CString csRet = L"";
    if( NULL != name )
    {
        for( DWORD dwIndex = 0; dwIndex < m_countHandlers; dwIndex++)
        {
            if( 0 == (m_pGuidFriendlyNamePair+dwIndex)->name.CompareNoCase ( name ) )
            {
                csRet = (m_pGuidFriendlyNamePair+dwIndex)->guid;
            }
        }
    }

    return csRet ;	
}

// Set the preview handler 
// take the inputs from the Dialog and set the handler for the extension
VOID CPreviewAppDlg::OnBnClickedButtonSet()
{
    CString csKey = L"";
    CString clsid = L"";
    HKEY hkShellEx = NULL;
    HKEY hkPreviewer = NULL;
    DWORD dwRes = ERROR_SUCCESS;

    UpdateData(TRUE);
    clsid =  ClsidFromFriendlyName( (WCHAR*)( LPCTSTR )m_szAssociationSelectionFromCombo );	
    csKey.Format( L"%s\\ShellEx",this->m_szedtExtn );

    dwRes = RegCreateKey( HKEY_CLASSES_ROOT,csKey,&hkShellEx );
    if( ERROR_SUCCESS == dwRes )
    {
        dwRes = RegCreateKey( hkShellEx,L"{8895b1c6-b41f-4c1c-a562-0d564250836f}",&hkPreviewer );
    }
    if( ERROR_SUCCESS == dwRes )
    {
        RegSetValue( hkPreviewer,NULL,REG_SZ , clsid , clsid.GetLength()*sizeof( WCHAR )  );
    }
    if( ERROR_SUCCESS != dwRes )
    {
        SetDlgItemText(IDC_STATIC_EDIT_STATUS, L"Edit Failed - Run as Administrator Set the Previewer"  );
    } else
    {
        SetDlgItemText(IDC_STATIC_EDIT_STATUS, L"Edit Success "  );
    }
    if( hkPreviewer )
    {
        RegCloseKey( hkPreviewer );
    }
    if ( hkShellEx )
    {
        RegCloseKey( hkShellEx );
    }

}
