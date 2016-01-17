// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

//#import "D:\\Program Files (x86)\\Common Files\\Adobe\\Acrobat\\ActiveX\\AcroPDF.dll" no_namespace
// CAcroAXDocShim 包装类

class CAcroAXDocShim : public COleDispatchDriver
{
public:
	CAcroAXDocShim(){} // 调用 COleDispatchDriver 默认构造函数
	CAcroAXDocShim(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroAXDocShim(const CAcroAXDocShim& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IAcroAXDocShim 方法
public:
	CString get_src()
	{
		CString result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_src(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL LoadFile(LPCTSTR fileName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, fileName);
		return result;
	}
	void setShowToolbar(BOOL On)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, On);
	}
	void gotoFirstPage()
	{
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void gotoLastPage()
	{
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void gotoNextPage()
	{
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void gotoPreviousPage()
	{
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void setCurrentPage(long n)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, n);
	}
	void goForwardStack()
	{
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void goBackwardStack()
	{
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void setPageMode(LPCTSTR pageMode)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pageMode);
	}
	void setLayoutMode(LPCTSTR layoutMode)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, layoutMode);
	}
	void setNamedDest(LPCTSTR namedDest)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, namedDest);
	}
	void Print()
	{
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void printWithDialog()
	{
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void setZoom(float percent)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, percent);
	}
	void setZoomScroll(float percent, float left, float top)
	{
		static BYTE parms[] = VTS_R4 VTS_R4 VTS_R4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms, percent, left, top);
	}
	void setView(LPCTSTR viewMode)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms, viewMode);
	}
	void setViewScroll(LPCTSTR viewMode, float offset)
	{
		static BYTE parms[] = VTS_BSTR VTS_R4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms, viewMode, offset);
	}
	void setViewRect(float left, float top, float width, float height)
	{
		static BYTE parms[] = VTS_R4 VTS_R4 VTS_R4 VTS_R4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
	}
	void printPages(long from, long to)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms, from, to);
	}
	void printPagesFit(long from, long to, BOOL shrinkToFit)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, parms, from, to, shrinkToFit);
	}
	void printAll()
	{
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void printAllFit(BOOL shrinkToFit)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms, shrinkToFit);
	}
	void setShowScrollbars(BOOL On)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, On);
	}
	VARIANT GetVersions()
	{
		VARIANT result;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void setCurrentHightlight(long a, long b, long c, long d)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, a, b, c, d);
	}
	void setCurrentHighlight(long a, long b, long c, long d)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, a, b, c, d);
	}
	void postMessage(VARIANT& strArray)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &strArray);
	}
	VARIANT get_messageHandler()
	{
		VARIANT result;
		InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_messageHandler(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	void execCommand(VARIANT& strArray)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &strArray);
	}

	// IAcroAXDocShim 属性
public:

};
