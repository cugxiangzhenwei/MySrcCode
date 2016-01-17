// CopyTcharToPidl.h: interface for the CCopyTcharToPidl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COPYTCHARTOPIDL_H__A7DD397C_D04A_4050_81BB_1463C70D2C54__INCLUDED_)
#define AFX_COPYTCHARTOPIDL_H__A7DD397C_D04A_4050_81BB_1463C70D2C54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PidlMgr.h"

class CCopyTcharToPidl  
{
public:
    static void init ( LPITEMIDLIST* p ) 
    {
        // No init needed.
    }
    
    static HRESULT copy ( LPITEMIDLIST* pTo, const TCHAR* pFrom )
    {
        *pTo = m_PidlMgr.Create ( *pFrom );
        return (NULL != *pTo) ? S_OK : E_OUTOFMEMORY;
    }

    static void destroy ( LPITEMIDLIST* p ) 
    {
        m_PidlMgr.Delete ( *p ); 
    }

private:
    static CPidlMgr m_PidlMgr;
};

typedef CComEnumOnSTL<IEnumIDList, &IID_IEnumIDList, LPITEMIDLIST,
                      CCopyTcharToPidl, std::vector<TCHAR> > CEnumIDListImpl;

#endif // !defined(AFX_COPYTCHARTOPIDL_H__A7DD397C_D04A_4050_81BB_1463C70D2C54__INCLUDED_)
