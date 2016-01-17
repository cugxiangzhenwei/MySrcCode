#pragma once
#include <BCGPMenuBar.h>
#include <vector>
#include <GdiplusBitmap.h>
using namespace std;
using namespace Gdiplus;
// CCxBCGPMenuBar

class CMineBCGPMenuBar : public CBCGPMenuBar
{
	DECLARE_SERIAL(CMineBCGPMenuBar)

public:
	CMineBCGPMenuBar();
	virtual ~CMineBCGPMenuBar();
public:

	enum {
		IMG_NEW_PROCESS,
		IMG_OPEN_PROCESS,
		IMG_SAVE,
		IMG_DATALINKER,
		//------------
		IMG_RUN_PROCESS,
		//------------
		IMG_HISTORY_MONITOR,
		IMG_PREVIEW,
		//---------------
		//IMG_ABOUT,
		IMG_HELP,
		IMG_COMPONENT,
		IMG_SAVEAS,
		IMG_CLOSE,
		IMG_INFO,
		IMG_PUBLISH,
		IMG_EXIT
	};

	void LoadData();
	void ClearData();

	CBCGPToolBarImages m_BCGToolbarImages;
	vector< Bitmap * > m_vImages;
	vector< HBITMAP > m_vHBitmaps;

	static LPCTSTR m_strHotKey;

protected:
	DECLARE_MESSAGE_MAP()

};


