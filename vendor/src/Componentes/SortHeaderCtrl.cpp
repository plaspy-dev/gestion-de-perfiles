/*----------------------------------------------------------------------
Copyright (C)2001 MJSoft. All Rights Reserved.
          This source may be used freely as long as it is not sold for
					profit and this copyright information is not altered or removed.
					Visit the web-site at www.mjsoft.co.uk
					e-mail comments to info@mjsoft.co.uk
					The code for drawing the arrow in the header control was
					written by Zafir Anjum
File:     SortHeaderCtrl.cpp
Purpose:  Provides the header control, with drawing of the arrows, for
          the list control.
----------------------------------------------------------------------*/

#include "stdafx.h"
# include "Componentes.h"
#include "SortHeaderCtrl.h"
# include "newmenu/newmenu.h"
# include "mylistctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSortHeaderCtrl

CSortHeaderCtrl::CSortHeaderCtrl()
	: m_iSortColumn( -1 )
	, m_bSortAscending( TRUE )
{
	FUNCTION_STACK_TRACE("FFFFE0A2");
	m_nItemOver= -1;
	m_bShowOver= false;
	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
	m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);
	m_crText		= ::GetSysColor(COLOR_WINDOWTEXT);
}

CSortHeaderCtrl::~CSortHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CSortHeaderCtrl, CHeaderCtrl)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
    ON_WM_MOUSEOUT()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONUP()
	ON_COMMAND(1201, OnAjustarContenido)
	ON_COMMAND(1202, OnAjustarColumnas)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortHeaderCtrl message handlers

