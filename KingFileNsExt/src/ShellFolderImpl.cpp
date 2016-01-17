// ShellFolderImpl.cpp : Implementation of CShellFolderImpl

#include "stdafx.h"
#include "KingFileNsExt.h"
#include "ShellFolderImpl.h"
#include "CopyTcharToPidl.h"
#include "ShellViewImpl.h"

// newapis.h has a wrapper for GetDiskFreeSpaceEx() that gets called on
// OSes that don't have that API.
#define WANT_GETDISKFREESPACEEX_WRAPPER
#define COMPILE_NEWAPIS_STUBS
//#include <newapis.h>

/////////////////////////////////////////////////////////////////////////////
// CShellFolderImpl methods

STDMETHODIMP CShellFolderImpl::GetClassID ( CLSID* pClsid )
{
    if ( NULL == pClsid )
        return E_POINTER;

    // Return our GUID to the shell.

    *pClsid = CLSID_ShellFolderImpl;    // defined in SimpleNsExt_i.c
    return S_OK;
}


// Initialize() is passed the PIDL of the folder where our extension is.
// In our case, it's the PIDL for the desktop.
STDMETHODIMP CShellFolderImpl::Initialize ( LPCITEMIDLIST pidl )
{
    // No special init needed.
	return S_OK;
}


// BindToObject() is called when a folder in our part of the namespace is being
// browsed.  We create a new CShellFolderImpl, initialize it with the pidl 
// being browsed, and return that new object to the shell.
STDMETHODIMP CShellFolderImpl::BindToObject ( LPCITEMIDLIST pidl, LPBC pbcReserved,
                                              REFIID riid, void** ppvOut )
{
HRESULT hr;
CComObject<CShellFolderImpl>* pShellFolder;

    // Create a new CShellFolderImpl COM object.

    hr = CComObject<CShellFolderImpl>::CreateInstance ( &pShellFolder );

    if ( FAILED(hr) )
        return hr;

    // AddRef() the object while we're using it.

    pShellFolder->AddRef();

    // Object initialization - pass the object its parent folder (this)
    // and the pidl it will be browsing to.

    hr = pShellFolder->_init ( this, pidl );

    if ( FAILED(hr) )
        {
        pShellFolder->Release();
        return hr;
        }

    // Return the requested interface back to the browser.

    hr = pShellFolder->QueryInterface ( riid, ppvOut );

    pShellFolder->Release();
    return hr;
}


// CompareIDs() is responsible for returning the sort order of two PIDLs.
// lParam is a pointer to a CListSortInfo set up by the caller.  Returns
// -1/0/1 like strcmp().
STDMETHODIMP CShellFolderImpl::CompareIDs ( LPARAM lParam, 
                                            LPCITEMIDLIST pidl1, 
                                            LPCITEMIDLIST pidl2 )
{
TCHAR chDrive1 = m_PidlMgr.GetData ( pidl1 );
TCHAR chDrive2 = m_PidlMgr.GetData ( pidl2 );
TCHAR szPath1[] = _T("?:\\"), szPath2[] = _T("?:\\");
CListSortInfo* pSortInfo = (CListSortInfo*) lParam;
HRESULT hrRet;

    // Validate input parameters.
    ATLASSERT(NULL != lParam);
    ATLASSERT(unsigned(pSortInfo->nSortedField) < SIMPNS_SORT_LAST);
    ATLASSERT(chDrive1 >= 'A' && chDrive1 <= 'Z');
    ATLASSERT(chDrive2 >= 'A' && chDrive2 <= 'Z');

    *szPath1 = chDrive1;
    *szPath2 = chDrive2;
    
    switch ( pSortInfo->nSortedField )
        {
        case SIMPNS_SORT_DRIVELETTER:
            {
            // Sort alphabetically by drive letter.

            if ( chDrive1 == chDrive2 )
                hrRet = 0;
            else if ( chDrive1 < chDrive2 )
                hrRet = -1;
            else
                hrRet = 1;
            }
        break;

        case SIMPNS_SORT_VOLUMENAME:
            {
            TCHAR szVolumeName1[MAX_PATH], szVolumeName2[MAX_PATH];
            BOOL bReadVolInfo1, bReadVolInfo2;

            // Get the volume labels for both drives, and sort by those names.

            bReadVolInfo1 = GetVolumeInformation ( szPath1, szVolumeName1,
                                                   countof(szVolumeName1),
                                                   NULL, NULL, NULL, NULL, 0 );

            bReadVolInfo2 = GetVolumeInformation ( szPath2, szVolumeName2,
                                                   countof(szVolumeName2),
                                                   NULL, NULL, NULL, NULL, 0 );

            // If we were able to read the label for only 1 drive, that drive
            // sorts first.  Otherwise, return the normal strcmp() comparison
            // of the labels.

            if ( !bReadVolInfo1 && !bReadVolInfo2 )
                hrRet = 0;
            else if ( bReadVolInfo1 && !bReadVolInfo2 )
                hrRet = -1;
            else if ( !bReadVolInfo1 && bReadVolInfo2 )
                hrRet = 1;
            else
                hrRet = lstrcmpi ( szVolumeName1, szVolumeName2 );
            }
        break;

        case SIMPNS_SORT_FREESPACE:
            {
            ULARGE_INTEGER uliFreeSpace1, uliFreeSpace2;
            BOOL bReadSpace1, bReadSpace2;

            // Get the free space for both drives, and sort by that number.

            bReadSpace1 = GetDiskFreeSpaceEx ( szPath1, &uliFreeSpace1, NULL, NULL );
            bReadSpace2 = GetDiskFreeSpaceEx ( szPath2, &uliFreeSpace2, NULL, NULL );

            // If we were able to read the space for only 1 drive, that drive
            // sorts first.  Otherwise, return the normal comparison of the
            // sizes.

            if ( !bReadSpace1 && !bReadSpace2 )
                hrRet = 0;
            else if ( bReadSpace1 && !bReadSpace2 )
                hrRet = -1;
            else if ( !bReadSpace1 && bReadSpace2 )
                hrRet = 1;
            else if ( uliFreeSpace1.QuadPart == uliFreeSpace2.QuadPart )
                hrRet = 0;
            else if ( uliFreeSpace1.QuadPart < uliFreeSpace2.QuadPart )
                hrRet = -1;
            else
                hrRet = 1;
            }
        break;

        case SIMPNS_SORT_TOTALSPACE:
            {
            ULARGE_INTEGER uliTotalSpace1, uliTotalSpace2;
            BOOL bReadSpace1, bReadSpace2;

            // Get the total space for both drives, and sort by that number.

            bReadSpace1 = GetDiskFreeSpaceEx ( szPath1, NULL, &uliTotalSpace1, NULL );
            bReadSpace2 = GetDiskFreeSpaceEx ( szPath2, NULL, &uliTotalSpace2, NULL );

            // If we were able to read the space for only 1 drive, that drive
            // sorts first.  Otherwise, return the normal comparison of the
            // sizes.

            if ( !bReadSpace1 && !bReadSpace2 )
                hrRet = 0;
            else if ( bReadSpace1 && !bReadSpace2 )
                hrRet = -1;
            else if ( !bReadSpace1 && bReadSpace2 )
                hrRet = 1;
            else if ( uliTotalSpace1.QuadPart == uliTotalSpace2.QuadPart )
                hrRet = 0;
            else if ( uliTotalSpace1.QuadPart < uliTotalSpace2.QuadPart )
                hrRet = -1;
            else
                hrRet = 1;
            }
        break;

        DEFAULT_UNREACHABLE;
        }

    // If the sort order is reversed (z->a or highest->lowest), negate the
    // return value.

    if ( !pSortInfo->bForwardSort )
        hrRet *= -1;

    return hrRet;
}


