// TreeCtrlXP.cpp: archivo de implementación
//

#include "stdafx.h"
#include "TreeCtrlXP.h"
# include "draw.h"


// CTreeCtrlXP

IMPLEMENT_DYNAMIC(CTreeCtrlXP, CTreeCtrl)
CTreeCtrlXP::CTreeCtrlXP()
{
}

CTreeCtrlXP::~CTreeCtrlXP()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlXP, CTreeCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// Controladores de mensajes de CTreeCtrlXP

void CTreeCtrlXP::OnPaint()
{
	CPaintDC dc(this);
	// Create a memory DC compatible with the paint DC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CRect rcClip, rcClient;
	dc.GetClipBox( &rcClip );
	GetClientRect(&rcClient);

	// Select a compatible bitmap into the memory DC
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
	memDC.SelectObject( &bitmap );
	
	// Set clip region to be same as that in paint DC
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcClip );
	memDC.SelectClipRgn(&rgn);
	rgn.DeleteObject();
	
	// First let the control do its default drawing.
	CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

	HTREEITEM hItem = GetFirstVisibleItem();

	LOGFONT logfont;

	CFont *pFont = GetFont();
	CFont *pFontDC;
	pFont->GetLogFont( &logfont );
	CRect rect;

	int iItemCount = GetVisibleCount() + 1;

	while(hItem && iItemCount--)
	{
		CFont fontDC;
		
		logfont.lfWeight = GetItemState(hItem, TVIS_BOLD)&TVIS_BOLD ? FW_BOLD : FW_NORMAL;

		fontDC.CreateFontIndirect(&logfont);
		pFontDC = memDC.SelectObject(&fontDC);

		memDC.SetTextColor(GetItemState(hItem, TVIS_BOLD)&TVIS_BOLD ?
			GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOWTEXT));

		CString sItem = GetItemText(hItem);
		GetItemRect(hItem, &rect, true);

		rect.InflateRect(1,0,1,0);

		memDC.FillSolidRect(rect, ::GetSysColor(COLOR_WINDOW));
		memDC.SetBkColor(GetItemState(hItem,TVIS_DROPHILITED | TVIS_SELECTED)&(TVIS_DROPHILITED | TVIS_SELECTED) ?
			HLS_TRANSFORM( ::GetSysColor(COLOR_HIGHLIGHT),  +80, -66) :GetSysColor(COLOR_WINDOW));

		if(GetItemState(hItem,TVIS_DROPHILITED | TVIS_SELECTED)&(TVIS_DROPHILITED | TVIS_SELECTED)) {
			
			memDC.FillSolidRect(rect, HLS_TRANSFORM( ::GetSysColor(COLOR_HIGHLIGHT),  +80, -66));

			if(GetFocus()==this) {

				CPen pen, *oldpen;
				pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
				oldpen= memDC.SelectObject(&pen);
				memDC.SelectObject(::GetStockObject (NULL_BRUSH));
				memDC.Rectangle(rect);
				memDC.SelectObject(oldpen);
			}
			rect.InflateRect(-4, 0,-1,0);
		} else rect.InflateRect(-3, 0,0,0);

		memDC.DrawText(sItem,rect,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

		memDC.SelectObject(pFontDC);
		hItem = GetNextVisibleItem(hItem);
	}

	dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC, 
				rcClip.left, rcClip.top, SRCCOPY);

	memDC.DeleteDC();
}