////////////////////////////////////////////////////////////////////////////////
void CSortHeaderCtrl::OnMouseMove (UINT nFlags, CPoint point)
{
	FUNCTION_STACK_TRACE("FFFFE0A3");
    Default();
    m_MouseMgr.OnMouseMove();
	CRect rect;
	for( int i=0; i<GetItemCount(); i++) {
		GetItemRect(i, rect);
		if( rect.PtInRect( point)) {
			if( m_nItemOver!= i) {
				m_nItemOver= i;
				Invalidate();
			}
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void CSortHeaderCtrl::OnMouseOut ()
{
	FUNCTION_STACK_TRACE("FFFFE0A4");
	m_MouseMgr.OnMouseOut();
	if( m_nItemOver!=-1) {
		m_nItemOver= -1;
		Invalidate();
	}
}

void CSortHeaderCtrl::SetSortArrow( const int iSortColumn, const BOOL bSortAscending )
{
	FUNCTION_STACK_TRACE("FFFFE0A5");
	m_iSortColumn = iSortColumn;
	m_bSortAscending = bSortAscending;
	Invalidate();
}

int CSortHeaderCtrl::GetSortColumn(BOOL* bSortCol )
{
	FUNCTION_STACK_TRACE("FFFFE0A6");
	*bSortCol =m_bSortAscending;
	return m_iSortColumn;
}

void CSortHeaderCtrl::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	// ASSERT(FALSE);

	// attath to the device context.
	CDC dc;
	VERIFY( dc.Attach( lpDrawItemStruct->hDC ) );

	if(!dc.m_hDC)
		return ;
	// save the device context.
	const int iSavedDC = dc.SaveDC();

	// get the column rect.
	CRect rc( lpDrawItemStruct->rcItem );
	rc.DeflateRect(1,1,1,1);

	// set the clipping region to limit drawing within the column.
	CRgn rgn;
	VERIFY( rgn.CreateRectRgnIndirect( &rc ) );
	(void)dc.SelectObject( &rgn );
	VERIFY( rgn.DeleteObject() );

	// draw the background,
	CBrush brush( GetSysColor( COLOR_3DFACE ) );
	dc.FillRect( rc, &brush );

	// get the column text and format.
	TCHAR szText[ 256 ];
	HD_ITEM hditem;

	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = szText;
	hditem.cchTextMax = 255;

	VERIFY( GetItem( lpDrawItemStruct->itemID, &hditem ) );

	// determine the format for drawing the column label.
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_END_ELLIPSIS ;

	if( hditem.fmt & HDF_CENTER)
		uFormat |= DT_CENTER;
	else if( hditem.fmt & HDF_RIGHT)
		uFormat |= DT_RIGHT;
	else
		uFormat |= DT_LEFT;

	// adjust the rect if the mouse button is pressed on it.
	if( lpDrawItemStruct->itemState == ODS_SELECTED )
	{
		rc.left++;
		rc.top += 2;
		rc.right++;
	}

	CRect rcIcon( lpDrawItemStruct->rcItem );
	const int iOffset = ( rcIcon.bottom - rcIcon.top ) / 4;

	// adjust the rect further if the sort arrow is to be displayed.
	if( lpDrawItemStruct->itemID == (UINT)m_iSortColumn )
		rc.right -= 3 * iOffset;

	rc.left += iOffset;
	rc.right -= iOffset;

	// draw the column label.
	if( rc.left < rc.right )
		(void)dc.DrawText( szText, -1, rc, uFormat );

	// draw the sort arrow.
	if( lpDrawItemStruct->itemID == (UINT)m_iSortColumn )
	{
		// set up the pens to use for drawing the arrow.
		CPen penLight( PS_SOLID, 1, GetSysColor( COLOR_3DHILIGHT ) );
		CPen penShadow( PS_SOLID, 1, GetSysColor( COLOR_3DSHADOW ) );
		CPen* pOldPen = dc.SelectObject( &penLight );

		if( m_bSortAscending )
		{
			// draw the arrow pointing upwards.
			dc.MoveTo( rcIcon.right - 2 * iOffset, iOffset);
			dc.LineTo( rcIcon.right - iOffset, rcIcon.bottom - iOffset - 1 );
			dc.LineTo( rcIcon.right - 3 * iOffset - 2, rcIcon.bottom - iOffset - 1 );
			(void)dc.SelectObject( &penShadow );
			dc.MoveTo( rcIcon.right - 3 * iOffset - 1, rcIcon.bottom - iOffset - 1 );
			dc.LineTo( rcIcon.right - 2 * iOffset, iOffset - 1);		
		}
		else
		{
			// draw the arrow pointing downwards.
			dc.MoveTo( rcIcon.right - iOffset - 1, iOffset );
			dc.LineTo( rcIcon.right - 2 * iOffset - 1, rcIcon.bottom - iOffset );
			(void)dc.SelectObject( &penShadow );
			dc.MoveTo( rcIcon.right - 2 * iOffset - 2, rcIcon.bottom - iOffset );
			dc.LineTo( rcIcon.right - 3 * iOffset - 1, iOffset );
			dc.LineTo( rcIcon.right - iOffset - 1, iOffset );		
		}

		// restore the pen.
		(void)dc.SelectObject( pOldPen );
	}

	// restore the previous device context.
	VERIFY( dc.RestoreDC( iSavedDC ) );

	// detach the device context before returning.
	(void)dc.Detach();
}


void CSortHeaderCtrl::Serialize( CArchive& ar )
{
	if( ar.IsStoring() )
	{
		const int iItemCount = GetItemCount();
		if( iItemCount != -1 )
		{
			ar << iItemCount;

			HD_ITEM hdItem = { 0 };
			hdItem.mask = HDI_WIDTH;

			for( int i = 0; i < iItemCount; i++ )
			{
				VERIFY( GetItem( i, &hdItem ) );
				ar << hdItem.cxy;
			}
		}
	}
	else
	{
		int iItemCount;
		ar >> iItemCount;
		
		if( GetItemCount() != iItemCount )
			TRACE0( _T("Different number of columns in registry.") );
		else
		{
			HD_ITEM hdItem = { 0 };
			hdItem.mask = HDI_WIDTH;

			for( int i = 0; i < iItemCount; i++ )
			{
				ar >> hdItem.cxy;
				VERIFY( SetItem( i, &hdItem ) );
			}
		}
	}
}

void CSortHeaderCtrl::OnSysColorChange() 
{
	FUNCTION_STACK_TRACE("FFFFE0A7");
	TRACE(_T("in CXHeaderCtrl::OnSysColorChange\n"));

	CHeaderCtrl::OnSysColorChange();
	
	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
	m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);
	m_crText		= ::GetSysColor(COLOR_WINDOWTEXT);
}

void CSortHeaderCtrl::OnPaint()
{
	if(m_WVersion.IsVistaorLater()) {
		Default();
		if(m_iSortColumn!=-1) {

			CClientDC dc(this);
			if(!dc.m_hDC)
				return ;
			CRect rcIcon;
			
			GetItemRect(m_iSortColumn, rcIcon);
			const int iOffset = ( rcIcon.bottom - rcIcon.top ) / 4;

			CPen penLight( PS_SOLID, 1, GetSysColor( COLOR_3DHILIGHT ) );
			CPen penShadow( PS_SOLID, 1, GetSysColor( COLOR_3DSHADOW ) );
			CPen* pOldPen = dc.SelectObject( &penLight );

			if( m_bSortAscending )
			{
				// draw the arrow pointing upwards.
				dc.MoveTo( rcIcon.right - 2 * iOffset, iOffset);
				dc.LineTo( rcIcon.right - iOffset, rcIcon.bottom - iOffset - 1 );
				dc.LineTo( rcIcon.right - 3 * iOffset - 2, rcIcon.bottom - iOffset - 1 );
				(void)dc.SelectObject( &penShadow );
				dc.MoveTo( rcIcon.right - 3 * iOffset - 1, rcIcon.bottom - iOffset - 1 );
				dc.LineTo( rcIcon.right - 2 * iOffset, iOffset - 1);
			}
			else
			{
				// draw the arrow pointing downwards.
				dc.MoveTo( rcIcon.right - iOffset - 1, iOffset );
				dc.LineTo( rcIcon.right - 2 * iOffset - 1, rcIcon.bottom - iOffset );
				(void)dc.SelectObject( &penShadow );
				dc.MoveTo( rcIcon.right - 2 * iOffset - 2, rcIcon.bottom - iOffset );
				dc.LineTo( rcIcon.right - 3 * iOffset - 1, iOffset );
				dc.LineTo( rcIcon.right - iOffset - 1, iOffset );
			}

			// restore the pen.
			(void)dc.SelectObject( pOldPen );
		}
		return;
	}
	CPaintDC dc(this);
	if( GetItemCount()) {
		if(!dc.m_hDC)
			return ;
		DrawCtrl(&dc);
	}
}

void CSortHeaderCtrl::DrawCtrl(CDC* pDC)
{
	FUNCTION_STACK_TRACE("FFFFE0A8");

	if(!pDC || !pDC->m_hDC)
		return ;

	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) == ERROR)
		return;

	CRect rectClient, rectItem;
	GetClientRect(&rectClient);

    // pDC->FillSolidRect(rectClip, m_cr3DFace);

	{
		TRIVERTEX        vert[2];
		GRADIENT_RECT    gRect;

		vert [0] .x      = rectClip.left;
		vert [0] .y      = rectClip.top;
		vert [0] .Red    = 0xff00;
		vert [0] .Green  = 0xff00;
		vert [0] .Blue   = 0xff00;
		vert [0] .Alpha  = 0x0000;

		vert [1] .x      = rectClip.right;
		vert [1] .y      = rectClip.bottom;
		vert [1] .Red    = GetRValue(m_cr3DFace)<<8;
		vert [1] .Green  = GetGValue(m_cr3DFace)<<8;
		vert [1] .Blue   = GetBValue(m_cr3DFace)<<8;
		vert [1] .Alpha  = 0x0000;

		gRect.UpperLeft  = 0;
		gRect.LowerRight = 1;

		pDC->GradientFill(vert,2,&gRect,1,GRADIENT_FILL_RECT_V);
	}

	int iItems = GetItemCount();
	ASSERT(iItems >= 0);

	CPen penHighLight(PS_SOLID, 1, m_cr3DHighLight);
	CPen penShadow(PS_SOLID, 1, m_cr3DShadow);
	CPen* pPen = pDC->GetCurrentPen();

	CFont* pFont = pDC->SelectObject(GetFont());

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(m_cr3DFace);
	pDC->SetTextColor(m_crBtnText);

	int iWidth = 0;

	for (int i = 0; i < iItems; i++)
	{
		int iItem = OrderToIndex(i);

		TCHAR szText[255];

		HDITEM hditem;
		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT|HDI_IMAGE|HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));

		VERIFY(GetItemRect(iItem, rectItem));

		if (rectItem.right >= rectClip.left || rectItem.left <= rectClip.right)
		{
			if (hditem.fmt & HDF_OWNERDRAW)
			{
				DRAWITEMSTRUCT disItem;
				disItem.CtlType = ODT_BUTTON;
				disItem.CtlID = GetDlgCtrlID();
				disItem.itemID = iItem;
				disItem.itemAction = ODA_DRAWENTIRE;
				disItem.itemState = 0;
				disItem.hwndItem = m_hWnd;
				disItem.hDC = pDC->m_hDC;
				disItem.rcItem = rectItem;
				disItem.itemData = 0;

				DrawItem(&disItem);
			}
			else
			{
				rectItem.DeflateRect(6, 0);
				DrawItem(pDC, rectItem, &hditem, m_iSortColumn== i, m_nItemOver==i);
				rectItem.InflateRect(6, 0);
			}

			if (i < iItems-1)
			{
				pDC->SelectObject(&penShadow);
				pDC->MoveTo(rectItem.right-1, rectItem.top+2);
				pDC->LineTo(rectItem.right-1, rectItem.bottom-2);

				pDC->SelectObject(&penHighLight);
				pDC->MoveTo(rectItem.right, rectItem.top+2);
				pDC->LineTo(rectItem.right, rectItem.bottom-2);
			}
		}

		iWidth += hditem.cxy;
	}

	if (iWidth > 0)
	{
		rectClient.right = rectClient.left + iWidth;
		pDC->Draw3dRect(rectClient, m_cr3DHighLight, m_cr3DShadow);
	}

	pDC->SelectObject(pFont);
	pDC->SelectObject(pPen);

	penHighLight.DeleteObject();
	penShadow.DeleteObject();
}

