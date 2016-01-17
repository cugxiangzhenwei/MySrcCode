// CDCAJAX.h : 由 Microsoft Visual C++ 创建的 ActiveX 控件包装类的声明

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDCAJAX

class CDCAJAX : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDCAJAX)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x78E87ACB, 0x656E, 0x4257, { 0x96, 0x1D, 0x3F, 0xAD, 0xBE, 0x77, 0xA6, 0x26 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 特性
public:

// 操作
public:

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL Open(LPCTSTR lpszFileName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, lpszFileName);
		return result;
	}
	void Close()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long GetPageCount()
	{
		long result;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void GetPageSize(long index, VARIANT * cx, VARIANT * cy)
	{
		static BYTE parms[] = VTS_I4 VTS_PVARIANT VTS_PVARIANT ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index, cx, cy);
	}
	void TurnToPage(long index, long type)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index, type);
	}
	void Zoom(long type, float num)
	{
		static BYTE parms[] = VTS_I4 VTS_R4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, type, num);
	}
	void SetPageBrowseType(long type)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, type);
	}
	long GetCurPage()
	{
		long result;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPUNKNOWN GetHotItems()
	{
		LPUNKNOWN result;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	LPUNKNOWN GetCatalog()
	{
		LPUNKNOWN result;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	LPUNKNOWN XObjectFromPoint(long x, long y, VARIANT * type)
	{
		LPUNKNOWN result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, parms, x, y, type);
		return result;
	}
	void GetViewPage(VARIANT * first, VARIANT * last)
	{
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, first, last);
	}
	long GetViewHwnd()
	{
		long result;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetViewMessageHook(long hookproc)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hookproc);
	}
	void Rotate(long index, long angle)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index, angle);
	}
	void GetScrollInfo(VARIANT * cx, VARIANT * cy, VARIANT * xPage, VARIANT * yPage)
	{
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms, cx, cy, xPage, yPage);
	}
	void SetScrollPos(long bar, long pos)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bar, pos);
	}
	long GetScrollPos(long bar)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bar);
		return result;
	}
	void GetPageRect(long index, VARIANT * left, VARIANT * right, VARIANT * top, VARIANT * bottom)
	{
		static BYTE parms[] = VTS_I4 VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index, left, right, top, bottom);
	}
	BOOL AddNote(long type, LPUNKNOWN * ppUnkown)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_PUNKNOWN ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, type, ppUnkown);
		return result;
	}
	BOOL Copy()
	{
		BOOL result;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString OCR()
	{
		CString result;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long GetSelected(LPUNKNOWN * ppUnkown)
	{
		long result;
		static BYTE parms[] = VTS_PUNKNOWN ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ppUnkown);
		return result;
	}
	void DeleteNote(LPUNKNOWN * pval, long lCount)
	{
		static BYTE parms[] = VTS_PUNKNOWN VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pval, lCount);
	}
	LPUNKNOWN GetNoteItems()
	{
		LPUNKNOWN result;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	BOOL Save()
	{
		BOOL result;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPUNKNOWN Search(LPCTSTR keyword, long page)
	{
		LPUNKNOWN result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, parms, keyword, page);
		return result;
	}
	void GotoSearchResult(long index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index);
	}
	void GotoNote(LPUNKNOWN pval)
	{
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pval);
	}
	BOOL GetSysMetrics(LPCTSTR paramname, VARIANT * value)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_PVARIANT ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, paramname, value);
		return result;
	}
	BOOL SetSysMetrics(LPCTSTR paramname, VARIANT * value)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_PVARIANT ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, paramname, value);
		return result;
	}
	void RefreshDisplay()
	{
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL SendSelectionToWord()
	{
		BOOL result;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPUNKNOWN GetNoteSets()
	{
		LPUNKNOWN result;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	void SwitchNoteSet(LPUNKNOWN pval)
	{
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pval);
	}
	CString ExportNoteSet(LPUNKNOWN pNoteSet, LPCTSTR bstrPages, LPCTSTR bstrAuthor, long lType)
	{
		CString result;
		static BYTE parms[] = VTS_UNKNOWN VTS_BSTR VTS_BSTR VTS_I4 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, pNoteSet, bstrPages, bstrAuthor, lType);
		return result;
	}
	BOOL UpdateNoteSet(LPUNKNOWN pNoteSet)
	{
		BOOL result;
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, pNoteSet);
		return result;
	}
	LPUNKNOWN GetDictClass()
	{
		LPUNKNOWN result;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	BOOL SwitchDict(long id)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, id);
		return result;
	}
	BOOL DeleteNoteSet(LPUNKNOWN pval, BOOL Force)
	{
		BOOL result;
		static BYTE parms[] = VTS_UNKNOWN VTS_BOOL ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, pval, Force);
		return result;
	}
	BOOL ImportNoteSet(LPCTSTR bstrSrc, BOOL IsFileName, LPUNKNOWN pNotSet, BOOL AsNoteSet, BOOL ForceImport)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_UNKNOWN VTS_BOOL VTS_BOOL ;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bstrSrc, IsFileName, pNotSet, AsNoteSet, ForceImport);
		return result;
	}
	CString ExportNoteItem(LPUNKNOWN pUnk)
	{
		CString result;
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, pUnk);
		return result;
	}
	LPUNKNOWN ImportNoteItem(LPCTSTR bstrXML)
	{
		LPUNKNOWN result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, parms, bstrXML);
		return result;
	}
	BOOL DeleteNoteItem(LPCTSTR bstrID, long nPage)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bstrID, nPage);
		return result;
	}
	CString GetDOI()
	{
		CString result;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long GetPageBrowseType()
	{
		long result;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetPreURL(LPCTSTR lpszPreURL)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lpszPreURL);
	}
	void SetURLEvent(BOOL bURLEvent)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bURLEvent);
	}
	void SetModified(BOOL bModified)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bModified);
	}
	void SetReadOnly(BOOL bReadOnly)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bReadOnly);
	}
	LPUNKNOWN AddNoteEx(long type)
	{
		LPUNKNOWN result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, parms, type);
		return result;
	}
	long GetSelectedType()
	{
		long result;
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPUNKNOWN GetSelectedEx()
	{
		LPUNKNOWN result;
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	CString GetNoteFileName()
	{
		CString result;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL NoteSetCanEdit()
	{
		BOOL result;
		InvokeHelper(0x3e, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL Print()
	{
		BOOL result;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString MARK(LPCTSTR bstrMark, long lStartPage, long lColor)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 ;
		InvokeHelper(0x40, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, bstrMark, lStartPage, lColor);
		return result;
	}
	void DeleteAllMark()
	{
		InvokeHelper(0x41, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPUNKNOWN GetSelectRect()
	{
		LPUNKNOWN result;
		InvokeHelper(0x42, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	BOOL ExportNoteItemEx(LPUNKNOWN pUnk, LPUNKNOWN pStream)
	{
		BOOL result;
		static BYTE parms[] = VTS_UNKNOWN VTS_UNKNOWN ;
		InvokeHelper(0x43, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, pUnk, pStream);
		return result;
	}
	LPUNKNOWN ImportNoteItemEx(LPUNKNOWN pStream)
	{
		LPUNKNOWN result;
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0x44, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, parms, pStream);
		return result;
	}
	void SelectNoteItem(LPUNKNOWN pval, BOOL bSelect, BOOL bGoto)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_BOOL VTS_BOOL ;
		InvokeHelper(0x45, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pval, bSelect, bGoto);
	}
	CString GetPageText(long nPage)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x46, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nPage);
		return result;
	}
	void SetNoteAuthor(LPCTSTR lpszNoteAuthor)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lpszNoteAuthor);
	}
	BOOL SetNoteItemEditing(LPUNKNOWN pUnk, BOOL bEditing, LPCTSTR lpszEditingMsg)
	{
		BOOL result;
		static BYTE parms[] = VTS_UNKNOWN VTS_BOOL VTS_BSTR ;
		InvokeHelper(0x4b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, pUnk, bEditing, lpszEditingMsg);
		return result;
	}
	void Undo(long lStepCount)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lStepCount);
	}
	void Redo(long lStepCount)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lStepCount);
	}
	long GetUndoSize()
	{
		long result;
		InvokeHelper(0x4e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetRedoSize()
	{
		long result;
		InvokeHelper(0x4f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetUndoActionName(long lUndoActionIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x50, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, lUndoActionIndex);
		return result;
	}
	CString GetRedoActionName(long lRedoActionIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x51, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, lRedoActionIndex);
		return result;
	}
	void Backward()
	{
		InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Forward()
	{
		InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL IsBackwardEnable()
	{
		BOOL result;
		InvokeHelper(0x54, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL IsForwardEnable()
	{
		BOOL result;
		InvokeHelper(0x55, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL SaveInFile()
	{
		BOOL result;
		InvokeHelper(0x56, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL PrintEx(VARIANT * value, long lPrintType)
	{
		BOOL result;
		static BYTE parms[] = VTS_PVARIANT VTS_I4 ;
		InvokeHelper(0x58, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, value, lPrintType);
		return result;
	}
	BOOL DBLClickToZoomPage(long lType)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x59, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, lType);
		return result;
	}
	void InitLineWidth(short nWidth)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x5a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nWidth);
	}
	void InitLineColor(long lColor)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lColor);
	}
	void InitLineTransparent(BOOL bTransparent)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bTransparent);
	}
	void SetNoteItemAnchorTitle(LPUNKNOWN pUnk, LPCTSTR pszTitle, long lFontSize, long lTransparency, long color, long bkcolor, BOOL bShow)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x5f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pUnk, pszTitle, lFontSize, lTransparency, color, bkcolor, bShow);
	}
	void FullScreen(BOOL bFullScreen)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x66, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bFullScreen);
	}
	void TurnToPageEx(long index, long x, long y, BOOL bIsPercent)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index, x, y, bIsPercent);
	}
	long GetPagePos(long sx, long sy, VARIANT * px, VARIANT * py)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PVARIANT VTS_PVARIANT ;
		InvokeHelper(0x6a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sx, sy, px, py);
		return result;
	}
	long GetScaleMode()
	{
		long result;
		InvokeHelper(0x6b, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL PageIsReady(long PageIndex)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, PageIndex);
		return result;
	}
	BOOL GetUserInfo()
	{
		BOOL result;
		InvokeHelper(0x80, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void BeginMark(LPWSTR lpszFirstText, LPWSTR lpszSecondText)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_UNKNOWN ;
		InvokeHelper(0x81, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lpszFirstText, lpszSecondText);
	}
	void EndMark()
	{
		InvokeHelper(0x82, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}


};
