// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

//#import "D:\\Program Files (x86)\\Radaee PDF Reader(OCX)\\RadaeePDF.dll" no_namespace
// CRadaeePDFDocument 包装类

class CRadaeePDFDocument : public COleDispatchDriver
{
public:
	CRadaeePDFDocument(){} // 调用 COleDispatchDriver 默认构造函数
	CRadaeePDFDocument(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CRadaeePDFDocument(const CRadaeePDFDocument& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IPDFDocument 方法
public:
	BOOL open(LPCTSTR filename, LPCTSTR password)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, filename, password);
		return result;
	}
	void close()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get_page_count()
	{
		long result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_error_code()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	double get_page_width(long pageno)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, pageno);
		return result;
	}
	double get_page_height(long pageno)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, pageno);
		return result;
	}
	CString get_meta_data(LPCTSTR name)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, name);
		return result;
	}
	LPDISPATCH get_first_bookmark()
	{
		LPDISPATCH result;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long save_as(LPCTSTR filename, long type)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, filename, type);
		return result;
	}
	long print(LPCTSTR title, long page_start, long page_end, double page_ratio, long copies, long align, long printer)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 VTS_R8 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, title, page_start, page_end, page_ratio, copies, align, printer);
		return result;
	}
	long render_page(long hdc, long page_no, double resx, double resy, long offx, long offy, BOOL print_mode)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 VTS_R8 VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, hdc, page_no, resx, resy, offx, offy, print_mode);
		return result;
	}
	void cancel_session(long session)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, session);
	}
	long print2(LPCTSTR title, long page_start, long page_end, double page_ratio, long copies, long align, LPCTSTR printer, long hDevMode)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 VTS_R8 VTS_I4 VTS_I4 VTS_BSTR VTS_I4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, title, page_start, page_end, page_ratio, copies, align, printer, hDevMode);
		return result;
	}
	BOOL open_url(LPDISPATCH buf, LPCTSTR password)
	{
		BOOL result;
		static BYTE parms[] = VTS_DISPATCH VTS_BSTR ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, buf, password);
		return result;
	}
	BOOL save_copy(LPCTSTR dest, BOOL remove_sec)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BOOL ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dest, remove_sec);
		return result;
	}
	void set_mark(LPCTSTR img, long x, long y, long w, long h, long alignx, long aligny, long flag)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, img, x, y, w, h, alignx, aligny, flag);
	}
	void set_mark_pages(long start, long count)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms, start, count);
	}
	void clear_mark()
	{
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void set_mark2(LPUNKNOWN str, long x, long y, long w, long h, long alignx, long aligny, long flag)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms, str, x, y, w, h, alignx, aligny, flag);
	}
	BOOL open_stream(LPUNKNOWN str, LPCTSTR password)
	{
		BOOL result;
		static BYTE parms[] = VTS_UNKNOWN VTS_BSTR ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, str, password);
		return result;
	}
	void set_filter_password(LPCTSTR password)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms, password);
	}
	BOOL save_page(LPCTSTR file_name, long pageno, double resx, double resy)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_R8 VTS_R8 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, file_name, pageno, resx, resy);
		return result;
	}
	long save_page_mem(long pageno, double resx, double resy)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pageno, resx, resy);
		return result;
	}
	void release_page_mem(long mem)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mem);
	}

	// IPDFDocument 属性
public:

};
