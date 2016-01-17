// PidlMgr.h: interface for the CPidlMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIDLMGR_H__01C0306D_444C_4F8B_8FAF_862FA63AA9F5__INCLUDED_)
#define AFX_PIDLMGR_H__01C0306D_444C_4F8B_8FAF_862FA63AA9F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Data structure to be stored in our PIDLs.
struct PIDLDATA
{
    TCHAR chDriveLtr;
};

// Class that creates/destroyes PIDLs and gets data from PIDLs.
class CPidlMgr  
{
public:
   CPidlMgr();
   ~CPidlMgr();

   LPITEMIDLIST Create ( const TCHAR );
   LPITEMIDLIST Copy ( LPCITEMIDLIST );
   void Delete ( LPITEMIDLIST );
   UINT GetSize ( LPCITEMIDLIST );

   LPITEMIDLIST GetNextItem ( LPCITEMIDLIST );
   LPITEMIDLIST GetLastItem ( LPCITEMIDLIST );

   TCHAR GetData ( LPCITEMIDLIST );
   void GetPidlDescription ( LPCITEMIDLIST, LPTSTR );

private:
   CComPtr<IMalloc> m_spMalloc;
};

#endif // !defined(AFX_PIDLMGR_H__01C0306D_444C_4F8B_8FAF_862FA63AA9F5__INCLUDED_)
