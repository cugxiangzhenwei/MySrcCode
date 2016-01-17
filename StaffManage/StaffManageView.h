
// StaffManageView.h : CStaffManageView 类的接口
//


#pragma once

class CStaffManageSet;

class CStaffManageView : public CRecordView
{
protected: // 仅从序列化创建
	CStaffManageView();
	DECLARE_DYNCREATE(CStaffManageView)

public:
	enum{ IDD = IDD_StaffManage_FORM };
	CStaffManageSet* m_pSet;

// 属性
public:
	CStaffManageDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual CRecordset* OnGetRecordset();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void UpdateCtrlReadOnlyState(BOOL bEnable);

// 实现
public:
	virtual ~CStaffManageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddreco();
	afx_msg void OnBnClickedRecodele();
	afx_msg void OnBnClickedSortreco();
	afx_msg void OnBnClickedFilterreco();
	afx_msg void OnBnClickedSetPhoto();
protected:
	virtual void OnDraw(CDC* /*pDC*/);
	void ClearPhotoView();
private:
	HBITMAP			m_hPhotoBitmap;
	bool			m_bClearPicture;
public:
	afx_msg void OnBnClickedRecoModify();
};

#ifndef _DEBUG  // StaffManageView.cpp 中的调试版本
inline CStaffManageDoc* CStaffManageView::GetDocument() const
   { return reinterpret_cast<CStaffManageDoc*>(m_pDocument); }
#endif

