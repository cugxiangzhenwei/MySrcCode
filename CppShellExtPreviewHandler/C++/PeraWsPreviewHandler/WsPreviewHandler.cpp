/****************************** Module Header ******************************\
Module Name:  RecipePreviewHandler.cpp
Project:      CppShellExtPreviewHandler
Copyright (c) Microsoft Corporation.



This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/
#include "stdafx.h"
#include "WsPreviewHandler.h"
#include <Shlwapi.h>
#include <Wincrypt.h>   // For CryptStringToBinary.
#include <msxml6.h>
#include <WindowsX.h>
#include <assert.h>
#include "resource.h"

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "msxml6.lib")


extern HINSTANCE   g_hInst;
extern long g_cDllRef;


inline int RECTWIDTH(const RECT &rc)
{
    return (rc.right - rc.left);
}

inline int RECTHEIGHT(const RECT &rc )
{
    return (rc.bottom - rc.top);
}


WsPreviewHandler::WsPreviewHandler() : m_cRef(1), m_pStream(NULL), 
    m_hwndParent(NULL), m_punkSite(NULL)
{
    InterlockedIncrement(&g_cDllRef);
}

WsPreviewHandler::~WsPreviewHandler()
{
	if (m_WsMainDlg.m_hWnd)
	{
		m_WsMainDlg.DestroyWindow();
	}

    if (m_punkSite)
    {
        m_punkSite->Release();
        m_punkSite = NULL;
    }
    if (m_pStream)
    {
        m_pStream->Release();
        m_pStream = NULL;
    }

    InterlockedDecrement(&g_cDllRef);
}


#pragma region IUnknown

// Query to the interface the component supported.
IFACEMETHODIMP WsPreviewHandler::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(WsPreviewHandler, IPreviewHandler),
       // QITABENT(WsPreviewHandler, IInitializeWithStream), 
		QITABENT(WsPreviewHandler, IInitializeWithFile), 
        QITABENT(WsPreviewHandler, IPreviewHandlerVisuals), 
        QITABENT(WsPreviewHandler, IOleWindow), 
        QITABENT(WsPreviewHandler, IObjectWithSite), 
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

// Increase the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) WsPreviewHandler::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

// Decrease the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) WsPreviewHandler::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

#pragma endregion


#pragma region IInitializeWithStream
// Initializes the preview handler with a stream. 
// Store the IStream and mode parameters so that you can read the item's data 
// when you are ready to preview the item. Do not load the data in Initialize. 
// Load the data in IPreviewHandler::DoPreview just before you render.
//IFACEMETHODIMP WsPreviewHandler::Initialize(IStream *pStream, DWORD grfMode)
//{
//    HRESULT hr = E_INVALIDARG;
//    if (pStream)
//    {
//		AfxMessageBox(L"WsPreviewHandler::Initialize IInitializeWithStream");
//        // Initialize can be called more than once, so release existing valid 
//        // m_pStream.
//        if (m_pStream)
//        {
//            m_pStream->Release();
//            m_pStream = NULL;
//        }
//
//        m_pStream = pStream;
//        m_pStream->AddRef();
//        hr = S_OK;
//    }
//    return hr;
//}

#pragma endregion

#pragma  region IInitializeWithFile
IFACEMETHODIMP WsPreviewHandler::Initialize( LPCWSTR pszFilePath, DWORD grfMode)
{
	HRESULT hr = E_INVALIDARG;
	if (pszFilePath)
	{
		m_strFilePath = pszFilePath;
		hr = S_OK;
	}
	return hr;
}
#pragma  endregion

#pragma region IPreviewHandler

// This method gets called when the previewer gets created. It sets the parent 
// window of the previewer window, as well as the area within the parent to be 
// used for the previewer window.
IFACEMETHODIMP WsPreviewHandler::SetWindow(HWND hwnd, const RECT *prc)
{
    if (hwnd && prc)
    {
        m_hwndParent = hwnd;  // Cache the HWND for later use
        m_rcParent = *prc;    // Cache the RECT for later use

		if (m_WsMainDlg.m_hWnd)
		{
			// Update preview window parent and rect information
			m_WsMainDlg.SetParent(CWnd::FromHandle(m_hwndParent));
			m_WsMainDlg.SetWindowPos(NULL, m_rcParent.left, m_rcParent.top, 
				RECTWIDTH(m_rcParent), RECTHEIGHT(m_rcParent), 
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
    }
    return S_OK;
}

// Directs the preview handler to set focus to itself.
IFACEMETHODIMP WsPreviewHandler::SetFocus()
{
    HRESULT hr = S_FALSE;
	if (m_WsMainDlg.m_hWnd)
	{
		m_WsMainDlg.SetFocus();
		hr = S_OK;
	}
    return hr;
}

// Directs the preview handler to return the HWND from calling the GetFocus 
// function.
IFACEMETHODIMP WsPreviewHandler::QueryFocus(HWND *phwnd)
{
    HRESULT hr = E_INVALIDARG;
    if (phwnd)
    {
        *phwnd = ::GetFocus();
        if (*phwnd)
        {
            hr = S_OK;
        }
        else
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
        }
    }
    return hr;
}

// Directs the preview handler to handle a keystroke passed up from the 
// message pump of the process in which the preview handler is running.
HRESULT WsPreviewHandler::TranslateAccelerator(MSG *pmsg)
{
    HRESULT hr = S_FALSE;
    IPreviewHandlerFrame *pFrame = NULL;
    if (m_punkSite && SUCCEEDED(m_punkSite->QueryInterface(&pFrame)))
    {
        // If your previewer has multiple tab stops, you will need to do 
        // appropriate first/last child checking. This sample previewer has 
        // no tabstops, so we want to just forward this message out.
        hr = pFrame->TranslateAccelerator(pmsg);

        pFrame->Release();
    }
    return hr;
}

// This method gets called when the size of the previewer window changes 
// (user resizes the Reading Pane). It directs the preview handler to change 
// the area within the parent hwnd that it draws into.
IFACEMETHODIMP WsPreviewHandler::SetRect(const RECT *prc)
{
    HRESULT hr = E_INVALIDARG;
    if (prc != NULL)
    {
        m_rcParent = *prc;

		if (m_WsMainDlg.m_hWnd)
		{
			m_WsMainDlg.SetWindowPos(NULL, m_rcParent.left, m_rcParent.top,
				(m_rcParent.right - m_rcParent.left), // Width
				(m_rcParent.bottom - m_rcParent.top), // Height
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
        hr = S_OK;
    }
    return hr;
}

// The method directs the preview handler to load data from the source 
// specified in an earlier Initialize method call, and to begin rendering to 
// the previewer window.
IFACEMETHODIMP WsPreviewHandler::DoPreview()
{
    // Cannot call more than once.
    // (Unload should be called before another DoPreview)
    if (m_WsMainDlg.m_hWnd != NULL || (!m_pStream && m_strFilePath.IsEmpty())) 
    {
        return E_FAIL;
    }

	/*COleStreamFile osfRead;
	osfRead.Attach(m_pStream);
	long lLength=osfRead.GetLength();
	CByteArray baBuf;
	baBuf.SetSize(lLength);
	osfRead.Read(baBuf.GetData(),lLength);
	BYTE * pData = baBuf.GetData();*/

	//AfxMessageBox(L"WsPreviewHandler::DoPreview进入");
	if (m_WsMainDlg.m_hWnd)
	{
		m_WsMainDlg.DestroyWindow();
	}
	//记录当前资源句柄
	HINSTANCE hOldInstance = AfxGetModuleState()->m_hCurrentInstanceHandle;
	HINSTANCE hDllInstance = GetModuleHandle(L"PeraWsPreviewHandler.dll");
	//设置主模块资源句柄
	AfxSetResourceHandle(hDllInstance);
	m_WsMainDlg.Create(CWsMainDlg::IDD,CWnd::FromHandle(m_hwndParent));
	m_WsMainDlg.SetWindowPos(NULL, m_rcParent.left, m_rcParent.top,
		RECTWIDTH(m_rcParent), RECTHEIGHT(m_rcParent), 
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	m_WsMainDlg.ShowWindow(SW_SHOW);
	m_WsMainDlg.m_MapCore.OpenLayer(m_strFilePath);
	m_WsMainDlg.m_MapCore.Invalidate();
	//恢复当前模块句柄
	AfxSetResourceHandle(hOldInstance);

	return S_OK;
}

