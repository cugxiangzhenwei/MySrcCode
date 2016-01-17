#include "stdafx.h"
#include "MineToolBar.h"
#include "UserDefine.h"
extern Bitmap * GetNewBitmap(LPCTSTR lpszPath);

BEGIN_MESSAGE_MAP(CMimeToolBar, CBCGPToolBar)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(CMimeToolBar, CBCGPToolBar, VERSIONABLE_SCHEMA | 1)

CMimeToolBar::CMimeToolBar()
{

}
CMimeToolBar::~CMimeToolBar()
{

}
void CMimeToolBar::LoadUsrImageButton()
{
	m_BCGToolbarImages.SetImageSize( CSize(16, 16) );
	static const LPCTSTR arrImageName[] = {
		"NewProcess.png", "OpenProcess.png", "Save.png", "SaveAs.png", "Info.png", "Publish.png", "DataLinker.png", 
		"RunProcess.png",
		"Monitor.png", "Preview.png", 
		"Help.png"/*, "About.png" */
	};

	CString strToolBarResourceDir  = "C:\\WorkSpace\\Client\\Res\\MainToolbar\\";
	int nCount = sizeof( arrImageName ) / sizeof( LPCTSTR );

	Bitmap * pBitmap = NULL;
	HBITMAP hbmp = NULL;

	for (int i=0; i<nCount; i++)
	{
		pBitmap = GetNewBitmap( strToolBarResourceDir + arrImageName[i] );
		if ( pBitmap == NULL )
			return;

		m_vImages.push_back( pBitmap );
		pBitmap->GetHBITMAP( NULL, &hbmp);
		if ( hbmp == NULL )
			return;

		m_vHBitmaps.push_back( hbmp );
		m_BCGToolbarImages.AddImage( hbmp );
	}

	if ( m_BCGToolbarImages.GetCount() == 0 ) return;

	int nImgPos = m_Images.GetCount() > 0 ? m_Images.GetCount() : 0;
	SetSizes( CSize(32, 22), CSize(16, 16) );
	for (int i=0; i<m_BCGToolbarImages.GetCount(); i++)
	{
		m_Images.AddImage( m_vHBitmaps[i] );
		m_ImagesLocked.AddImage( m_vHBitmaps[i] );
		m_LargeImages.AddImage( m_vHBitmaps[i] );
		m_LargeImagesLocked.AddImage( m_vHBitmaps[i] );
	}

	m_BCGToolbarImages.ConvertTo32Bits();
	m_Images.ConvertTo32Bits();
	m_ImagesLocked.ConvertTo32Bits();
	m_LargeImages.ConvertTo32Bits();
	m_LargeImagesLocked.ConvertTo32Bits();

	LoadButton( IDM_TOOLBAR_BUTTON_FIRST
		, nImgPos
		, m_BCGToolbarImages.GetCount() );
}
void CMimeToolBar::LoadButton( int nFirstCmd, int nFirstImg, int nCount )
{
	RemoveAllButtons ();
	while (!m_OrigButtons.IsEmpty ())
		delete m_OrigButtons.RemoveHead ();

	for (int i=0; i<nCount; i++)
	{
		switch (i)
		{
		case IMG_RUN_PROCESS:
		case IMG_HISTORY_MONITOR:
		case IMG_HELP:
			InsertSeparator( m_Buttons.GetCount());
			break;
		default:
			break;
		}
		m_DefaultImages.SetAt (IDM_TOOLBAR_BUTTON_FIRST + i, nFirstImg + i);

		m_OrigButtons.AddTail (new CBCGPToolbarButton (IDM_TOOLBAR_BUTTON_FIRST + i
			, nFirstImg + i, NULL, TRUE));

		InsertButton (CBCGPToolbarButton (IDM_TOOLBAR_BUTTON_FIRST + i
			, nFirstImg + i
			, NULL
			, FALSE
			, m_bLocked));
	}

	CWnd* pParentFrame = (m_pDockSite == NULL) ?
		GetParent () : m_pDockSite;

	if (pParentFrame != NULL)
	{
		pParentFrame->SendMessage (BCGM_RESETTOOLBAR, (WPARAM) m_uiOriginalResID);

		while (!m_OrigResetButtons.IsEmpty ())
		{
			delete m_OrigResetButtons.RemoveHead ();
		}

		//-------------------------------------------
		//	Store Buttons state after OnToolbarReset
		//-------------------------------------------
		int i = 0;
		for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL; i++)
		{
			CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) m_Buttons.GetNext (pos);

			if(pButton != NULL && pButton->IsKindOf(RUNTIME_CLASS(CBCGPToolbarButton)))
			{
				CRuntimeClass* pRTC = pButton->GetRuntimeClass();
				CBCGPToolbarButton* pBtn = (CBCGPToolbarButton*)pRTC->CreateObject();
				pBtn->CopyFrom(*pButton);
				m_OrigResetButtons.AddTail(pBtn); 

			}
		}
	}
}
//**********************************************************************************
CSize CMimeToolBar::CalcSize (BOOL bVertDock)
{
	if (m_Buttons.IsEmpty ())
	{
		return GetButtonSize ();
	}

	CClientDC dc (this);
	CFont* pOldFont = NULL;
	
	if (!bVertDock)
	{
		pOldFont = SelectDefaultFont (&dc);
	}
	else
	{
		pOldFont = dc.SelectObject (&globalData.fontVert);
	}

	ASSERT (pOldFont != NULL);

	CSize sizeGrid (GetColumnWidth (), GetRowHeight ());
	CSize sizeResult = sizeGrid;

	CRect rect; rect.SetRectEmpty();
	CalcInsideRect (rect, !bVertDock);

	int iStartX = bVertDock ? 0 : 4;
	int iStartY = bVertDock ? 1 : 0;

	CPoint cur (iStartX, iStartY);

	for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) m_Buttons.GetNext (pos);
		if (pButton == NULL)
		{
			break;
		}

		ASSERT_VALID (pButton);

		if (pos == NULL && m_pCustomizeBtn != NULL && IsFloating ())
		{
// 			ASSERT_VALID (m_pCustomizeBtn);
// 			ASSERT (m_pCustomizeBtn == pButton);	// Should be last
			break;
		}

		CSize sizeDefault (sizeGrid.cx, 
				m_bDrawTextLabels ? GetButtonSize ().cy : sizeGrid.cy);
		CSize sizeButton = pButton->OnCalculateSize (&dc, sizeDefault, !bVertDock);

		if (m_bDrawTextLabels)
		{
			sizeButton.cy = m_nMaxBtnHeight;
		}

		if (!bVertDock)
		{
			if ((cur.x == iStartX || pButton->m_bWrap)
				&& pButton->m_nStyle & TBBS_SEPARATOR)
			{
				sizeButton = CSize (0, 0);
			}

			sizeResult.cx = max (cur.x + sizeButton.cx, sizeResult.cx);
			sizeResult.cy = max (cur.y + sizeButton.cy, sizeResult.cy);

			cur.x += sizeButton.cx + 2;

			if (pButton->m_bWrap)
			{
				cur.x = iStartX;
				cur.y += sizeGrid.cy + LINE_OFFSET;
			}
		}
		else
		{
			sizeResult.cx = max (cur.x + sizeButton.cx, sizeResult.cx);
			sizeResult.cy = max (cur.y + sizeButton.cy, sizeResult.cy);

			cur.x = iStartX;
			cur.y += sizeButton.cy;
		}
	}

	dc.SelectObject (pOldFont);
	return sizeResult;
}

