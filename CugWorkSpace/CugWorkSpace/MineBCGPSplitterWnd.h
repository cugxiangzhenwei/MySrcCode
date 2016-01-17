#pragma once
#include <BCGPSplitterWnd.h>


// CCxBCGPSplitterWnd

class CMineBCGPSplitterWnd : public CBCGPSplitterWnd
{
	DECLARE_DYNAMIC(CMineBCGPSplitterWnd)

public:
	CMineBCGPSplitterWnd();
	virtual ~CMineBCGPSplitterWnd();
	virtual void CMineBCGPSplitterWnd::RecalcLayout();

protected:
	DECLARE_MESSAGE_MAP()

	virtual void StartTracking( int ht );

	virtual void StopTracking( BOOL bAccept );

	afx_msg	void OnMouseMove(UINT nFlags, CPoint pt);

	virtual void GetHitRect(int ht, CRect& rect);

};


