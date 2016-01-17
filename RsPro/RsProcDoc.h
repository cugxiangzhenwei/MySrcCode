
#if !defined(AFX_RSPROCDOC_H__F64D9AA3_15EF_4074_BA07_15AC1DB9A68F__INCLUDED_)
#define AFX_RSPROCDOC_H__F64D9AA3_15EF_4074_BA07_15AC1DB9A68F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/**
*
*CRsProcDoc 类的定义,继承于CDocument
*
*/
class CRsProcDoc : public CDocument
{
protected: // create from serialization only
	CRsProcDoc();
	DECLARE_DYNCREATE(CRsProcDoc)
public:
  int m_nbands;				//!<...波段数...
  LONG m_nWidth;			//!<...影像宽度...
  LONG m_nHeigth;			//!<...影像高度...
  GDALDataset *poDataset;	//!<...栅格数据集指针...
  GDALRasterBand **pbands;	/**<...指向波段指针数组的指针...*/
  BYTE *pBandBlock[3];		/**<...影像数据块指针...*/
  int max[3];		    	//!<...统计每个波段灰度象元最多的像素的个数...
  int count[3][256];		///<...影像三个波段的灰度数组...
  int max_grey[3];			///<...用于存放各个波段最大灰度值的数组...
  int min_grey[3];			/*!<...用于存放各个波段最小灰度值的数组...*/

	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void Calculate();
	HBITMAP CopyScreenToBitmap(LPRECT lpRect);
	int SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName);

public:
	virtual ~CRsProcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRsProcDoc)
	afx_msg void OnHistShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSPROCDOC_H__F64D9AA3_15EF_4074_BA07_15AC1DB9A68F__INCLUDED_)
