
// StaffManageView.cpp : CStaffManageView 类的实现
//

#include "stdafx.h"
#include "StaffManage.h"

#include "StaffManageSet.h"
#include "StaffManageDoc.h"
#include "StaffManageView.h"
#include "DialogQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HBITMAP BufferToHBITMAP(CWnd *pWnd,char * pBMPBuffer)
{
	HBITMAP				hBmp;
	LPSTR				hDIB,lpBuffer = pBMPBuffer;
	LPVOID				lpDIBBits;
	BITMAPFILEHEADER	bmfHeader;
	DWORD				bmfHeaderLen;

	bmfHeaderLen = sizeof(bmfHeader);
	strncpy((LPSTR)&bmfHeader,(LPSTR)lpBuffer,bmfHeaderLen);

	if (bmfHeader.bfType != (*(WORD*)"BM"))
		return NULL;//我copy《Windows程序设计》上的做法。

	hDIB = lpBuffer + bmfHeaderLen;
	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	lpDIBBits=(lpBuffer)+((BITMAPFILEHEADER *)lpBuffer)->bfOffBits;//这行功能和上面被注释掉的代码相同，容易理解。
	CClientDC dc(pWnd);
	hBmp = CreateDIBitmap(dc.m_hDC,&bmiHeader,CBM_INIT,lpDIBBits,&bmInfo,DIB_RGB_COLORS);
	return hBmp;
}

// CStaffManageView

IMPLEMENT_DYNCREATE(CStaffManageView, CRecordView)

BEGIN_MESSAGE_MAP(CStaffManageView, CRecordView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CStaffManageView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_ADDRECO, &CStaffManageView::OnBnClickedAddreco)
	ON_BN_CLICKED(IDC_RECODELE, &CStaffManageView::OnBnClickedRecodele)
	ON_BN_CLICKED(IDC_SORTRECO, &CStaffManageView::OnBnClickedSortreco)
	ON_BN_CLICKED(IDC_FILTERRECO, &CStaffManageView::OnBnClickedFilterreco)
	ON_BN_CLICKED(IDC_SET_PHOTO, &CStaffManageView::OnBnClickedSetPhoto)
	ON_BN_CLICKED(IDC_RECO_MODIFY, &CStaffManageView::OnBnClickedRecoModify)
END_MESSAGE_MAP()

// CStaffManageView 构造/析构

CStaffManageView::CStaffManageView()
: CRecordView(CStaffManageView::IDD)
{
	m_pSet = NULL;
	m_hPhotoBitmap = NULL;
	m_bClearPicture = false;
	// TODO: 在此处添加构造代码

}

CStaffManageView::~CStaffManageView()
{
}

void CStaffManageView::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	// 可以在此处插入 DDX_Field* 函数以将控件“连接”到数据库字段，例如
	DDX_FieldText(pDX, IDC_EDITZGBH, m_pSet->m_iStaffID, m_pSet); // 职工编号
	DDX_FieldText(pDX, IDC_EDITZGXM, m_pSet->m_strStaffName, m_pSet); // 职工姓名

	DDX_FieldText(pDX, IDC_EDITZGNL, m_pSet->m_iStaffAge, m_pSet); // 职工年龄
	DDX_FieldText(pDX, IDC_EDITSZBM, m_pSet->m_strDepartment, m_pSet); // 职工部门


	DDX_FieldText(pDX, IDC_EDITZGXB, m_pSet->m_strStaffSex, m_pSet); //职工性别
	DDX_FieldText(pDX, IDC_EDITZC, m_pSet->m_strDuty, m_pSet); //职称


	//DDX_FieldText(pDX, IDC_EDITGZSJ, m_pSet->m_WorkTime, m_pSet); // 工作时间
	DDX_FieldText(pDX, IDC_EDITJBGZ, m_pSet->m_fStaffPrice, m_pSet); // 基本工资
	DDX_FieldText(pDX, IDC_EDIT_GRADUATA, m_pSet->m_strWhereGradute, m_pSet); // 毕业院校

	// 有关更多信息，请参阅 MSDN 和 ODBC 示例
	DDX_DateTimeCtrl(pDX, IDC_TIME_RZSJ, m_pSet->m_WorkTime); // 工作时间

	CWnd *pPictureWnd = GetDlgItem(IDC_STATIC_PHOTO);
	if (pPictureWnd)
	{
		if (m_pSet->m_PtrPhoto.m_hData && m_pSet->m_PtrPhoto.m_dwDataLength > 0)
		{
			LPSTR buffer = (LPSTR)GlobalLock(m_pSet->m_PtrPhoto.m_hData);           
			m_hPhotoBitmap = BufferToHBITMAP(pPictureWnd,buffer);
			GlobalUnlock(m_pSet->m_PtrPhoto.m_hData);

			m_bClearPicture = false;
			this->RedrawWindow();
		}
		else
		{
			m_bClearPicture = true;
			this->RedrawWindow();
		}
		
	}
}

