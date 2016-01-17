#pragma once

// DocViewCorePropPage.h : CDocViewCorePropPage 属性页类的声明。


// CDocViewCorePropPage : 有关实现的信息，请参阅 DocViewCorePropPage.cpp。

class CDocViewCorePropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CDocViewCorePropPage)
	DECLARE_OLECREATE_EX(CDocViewCorePropPage)

// 构造函数
public:
	CDocViewCorePropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_DOCVIEWCORE };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

