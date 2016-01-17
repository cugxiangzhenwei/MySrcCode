/************************************************************************ 
* 文件名：    SkinTab.h 
* 文件描述：  图片TAB 
* 创建人：    黄锐坤(Nuk), 2006年08月28日
* 版本号：    1.0 
************************************************************************/ 
#pragma once

#define WM_TAB_SELECT WM_USER+104

#define D_TOP   2
#define D_LEFT  4
// CSkinTab
#define  MAX_TEXT_COUNT 8

typedef struct _DLG_COLOR_SKIN
{
    int m_ColorR;
	int m_ColorG;
	int m_ColorB;
	int m_ColorFlag;
	int R_OFFSET ;
	int G_OFFSET ;
	int B_OFFSET ;
}DLG_COLOR_SKIN , *pDLG_COLOR_SKIN;


class CSkinTab : public CStatic
{
	DECLARE_DYNAMIC(CSkinTab)

public:
	CSkinTab();
	virtual ~CSkinTab();
    void SetSelTab();
	void SetSkinColor(DLG_COLOR_SKIN szDLG_COLOR_SKIN);
	void SetTextFile(char *pText,int nCount);
private:
	//色彩
	DLG_COLOR_SKIN m_szTab_COLOR;
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
protected:
	CRect	*m_pTabRect;
	int		m_nTabCount;
	int		m_nCurDownIndex;
	int		m_nCurOverIndex;
	LPTRACKMOUSEEVENT   lpET;
	CBitmap *m_pTabBmpNormal;
	CBitmap *m_pTabBmpOver;
	CBitmap *m_pTabBmpDown;
	HICON   *m_phIcon; 
	bool    m_bDCStored;
	CDC     m_memDC;
	char    m_TextTitle[MAX_TEXT_COUNT][64];
protected:
	int GetBitmapWidth (HBITMAP hBitmap);
	int GetBitmapHeight (HBITMAP hBitmap);
	void DrawBitmap(CDC* dc, HBITMAP hbmp, CRect rect);
	void DrawIcon(CDC* dc, HICON hIcon, CRect rect, UINT align);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	void SetCount(int nCount);
	void SetRect(int nIndex, CRect rc);
	void SetSkin(int nIndex, UINT nNormalID, UINT nOverID, UINT nDownID);	
	void SetSkin(int nIndex, CString strNormal, CString strOver, CString strDown);	
	void SetIcon(int nIndex, CString strIcon);
	void SetIcon(int nIndex, UINT nIconID);

private:
	void SetBackImage(int index,CString strName);
    void DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc);
	void SetImageSize(CBitmap *pBitmap, CSize &sz);
	void SetImage(CBitmap &bitmap, CString strName);
private:
	CBitmap m_BitmapBack[2];  //正常背景,选中背景
	CSize   m_szBitmapBack[2];
};


