// CxBCGPSplitterWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "MineBCGPSplitterWnd.h"

static enum HitTestValue
{
	noHit                   = 0,
	vSplitterBox            = 1,
	hSplitterBox            = 2,
	bothSplitterBox         = 3,        // just for keyboard
	vSplitterBar1           = 101,
	vSplitterBar15          = 115,
	hSplitterBar1           = 201,
	hSplitterBar15          = 215,
	splitterIntersection1   = 301,
	splitterIntersection225 = 525
};

// CCxBCGPSplitterWnd

BEGIN_MESSAGE_MAP(CMineBCGPSplitterWnd, CBCGPSplitterWnd)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CMineBCGPSplitterWnd, CBCGPSplitterWnd)

CMineBCGPSplitterWnd::CMineBCGPSplitterWnd()
{
	m_cxSplitter = 7;    //must >=4 ,拖动时拖动条的宽度 
	m_cySplitter = 4;   
}

CMineBCGPSplitterWnd::~CMineBCGPSplitterWnd()
{
}

void CMineBCGPSplitterWnd::RecalcLayout()
{
	CBCGPSplitterWnd::RecalcLayout();

	CRect rcView;
	CWnd* pWnd = GetDlgItem(IdFromRowCol(0,0));
	pWnd->GetWindowRect(&rcView);
	ScreenToClient(&rcView);
	rcView.DeflateRect(-2, 0, 1, -2);
	pWnd->MoveWindow(rcView);

	pWnd = GetDlgItem(IdFromRowCol(0,1));
	pWnd->GetWindowRect(&rcView);
	ScreenToClient(&rcView);
	rcView.DeflateRect(-2, 0, -2, -2);
	pWnd->MoveWindow(rcView);
}

void CMineBCGPSplitterWnd::StartTracking( int ht )
{
	//Splitter 不能有 WS_CLIPCHILDREN 属性
	ModifyStyle(WS_CLIPCHILDREN, 0);
	CBCGPSplitterWnd::StartTracking(ht);
}

void CMineBCGPSplitterWnd::StopTracking( BOOL bAccept )
{
	CBCGPSplitterWnd::StopTracking(bAccept);
	ModifyStyle(0, WS_CLIPCHILDREN);
}


void CMineBCGPSplitterWnd::OnMouseMove(UINT /*nFlags*/, CPoint pt)
{
	if (GetCapture() != this)
		StopTracking(FALSE);

	if (m_bTracking)
	{
		// move tracker to current cursor position

		pt.Offset(m_ptTrackOffset); // pt is the upper right of hit detect
		// limit the point to the valid split range
		if (pt.y < m_rectLimit.top)
			pt.y = m_rectLimit.top;
		else if (pt.y > m_rectLimit.bottom)
			pt.y = m_rectLimit.bottom;
		if (pt.x < m_rectLimit.left)
			pt.x = m_rectLimit.left;
		else if (pt.x > m_rectLimit.right)
			pt.x = m_rectLimit.right;

		if (m_htTrack == vSplitterBox ||
			m_htTrack >= vSplitterBar1 && m_htTrack <= vSplitterBar15)
		{
			if (m_rectTracker.top != pt.y)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(0, pt.y - m_rectTracker.top);
				OnInvertTracker(m_rectTracker);
			}
		}
		else if (m_htTrack == hSplitterBox ||
			m_htTrack >= hSplitterBar1 && m_htTrack <= hSplitterBar15)
		{
			if (m_rectTracker.left != pt.x)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(pt.x - m_rectTracker.left, 0);
				OnInvertTracker(m_rectTracker);
			}
		}
		else if (m_htTrack == bothSplitterBox ||
			(m_htTrack >= splitterIntersection1 &&
			m_htTrack <= splitterIntersection225))
		{
			if (m_rectTracker.top != pt.y)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(0, pt.y - m_rectTracker.top);
				OnInvertTracker(m_rectTracker);
			}
			if (m_rectTracker2.left != pt.x)
			{
				OnInvertTracker(m_rectTracker2);
				m_rectTracker2.OffsetRect(pt.x - m_rectTracker2.left, 0);
				OnInvertTracker(m_rectTracker2);
			}
		}
	}
	else
	{
		// simply hit-test and set appropriate cursor

		int ht = HitTest(pt);
		SetSplitCursor(ht);
	}
}


void CMineBCGPSplitterWnd::GetHitRect(int ht, CRect& rectHit)
{
	ASSERT_VALID(this);

	CRect rectClient;
	GetClientRect(&rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
	int cx = rectClient.Width();
	int cy = rectClient.Height();
	int x = rectClient.top;
	int y = rectClient.left;

	// hit rectangle does not include border
	// m_rectLimit will be limited to valid tracking rect
	// m_ptTrackOffset will be set to appropriate tracking offset
	m_ptTrackOffset.x = 0;
	m_ptTrackOffset.y = 0;

	if (ht == vSplitterBox)
	{
		cy = m_cySplitter - (2*m_cyBorder - 1);
		m_ptTrackOffset.y = -(cy / 2);
		ASSERT(m_pRowInfo[0].nCurSize > 0);
		m_rectLimit.bottom -= cy;
	}
	else if (ht == hSplitterBox)
	{
		cx = m_cxSplitter - (2*m_cxBorder - 1);
		m_ptTrackOffset.x = -(cx / 2);
		ASSERT(m_pColInfo[0].nCurSize > 0);
		m_rectLimit.right -= cx;
	}
	else if (ht >= vSplitterBar1 && ht <= vSplitterBar15)
	{
		cy = m_cySplitter - (2*m_cyBorder - 1);
		m_ptTrackOffset.y = -(cy / 2);
		int row;
		for (row = 0; row < ht - vSplitterBar1; row++)
			y += m_pRowInfo[row].nCurSize + m_cySplitterGap;
		m_rectLimit.top = y;
		y += m_pRowInfo[row].nCurSize + m_cyBorderShare + 1;
		m_rectLimit.bottom -= cy;
	}
	else if (ht >= hSplitterBar1 && ht <= hSplitterBar15)
	{
		cx = m_cxSplitter - (2*m_cxBorder - 1);
		m_ptTrackOffset.x = -(cx / 2);
		int col;
		for (col = 0; col < ht - hSplitterBar1; col++)
			x += m_pColInfo[col].nCurSize + m_cxSplitterGap;
		m_rectLimit.left = x;
		x += m_pColInfo[col].nCurSize + m_cxBorderShare + 1;
		m_rectLimit.right -= cx;
	}
	else
	{
		TRACE(traceAppMsg, 0, "Error: GetHitRect(%d): Not Found!\n", ht);
		ASSERT(FALSE);
	}

	rectHit.right = (rectHit.left = x) + cx;
	rectHit.bottom = (rectHit.top = y) + cy;
}
