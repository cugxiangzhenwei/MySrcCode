#pragma once
#include <BCGPToolBar.h>
#include <vector>
#include <GdiplusBitmap.h>
using namespace std;
using namespace Gdiplus;
class CMimeToolBar:public CBCGPToolBar
{
	DECLARE_SERIAL(CMimeToolBar)
public:
	enum {
		IMG_NEW_PROCESS,
		IMG_OPEN_PROCESS,
		IMG_SAVE,
		IMG_SAVEAS,
		IMG_INFO,
		IMG_PUBLISH,
		IMG_DATALINKER,
		//------------
		IMG_RUN_PROCESS,
		//------------
		IMG_HISTORY_MONITOR,
		IMG_PREVIEW,
		//---------------
		IMG_HELP,
		//IMG_ABOUT
	};

	CMimeToolBar();
	virtual ~CMimeToolBar();
	void LoadUsrImageButton();
private:
	vector< Bitmap * > m_vImages;
	vector< HBITMAP > m_vHBitmaps;
	CBCGPToolBarImages m_BCGToolbarImages;
	void LoadButton( int nFirstCmd, int nFirstImg, int nCount );

	virtual void AdjustLocations ();
	virtual CSize CalcSize (BOOL bVertDock);
	virtual BOOL OnUserToolTip (CBCGPToolbarButton* pButton, CString& strTTText) const;
protected:
	DECLARE_MESSAGE_MAP()
};