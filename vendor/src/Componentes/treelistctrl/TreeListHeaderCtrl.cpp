// TreeListHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
# include "Componentes.h"
#include "treelistctrl/TreeListColumnInfo.h"
# include "memdc.h"
#include "treelistctrl/TreeListHeaderCtrl.h"
#include "treelistctrl/TreeListCtrl.h"
#include "ResourceDLL.h"
# include "newmenu/newmenu.h"

# include <resource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeListHeaderCtrl

CTreeListHeaderCtrl::CTreeListHeaderCtrl() :
	m_dwStyle( 0 ),
	m_bSubclassFromCreate( FALSE ),
	m_bOffScreenBuffer( TRUE ),
	m_pImageList( NULL ),
	m_pTreeListCtrl( NULL ),
	m_hSize( NULL ),
	m_hSplit( NULL ),
	m_hArrow( NULL ),
	m_bSplit( FALSE ),
	m_bSizing( FALSE ),
	m_nHeaderHeight( 0 )
{
	// register window class
	WNDCLASS wndclass;
	HINSTANCE hInst = AfxGetInstanceHandle();
	CResourceDLL	resource;

	if( !::GetClassInfo(hInst, TREELISTHEADERCTRL_CLASSNAME, &wndclass) )
	{
        wndclass.style			= CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc	= ::DefWindowProc;
		wndclass.cbClsExtra		= 0;
		wndclass.cbWndExtra		= 0;
		wndclass.hInstance		= hInst;
		wndclass.hIcon			= NULL;
		wndclass.hCursor		=  AfxGetApp()->LoadStandardCursor( IDC_ARROW );
		wndclass.hbrBackground	= (HBRUSH)COLOR_WINDOW;
		wndclass.lpszMenuName	= NULL;
		wndclass.lpszClassName	= TREELISTHEADERCTRL_CLASSNAME;

		if (!AfxRegisterClass(&wndclass))
			AfxThrowResourceException();
	}

	// default color
	m_cr3DBkgnd		= 0x808080;
	m_cr3DFace		= GetSysColor( COLOR_3DFACE );
	m_cr3DLight		= GetSysColor( COLOR_3DHILIGHT );
	m_cr3DShadow	= GetSysColor( COLOR_3DSHADOW );
	m_crText		= GetSysColor( COLOR_BTNTEXT );
	m_crTextOver    = GetSysColor( COLOR_HIGHLIGHT);

	// default font
	m_Font.CreateStockObject( SYSTEM_FIXED_FONT );

	// load cursors
	m_hSize		= AfxGetApp()->LoadCursor(IDC_CURSOR_RESIZE);
	m_hSplit	= AfxGetApp()->LoadCursor(IDC_CURSOR_RESIZE);
	m_hArrow	= AfxGetApp()->LoadStandardCursor( IDC_ARROW);

	m_nItemOver= -1;
	m_bShowOver= true;
	m_iSortColumn= -1;
}

CTreeListHeaderCtrl::~CTreeListHeaderCtrl()
{
	// free font
	if( m_Font.m_hObject != NULL )
		m_Font.DeleteObject();

	// free cursors
	if( m_hSize != NULL )
		DestroyCursor( m_hSize );

	if( m_hSplit != NULL )
		DestroyCursor( m_hSplit );

	if( m_hArrow != NULL )
		DestroyCursor( m_hArrow );
}