//**********************************************************************************
void CMimeToolBar::AdjustLocations ()
{
	ASSERT_VALID(this);

	if (m_Buttons.IsEmpty () || GetSafeHwnd () == NULL)
	{
		return;
	}

	BOOL bHorz = GetCurrentAlignment () & CBRS_ORIENT_HORZ ? TRUE : FALSE;

	CRect rectClient;
	GetClientRect (rectClient);

	int xRight = rectClient.right;

	CClientDC dc (this);
	CFont* pOldFont;
	if (bHorz)
	{
		pOldFont = SelectDefaultFont (&dc);
	}
	else
	{
		pOldFont = (CFont*) dc.SelectObject (&globalData.fontVert);
	}
	
	ASSERT (pOldFont != NULL);

	int iStartOffset;
	if (bHorz)
	{
		iStartOffset = rectClient.left + 1;
	}
	else
	{
		iStartOffset = rectClient.top + 1;
	}

	iStartOffset += 3;
	int iOffset = iStartOffset;
	int y = rectClient.top;

	CSize sizeGrid (GetColumnWidth (), GetRowHeight ());

	CSize sizeCustButton (0, 0);

	BOOL bHideChevronInCustomizeMode = IsCustomizeMode() && DYNAMIC_DOWNCAST(CBCGPReBar, GetParent()) != NULL;

	if (m_pCustomizeBtn != NULL && !IsFloating () && !bHideChevronInCustomizeMode)
	{
// 		ASSERT_VALID (m_pCustomizeBtn);
// 		ASSERT (m_pCustomizeBtn == m_Buttons.GetTail ());	// Should be last

// 		m_pCustomizeBtn->m_lstInvisibleButtons.RemoveAll ();
// 
// 		BOOL bIsEmpty = m_pCustomizeBtn->m_bIsEmpty;
// 		m_pCustomizeBtn->m_bIsEmpty = FALSE;
// 
// 		sizeCustButton = m_pCustomizeBtn->OnCalculateSize (&dc,
// 			CSize (	bHorz ? sizeGrid.cx : rectClient.Width (), 
// 			bHorz ? rectClient.Height () : sizeGrid.cy), bHorz);
// 
// 		m_pCustomizeBtn->m_bIsEmpty = bIsEmpty;
	}

	BOOL bPrevWasSeparator = FALSE;
	int nRowActualWidth = 0;
	for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL;)
	{
		POSITION posSave = pos;

		CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) m_Buttons.GetNext (pos);
		if (pButton == NULL)
		{
			break;
		}

		ASSERT_VALID (pButton);

		BOOL bVisible = TRUE;

		CSize sizeButton = pButton->OnCalculateSize (&dc, sizeGrid, bHorz);
		if (pButton->m_bTextBelow && bHorz)
		{
			sizeButton.cy =  sizeGrid.cy;
		}

		if (pButton->m_nStyle & TBBS_SEPARATOR)
		{
			if (iOffset == iStartOffset || bPrevWasSeparator)
			{
				sizeButton = CSize (0, 0);
				bVisible = FALSE;
			}
			else
			{
				bPrevWasSeparator = TRUE;
			}
		}

		int iOffsetPrev = iOffset;

		CRect rectButton;
		if (bHorz)
		{
			rectButton.left = iOffset;
			rectButton.right = rectButton.left + sizeButton.cx;
			rectButton.top = y;
			rectButton.bottom = rectButton.top + sizeButton.cy;
			
			iOffset += sizeButton.cx + 2;
			nRowActualWidth += sizeButton.cx;
		}
		else
		{
			rectButton.left = rectClient.left;
			rectButton.right = rectClient.left + sizeButton.cx;
			rectButton.top = iOffset;
			rectButton.bottom = iOffset + sizeButton.cy;

			iOffset += sizeButton.cy;
		}

