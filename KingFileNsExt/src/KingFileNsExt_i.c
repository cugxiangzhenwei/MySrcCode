

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Dec 13 20:19:53 2015
 */
/* Compiler settings for KingFileNsExt.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IShellFolderImpl,0x68DBA4F0,0xA70F,0x4BD2,0x88,0x3C,0x8E,0x76,0x54,0x15,0xE1,0x36);


MIDL_DEFINE_GUID(IID, IID_IShellViewImpl,0x0EA6B88C,0x6FC4,0x4DBC,0xAE,0x9C,0x7D,0x3B,0xFE,0x2C,0x9F,0xF9);


MIDL_DEFINE_GUID(IID, LIBID_SIMPLENSEXTLib,0x15428555,0xA5FB,0x467D,0xA2,0xEB,0x3C,0x31,0x31,0x8C,0xBB,0xAF);


MIDL_DEFINE_GUID(CLSID, CLSID_ShellFolderImpl,0x4145E10E,0x36DB,0x4F2C,0x90,0x62,0x5D,0xE1,0xAF,0x40,0xBB,0x31);


MIDL_DEFINE_GUID(CLSID, CLSID_ShellViewImpl,0x2C045228,0x65B6,0x471D,0xAC,0x39,0x65,0x33,0x11,0x12,0xEA,0x1A);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