// CreateViewObject() creates a new COM object that implements IShellView.
STDMETHODIMP CShellFolderImpl::CreateViewObject ( HWND hwndOwner, 
                                                  REFIID riid, void** ppvOut )
{
HRESULT hr;
CComObject<CShellViewImpl>* pShellView;

    if ( NULL == ppvOut )
        return E_POINTER;

    *ppvOut = NULL;

    // Create a new CShellViewImpl COM object.

    hr = CComObject<CShellViewImpl>::CreateInstance ( &pShellView );

    if ( FAILED(hr) )
        return hr;

    // AddRef() the object while we're using it.

    pShellView->AddRef();
    
    // Object initialization - pass the object its containing folder (this).

    hr = pShellView->_init ( this );

    if ( FAILED(hr) )
        {
        pShellView->Release();
        return hr;
        }

    // Return the requested interface back to the shell.

    hr = pShellView->QueryInterface ( riid, ppvOut );

    pShellView->Release();
    return hr;
}


// EnumObjects() creates a COM object that implements IEnumIDList.
STDMETHODIMP CShellFolderImpl::EnumObjects ( HWND hwndOwner, DWORD dwFlags,
                                             LPENUMIDLIST* ppEnumIDList )
{
HRESULT hr;
DWORD   dwDrives;
int     i;

    if ( NULL == ppEnumIDList )
        return E_POINTER;

    *ppEnumIDList = NULL;

    // Enumerate all drives on the system and put the letters of the drives
    // into a vector.

    m_vecDriveLtrs.clear();

    for ( i = 0, dwDrives = GetLogicalDrives(); i <= 25; i++ )
        {
        if ( dwDrives & (1 << i) )
            {
            m_vecDriveLtrs.push_back ( 'A' + i );
            }
        }

    // Create an enumerator with CComEnumOnSTL<> and our copy policy class.

CComObject<CEnumIDListImpl>* pEnum;

    hr = CComObject<CEnumIDListImpl>::CreateInstance ( &pEnum );

    if ( FAILED(hr) )
        return hr;

    // AddRef() the object while we're using it.

    pEnum->AddRef();

    // Init the enumerator.  Init() will AddRef() our IUnknown (obtained with
    // GetUnknown()) so this object will stay alive as long as the enumerator 
    // needs access to the vector m_vecDriveLtrs.

    hr = pEnum->Init ( GetUnknown(), m_vecDriveLtrs );

    // Return an IEnumIDList interface to the caller.

    if ( SUCCEEDED(hr) )
        hr = pEnum->QueryInterface ( IID_IEnumIDList, (void**) ppEnumIDList );

    pEnum->Release();

    return hr;
}


// GetAttributesOf() returns the attributes for the items whose PIDLs are
// passed in.
STDMETHODIMP CShellFolderImpl::GetAttributesOf ( UINT uCount, LPCITEMIDLIST aPidls[],
                                                 LPDWORD pdwAttribs )
{
    // Our items are all just plain ol' items, so the attributes are always
    // 0.  Normally, the attributes returned come from the SFGAO_* constants.

    *pdwAttribs = 0;
    return S_OK;
}


// GetUIObjectOf() returns an interface used to handle events in Explorer's
// tree control.
STDMETHODIMP CShellFolderImpl::GetUIObjectOf ( HWND hwndOwner, UINT uCount,
                                               LPCITEMIDLIST* pPidl, REFIID riid,
                                               LPUINT puReserved, void** ppvReturn )
{
	*ppvReturn = NULL;

    // No additional interfaces supported.  Goodbye!

    return E_NOINTERFACE;
}