BOOL CStaffManageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CRecordView::PreCreateWindow(cs);
}

void CStaffManageView::OnInitialUpdate()
{
	m_pSet = &GetDocument()->m_StaffManageSet;
	CRecordView::OnInitialUpdate();

}


// CStaffManageView 打印


void CStaffManageView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CStaffManageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CStaffManageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CStaffManageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CStaffManageView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CStaffManageView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CStaffManageView 诊断

#ifdef _DEBUG
void CStaffManageView::AssertValid() const
{
	CRecordView::AssertValid();
}

void CStaffManageView::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
}

CStaffManageDoc* CStaffManageView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStaffManageDoc)));
	return (CStaffManageDoc*)m_pDocument;
}
#endif //_DEBUG


// CStaffManageView 数据库支持
CRecordset* CStaffManageView::OnGetRecordset()
{
	return m_pSet;
}


void CStaffManageView::UpdateCtrlReadOnlyState(BOOL bReadOnly)
{
	CEdit * pEdit = (CEdit*)GetDlgItem(IDC_EDITZGXM);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDITZGXB);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDITZGNL);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDITZC);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDITJBGZ);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDITSZBM);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_GRADUATA);
	pEdit->SetReadOnly(bReadOnly);
}
// CStaffManageView 消息处理程序

void CStaffManageView::OnBnClickedAddreco()
{
	// TODO: 在此添加控件通知处理程序代码
	static bool bAdd = true;
	if (bAdd)
	{
		UpdateCtrlReadOnlyState(FALSE);
		GetDlgItem(IDC_SET_PHOTO)->ShowWindow(SW_SHOW);

		m_pSet->m_strSort = "职工编号";
		m_pSet->Requery();
		m_pSet->MoveLast();
		int iID = m_pSet->m_iStaffID + 1;
		m_pSet->AddNew();

		UpdateData(FALSE);

		CStringW strText = _T("");
		strText.Format(_T("%d"),iID);
		SetDlgItemText(IDC_EDITZGBH,strText);

		bAdd = false;
		SetDlgItemText(IDC_ADDRECO,_T("保存记录"));
	}
	else
	{
		SetDlgItemText(IDC_ADDRECO,_T("增加记录"));
		GetDlgItem(IDC_SET_PHOTO)->ShowWindow(SW_HIDE);
		UpdateData(TRUE);
		m_pSet->Update();
		bAdd = true;
		AfxMessageBox(_T("增加记录成功!"));
		UpdateCtrlReadOnlyState(TRUE);
		m_pSet->MoveLast();
	}

}

void CStaffManageView::OnBnClickedRecodele()
{
	// TODO: 在此添加控件通知处理程序代码

	if (MessageBox(_T("确定要删除该条记录？"),_T("提示"),MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
		return;

	m_pSet->Delete();
	m_pSet->MoveNext();

	if (m_pSet->IsEOF())
		m_pSet->MoveLast();

	if (m_pSet->IsBOF())
		m_pSet->SetFieldNull(NULL);

	UpdateData(FALSE);
	AfxMessageBox(_T("删除成功"));
}

void CStaffManageView::OnBnClickedSortreco()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pSet->m_strSort = "基本工资";
	m_pSet->Requery();
	UpdateData(FALSE);
}

void CStaffManageView::OnBnClickedFilterreco()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogQuery dlg;
	CString value;
	if (dlg.DoModal()!=IDOK)
		return;


	value = _T("职工姓名=") + dlg.m_query;
	m_pSet->m_strFilter = value;
	m_pSet->Requery();
	UpdateData(FALSE);
}