BEGIN_MESSAGE_MAP(CTreeListHeaderCtrl, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CANCELMODE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_MOUSEOUT()
	ON_COMMAND(1201, OnAjustarContenido)
	ON_COMMAND(1202, OnAjustarColumnas)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeListHeaderCtrl message handlers
BOOL CTreeListHeaderCtrl::Create()
{
	m_MouseMgr.Init(m_hWnd);

	CResourceDLL resorce;
	// create sort image
	if( !m_imgSort.Create( IDB_TREELIST_SORT, 8, 4, 0xFF00FF ) )
		return FALSE;

	m_pImageList = &m_imgSort;

	return TRUE;
}

BOOL CTreeListHeaderCtrl::Create( DWORD dwStyle, const RECT& rect, CTreeListCtrl* pParentWnd, UINT nID )
{
	// create window
	ASSERT_VALID( pParentWnd );

	m_dwStyle = dwStyle;
	m_pTreeListCtrl = pParentWnd;
	m_bSubclassFromCreate = TRUE;

	dwStyle |= WS_CHILD | WS_VISIBLE | WS_EX_TRANSPARENT;

	if( !CWnd::Create( TREELISTHEADERCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID ) )
		return FALSE;

	if( !Create() )
		return FALSE;

	return TRUE;
}

void CTreeListHeaderCtrl::SetHeaderFont()
{
	// set font
	CWnd* pWnd = GetParent();
	CTreeListCtrl* pTreeListCtrl = (CTreeListCtrl*)pWnd;

	if( pTreeListCtrl != NULL )
	{
		CFont* pFont = pTreeListCtrl->GetFont();
		if( pFont != NULL )
		{
			LOGFONT lf;
			pFont->GetLogFont( &lf );

			m_Font.DeleteObject();
			m_Font.CreateFontIndirect( &lf );
		}
	}
}

CFont* CTreeListHeaderCtrl::GetHeaderFont()
{
	// get font
	return &m_Font;
}


int CTreeListHeaderCtrl::SetHeaderHeight( int nHeight )
{
	// set header height
	int nOldHeight = m_nHeaderHeight;

	if( nHeight == -1 )
	{
		ASSERT( m_Font.m_hObject != NULL );
		
		LOGFONT lf;
		m_Font.GetLogFont( &lf );

		if( lf.lfHeight > 0 )
		{
			m_nHeaderHeight = lf.lfHeight + 10;
		}
		else if( lf.lfHeight == 0 )
		{
			m_nHeaderHeight = DEFAULT_HEIGHT;
		}
		else
		{
			m_nHeaderHeight = -lf.lfHeight + 10;
		}
	}
	else
	{
		m_nHeaderHeight = nHeight;
	}

	return nOldHeight;
}

int CTreeListHeaderCtrl::GetHeaderHeight()
{
	// get header height
	if( !(GetStyle()&TLHS_SHOWHEAD) )
		return 0;

	return m_nHeaderHeight;
}

int CTreeListHeaderCtrl::GetHeaderWidth()
{
	// get header width
	ASSERT( m_pTreeListCtrl != NULL );
	return m_pTreeListCtrl->GetWidth();
}

BOOL CTreeListHeaderCtrl::GetHeaderRect( CRect& rcHeader, int iCol )
{
	// get header rect
	CPtrArray&			arColumns	= m_pTreeListCtrl->m_arColumns;
	CArray<int,int>&	arShows		= m_pTreeListCtrl->m_arShows;

	BOOL bFound = FALSE;
	int nPerfix = 0;
	for( int iShow=0; iShow<arShows.GetSize(); iShow++ )
	{
		CTreeListColumnInfo* pColumnInfo;

		int iColumn = arShows[iShow];
		pColumnInfo = (CTreeListColumnInfo*)arColumns[iColumn];
		if( iColumn == iCol )
		{
			bFound = TRUE;
			rcHeader.SetRect( 0, 0, pColumnInfo->m_nWidth, GetHeaderHeight() );
			rcHeader.OffsetRect( nPerfix, 0 );
			break;
		}

		nPerfix += pColumnInfo->m_nWidth;
	}

	return bFound;
}

BOOL CTreeListHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// do not erase bkgnd
	return TRUE;
}

void CTreeListHeaderCtrl::OnPaint() 
{
	// paint header control
	CPaintDC dc(this);

	if( m_bOffScreenBuffer )
	{
		CMemDC TreeListDC( &dc );
		if( !TreeListDC.m_hDC) 
			Default();
		else DrawCtrl( &TreeListDC );
	}
	else
	{
		if( dc.m_hDC)
			DrawCtrl( &dc );
		else Default();
	}
}

BOOL CTreeListHeaderCtrl::DrawCtrl( CDC* pDC )
{
	if( !pDC || !pDC->m_hDC)
		return false;
	// draw control
	ASSERT_VALID( pDC );

	CRect rcClip;
	if( pDC->GetClipBox( &rcClip ) == ERROR )
		return FALSE;

	CRect rcClient;
	GetClientRect( &rcClient );

	if( m_pTreeListCtrl->GetStyle()&TLC_BKGNDCOLOR )
	{
		TRIVERTEX        vert[2];
		GRADIENT_RECT    gRect;

		vert [0] .x      = rcClient.left;
		vert [0] .y      = rcClient.top;
		vert [0] .Red    = 0xff00;
		vert [0] .Green  = 0xff00;
		vert [0] .Blue   = 0xff00;
		vert [0] .Alpha  = 0x0000;

		vert [1] .x      = rcClient.right;
		vert [1] .y      = rcClient.bottom;
		vert [1] .Red    = GetRValue(m_cr3DBkgnd)<<8;
		vert [1] .Green  = GetGValue(m_cr3DBkgnd)<<8;
		vert [1] .Blue   = GetBValue(m_cr3DBkgnd)<<8;
		vert [1] .Alpha  = 0x0000;

		gRect.UpperLeft  = 0;
		gRect.LowerRight = 1;

		pDC->GradientFill(vert,2,&gRect,1,GRADIENT_FILL_RECT_V);
		// pDC->FillSolidRect( &rcClient, RGB(0,0,255) /*m_cr3DBkgnd*/ );	
	}
	else
	{
		TRIVERTEX        vert[2];
		GRADIENT_RECT    gRect;

		vert [0] .x      = rcClient.left;
		vert [0] .y      = rcClient.top;
		vert [0] .Red    = 0xff00;
		vert [0] .Green  = 0xff00;
		vert [0] .Blue   = 0xff00;
		vert [0] .Alpha  = 0x0000;

		vert [1] .x      = rcClient.right;
		vert [1] .y      = rcClient.bottom;
		vert [1] .Red    = GetRValue(m_cr3DFace)<<8;
		vert [1] .Green  = GetGValue(m_cr3DFace)<<8;
		vert [1] .Blue   = GetBValue(m_cr3DFace)<<8;
		vert [1] .Alpha  = 0x0000;

		gRect.UpperLeft  = 0;
		gRect.LowerRight = 1;

		pDC->GradientFill(vert,2,&gRect,1,GRADIENT_FILL_RECT_V);

		// pDC->FillSolidRect( &rcClient, RGB(255,0,0)/*m_cr3DFace */);
		pDC->Draw3dRect( &rcClient, m_cr3DLight, m_cr3DShadow );
	}

	DrawHeaders( pDC );

	return TRUE;
}

