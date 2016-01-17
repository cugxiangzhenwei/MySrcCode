#ifndef COfficeXPMenu_H
#define COfficeXPMenu_H

#include <afxtempl.h>

class COfficeXPMenuData
{
	wchar_t *m_szMenuText;
public:
	COfficeXPMenuData () {menuIconNormal=-1;xoffset=-1;bitmap=NULL;pContext=NULL;
	nFlags=0;nID=0;syncflag=0;m_szMenuText=NULL;};
	void SetAnsiString(LPCSTR szAnsiString);
	void SetWideString(const wchar_t *szWideString);
	const wchar_t *GetWideString(void) {return m_szMenuText;};
	~COfficeXPMenuData ();
	CString GetString(void);//returns the menu text in ANSI or UNICODE
	int xoffset;
	int menuIconNormal;
	UINT nFlags,nID,syncflag;
	CImageList *bitmap;
	void *pContext; // used to attach user data
};

struct COfficeXPMenuInfo : public 
								#ifndef UNICODE 
								tagMENUITEMINFOA
								#else
								tagMENUITEMINFOW
								#endif
{
	COfficeXPMenuInfo()
	{
		memset(this, 0, sizeof(MENUITEMINFO));
		cbSize = sizeof(MENUITEMINFO);
	}
};

typedef enum 
{
	TYPE_NORMAL,
	TYPE_XP
} BC_MenuDrawModeOff;

typedef enum 
{
	COfficeXPMenu_NONE,
	COfficeXPMenu_HEAD, 
	COfficeXPMenu_TAIL,
	COfficeXPMenu_BOTH
} BC_SeperatorOff;

#ifndef UNICODE
#define AppendMenu AppendMenuA
#define InsertMenu InsertMenuA
#define InsertODMenu InsertODMenuA
#define AppendODMenu AppendODMenuA
#define AppendODPopupMenu AppendODPopupMenuA
#define ModifyODMenu ModifyODMenuA
#else
#define AppendMenu AppendMenuW
#define InsertMenu InsertMenuW
#define InsertODMenu InsertODMenuW
#define AppendODMenu AppendODMenuW
#define ModifyODMenu ModifyODMenuW
#define AppendODPopupMenu AppendODPopupMenuW
#endif


class COfficeXPMenu : public CMenu
{

public:
	COfficeXPMenu(); 
	virtual ~COfficeXPMenu();

	virtual BOOL LoadMenu(LPCTSTR lpszResourceName);
	virtual BOOL LoadMenu(int nResource);
	BOOL LoadToolbar(UINT nToolBar);
	BOOL LoadToolbars(const UINT *arID,int n);
	void AddFromToolBar(CToolBar* pToolBar, int nResourceID);
	BOOL LoadFromToolBar(UINT nID,UINT nToolBar,int& xoffset);
	BOOL AddBitmapToImageList(CImageList *list,UINT nResourceID);
	static HBITMAP LoadSysColorBitmap(int nResourceId);
	void LoadCheckmarkBitmap(int unselect,int select); // custom check mark bitmaps
	
	BOOL AppendMenuA(UINT nFlags,UINT nIDNewItem=0,const char *lpszNewItem=NULL,int nIconNormal=-1);
	BOOL AppendMenuA(UINT nFlags,UINT nIDNewItem,const char *lpszNewItem,CImageList *il,int xoffset);
	BOOL AppendMenuA(UINT nFlags,UINT nIDNewItem,const char *lpszNewItem,CBitmap *bmp);
	BOOL AppendMenuW(UINT nFlags,UINT nIDNewItem=0,wchar_t *lpszNewItem=NULL,int nIconNormal=-1);
	BOOL AppendMenuW(UINT nFlags,UINT nIDNewItem,wchar_t *lpszNewItem,CImageList *il,int xoffset);
	BOOL AppendMenuW(UINT nFlags,UINT nIDNewItem,wchar_t *lpszNewItem,CBitmap *bmp);
	BOOL AppendODMenuA(LPCSTR lpstrText,UINT nFlags = MF_OWNERDRAW,UINT nID = 0,int nIconNormal = -1);  
	BOOL AppendODMenuW(wchar_t *lpstrText,UINT nFlags = MF_OWNERDRAW,UINT nID = 0,int nIconNormal = -1);  
	BOOL AppendODMenuA(LPCSTR lpstrText,UINT nFlags,UINT nID,CImageList *il,int xoffset);
	BOOL AppendODMenuW(wchar_t *lpstrText,UINT nFlags,UINT nID,CImageList *il,int xoffset);
	
	COfficeXPMenu* AppendODPopupMenuA(LPCSTR lpstrText);
	COfficeXPMenu* AppendODPopupMenuW(wchar_t *lpstrText);

