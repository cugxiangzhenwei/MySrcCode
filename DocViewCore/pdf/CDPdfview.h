// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

//#import "F:\\面试题测试\\pdfview.ocx" no_namespace
// CDPdfview 包装类

class CDPdfview : public COleDispatchDriver
{
public:
	CDPdfview(){} // 调用 COleDispatchDriver 默认构造函数
	CDPdfview(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDPdfview(const CDPdfview& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// _DPdfview 方法
public:
	BOOL OpenPDF(LPCTSTR lpszPDFFile, LPCTSTR lpszUserPwd, LPCTSTR lpszOwnerPwd)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, lpszPDFFile, lpszUserPwd, lpszOwnerPwd);
		return result;
	}
	void ClosePDF()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long RunCommand(long nCode, long nPara1, long nPara2, long nPara3)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nCode, nPara1, nPara2, nPara3);
		return result;
	}
	void SetFindText(LPCTSTR lpszFindText)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lpszFindText);
	}
	void SetViewMode(long nViewMode)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nViewMode);
	}
	void RotateViewLeft()
	{
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RotateViewRight()
	{
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewNextPage()
	{
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewPreviousPage()
	{
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewFirstPage()
	{
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewLastPage()
	{
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewPage()
	{
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void FindPreviousText()
	{
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void FindNextText()
	{
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ZoomFitPage()
	{
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ZoomActualPage()
	{
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ZoomFitWidth()
	{
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Zoom(float nZoom)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nZoom);
	}
	void ViewModeSinglePage()
	{
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewModeFacing()
	{
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewModeContinuous()
	{
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewModeContinuousFacing()
	{
		InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ShowHideBookmarks()
	{
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetRegCode(LPCTSTR lpszRegCode)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lpszRegCode);
	}
	void ZoomIn()
	{
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ZoomOut()
	{
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ViewGotoPage(long nPageIndex)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nPageIndex);
	}
	long SetGotoPageNumber(long nPageIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nPageIndex);
		return result;
	}
	long GetCurrentPage()
	{
		long result;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPageTotalCount()
	{
		long result;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	float GetCurrentZoom()
	{
		float result;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_R4, (void*)&result, NULL);
		return result;
	}
	void SetMsgCallbackWnd(long hMsgWnd)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hMsgWnd);
	}
	long FlattenPDF(LPCTSTR lpszInPDF, LPCTSTR lpszOutPDF)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lpszInPDF, lpszOutPDF);
		return result;
	}
	BOOL EnableAnnotations(BOOL bEnable)
	{
		BOOL result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bEnable);
		return result;
	}
	BOOL OpenPDFFromMem(long lpPDFData, long nPDFDataLen, LPCTSTR lpszUserPwd, LPCTSTR lpszOwnerPwd)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, lpPDFData, nPDFDataLen, lpszUserPwd, lpszOwnerPwd);
		return result;
	}
	long EnableLaunchLink(long bEnable)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bEnable);
		return result;
	}
	long EnableMouseWhellInFacingMode(long bEnable)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bEnable);
		return result;
	}
	long GetScrollBar()
	{
		long result;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void ViewModeBook()
	{
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long ShowScrollBar(BOOL bShow)
	{
		long result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bShow);
		return result;
	}
	BOOL IsScrollBarShown()
	{
		BOOL result;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// _DPdfview 属性
public:

};