BOOL CTreeListHeaderCtrl::DrawHeaders( CDC* pDC )
{
	if( !pDC || !pDC->m_hDC)
		return false;
	// draw every column headers
	CPtrArray&			Columns = m_pTreeListCtrl->m_arColumns;
	CArray<int, int>&	Shows	= m_pTreeListCtrl->m_arShows;

	// no columns or zero size
	if( m_pTreeListCtrl->GetWidth() == 0 )
		return TRUE;

	int nWinPos = GetParent()->GetScrollPos( SB_HORZ );

	// get client area
	CRect rcClient;
	GetClientRect( &rcClient );
	
	// draw background
	CRect rcFace;
	rcFace.SetRect( 0, rcClient.top, GetHeaderWidth(), rcClient.bottom );
	rcFace.OffsetRect( -nWinPos, 0 );

	TRIVERTEX        vert[2];
	GRADIENT_RECT    gRect;

	vert [0] .x      = rcClient.left;
	vert [0] .y      = rcClient.top;
	vert [0] .Red    = 0xff00;
	vert [0] .Green  = 0xff00;
	vert [0] .Blue   = 0xff00;
	vert [0] .Alpha  = 0x0000;

	vert [1] .x      = rcClient.right;
	vert [1] .y      = rcClient.bottom;
	vert [1] .Red    = GetRValue(m_cr3DFace)<<8;
	vert [1] .Green  = GetGValue(m_cr3DFace)<<8;
	vert [1] .Blue   = GetBValue(m_cr3DFace)<<8;
	vert [1] .Alpha  = 0x0000;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;

	pDC->GradientFill(vert,2,&gRect,1,GRADIENT_FILL_RECT_V);
	// pDC->FillSolidRect( &rcFace, /*m_cr3DFace */ RGB(0,255,0));
	pDC->Draw3dRect( &rcFace, m_cr3DLight, m_cr3DShadow );
	
	CTreeListColumnInfo* pColumnInfo;
	int nHeaderPos = 0;
	for( int iShow=0; iShow<Shows.GetSize(); iShow++, nHeaderPos += pColumnInfo->m_nWidth )
	{
		int iCol;
		
		// do NOT draw zero column
		iCol = Shows[iShow];  //*m_pShow[iShow];
		pColumnInfo = (CTreeListColumnInfo*)Columns[iCol];
		if( pColumnInfo->m_nWidth == 0 )
			continue;

		CRect rcHeader;
		rcHeader.SetRect( 0, 0, pColumnInfo->m_nWidth, GetHeaderHeight() );
		rcHeader.OffsetRect( -nWinPos, 0 );
		rcHeader.OffsetRect( nHeaderPos, 0 );

		// do NOT draw out of screen column
		if( rcHeader.right < rcClient.left || rcHeader.left > rcClient.right )
			continue;

		DrawHeader( pDC, rcHeader, iCol );
	}

	return TRUE;
}