void CStaffManageView::OnBnClickedSetPhoto()
{
	CString strFilter = _T("BMP图像文件|*.bmp;*.dib||");

	CFileDialog     dlg(TRUE,NULL,NULL,0,strFilter,this);     
	if(dlg.DoModal()!=IDOK)
		return;

	//m_pSet->Edit();   //声明编辑当前记录 
	CFile     filePhoto;      
	CFileStatus     fileStatus;      
	CString     fileLocate = dlg.GetPathName();            
	filePhoto.Open(fileLocate,CFile::modeRead);           
	filePhoto.GetStatus(fileStatus);

	m_pSet->m_PtrPhoto.m_dwDataLength = (SQLUINTEGER)fileStatus.m_size;
	HGLOBAL     hGlobal     =     GlobalAlloc(GPTR,(SIZE_T)fileStatus.m_size);           
	m_pSet->m_PtrPhoto.m_hData     =     GlobalLock(hGlobal);
	filePhoto.Read(m_pSet->m_PtrPhoto.m_hData,(UINT)fileStatus.m_size);           

	//把您选择的文件的数据写入m_pSet-m_PtrPhoto           
	m_pSet->SetFieldDirty(&m_pSet->m_PtrPhoto);           

	m_pSet->SetFieldNull(&m_pSet->m_PtrPhoto,FALSE);           
	//m_pSet->Update();     //     更新记录           
	GlobalUnlock(hGlobal); 


	CWnd *pPictureWnd = GetDlgItem(IDC_STATIC_PHOTO);
	m_hPhotoBitmap = BufferToHBITMAP(pPictureWnd,(char*)m_pSet->m_PtrPhoto.m_hData);
	this->RedrawWindow();
}

void CStaffManageView::ClearPhotoView()
{
	CWnd *pPictureWnd = GetDlgItem(IDC_STATIC_PHOTO);
	if (!pPictureWnd)
		return;

	CRect rect;
	pPictureWnd->GetClientRect(rect);

	CDC  *pDC  = pPictureWnd->GetDC();
	CBrush brush(RGB(255,255,255));
	CBrush * pOldBrush = (CBrush*)pDC->SelectObject(&brush);
	pDC->Rectangle(rect);
	pDC->SelectObject(pOldBrush);
}
void CStaffManageView::OnDraw(CDC *pDC)
{
	if(!m_hPhotoBitmap)
		return;

	CWnd *pPictureWnd = GetDlgItem(IDC_STATIC_PHOTO);
	if (!pPictureWnd)
		return;

	HBITMAP OldBitmap;
	CDC MemDC;
	MemDC.CreateCompatibleDC(pPictureWnd->GetDC());
	OldBitmap=(HBITMAP)MemDC.SelectObject(m_hPhotoBitmap);

	CRect rect;
	pPictureWnd->GetWindowRect(rect);
	pPictureWnd->GetDC()->BitBlt(0 ,0 , rect.Width() , rect.Height(), &MemDC , 0 , 0 ,SRCCOPY);

	MemDC.SelectObject(OldBitmap);

	if (m_bClearPicture)
		ClearPhotoView();
}

void CStaffManageView::OnBnClickedRecoModify()
{
	static bool bEdit = true;
	if (bEdit)
	{
		m_pSet->Edit();
		UpdateCtrlReadOnlyState(FALSE);
		GetDlgItem(IDC_SET_PHOTO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ADDRECO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RECODELE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SORTRECO)->EnableWindow(FALSE);
		GetDlgItem(IDC_FILTERRECO)->EnableWindow(FALSE);

	/*	GetDlgItem(ID_RECORD_PREV)->EnableWindow(FALSE);
		GetDlgItem(ID_RECORD_NEXT)->EnableWindow(FALSE);
		GetDlgItem(ID_RECORD_FIRST)->EnableWindow(FALSE);
		GetDlgItem(ID_RECORD_LAST)->EnableWindow(FALSE);*/


		SetDlgItemText(IDC_RECO_MODIFY,_T("保存修改"));
		bEdit = false;
	}
	else
	{
		m_pSet->Update();  //更新记录 
		MessageBox(_T("修改记录成功!"),_T("提示"),MB_OK|MB_ICONINFORMATION);

		GetDlgItem(IDC_SET_PHOTO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ADDRECO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RECODELE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SORTRECO)->EnableWindow(TRUE);
		GetDlgItem(IDC_FILTERRECO)->EnableWindow(TRUE);

		/*GetDlgItem(ID_RECORD_PREV)->EnableWindow(TRUE);
		GetDlgItem(ID_RECORD_NEXT)->EnableWindow(TRUE);
		GetDlgItem(ID_RECORD_FIRST)->EnableWindow(TRUE);
		GetDlgItem(ID_RECORD_LAST)->EnableWindow(TRUE);*/

		UpdateCtrlReadOnlyState(TRUE);
		SetDlgItemText(IDC_RECO_MODIFY,_T("记录修改"));
		bEdit = true;
	}
}
