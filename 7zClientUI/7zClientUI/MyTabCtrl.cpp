// MyTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "7zClientUI.h"
#include "MyTabCtrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_WM_NCPAINT()
END_MESSAGE_MAP()



// CMyTabCtrl 消息处理程序




void CMyTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	if(lpDrawItemStruct->CtlType == ODT_TAB)  
	{   
		CRect rect = lpDrawItemStruct->rcItem;  
		INT nTabIndex = lpDrawItemStruct->itemID;  
		if (nTabIndex < 0) return;  

		TCHAR label[64];  
		TC_ITEM tci;  
		tci.mask = TCIF_TEXT|TCIF_IMAGE;  
		tci.pszText = label;       
		tci.cchTextMax = 63;          
		GetItem(nTabIndex, &tci );  

		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);  
		if (!pDC) return;  
		int nSavedDC = pDC->SaveDC();  

		//填充背景色  
		COLORREF rcBack;  
	
		if (lpDrawItemStruct->itemState & CDIS_SELECTED  )   
		{  
			rcBack = GetSysColor(COLOR_BTNFACE); 
		}  
		else if(lpDrawItemStruct->itemState & (CDIS_DISABLED | CDIS_GRAYED) )   
		{  
			rcBack = RGB(0, 255, 0);  
		}  
		else  
		{  
			rcBack = RGB(240,240,240);  
		}      

		pDC->FillSolidRect(rect, rcBack); 
		CPen *pOldPen = NULL;

		// 绘制边框
		CPen penBorder(PS_SOLID,1,RGB( 0xc6, 0x5e, 0x02 ));
		pOldPen = pDC->SelectObject(&penBorder);

	

		CRect rectClient;
		GetClientRect(rectClient);

		if (lpDrawItemStruct->itemState & CDIS_SELECTED  )   
		{  
			CRect rectBorder;
			int iBy = ::GetSystemMetrics(SM_CYEDGE)+1; 
			int iBx = ::GetSystemMetrics(SM_CXEDGE)+1; 

			CRect rectInner = rect;
			rectInner.InflateRect(-iBx,-iBy,-iBx,-iBy);
			
			pDC->MoveTo(rectInner.TopLeft());
			pDC->LineTo(rectInner.right ,rectInner.top);
			pDC->LineTo(rectInner.right ,rectInner.bottom);

			pDC->MoveTo(rectInner.TopLeft());
			pDC->LineTo(rectInner.left,rectInner.bottom);
		}
		else
		{
			pDC->MoveTo(0,25);
			pDC->LineTo(rectClient.right,25);

			CRect rectBorder;
			int iBy = ::GetSystemMetrics(SM_CYEDGE); 
			int iBx = ::GetSystemMetrics(SM_CXEDGE); 

			CRect rectInner = rect;
			rectInner.InflateRect(-iBx,-iBy,-iBx,-iBy);
			CPen penBorderGrey(PS_SOLID,1,RGB(0x99,0x99,0x99));
			pDC->SelectObject(&penBorderGrey);

			pDC->MoveTo(rectInner.TopLeft());
			pDC->LineTo(rectInner.right,rectInner.top);
			pDC->LineTo(rectInner.right,rectInner.bottom);

			pDC->MoveTo(rectInner.TopLeft());
			pDC->LineTo(rectInner.left,rectInner.bottom);
		}

	

		if (pOldPen)
		{
			pDC->SelectObject(pOldPen);
		}

		rect.top += ::GetSystemMetrics(SM_CYEDGE);  

		pDC->SetBkMode(TRANSPARENT);  

		//绘制图片  
		CImageList* pImageList = GetImageList();  
		if (pImageList && tci.iImage >= 0)   
		{  
			rect.left += pDC->GetTextExtent(_T(" ")).cx;       // Margin  

			// Get height of image so we   
			IMAGEINFO info;  
			pImageList->GetImageInfo(tci.iImage, &info);  
			CRect ImageRect(info.rcImage);  
			INT nYpos = rect.top;  

			pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);  
			rect.left += ImageRect.Width();  
		}  

		//绘制字体  
		COLORREF txtColor;  
		if (lpDrawItemStruct->itemState & CDIS_SELECTED  )   
		{  
			rect.top -= ::GetSystemMetrics(SM_CYEDGE);  

			txtColor =  RGB( 0xc6, 0x5e, 0x02 );  
		}  
		else if(lpDrawItemStruct->itemState & (CDIS_DISABLED | CDIS_GRAYED) )   
		{  
			txtColor = RGB(0x99,0x99,0x99);        
		}  
		else  
		{  
			txtColor =  RGB( 0x77, 0x77, 0x77 );  
		}      
		pDC->SetTextColor(txtColor);  
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);  

		pDC->RestoreDC(nSavedDC);  

	} 
}


void CMyTabCtrl::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CTabCtrl::OnNcPaint()
}