// 		if (m_pCustomizeBtn != NULL && pButton != m_pCustomizeBtn &&
// 			!IsFloating () && !IsCustomizeMode ())
// 		{
// 			CSize fakeSizeCustButton (sizeCustButton);
// 			class CCustomizeButton;
			//-------------------------------------------------------------------------
			// I assume, that the customize button is at the tail position at any time.
			//-------------------------------------------------------------------------
// 			if ((int) m_pCustomizeBtn->m_uiCustomizeCmdId <= 0 &&
// 				(pos != NULL && m_Buttons.GetAt (pos) == m_pCustomizeBtn) && 
// 				m_pCustomizeBtn->m_lstInvisibleButtons.IsEmpty ())
// 			{
// 				fakeSizeCustButton = CSize (0,0);
// 			}
// 
// 			if ((bHorz && rectButton.right > xRight - fakeSizeCustButton.cx) ||
// 				(!bHorz && rectButton.bottom > rectClient.bottom - fakeSizeCustButton.cy))
// 			{
// 				bVisible = FALSE;
// 				iOffset = iOffsetPrev;
// 				
// 				m_pCustomizeBtn->m_lstInvisibleButtons.AddTail (pButton);
// 			}
//		}

		pButton->Show (bVisible);
		pButton->SetRect (rectButton);

		if (bVisible)
		{
			bPrevWasSeparator = (pButton->m_nStyle & TBBS_SEPARATOR);
		}

		if ((pButton->m_bWrap || pos == NULL) && bHorz)
		{
			//-----------------------
			// Center buttons in row:
			//-----------------------
			int nShift = (xRight - nRowActualWidth - iStartOffset) / 2;
			if (IsFloating () && nShift > 0 && m_bTextLabels)
			{
				for (POSITION posRow = posSave; posRow != NULL;)
				{
					BOOL bThis = (posRow == posSave);

					CBCGPToolbarButton* pButtonRow = (CBCGPToolbarButton*) m_Buttons.GetPrev (posRow);
					ASSERT (pButtonRow != NULL);

					if (pButtonRow->m_bWrap && !bThis)
					{
						break;
					}

					CRect rect = pButtonRow->Rect ();
					rect.OffsetRect (nShift, 0);
					pButtonRow->SetRect (rect);
				}
			}

			iOffset = iStartOffset;
			nRowActualWidth = 0;
			y += sizeGrid.cy + LINE_OFFSET;
		}
	}

	if (m_pCustomizeBtn != NULL)
	{
		CRect rectButton = rectClient;

// 		if ((int) m_pCustomizeBtn->m_uiCustomizeCmdId <= 0 &&
// 			m_pCustomizeBtn->m_lstInvisibleButtons.IsEmpty () || IsFloating () || bHideChevronInCustomizeMode)
// 		{
// 			// Hide customize button:
// 			m_pCustomizeBtn->SetRect (CRect (0, 0, 0, 0));
// 			m_pCustomizeBtn->Show (FALSE);
// 		}
// 		else
		{
			if (bHorz)
			{
				rectButton.right = xRight - 1;
				rectButton.left = rectButton.right - sizeCustButton.cx + 1;
			}
			else
			{
				rectButton.bottom --;
				rectButton.top = rectButton.bottom - sizeCustButton.cy;
			}

// 			m_pCustomizeBtn->SetRect (rectButton);
// 			m_pCustomizeBtn->Show (TRUE);
		}
	}

	dc.SelectObject (pOldFont);
	UpdateTooltips ();
	RedrawCustomizeButton ();
}
BOOL CMimeToolBar::OnUserToolTip (CBCGPToolbarButton* pButton, CString& strTTText) const
{
	int iID = pButton->m_nID - IDM_TOOLBAR_BUTTON_FIRST;
	static CString strToolTipText[] = {
		"新建设计过程",
		"打开设计过程",
		"保存",
		"另存为",
		"属性",
		"发布",
		"数据映射",
		"运行设计过程",
		"作业管理",
		"分布式资源管理",
		"帮助"
	};

	strTTText = strToolTipText[iID];
	return TRUE;
}