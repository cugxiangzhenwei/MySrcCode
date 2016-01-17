// DocViewCorePropPage.cpp : CDocViewCorePropPage 属性页类的实现。

#include "stdafx.h"
#include "DocViewCore.h"
#include "DocViewCorePropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CDocViewCorePropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CDocViewCorePropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CDocViewCorePropPage, "DOCVIEWCORE.DocViewCorePropPage.1",
	0xcf260641, 0x2390, 0x4615, 0xae, 0x53, 0x60, 0x45, 0x76, 0xe7, 0xc4, 0x41)



// CDocViewCorePropPage::CDocViewCorePropPageFactory::UpdateRegistry -
// 添加或移除 CDocViewCorePropPage 的系统注册表项

BOOL CDocViewCorePropPage::CDocViewCorePropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_DOCVIEWCORE_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CDocViewCorePropPage::CDocViewCorePropPage - 构造函数

CDocViewCorePropPage::CDocViewCorePropPage() :
	COlePropertyPage(IDD, IDS_DOCVIEWCORE_PPG_CAPTION)
{
}



// CDocViewCorePropPage::DoDataExchange - 在页和属性间移动数据

void CDocViewCorePropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CDocViewCorePropPage 消息处理程序