BOOL CTreeListHeaderCtrl::DrawHeader( CDC* pDC, CRect rcHeader, int iCol )
{

	if( !pDC || !pDC->m_hDC)
		return false;

	// draw one column header
	CPtrArray&			Columns = m_pTreeListCtrl->m_arColumns;
	CArray<int, int>&	Shows	= m_pTreeListCtrl->m_arShows;
	CArray<int, int>&	Sorts	= m_pTreeListCtrl->m_arSorts;
	CTreeListColumnInfo* pColumn = (CTreeListColumnInfo*)Columns[iCol];

	ASSERT( Shows.GetSize() > 0 );

	CRect rc= rcHeader;

	CDC dc;
	VERIFY( dc.Attach( *pDC) );

	const int iSavedDC = dc.SaveDC();

	CRgn rgn;
	VERIFY( rgn.CreateRectRgnIndirect( &rc ) );
	(void)dc.SelectObject( &rgn );
	VERIFY( rgn.DeleteObject() );
   
	// dibujamos los bordes
	CRect rcGaps = rc;
	rcGaps.DeflateRect( 0, 2, 0, 2 );
	if( rcGaps.Height() > 0 )
	{
		pDC->IntersectClipRect( rc );
		pDC->IntersectClipRect( rcGaps );

		if( iCol != m_iCol )
		{
			if( iCol != Shows[0] )
				pDC->DrawEdge( rcGaps, BDR_RAISEDINNER, BF_LEFT );

			if( iCol != Shows[Shows.GetUpperBound()] )
				pDC->DrawEdge( rcGaps, BDR_RAISEDINNER, BF_RIGHT );
		}

		::ExtSelectClipRgn( pDC->m_hDC, DEFAULT_HRGN, RGN_COPY );
	}

	rc.DeflateRect(1,1,1,1);

	// creo que es para la imagen
	CRect rcIcon( rcHeader );
	const int iOffset = ( rcIcon.bottom - rcIcon.top ) / 4;

	if( iCol == m_iSortColumn )
		rc.right -= 3 * iOffset;
    
	rc.left += iOffset;
	rc.right -= iOffset;

	// dibujamos la imagen

	if( pColumn->m_dwFormat&TLF_CAPTION_IMAGE && pColumn->m_iImage>=0 )
	{
		pDC->IntersectClipRect( rc);

		CRect rcImage=rc;
		if( pColumn->m_dwFormat&TLF_IMAGEALIGN_LEFT )
		{
			rcImage.right	= rc.left + TLL_WIDTH;
			rc.left	= rc.left + TLL_WIDTH;
		}
		else if( pColumn->m_dwFormat&TLF_IMAGEALIGN_RIGHT )
		{
			rcImage.left	= rc.right - TLL_WIDTH;
			rc.right	= rc.right - TLL_WIDTH;
		}
		else if( pColumn->m_dwFormat&TLF_IMAGEALIGN_CENTER )
		{
			rc.left	= rc.right;
		}
		else
		{
			rc.right	= rc.left + TLL_WIDTH;
			rc.left	= rc.left + TLL_WIDTH;
		}

		POINT pt;
		if( !(pColumn->m_dwFormat&TLF_IMAGEALIGN_CENTER) )
		{
			pt.x = rcImage.left;
			pt.y = rcImage.top + ( rcImage.Height() - TLL_HEIGHT )/2;
		}
		else
		{
			pt.x = rcImage.left + ( rcImage.Width() - TLL_WIDTH )/2;
			pt.y = rcImage.top + ( rcImage.Height() - TLL_HEIGHT )/2;
		}

		dc.IntersectClipRect( rcImage );
		m_pImageList->Draw( &dc, pColumn->m_iImage, pt, ILD_TRANSPARENT );
		::ExtSelectClipRgn( dc, DEFAULT_HRGN, RGN_COPY );
	}

	if( rc.left < rc.right ) {
		CFont* pOldFont = dc.SelectObject( GetHeaderFont() );
		int nOldMode = dc.SetBkMode( TRANSPARENT );
		dc.SetTextColor( (m_bShowOver && iCol==m_nItemOver) ? m_crTextOver : m_crText);
		(void)dc.DrawText( pColumn->m_strCaption, -1, rc, pColumn->GetCaptionFormat()  | DT_NOPREFIX | DT_NOCLIP | DT_END_ELLIPSIS);
		dc.SetBkMode( nOldMode);
		dc.SelectObject(pOldFont);
	}

	if( iCol == m_iSortColumn )
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
	return TRUE;
}

void CTreeListHeaderCtrl::PreSubclassWindow() 
{
	CWnd::PreSubclassWindow();

	if( m_bSubclassFromCreate == FALSE )
	{
		if( !Create() )
		{
			AfxThrowMemoryException();
			return;
		}
	}

	return;
}

LRESULT CTreeListHeaderCtrl::OnLayout(WPARAM wParam, LPARAM lParam)
{
	// releayout
	CRect* pRect= (CRect*)lParam;
	
	if( pRect->Height() >= GetHeaderHeight() )
	{
		pRect->bottom = pRect->top + GetHeaderHeight();
	}

	MoveWindow( pRect, TRUE );

	return TRUE;
}

void CTreeListHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// mouse moving
	int iFlag;
	int iCol;
	if( !m_bSizing )
	{
		if( HitTestSize( point, &iFlag, &iCol ) )
		{
			if( iFlag & TLHT_ONHEADERSIZE )
			{
				m_bSplit = FALSE;
				SetCursor( m_hSize );
			}
			else
			{
				m_bSplit = TRUE;
				SetCursor( m_hSplit );
			}
		}
	}
	CWnd::OnMouseMove(nFlags, point);
	m_MouseMgr.OnMouseMove();
	if( HitTest(point, &iFlag, &iCol)) {
		if( m_nItemOver!= iCol) {
			m_nItemOver= iCol;
			Invalidate();
		}
	}
}

void CTreeListHeaderCtrl::OnMouseOut ()
{
	FUNCTION_STACK_TRACE("FFFFE0A4");
	m_MouseMgr.OnMouseOut();
	if( m_nItemOver!=-1) {
		m_nItemOver= -1;
		Invalidate();
	}
}

void CTreeListHeaderCtrl::SetSortArrow( const int iSortColumn, const BOOL bSortAscending )
{
	FUNCTION_STACK_TRACE("FFFFE0A5");
	m_iSortColumn = iSortColumn;
	m_bSortAscending = bSortAscending;
	if(::IsWindow(*this)) {
		Invalidate();
	}
}

int CTreeListHeaderCtrl::GetSortColumn(BOOL* bSortCol )
{
	FUNCTION_STACK_TRACE("FFFFE0A6");
	if(bSortCol)
		*bSortCol =m_bSortAscending;
	return m_iSortColumn;
}


void CTreeListHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// left button down
	m_pTreeListCtrl->SetFocus();

	CWnd::OnLButtonDown(nFlags, point);

	ASSERT( m_bSizing == FALSE );

	if( !m_bSizing)
	{
		int iFlag;
		int iCol;
		
		if( HitTestSize( point, &iFlag, &iCol ) )
		{
			m_iCol = iCol;
			BeginSizing( point );
		} else if( HitTest(point, &iFlag, &iCol)) {
			m_pTreeListCtrl->Sort(iCol);
		}
	}
}

void CTreeListHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// left button up
	if( m_bSizing )
	{
		EndSizing( point );
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CTreeListHeaderCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDblClk(nFlags, point);

	if( !m_bSizing)
	{
		int iFlag;
		int iCol;
		
		if( HitTestSize( point, &iFlag, &iCol ) )
		{
			m_pTreeListCtrl->SetColumnWidth( iCol, TLSCW_AUTOSIZE_USEHEADER );
		}
	}
}

void CTreeListHeaderCtrl::OnCancelMode() 
{
	// cancel internal event
	if( m_bSizing )
	{
		EndSizing();
	}
	CWnd::OnCancelMode();
}

BOOL CTreeListHeaderCtrl::BeginSizing( CPoint point )
{
	// begin a sizing
	CPtrArray&			arColumns	= m_pTreeListCtrl->m_arColumns;
	CArray<int, int>&	arShows		= m_pTreeListCtrl->m_arShows;

	ASSERT( m_bSizing == FALSE );

	m_bSizing = TRUE;
	SetCapture();

	if( !m_bSplit )
		SetCursor( m_hSize );
	else
		SetCursor( m_hSplit );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)arColumns[m_iCol];

	int nTrackPos = GetParent()->GetScrollPos( SB_HORZ ) + point.x;
	pColumnInfo->BeginTrack( nTrackPos );

	return TRUE;
}

BOOL CTreeListHeaderCtrl::EndSizing( CPoint point )
{
	// end a sizing
	CPtrArray&			arColumns	= m_pTreeListCtrl->m_arColumns;
	CArray<int, int>&	arShows		= m_pTreeListCtrl->m_arShows;

	m_bSizing = FALSE;

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)arColumns[m_iCol];
		
	int nTrackPos = GetParent()->GetScrollPos( SB_HORZ ) + point.x;
	pColumnInfo->EndTrack( nTrackPos );

	m_pTreeListCtrl->SetAllScroll();
	m_pTreeListCtrl->RedrawWindow();

	ReleaseCapture();
	return TRUE;
}

BOOL CTreeListHeaderCtrl::DoSizing( CPoint point )
{
	// do a sizing
	CPtrArray&			arColumns	= m_pTreeListCtrl->m_arColumns;
	CArray<int, int>&	arShows		= m_pTreeListCtrl->m_arShows;

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)arColumns[m_iCol];
		
	int nTrackPos = GetParent()->GetScrollPos( SB_HORZ ) + point.x;
	pColumnInfo->DoTrack( nTrackPos );

	m_pTreeListCtrl->SetAllScroll();
	m_pTreeListCtrl->RedrawWindow();

	return TRUE;
}