int CSortHeaderCtrl::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi, bool over)
{
	FUNCTION_STACK_TRACE("FFFFE0A9");

	if(!pDC || !pDC->m_hDC)
		return 0;

	CSize size;

	pDC->SetTextColor(over && m_bShowOver ? ::GetSysColor(COLOR_HIGHLIGHT) : m_crText);

	if (rect.Width() > 0 && lphdi->mask & HDI_TEXT && lphdi->fmt & HDF_STRING)
	{
		size = pDC->GetTextExtent(lphdi->pszText);

		// always center column headers
		pDC->DrawText(lphdi->pszText, -1, rect, 
			DT_LEFT|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
	}

	size.cx = rect.Width()>size.cx ? size.cx:rect.Width();
	return size.cx>0 ? size.cx:0;
}

void  CSortHeaderCtrl::DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi, bool sort, bool over)
{
	FUNCTION_STACK_TRACE("FFFFE0AA");

	if(!pDC || !pDC->m_hDC)
		return ;

	ASSERT(lphdi->mask & HDI_FORMAT);

	int iWidth = 0;

	CBitmap* pBitmap = NULL;
	BITMAP BitmapInfo;

	if (lphdi->fmt & HDF_BITMAP)
	{
		ASSERT(lphdi->mask & HDI_BITMAP);
		ASSERT(lphdi->hbm);

		pBitmap = CBitmap::FromHandle(lphdi->hbm);
		if (pBitmap)
			VERIFY(pBitmap->GetObject(sizeof(BITMAP), &BitmapInfo));
	}
	if( sort)
	{
		CRect rcIcon( rect);
		const int iOffset = ( rcIcon.bottom - rcIcon.top ) / 4;
		// set up the pens to use for drawing the arrow.

		CPen penLight( PS_SOLID, 1, m_cr3DHighLight);
		CPen penShadow( PS_SOLID, 1, m_cr3DShadow);
		CPen* pOldPen = pDC->SelectObject( &penLight );

		if( m_bSortAscending )
		{
			// draw the arrow pointing upwards.
			pDC->MoveTo( rcIcon.right - 2 * iOffset, iOffset);
			pDC->LineTo( rcIcon.right - iOffset, rcIcon.bottom - iOffset - 1 );
			pDC->LineTo( rcIcon.right - 3 * iOffset - 2, rcIcon.bottom - iOffset - 1 );
			(void)pDC->SelectObject( &penShadow );
			pDC->MoveTo( rcIcon.right - 3 * iOffset - 1, rcIcon.bottom - iOffset - 1 );
			pDC->LineTo( rcIcon.right - 2 * iOffset, iOffset - 1);
		}
		else
		{
			// draw the arrow pointing downwards.
			pDC->MoveTo( rcIcon.right - iOffset - 1, iOffset );
			pDC->LineTo( rcIcon.right - 2 * iOffset - 1, rcIcon.bottom - iOffset );
			(void)pDC->SelectObject( &penShadow );
			pDC->MoveTo( rcIcon.right - 2 * iOffset - 2, rcIcon.bottom - iOffset );
			pDC->LineTo( rcIcon.right - 3 * iOffset - 1, iOffset );
			pDC->LineTo( rcIcon.right - iOffset - 1, iOffset );
		}
		rect.right -= 3 * iOffset;
		rect.right -= iOffset;
	}
	DrawText(pDC, rect, lphdi, over);
}
void CSortHeaderCtrl::PreSubclassWindow()
{
	FUNCTION_STACK_TRACE("FFFFE0AB");
	 m_MouseMgr.Init(m_hWnd);
	// CHeaderCtrl::PreSubclassWindow();
}

BOOL CSortHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return CHeaderCtrl::OnEraseBkgnd(pDC);
}

void CSortHeaderCtrl::OnAjustarColumnas() {

	CWnd *parent= GetParent();
	if( parent->IsKindOf(RUNTIME_CLASS(CMyListCtrl))) {
		CMyListCtrl *lista= (CMyListCtrl *)parent;
		lista->AutoSizeColumns();
	}
}

void CSortHeaderCtrl::OnAjustarContenido() {

	CWnd *parent= GetParent();
	if( parent->IsKindOf(RUNTIME_CLASS(CMyListCtrl))) {
		CMyListCtrl *lista= (CMyListCtrl *)parent;
		lista->AutoSizeColumns(1);
	}
}

void CSortHeaderCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	if( GetItemCount()) {
		
		ClientToScreen(&point);
		CNewMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, 1202, "&Ajustar Columnas");
		menu.AppendMenu(MF_STRING, 1201, "A&justar al Contenido");
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	CHeaderCtrl::OnRButtonUp(nFlags, point);
}

int CSortHeaderCtrl::GetHeaderHeight() {

	CRect rect;
	GetWindowRect(rect);
	return rect.Height();
}
