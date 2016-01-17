// PreviewAppDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

typedef struct tagGUIDNAMEPAIR
{
    CString guid;
    CString name;
}GUIDNAMEPAIR,*PGUIDNAMEPAIR;

#define SAFERELEASE( X ) { if( X ) { X->Release(); X = NULL; } }

// CPreviewAppDlg dialog
class CPreviewAppDlg : public CDialog
{
    // Construction
public:
    CPreviewAppDlg(CWnd* pParent = NULL);	// standard constructor
    ~CPreviewAppDlg();	
    // Dialog Data
    enum { IDD = IDD_PREVIEWAPP_DIALOG };

protected:
    virtual VOID DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg VOID OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg VOID OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg VOID OnBnClickedBrowse();
    afx_msg VOID OnBnClickedNext();	
private:
    RECT m_rectPreview;
    CWnd *m_WndPreview;
    PGUIDNAMEPAIR m_pGuidFriendlyNamePair;
    DWORD m_countHandlers;	
    CList<CString,CString&> m_FileList;
    POSITION m_posNowShowing;
    IPreviewHandler *m_pIP ;
    IInitializeWithFile *m_pIFile ;
    IInitializeWithStream *m_pIStream ;
    WCHAR path[MAX_PATH];
    HGLOBAL m_hGlobal;
    IStream * m_pStream;	
    VOID ListInstalledHandlers( VOID );
    VOID ListAssociations( VOID );
    CString FriendlyNameFromGuid( WCHAR *Guid );
    CString ClsidFromFriendlyName( WCHAR * name);	
    DWORD WINAPI  DoPreview( HWND hWnd, RECT rectPreview, LPCTSTR szFile);
    CString GetClsidFromExtn( WCHAR * szExtn );

public:
    CListCtrl m_InstalledHandlers;
    CListCtrl m_AssociationsList;

private:
    CButton m_btnNext;
    CString m_szedtExtn;
    CComboBox m_cbAssociatedName;
public:
    CString m_szAssociationSelectionFromCombo;
    afx_msg VOID OnBnClickedButtonSet();
};