BOOL CTreeListHeaderCtrl::EndSizing()
{
	// end sizing
	CPoint	point;

	GetCursorPos( &point );
	ScreenToClient( &point );

	return EndSizing( point );
}

BOOL CTreeListHeaderCtrl::HitTestSize( CPoint pt, int* pFlag, int* pCol )
{
	// hit test for sizing
	ASSERT( pFlag != NULL );
	ASSERT( pCol != NULL );

	CPtrArray&			 arColumns	= m_pTreeListCtrl->m_arColumns;
	CArray<int, int>&	 arShows	= m_pTreeListCtrl->m_arShows;
	int nWinPos = m_pTreeListCtrl->GetScrollPos( SB_HORZ );

	*pFlag	= 0;
	*pCol	= 0;

	CRect rcClient;
	rcClient.SetRect( 0, 0, GetHeaderWidth(), GetHeaderHeight() );
	rcClient.right += 2;	// fix right border for resize
	rcClient.OffsetRect( -nWinPos, 0 );

	// check if in client rect
	if( !rcClient.PtInRect(pt) )
		return FALSE;

	// check if on gaps
	int nPos = pt.x + nWinPos;
	int nHeaderPos = 0;
	int nSpaceMin  = 0;
	BOOL bOnSize   = FALSE;
	for( int iShow = 0; iShow<arShows.GetSize(); iShow++ )
	{
		CTreeListColumnInfo* pColumnInfo;
		int iCol;

		iCol = arShows[iShow];
		pColumnInfo = (CTreeListColumnInfo*)arColumns[iCol];

		// calculate header right border position;
		nHeaderPos = nHeaderPos + pColumnInfo->m_nWidth;

		// filter nosize column
		if( pColumnInfo->m_dwFormat&TLF_NOSIZE )
			continue;

		// is close right border enough from left
		if( nHeaderPos-3 <= nPos && nPos < nHeaderPos )
		{
			int nGap = nHeaderPos - nPos;
			if( !bOnSize )
			{
				// find first closed right border from left
				*pCol		= iCol;
				nSpaceMin	= nGap;
				bOnSize		= TRUE;
			}
			else
			{
				// find more closed right border from left
				if( nGap < nSpaceMin )
				{
					*pCol	  = iCol;
					nSpaceMin = nGap;
				}
			}
		}
		
		// is close right border enough from right
		if( nHeaderPos <= nPos && nPos <= nHeaderPos+2 )
		{
			int nGap = nPos - nHeaderPos;
			if( !bOnSize )
			{
				// find first closed right border form right
				*pCol		= iCol;
				nSpaceMin	= nGap;
				bOnSize		= TRUE;
			}
			else
			{
				// find more closed right broder from right
				if( nGap <= nSpaceMin )
				{
					*pCol	  = iCol;
					nSpaceMin = nGap;
				}
			}
		}
	} // end for

	if( bOnSize )
	{
		CTreeListColumnInfo* pSizeColumn = ( CTreeListColumnInfo* )arColumns[*pCol];
		if( pSizeColumn->m_nWidth != 0 )
			*pFlag = TLHT_ONHEADERSIZE;
		else
			*pFlag = TLHT_ONHEADERSPLIT;
	}

	return bOnSize;
}