	BOOL InsertMenuA(UINT nPosition,UINT nFlags,UINT nIDNewItem=0,const char *lpszNewItem=NULL,int nIconNormal=-1);
	BOOL InsertMenuA(UINT nPosition,UINT nFlags,UINT nIDNewItem,const char *lpszNewItem,CImageList *il,int xoffset);
	BOOL InsertMenuA(UINT nPosition,UINT nFlags,UINT nIDNewItem,const char *lpszNewItem,CBitmap *bmp);
	BOOL InsertMenuW(UINT nPosition,UINT nFlags,UINT nIDNewItem=0,wchar_t *lpszNewItem=NULL,int nIconNormal=-1);
	BOOL InsertMenuW(UINT nPosition,UINT nFlags,UINT nIDNewItem,wchar_t *lpszNewItem,CImageList *il,int xoffset);
	BOOL InsertMenuW(UINT nPosition,UINT nFlags,UINT nIDNewItem,wchar_t *lpszNewItem,CBitmap *bmp);
	BOOL InsertODMenuA(UINT nPosition,LPCSTR lpstrText,UINT nFlags = MF_OWNERDRAW,UINT nID = 0,int nIconNormal = -1); 
	BOOL InsertODMenuW(UINT nPosition,wchar_t *lpstrText,UINT nFlags = MF_OWNERDRAW,UINT nID = 0,int nIconNormal = -1);  
	BOOL InsertODMenuA(UINT nPosition,LPCSTR lpstrText,UINT nFlags,UINT nID,CImageList *il,int xoffset);
	BOOL InsertODMenuW(UINT nPosition,wchar_t *lpstrText,UINT nFlags,UINT nID,CImageList *il,int xoffset);
	
	BOOL ModifyODMenuA(const char *lpstrText,UINT nID=0,int nIconNormal=-1);
	BOOL ModifyODMenuA(const char *lpstrText,UINT nID,CImageList *il,int xoffset);
	BOOL ModifyODMenuA(const char *lpstrText,UINT nID,CBitmap *bmp);
	BOOL ModifyODMenuA(const char *lpstrText,const char *OptionText,int nIconNormal);
	BOOL ModifyODMenuW(wchar_t *lpstrText,UINT nID=0,int nIconNormal=-1);
	BOOL ModifyODMenuW(wchar_t *lpstrText,UINT nID,CImageList *il,int xoffset);
	BOOL ModifyODMenuW(wchar_t *lpstrText,UINT nID,CBitmap *bmp);
	BOOL ModifyODMenuW(wchar_t *lpstrText,wchar_t *OptionText,int nIconNormal);

	BOOL ModifyODMenuA(const char *lpstrText,UINT nID,COLORREF fill,COLORREF border,int hatchstyle=-1);
	BOOL ModifyODMenuW(wchar_t *lpstrText,UINT nID,COLORREF fill,COLORREF border,int hatchstyle=-1);
	

	BOOL	RemoveMenu(UINT uiId,UINT nFlags);
	BOOL	DeleteMenu(UINT uiId,UINT nFlags);

	int RemoveMenu(char* pText, BC_SeperatorOff sPos=COfficeXPMenu_NONE);
	int RemoveMenu(wchar_t* pText, BC_SeperatorOff sPos=COfficeXPMenu_NONE);
	

	virtual BOOL DestroyMenu();


	BOOL GetMenuText(UINT id,CString &string,UINT nFlags = MF_BYPOSITION);
	BOOL SetMenuText(UINT id,CString string, UINT nFlags = MF_BYPOSITION);

	CMenu* GetSubMenu (LPCTSTR lpszSubMenuName);
	CMenu* GetSubMenu (int nPos);
	int GetMenuPosition(char* pText);
	int GetMenuPosition(wchar_t* pText);


	virtual void DrawItem( LPDRAWITEMSTRUCT);  // Draw an item
	virtual void MeasureItem( LPMEASUREITEMSTRUCT );  // Measure an item

	static void UpdateMenu(CMenu *pmenu);
	static BOOL IsMenu(CMenu *submenu);
	static BOOL IsMenu(HMENU submenu);
	static LRESULT FindKeyboardShortcut(UINT nChar,UINT nFlags,CMenu *pMenu);


	static void SetType(UINT mode){
		COfficeXPMenu::original_drawmode=mode;
		COfficeXPMenu::xp_drawmode=mode;
	};

	static void SetSelectDisableMode(BOOL mode)
	{
		COfficeXPMenu::original_select_disabled=mode;
		COfficeXPMenu::xp_select_disabled=mode;
	};
	static int COfficeXPMenu::GetMenuDrawMode(void);


	void SetIconSize (int, int); 

	void SetBitmapBackground(COLORREF color);
	void UnSetBitmapBackground(void);

