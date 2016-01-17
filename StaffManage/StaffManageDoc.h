
// StaffManageDoc.h : CStaffManageDoc 类的接口
//


#pragma once
#include "StaffManageSet.h"


class CStaffManageDoc : public CDocument
{
protected: // 仅从序列化创建
	CStaffManageDoc();
	DECLARE_DYNCREATE(CStaffManageDoc)

// 属性
public:
	CStaffManageSet m_StaffManageSet;

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();

// 实现
public:
	virtual ~CStaffManageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