// This method gets called when a shell item is de-selected. It directs the 
// preview handler to cease rendering a preview and to release all resources 
// that have been allocated based on the item passed in during the 
// initialization.
HRESULT WsPreviewHandler::Unload()
{
    if (m_pStream)
    {
        m_pStream->Release();
        m_pStream = NULL;
    }
	if (m_WsMainDlg.m_hWnd)
	{
		m_WsMainDlg.DestroyWindow();
	}
	if (!m_strFilePath.IsEmpty())
	{
		m_strFilePath.Empty();
	}

    return S_OK;
}

#pragma endregion


#pragma region IPreviewHandlerVisuals (Optional)

// Sets the background color of the preview handler.
IFACEMETHODIMP WsPreviewHandler::SetBackgroundColor(COLORREF color)
{
    return S_OK;
}

// Sets the font attributes to be used for text within the preview handler.
IFACEMETHODIMP WsPreviewHandler::SetFont(const LOGFONTW *plf)
{
    return S_OK;
}

// Sets the color of the text within the preview handler.
IFACEMETHODIMP WsPreviewHandler::SetTextColor(COLORREF color)
{
    return S_OK;
}

#pragma endregion


#pragma region IOleWindow

// Retrieves a handle to one of the windows participating in in-place 
// activation (frame, document, parent, or in-place object window).
IFACEMETHODIMP WsPreviewHandler::GetWindow(HWND *phwnd)
{
    HRESULT hr = E_INVALIDARG;
    if (phwnd)
    {
        *phwnd = m_hwndParent;
        hr = S_OK;
    }
    return hr;
}

// Determines whether context-sensitive help mode should be entered during an 
// in-place activation session
IFACEMETHODIMP WsPreviewHandler::ContextSensitiveHelp(BOOL fEnterMode)
{
    return E_NOTIMPL;
}

#pragma endregion


#pragma region IObjectWithSite

// Provides the site's IUnknown pointer to the object.
IFACEMETHODIMP WsPreviewHandler::SetSite(IUnknown *punkSite)
{
    if (m_punkSite)
    {
        m_punkSite->Release();
        m_punkSite = NULL;
    }
    return punkSite ? punkSite->QueryInterface(&m_punkSite) : S_OK;
}

// Gets the last site set with IObjectWithSite::SetSite. If there is no known 
// site, the object returns a failure code.
IFACEMETHODIMP WsPreviewHandler::GetSite(REFIID riid, void **ppv)
{
    *ppv = NULL;
    return m_punkSite ? m_punkSite->QueryInterface(riid, ppv) : E_FAIL;
}

#pragma endregion