	BOOL GetDisableOldStyle(void);
	void SetDisableOldStyle(void);
	void UnSetDisableOldStyle(void);


protected:
	static BOOL IsNewShell(void);
	COfficeXPMenuData *COfficeXPMenu::FindMenuItem(UINT nID);
	COfficeXPMenu *FindMenuOption(int nId,int& nLoc);
	COfficeXPMenu *FindAnotherMenuOption(int nId,int& nLoc,CArray<COfficeXPMenu*,COfficeXPMenu*>&bcsubs,
								  CArray<int,int&>&bclocs);
	COfficeXPMenuData *FindMenuOption(wchar_t *lpstrText);
	void InsertSpaces(void);
	void DrawCheckMark(CDC *pDC,int x,int y,COLORREF color);
	void DrawRadioDot(CDC *pDC,int x,int y,COLORREF color);
	COfficeXPMenuData *NewODMenu(UINT pos,UINT nFlags,UINT nID,CString string);
	void SynchronizeMenu(void);
	void COfficeXPMenu::InitializeMenuList(int value);
	void COfficeXPMenu::DeleteMenuList(void);
	COfficeXPMenuData *COfficeXPMenu::FindMenuList(UINT nID);
	void DrawItem_Win9xNT2000 (LPDRAWITEMSTRUCT lpDIS);
	void DrawItem_WinXP (LPDRAWITEMSTRUCT lpDIS);
	BOOL Draw3DCheckmark(CDC *dc, const CRect& rc,BOOL bSelected,HBITMAP hbmCheck);
	BOOL DrawXPCheckmark(CDC *dc, const CRect& rc, HBITMAP hbmCheck,COLORREF &colorout);
	void DitherBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, 
		int nHeight, HBITMAP hbm, int nXSrc, int nYSrc,COLORREF bgcolor);
	void DitherBlt2(CDC *drawdc, int nXDest, int nYDest, int nWidth, 
		int nHeight, CBitmap &bmp, int nXSrc, int nYSrc,COLORREF bgcolor);
	BOOL GetBitmapFromImageList(CDC* pDC,CImageList *imglist,int nIndex,CBitmap &bmp);
	BOOL ImageListDuplicate(CImageList *il,int xoffset,CImageList *newlist);
	static COLORREF LightenColor(COLORREF col,double factor);
	static WORD NumBitmapColors(LPBITMAPINFOHEADER lpBitmap);
	void ColorBitmap(CDC* pDC, CBitmap& bmp,CSize size,COLORREF fill,COLORREF border,int hatchstyle=-1);
	

protected:
	CTypedPtrArray<CPtrArray, COfficeXPMenuData*> m_MenuList;  // Stores list of menu items 

	CTypedPtrArray<CPtrArray, HMENU>  m_SubMenus;  // Stores list of sub-menus 

	static CTypedPtrArray<CPtrArray, HMENU>  m_AllSubMenus;
	int m_iconX,m_iconY;
	COLORREF m_bitmapBackground;
	BOOL m_bitmapBackgroundFlag;
	BOOL disable_old_style;
	static UINT original_drawmode,xp_drawmode;
	static BOOL xp_select_disabled,original_select_disabled;
	CImageList *checkmaps;
	BOOL checkmapsshare;
	int m_selectcheck;
	int m_unselectcheck;
	BOOL m_bDynIcons;
}; 

#define COfficeXPMenu_USE_MEMDC

#ifdef COfficeXPMenu_USE_MEMDC



class COfficeXPMenuMemDC : public CDC
{
public:


    COfficeXPMenuMemDC(CDC* pDC,LPCRECT lpSrcRect) : CDC()
    {
        ASSERT(pDC != NULL);

		m_rect.CopyRect(lpSrcRect);
        m_pDC = pDC;
        m_pOldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();
              
        if (m_bMemDC)    // Create a Memory DC
        {
            CreateCompatibleDC(pDC);
            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
            m_pOldBitmap = SelectObject(&m_bitmap);
            SetWindowOrg(m_rect.left, m_rect.top);
        }
        else        // Make a copy of the relevent parts of the current DC for printing
        {
            m_bPrinting = pDC->m_bPrinting;
            m_hDC       = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }
    }
    

    ~COfficeXPMenuMemDC()
    {
        if (m_bMemDC) 
        {    

            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                          this, m_rect.left, m_rect.top, SRCCOPY);


            SelectObject(m_pOldBitmap);
        } else {

            m_hDC = m_hAttribDC = NULL;
        }
    }


    COfficeXPMenuMemDC* operator->() {return this;}
        

    operator COfficeXPMenuMemDC*() {return this;}

private:
    CBitmap  m_bitmap;      // Offscreen bitmap
    CBitmap* m_pOldBitmap;  // bitmap originally found in COfficeXPMenuMemDC
    CDC*     m_pDC;         // Saves CDC passed in constructor
    CRect    m_rect;        // Rectangle of drawing area.
    BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
};

#endif

#endif

//*************************************************************************