BOOL CTreeListHeaderCtrl::HitTestGaps( CPoint pt, int* pFlag, int* pCol )
{
	// hit test for show
	ASSERT( pFlag != NULL );
	ASSERT( pCol != NULL );

	CPtrArray&			arColumns = m_pTreeListCtrl->m_arColumns;
	CArray<int, int>&	arShows	  = m_pTreeListCtrl->m_arShows;
	int nWinPos = m_pTreeListCtrl->GetScrollPos( SB_HORZ );

	*pFlag	= 0;
	*pCol	= 0;

	CRect rcClient;
	rcClient.SetRect( 0, 0, GetHeaderWidth(), GetHeaderHeight() );
	rcClient.left -= 16;		// fix left border for reorder
	rcClient.right += 16;	// fix right border for reorder
	rcClient.OffsetRect( -nWinPos, 0 );

	// check if in client rect
	if( !rcClient.PtInRect( pt ) )
		return FALSE;

	// check on which column
	int nPos = pt.x + nWinPos;
	int nHeaderPos	= 0;
	int nSpaceMin	= 0;
	BOOL bOnGaps	= FALSE;
	CTreeListColumnInfo* pColumnInfo;
	int iCol;
	int nDelta;

	// check first left border;
	if( arShows.GetSize() > 0 )
	{
		iCol = arShows[0];
		nDelta = 16;
		if( nHeaderPos-nDelta <= nPos && nPos< nHeaderPos )
		{
			// find first closed first left border from left
			ASSERT( !bOnGaps );
			int nGap	= nHeaderPos - nPos;
			*pCol		= iCol;
			*pFlag		= TLHT_ONHEADERLEFT;
			nSpaceMin	= nGap;
			bOnGaps		= TRUE;
		}

		// is close right border enough from right
		CTreeListColumnInfo* pFirstColumnInfo;
		pFirstColumnInfo = (CTreeListColumnInfo*)arColumns[arShows[0]];
		nDelta = max( pFirstColumnInfo->m_nWidth/2, 16 );

		if( nHeaderPos <= nPos && nPos <= nHeaderPos+nDelta )
		{
			// find first closed first left border from right
			ASSERT( !bOnGaps );
			int nGap	= nPos - nHeaderPos;
			*pCol		= iCol;
			*pFlag		= TLHT_ONHEADERLEFT;
			nSpaceMin	= nGap;
			bOnGaps		= TRUE;
		}
	}

	// check all right border;
	for( int iShow = 0; iShow<arShows.GetSize(); iShow++ )
	{
		iCol = arShows[iShow];
		pColumnInfo = (CTreeListColumnInfo*)arColumns[iCol];

		// calculate header right border position;
		nHeaderPos = nHeaderPos + pColumnInfo->m_nWidth;

		// is close right border enough from left
		nDelta = max(pColumnInfo->m_nWidth / 2, 16 );
		if( nHeaderPos-nDelta <= nPos && nPos < nHeaderPos )
		{
			int nGap = nHeaderPos - nPos;
			if( !bOnGaps )
			{
				// find first closed right border from left
				*pCol		= iCol;
				*pFlag		= TLHT_ONHEADERRIGHT;
				nSpaceMin	= nGap;
				bOnGaps		= TRUE;
			}
			else
			{
				// find more closed right border from left
				if( nGap < nSpaceMin )
				{
					*pCol		= iCol;
					*pFlag		= TLHT_ONHEADERRIGHT;
					nSpaceMin	= nGap;
				}
			}
		}

		// is close right border enough from right
		if( iShow < arShows.GetUpperBound() )
		{
			CTreeListColumnInfo* pNextColumnInfo;
			pNextColumnInfo = (CTreeListColumnInfo*)arColumns[arShows[iShow+1]];
			nDelta = max( pNextColumnInfo->m_nWidth/2, 16 );
		}
		else
		{
			nDelta = 16;
		}

		if( nHeaderPos <= nPos && nPos <= nHeaderPos+nDelta )
		{
			int nGap = nPos - nHeaderPos;
			if( !bOnGaps )
			{
				// find first closed right border from right
				*pCol		= iCol;
				*pFlag		= TLHT_ONHEADERRIGHT;
				nSpaceMin	= nGap;
				bOnGaps		= TRUE;
			}
			else
			{
				// fine more closed right border from right
				if( nGap <= nSpaceMin )
				{
					*pCol		= iCol;
					*pFlag		= TLHT_ONHEADERRIGHT;
					nSpaceMin	= nGap;
				}
			}
		}
	} // end for
				
	return bOnGaps;
}

