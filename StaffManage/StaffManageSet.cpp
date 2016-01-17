
// StaffManageSet.cpp : CStaffManageSet 类的实现
//

#include "stdafx.h"
#include "StaffManage.h"
#include "StaffManageSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStaffManageSet 实现

// 代码生成在 2013年7月29日, 23:13

IMPLEMENT_DYNAMIC(CStaffManageSet, CRecordset)

CStaffManageSet::CStaffManageSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_iStaffID = 0;
	m_strStaffName = L"";
	m_strStaffSex = L"";
	m_strDepartment = L"";
	m_iStaffAge = 0;
	m_WorkTime;
	m_fStaffPrice = 0.0;
	m_strDuty = L"";
	m_strWhereGradute = L"";
	m_nFields = 10;
	m_nDefaultType = dynaset;
}
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CStaffManageSet::GetDefaultConnect()
{
	return _T("DSN=人事管理数据库;DBQ=人事管理数据库.mdb;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString CStaffManageSet::GetDefaultSQL()
{
	return _T("[人事管理数据表]");
}

void CStaffManageSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[职工编号]"), m_iStaffID);
	RFX_Text(pFX, _T("[职工姓名]"), m_strStaffName);
	RFX_Text(pFX, _T("[职工性别]"), m_strStaffSex);
	RFX_Text(pFX, _T("[所在部门]"), m_strDepartment);
	RFX_Int(pFX, _T("[职工年龄]"), m_iStaffAge);
	RFX_Date(pFX, _T("[入职时间]"), m_WorkTime);
	RFX_Single(pFX, _T("[基本工资]"), m_fStaffPrice);
	RFX_Text(pFX, _T("[职称]"), m_strDuty);
	RFX_Text(pFX, _T("[毕业院校]"), m_strWhereGradute);
	RFX_LongBinary(pFX, _T("[照片]"),m_PtrPhoto);
}
/////////////////////////////////////////////////////////////////////////////
// CStaffManageSet 诊断

#ifdef _DEBUG
void CStaffManageSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CStaffManageSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