BOOL CTreeListHeaderCtrl::HitTest( CPoint pt, int* pFlag, int* pCol )
{
	// hit test for header
	ASSERT( pFlag != NULL );
	ASSERT( pCol != NULL );

	CPtrArray&			 arColumns	= m_pTreeListCtrl->m_arColumns;
	CArray<int, int>&	 arShows	= m_pTreeListCtrl->m_arShows;
	CTreeListColumnInfo* pColumnInfo= NULL;
	int nWinPos = m_pTreeListCtrl->GetScrollPos( SB_HORZ );

	*pFlag = 0;
	*pCol = 0;

	CRect rcClient;
	rcClient.SetRect( 0, 0, GetHeaderWidth(), GetHeaderHeight() );
	rcClient.OffsetRect( -nWinPos, 0 );

	// check if in client rect
	if( !rcClient.PtInRect(pt) )
		return NULL;

	// check if on header
	int nPerfix = 0;
	for( int iShow = 0; iShow<arShows.GetSize(); iShow++ )
	{
		int iCol = arShows[iShow];
		pColumnInfo	 = (CTreeListColumnInfo*)arColumns[iCol];

		CRect rcColumn;
		rcColumn.SetRect( 0, 0, pColumnInfo->m_nWidth, GetHeaderHeight() );
		rcColumn.OffsetRect( nPerfix, 0 );
		rcColumn.OffsetRect( -nWinPos, 0 );

		if( rcColumn.PtInRect( pt ) )
		{
			CRect rcHeader;
			
			// if on border
			rcHeader = rcColumn;
			rcHeader.DeflateRect( 2, 0, 3, 0 );
			if( rcHeader.Width()>0 )
			{
				if( !rcHeader.PtInRect( pt ) )
					*pFlag |= TLHT_ONHEADERGAPS;
			}

			// if on sort arrow
			if( pColumnInfo->m_dwFormat&TLF_CAPTION_SORT )
			{
				CRect rcSort;
				rcSort = rcHeader;
				rcSort.left = rcSort.right - TLL_WIDTH/2;
				
				if( rcSort.PtInRect( pt ) )
					*pFlag |= TLHT_ONHEADERSORT;

				rcHeader.DeflateRect( 0, 0, TLL_WIDTH/2, 0 );
			}

			// if on image
			if( rcHeader.Width()>0 && pColumnInfo->m_dwFormat&TLF_CAPTION_IMAGE )
			{
				CRect rcImage;
				if( pColumnInfo->m_dwFormat&TLF_IMAGEALIGN_LEFT )
				{
					rcImage			= rcHeader;
					rcImage.right	= rcHeader.left + TLL_WIDTH;
					rcHeader.left	= rcHeader.left + TLL_WIDTH;
				}
				else if( pColumnInfo->m_dwFormat&TLF_IMAGEALIGN_RIGHT )
				{
					rcImage			= rcHeader;
					rcImage.left	= rcHeader.right - TLL_WIDTH;
					rcImage.right	= rcHeader.right - TLL_WIDTH;
				}
				else if( pColumnInfo->m_dwFormat&TLF_IMAGEALIGN_CENTER )
				{
					rcImage			= rcHeader;
					rcHeader.left	= rcHeader.right;
				}
				else
				{
					rcImage			= rcHeader;
					rcImage.right	= rcHeader.left + TLL_WIDTH;
					rcHeader.left	= rcHeader.left + TLL_WIDTH;
				}

				if( rcImage.PtInRect( pt ) )
					*pFlag |= TLHT_ONHEADERIMAGE;
			}
			
			// if on text
			if( rcHeader.Width()>0 && pColumnInfo->m_dwFormat&TLF_CAPTION_TEXT )
			{
				if( rcHeader.PtInRect( pt ) )
					*pFlag |= TLHT_ONHEADERTEXT;
			}

			*pCol = iCol;

			return TRUE;
		}

		nPerfix += rcColumn.Width();
	}

	return FALSE;
}

int CTreeListHeaderCtrl::GetSortIndex( int iCol )
{
	CArray<int, int>&	Sorts	= m_pTreeListCtrl->m_arSorts;

	int iRet = -1;

	for( int iSort = 0; iSort<Sorts.GetSize(); iSort++ )
	{
		if( iCol == Sorts[iSort] )
		{
			iRet = iSort;
			break;
		}
	}

	return iRet;
}

DWORD CTreeListHeaderCtrl::GetStyle()
{
	return m_dwStyle;
}

DWORD CTreeListHeaderCtrl::SetStyle( DWORD dwStyle )
{
	DWORD dwoldStyle = m_dwStyle;
	m_dwStyle = dwStyle;
	return dwoldStyle;
}

void CTreeListHeaderCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonDown(nFlags, point);
}

void CTreeListHeaderCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if( m_pTreeListCtrl && !m_pTreeListCtrl->m_arColumns.IsEmpty()) {
		
		ClientToScreen(&point);
		CNewMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, 1202, "&Ajustar Columnas");
		menu.AppendMenu(MF_STRING, 1201, "A&justar al Contenido");
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	CWnd::OnRButtonUp(nFlags, point);
}

void CTreeListHeaderCtrl::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonDblClk(nFlags, point);

	if( !m_bSizing)
	{
		int iFlag;
		int iCol;
		
		if( HitTestSize( point, &iFlag, &iCol ) )
		{
			m_pTreeListCtrl->SetColumnWidth( iCol, TLSCW_AUTOSIZE );
		}
	}
}

void CTreeListHeaderCtrl::OnSysColorChange()
{
	CWnd::OnSysColorChange();
	m_cr3DBkgnd		= 0x808080;
	m_cr3DFace		= GetSysColor( COLOR_3DFACE );
	m_cr3DLight		= GetSysColor( COLOR_3DHILIGHT );
	m_cr3DShadow	= GetSysColor( COLOR_3DSHADOW );
	m_crText		= GetSysColor( COLOR_BTNTEXT );
	m_crTextOver    = GetSysColor( COLOR_HIGHLIGHT);
}

void CTreeListHeaderCtrl::OnAjustarColumnas() {

	if( m_pTreeListCtrl) {
		m_pTreeListCtrl->AutosizeColumns();
	}
}

void CTreeListHeaderCtrl::OnAjustarContenido() {

	if( m_pTreeListCtrl) {
		m_pTreeListCtrl->AutosizeColumns(false);
	}
}
