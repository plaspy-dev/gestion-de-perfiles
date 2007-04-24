// TreeListCtrl.cpp : implementation file
//

#include "stdafx.h"
# include "Componentes.h"
#include "memdc.h"
#include "treelistctrl/TreeListHeaderCtrl.h"
#include "treelistctrl/TreeListCtrl.h"
#include "ResourceDLL.h"
#include "treelistctrl/TreeListItem.h"
# include "treelistctrl/threadtreestat.h"
# include <resource.h>
# include "EditFloat.h"
# include "draw.h"
# include <windowsx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


# define USE_MUTEX
# define TIMEOUT_MUTEX	500


/////////////////////////////////////////////////////////////////////////////
// CTreeListCtrl
IMPLEMENT_DYNCREATE(CTreeListCtrl, CWnd)

CTreeListCtrl::CTreeListCtrl() : 
	m_nItemHeight( 0 ),
	m_dwStyle( 0 ),
	m_dwState( TLS_OFFSCREENBUFFER ),
	m_iSelCol( -1 ),
	m_pFocusItem( NULL ),
	m_pImageList( NULL )
{
	// construct

	// create windows class
	WNDCLASS wndclass;
	HINSTANCE hInst = AfxGetInstanceHandle();
	CResourceDLL	resource;

	if( !::GetClassInfo(hInst, TREELISTCTRL_CLASSNAME, &wndclass) )
	{
        wndclass.style			= CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wndclass.lpfnWndProc	= ::DefWindowProc;
		wndclass.cbClsExtra		= 0;
		wndclass.cbWndExtra		= 0;
		wndclass.hInstance		= hInst;
		wndclass.hIcon			= NULL;
		wndclass.hCursor		= AfxGetApp()->LoadStandardCursor( IDC_ARROW);
		wndclass.hbrBackground	= (HBRUSH)COLOR_WINDOW;
		wndclass.lpszMenuName	= NULL;
		wndclass.lpszClassName	= TREELISTCTRL_CLASSNAME;

		if (!AfxRegisterClass(&wndclass))
			AfxThrowResourceException();
	}

	// back ground colors of normal status
	m_crBkColor				= ::GetSysColor(COLOR_WINDOW);

	// back ground colors of selected item
	m_crBkSelectedNoFocus	=  HLS_TRANSFORM( ::GetSysColor(COLOR_HIGHLIGHT),  +80, -66);

	// text colors
	// normal text color
	m_crText				= ::GetSysColor(COLOR_WINDOWTEXT);

	// grid color
	m_crGrid				= ::GetSysColor(COLOR_3DLIGHT);
	m_crTreeLine			= ::GetSysColor(COLOR_3DDKSHADOW);

	m_Palette.CreateStockObject( DEFAULT_PALETTE );
	m_Font.CreateStockObject( SYSTEM_FIXED_FONT );

	LOGFONT lfont;
	m_Font.GetLogFont(&lfont);
	lfont.lfWeight = FW_BOLD;
	m_pFontBold.CreateFontIndirect(&lfont);

	/*m_pRootItem = new CTreeListItem( 1 );
	m_pRootItem->Expand();*/
	
	m_pThread= NULL;
	m_hMenuContextual= NULL;
	m_bCanShowMenu= false;
	m_szVacia= _T("No hay elementos");
	m_nSortedColumn= -1;
	m_bAscendente= true;
	m_hSelect=NULL;
	m_nOrderBy= -1;
	m_bCanSort= true;
	m_bCanStat=true;
	m_bCustomSort=false;
	m_pRootItem= m_pItemMap.GetRootItem();
}

CTreeListCtrl::~CTreeListCtrl()
{
	if(m_pThread) 
		delete m_pThread;
	// deconstruct
	m_wndHeader.DestroyWindow();

	if( m_Palette.m_hObject != NULL )
		m_Palette.DeleteObject();

	if( m_Font.m_hObject != NULL )
		m_Font.DeleteObject();
	if( m_pFontBold.m_hObject!=NULL)
		m_pFontBold.DeleteObject();

	while( m_arColumns.GetSize() > 0 )
	{
		CTreeListColumnInfo* pColumnInfo;
		int nIndex;
		
		nIndex = m_arColumns.GetUpperBound();
		pColumnInfo = (CTreeListColumnInfo*) m_arColumns[nIndex];

		m_arColumns.RemoveAt( nIndex );
		delete pColumnInfo;
	}

	if(m_hMenuContextual)
		delete m_hMenuContextual;
	m_imgCheck.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CTreeListCtrl, CWnd)
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_ENABLE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CANCELMODE()
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(ID_EDIT_UNDO,CollapseAll)
	ON_COMMAND(ID_EDIT_CUT, ExpandAll)
	ON_MESSAGE(WM_COPY,OnCopy)
	ON_MESSAGE(WM_UNDO,OnUndo)
	ON_MESSAGE(WM_CUT,OnCut)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeListCtrl message handler
BOOL CTreeListCtrl::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	// create control
	SetState( TLS_SUBCLASSFROMCREATE );
	DWORD dwExStyle = 0;

	if( dwStyle&WS_BORDER )
	{
		dwStyle &= ~WS_BORDER;
		dwExStyle |= WS_EX_CLIENTEDGE;
	}

	if( !CWnd::CreateEx( dwExStyle, TREELISTCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID ) )
		return FALSE;

	if( !Create() )
		return FALSE;

	return TRUE;
}

BOOL CTreeListCtrl::Create()
{
	// create control
	CRect Rect( 0, 0, 0, 0);

	if( !m_wndHeader.Create( 0, Rect, this, 0 ) )
		return FALSE;

	CResourceDLL resorce;

	if( !m_imgCheck.Create( IDB_TREELIST_CHECK, 16, 3, 0xFF00FF ) )
		return FALSE;

	SetFont();
	SetItemHeight();
	
	m_wndHeader.SetHeaderFont();
	m_wndHeader.SetHeaderHeight();
	
	Layout();

	return TRUE;
}

void CTreeListCtrl::PreSubclassWindow() 
{
	// pre subclass window
	CWnd::PreSubclassWindow();

	if( !GetState( TLS_SUBCLASSFROMCREATE ) )
	{
		if( !Create() )
		{
			AfxThrowMemoryException();
			return;
		}
	}

	return;
}

void CTreeListCtrl::OnDestroy() 
{
	if( m_pThread) {
		m_pThread->Kill();
		delete m_pThread;
		m_pThread= NULL;
	}
	CWnd::OnDestroy();
	
	m_arShows.RemoveAll();
	m_arSorts.RemoveAll();

	DeleteItem( TLI_ROOT );
}

void CTreeListCtrl::SetCustomSort(bool custom/*=true*/) {

	m_bCustomSort=custom;
}

void CTreeListCtrl::SetCanSort(bool sort) {

	m_bCanSort= sort;
    m_wndHeader.m_bShowOver=m_bCanSort;
}

void CTreeListCtrl::SetAllScroll()
{
	// set all scroll bars
	CRect rcPreClient;
	GetWindowRect( &rcPreClient );
	rcPreClient.DeflateRect( 1, 1 );

	int cx = ::GetSystemMetrics( SM_CXVSCROLL );
	int cy = ::GetSystemMetrics( SM_CYHSCROLL );

	SCROLLINFO siv;
	SCROLLINFO sih;

	GetScrollInfo( SB_VERT, &siv, SIF_POS | SIF_RANGE );
	GetScrollInfo( SB_HORZ, &sih, SIF_POS | SIF_RANGE );

	BOOL bHorzScroll = FALSE;
	BOOL bVertScroll = FALSE;

	if( rcPreClient.Width() < GetWidth() )
	{
		bHorzScroll = TRUE;

		if( rcPreClient.Height() - cy < m_wndHeader.GetHeaderHeight() + GetVisibleCount() * GetItemHeight() )
			bVertScroll = TRUE;
	}
	else if( rcPreClient.Height() < m_wndHeader.GetHeaderHeight() + GetVisibleCount() * GetItemHeight() )
	{
		bVertScroll = TRUE;

		if( rcPreClient.Width() - cx < GetWidth() )
			bHorzScroll = TRUE;
	}

	if( sih.nPos != sih.nMin )
		bHorzScroll = TRUE;

	if( siv.nPos != siv.nMin )
		bVertScroll = TRUE;

	if( bHorzScroll )
	{
		if( rcPreClient.Width() > cx )
			rcPreClient.right -= cx;
		else
			rcPreClient.right = rcPreClient.left;
	}

	if( bVertScroll )
	{
		if( rcPreClient.Height() > cy )
			rcPreClient.bottom -= cy;
		else
			rcPreClient.bottom = rcPreClient.top;
	}

	SetHorzScroll( &rcPreClient );
	SetVertScroll( &rcPreClient );
}

void CTreeListCtrl::SetHorzScroll( CRect* pPreClient )
{
	// set horizonal scroll
	SCROLLINFO si;
	GetScrollInfo( SB_HORZ, &si, SIF_ALL );

	if( pPreClient->Width() < GetWidth() )
	{
		si.cbSize	= sizeof( SCROLLINFO );
		si.nMin		= 0;
		if(si.nMax==GetWidth() - 1)
			return;
		si.nMax		= GetWidth() - 1;
		si.nPage	= pPreClient->Width();
		si.nPos		= si.nPos;
		si.fMask	=  SIF_PAGE | SIF_RANGE;
		if( si.nPos > si.nMax - (int)si.nPage ) {
			si.nPos = si.nMax - (int)si.nPage;
			si.fMask|= SIF_POS;
		}
	}
	else
	{
		si.cbSize	= sizeof( SCROLLINFO );
		si.nMin		= 0;
		si.nMax		= 0;
		si.nPage	= 0;
		si.nPos		= 0;
		if(si.nMax==0)
			return;
		si.fMask	=  SIF_PAGE | SIF_RANGE | SIF_POS;
	}
	SetScrollInfo( SB_HORZ, &si, TRUE );
}

void CTreeListCtrl::SetVertScroll( CRect* pPreClient )
{
	// set vertical scroll
	SCROLLINFO si;
	GetScrollInfo( SB_VERT, &si, SIF_ALL );

	if( pPreClient->Height() < m_wndHeader.GetHeaderHeight() + GetVisibleCount() * GetItemHeight() || si.nPos != si.nMin )
	{
		if(si.nMax== GetVisibleCount() - 1)
			return;

		int nShowRows	=	m_rcTreeList.Height() / GetItemHeight();

		si.cbSize	= sizeof( SCROLLINFO );
		si.nMin		= 0;
		si.nMax		= GetVisibleCount() - 1;
		si.nPos		= si.nPos;
		si.nPage	= 10; // max( nShowRows, 1 ); // min( pPreClient->Height()/m_nRowHeight, si.nMax -si.nPos );
		si.fMask	=  SIF_PAGE | SIF_RANGE;
		if( si.nPos > si.nMax ) {
			si.nPos = si.nMax;
			si.fMask|= SIF_POS;
		}
	}
	else
	{
		if(si.nMax== 0)
			return;

		si.cbSize	= sizeof( SCROLLINFO );
		si.nMin		= 0;
		si.nMax		= 0;
		si.nPage	= 0;
		si.nPos		= 0;
		si.fMask	=  SIF_PAGE | SIF_RANGE | SIF_POS;
	}
	SetScrollInfo( SB_VERT, &si, TRUE );
}


void CTreeListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// on horizonal scroll
	SCROLLINFO si;
	GetScrollInfo( SB_HORZ, &si, SIF_ALL );

	switch( nSBCode )
	{
	case SB_LINEUP:
		if( si.nPos > si.nMin )
			si.nPos--;
		break;

	case SB_LINEDOWN:
		if( si.nPos <= si.nMax - (int)si.nPage )
			si.nPos++;
		break;

	case SB_PAGEUP:
		si.nPos -= si.nPage;
		if( si.nPos < si.nMin )
			si.nPos = si.nMin;
		break;

	case SB_PAGEDOWN:
		si.nPos += si.nPage;
		if( si.nPos > si.nMax )
			si.nPos = si.nMax;
		break;

	case SB_TOP:
		si.nPos = si.nMin;
		break;

	case SB_BOTTOM:
		si.nPos = si.nMax;
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		si.nPos = si.nTrackPos;
		break;

	default:
		break;
	}
	
	SetScrollPos( SB_HORZ, si.nPos );
	m_wndHeader.RedrawWindow();
	RedrawWindow();
}

void CTreeListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// on vertival scroll
	SCROLLINFO si;
	GetScrollInfo( SB_VERT, &si, SIF_ALL );

	switch( nSBCode )
	{
	case SB_LINEUP:
		if( si.nPos > si.nMin )
			si.nPos--;
		break;

	case SB_LINEDOWN:
		if( si.nPos <= si.nMax - (int)si.nPage )
			si.nPos++;;
		break;

	case SB_PAGEUP:
		si.nPos -= si.nPage;
		if( si.nPos < si.nMin )
			si.nPos = si.nMin;
		break;

	case SB_PAGEDOWN:
		si.nPos += si.nPage;
		if( si.nPos > si.nMax )
			si.nPos = si.nMax;
		break;

	case SB_TOP:
		si.nPos = si.nMin;
		break;

	case SB_BOTTOM:
		si.nPos = si.nMax;
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		si.nPos = si.nTrackPos;
		break;

	default:
		break;
	}

	if( si.nPos == si.nMin )
	{
		if(  si.nMax-si.nMin+1 <= m_rcClient.Height()/m_nItemHeight )
		{
			Layout();
		}
	}
	SetScrollPos( SB_VERT, si.nPos );
	Invalidate();
}

LRESULT CTreeListCtrl::Notify( DWORD dwMessage, HTREELISTITEM pItem, int iCol )
{
	// notify a message
	NMTREELIST nm;
	nm.hdr.hwndFrom = GetSafeHwnd();
	nm.hdr.idFrom	= GetDlgCtrlID();
	nm.hdr.code		= dwMessage;

	nm.pItem	= pItem;
	nm.iCol		= iCol;

	if( nm.iCol == -1 )
		nm.iCol = 0;

	CWnd* pWnd		= GetParent();
	LRESULT lResult	= 0;

	if( pWnd == NULL ) {
		lResult = 0;
	} else {
		lResult = pWnd->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nm );
	}

	return lResult;	
}

void CTreeListCtrl::Layout()
{
	// layout size
	if( !IsWindow( m_wndHeader.GetSafeHwnd() ) )
		return;
	SetAllScroll();
	GetClientRect( &m_rcClient );
	m_rcHeader		= m_rcClient;
	m_wndHeader.SendMessage( HDM_LAYOUT, (WPARAM)0, (LPARAM)&m_rcHeader );
	if( m_rcHeader.Height() == 0 )
	{
		m_rcTreeList = m_rcClient;
	}
	else if( m_rcHeader.Height() == m_rcClient.Height() )
	{
		m_rcTreeList= CRect(m_rcClient.left, m_rcHeader.bottom, m_rcClient.right, m_rcHeader.bottom);
	}
	else
	{
		m_rcTreeList= CRect(m_rcClient.left, m_rcHeader.bottom, m_rcClient.right, m_rcClient.bottom);
	}
	SetAllScroll();
}
int CTreeListCtrl::GetWidth()
{
	// retrieve total width
	int nWidth = 0;

	for( int iShow=0; iShow<m_arShows.GetSize(); iShow++ )
	{
		CTreeListColumnInfo* pColumnInfo;
		int iCol;

		iCol = m_arShows[iShow];
		pColumnInfo = (CTreeListColumnInfo*)m_arColumns[iCol];

		nWidth += pColumnInfo->m_nWidth;
	}

	return nWidth;
}

int CTreeListCtrl::GetHeight()
{
	// retrieve total height
	return GetItemHeight() * GetVisibleCount();
}

int CTreeListCtrl::GetColumns()
{
	// retrieve total number of columns
	return m_arColumns.GetSize();
}

int CTreeListCtrl::GetCount()
{
	// retrieve a count of the items in treelist control
	return m_pItemMap.GetChildCount(m_pItemMap.GetRootItem());
}

CImageList* CTreeListCtrl::GetImageList()
{
	// retrieve global point of image list
	return m_pImageList;
}

void CTreeListCtrl::SetImageList( CImageList* pImageList )
{
	// set global point of image list
	ASSERT_VALID( pImageList );
	m_pImageList = pImageList;

	Invalidate();
}

HTREELISTITEM CTreeListCtrl::GetNextItem( HTREELISTITEM pItem, UINT nCode )
{
	// retrieve next item specified by pItem
	HTREELISTITEM pRetItem = NULL;

	switch( nCode )
	{
	case TLGN_ROOT:				pRetItem = GetRootItem();					break;
	case TLGN_NEXT:				pRetItem = GetNextSiblingItem( pItem );		break;
	case TLGN_PREVIOUS:			pRetItem = GetPrevSiblingItem( pItem );		break;
	case TLGN_PARENT:			pRetItem = GetParentItem( pItem );			break;
	case TLGN_CHILD:			pRetItem = GetChildItem( pItem );			break;
	case TLGN_FIRSTVISIBLE:		pRetItem = GetFirstVisibleItem();			break;
	case TLGN_NEXTVISIBLE:		pRetItem = GetNextVisibleItem( pItem );		break;
	case TLGN_PREVIOUSVISIBLE:	pRetItem = GetPrevVisibleItem( pItem );		break;
	case TLGN_DROPHILITE:		pRetItem = NULL;							break;
	case TLGN_CARET:			pRetItem = GetSelectedItem();				break;
	case TLGN_NEXTSELECT:		pRetItem = GetNextSelectedItem( pItem );	break;
	default:					pRetItem = NULL;							break;
	}

	return pRetItem;
}

BOOL CTreeListCtrl::ItemHasChildren( HTREELISTITEM pItem )
{
	// determine whether the treelist item specified by pItem has a child item
	return m_pItemMap.ItemHasChildren(pItem);
}
HTREELISTITEM CTreeListCtrl::GetChildItem(HTREELISTITEM pItem) 
{
	return m_pItemMap.GetChildItem(pItem);
}

HTREELISTITEM CTreeListCtrl::GetNextSiblingItem( HTREELISTITEM pItem )
{
	// retrieve next sibling item specified by pItem
	return m_pItemMap.GetNextSiblingItem(pItem);
}

HTREELISTITEM CTreeListCtrl::GetPrevSiblingItem( HTREELISTITEM pItem )
{
	// retrieve prev sibling item specified by pItem
	return m_pItemMap.GetPrevSiblingItem(pItem);
}

HTREELISTITEM CTreeListCtrl::GetParentItem( HTREELISTITEM pItem )
{
	// retrieve parent item specified by pItem
	return m_pItemMap.GetParentItem(pItem);
}

HTREELISTITEM CTreeListCtrl::GetFirstVisibleItem()
{
	// retrieve first visible item of treelist
	return m_pItemMap.GetChildItem(m_pItemMap.GetRootItem());
}

HTREELISTITEM CTreeListCtrl::GetNextVisibleItem( HTREELISTITEM pItem )
{
	// retrieve next visible item of treelist
	ASSERT( pItem != NULL );
	return GetNextVisibleItem( pItem, TRUE );
}

HTREELISTITEM CTreeListCtrl::GetNextVisibleItem( HTREELISTITEM pItem, BOOL bChild )
{
	// retrieve next visible item of treelist
	ASSERT( pItem != NULL );
	if( m_pItemMap.GetItemState(pItem)&TLIS_EXPANDED )
		if( bChild && m_pItemMap.ItemHasChildren(pItem)) {
			return m_pItemMap.GetChildItem(pItem);
		}

	if( m_pItemMap.GetNextSiblingItem(pItem)!= NULL ) 
		return m_pItemMap.GetNextSiblingItem(pItem);

	if( pItem != m_pRootItem )
	{
		pItem = m_pItemMap.GetParentItem(pItem);
		if( pItem != m_pRootItem ) {
			return GetNextVisibleItem( pItem, FALSE );
		}
	}
	return NULL;
}

HTREELISTITEM CTreeListCtrl::GetPrevVisibleItem( HTREELISTITEM pItem )
{
	// retrieve previous visible item of treelist
	ASSERT( pItem != NULL );
	return GetPrevVisibleItem( pItem, TRUE );
}

HTREELISTITEM CTreeListCtrl::GetPrevVisibleItem( HTREELISTITEM pItem, BOOL bChild )
{
	// retrieve previous visible item of treelist
	ASSERT( pItem != NULL );
	if( m_pItemMap.GetPrevSiblingItem(pItem)!= NULL )
	{
		pItem = m_pItemMap.GetPrevSiblingItem(pItem);
		if( m_pItemMap.GetItemState(pItem)&TLIS_EXPANDED && m_pItemMap.ItemHasChildren(pItem))
		{
			pItem = GetLastVisibleItem( pItem );
		}
		return pItem;
	}

	pItem = m_pItemMap.GetParentItem(pItem);
	if( pItem != m_pRootItem ) {
		return pItem;
	}
	return FALSE;
}

HTREELISTITEM CTreeListCtrl::GetLastVisibleItem( HTREELISTITEM pItem )
{
	// retrieve last visible child item of item specified by pItem
	ASSERT( pItem != NULL && m_pItemMap.ItemHasChildren(pItem));

	HTREELISTITEM pChild;
	pChild = m_pItemMap.GetChildItem(pItem);

	while( m_pItemMap.GetNextSiblingItem(pChild)!= NULL )
	{
		pChild = m_pItemMap.GetNextSiblingItem(pChild);
	}

	if( m_pItemMap.GetItemState(pChild)&TLIS_EXPANDED && m_pItemMap.ItemHasChildren(pChild)) {
		return GetLastVisibleItem( pChild );
	} else {
		return pChild;
	}
}


HTREELISTITEM CTreeListCtrl::GetSelectedItem()
{
	// always retrieve first selected item
	POSITION pos = GetFirstSelectedItemPosition();
	return GetNextSelectedItem( pos );
}

HTREELISTITEM CTreeListCtrl::GetNextSelectedItem( HTREELISTITEM pItem )
{
	// retrieve next selected item specified by pItem
	if( pItem == NULL )
		pItem= m_pRootItem;
		// return NULL;

	if( pItem == m_pRootItem && m_arSelects.GetSize() > 0 ) {
		
		// reordenamos los seleccionados
		int count= m_arSelects.GetCount();
		if( count>1) {

			HTREELISTITEM item;
			DWORD *ptr= new DWORD[count];

			for( int i=0; i<count; i++) {
				if(item= (HTREELISTITEM)m_arSelects.GetAt(i)) 
					m_pItemMap.SetItemDataSort(item,GetTreeListItem(item));
			}

			m_pItemMap.SortSubItems(0, m_arSelects, ptr, count);

			if( ptr)
				delete ptr;
		}
		return (HTREELISTITEM)m_arSelects[0];
	}

	for( int iSel = 0; iSel<m_arSelects.GetSize()-1; iSel++ )
	{
		if( pItem == (HTREELISTITEM)m_arSelects[iSel] ) {
			return (HTREELISTITEM)m_arSelects[iSel+1];
		}
	}
	return NULL;
}

HTREELISTITEM CTreeListCtrl::GetRootItem()
{
	// retrieve root item of treelist control
	return m_pItemMap.GetChildItem(m_pRootItem);
}

DWORD CTreeListCtrl::GetItemState(HTREELISTITEM pItem, DWORD nStateMask )
{
	return m_pItemMap.GetItemState(pItem, nStateMask);
}

void CTreeListCtrl::SetItemState(HTREELISTITEM pItem, DWORD dwAddState, DWORD dwRemoveState)
{
	// set status of item specified by pItem
	ASSERT( pItem != NULL );
	m_pItemMap.SetItemState(pItem , dwAddState, dwRemoveState );
	Invalidate();
}

void CTreeListCtrl::GetItemImage( HTREELISTITEM pItem, int& nImage, int& nExpandImage, int& nSelectedImage, int& nExpandSelectedImage )
{
	// retrieve images of item specified by pItem
	ASSERT( pItem != NULL );
	m_pItemMap.GetItemImage(pItem, nImage, nExpandImage, nSelectedImage, nExpandSelectedImage );
}

void CTreeListCtrl::SetItemImage( HTREELISTITEM pItem, int nImage, int nExpandImage, int nSelectedImage, int nExpandSelectedImage )
{
	// set images of item specified by pItem
	ASSERT( pItem != NULL );
	m_pItemMap.SetItemState(pItem, TLIS_SHOWTREEIMAGE);
	m_pItemMap.SetItemImage(pItem, nImage, nExpandImage, nSelectedImage, nExpandSelectedImage );
	Invalidate();
}	

LPCTSTR	CTreeListCtrl::GetItemText( HTREELISTITEM pItem, int nSubItem )
{
	// retrieve text of item specified by pItem
	ASSERT( pItem != NULL );
	return m_pItemMap.GetItemText(pItem, nSubItem);
}

BOOL CTreeListCtrl::SetItemText( HTREELISTITEM pItem, LPCTSTR lpszText, int nSubItem )
{
	// set text of item specified by pItem
	ASSERT( pItem != NULL );
	BOOL bRet = m_pItemMap.SetItemText(pItem, lpszText, nSubItem);
	Invalidate();
	return bRet;
}

COLORREF CTreeListCtrl::GetItemColor(HTREELISTITEM pItem) {

	return m_pItemMap.GetItemColor(pItem);
}

void CTreeListCtrl::SetItemColor(HTREELISTITEM pItem, COLORREF color) {

	m_pItemMap.SetItemColor(pItem, color);
	Invalidate();
}

void CTreeListCtrl::SetItemBold(HTREELISTITEM pItem, bool bold/*=true*/) {

	m_pItemMap.SetItemBold(pItem, bold);
	Invalidate();
}

int	CTreeListCtrl::GetChildCount(HTREELISTITEM pItem) {

	return m_pItemMap.GetChildCount(pItem);
}

DWORD_PTR CTreeListCtrl::GetItemData( HTREELISTITEM pItem )
{
	return m_pItemMap.GetItemData(pItem);
}

void CTreeListCtrl::SetItemData( HTREELISTITEM pItem, DWORD_PTR dwData )
{
	m_pItemMap.SetItemData(pItem, dwData);
}

DWORD_PTR CTreeListCtrl::GetItemDataSort( HTREELISTITEM pItem )
{
	// retrieve 32-bit data of item specified by pItem
	ASSERT( pItem != NULL );
	return m_pItemMap.GetItemDataSort(pItem);
}

void CTreeListCtrl::SetItemDataSort( HTREELISTITEM pItem, DWORD_PTR dwData )
{
	m_pItemMap.SetItemDataSort(pItem, dwData);
}

int CTreeListCtrl::GetVisibleCount()
{
	// retrieve the  number of visible items in the treelist control
	return GetVisibleCount( m_pRootItem );
}

int CTreeListCtrl::GetItemHeight()
{
	// retrieve the height of the item
	return m_nItemHeight;
}

int CTreeListCtrl::SetItemHeight( int cyHeight )
{
	// set the height of the item
	int nOldHeight = m_nItemHeight;

	if( cyHeight == -1 )
	{
		ASSERT( m_Font.m_hObject != NULL );

		LOGFONT lf;
		m_Font.GetLogFont( &lf );
	
		if( lf.lfHeight < 0 )
		{
			m_nItemHeight = -lf.lfHeight + 5;
		}
		else if( lf.lfHeight == 0 )
		{
			m_nItemHeight = DEFAULT_HEIGHT;
		}
		else
		{
			m_nItemHeight = lf.lfHeight + 5;
		}
	}
	else
	{
		m_nItemHeight = cyHeight;
	}

	if( m_nItemHeight % 2 == 1 )
		m_nItemHeight ++;

	Invalidate();

	return nOldHeight;
}

COLORREF CTreeListCtrl::GetBkColor( int nColor )
{
	// set background color of treelist control
	return m_crBkColor;
}

COLORREF CTreeListCtrl::SetBkColor( COLORREF clr, int nColor )
{
	// retrieve background color of treelist control
	m_crBkColor= clr;
	Invalidate();
	return m_crBkColor;
}

COLORREF CTreeListCtrl::GetTextColor()
{
	// retrieve the color of the text
	return m_crText;
}

COLORREF CTreeListCtrl::SetTextColor( COLORREF clr )
{
	// set the color of the text
	COLORREF oldclr;
	oldclr = m_crText;
	m_crText = clr;

	Invalidate();

	return m_crText;
}

int CTreeListCtrl::GetCheck( HTREELISTITEM pItem )
{
	return m_pItemMap.GetItemCheck(pItem);
}

void CTreeListCtrl::SetCheck( HTREELISTITEM pItem, BOOL bCheck )
{
	// set an item checked box state
	ASSERT( pItem != NULL );
	m_pItemMap.SetItemCheck(pItem, bCheck);
	if( m_pItemMap.GetItemCheck(pItem) )
	{
		SetItemChildStatus( pItem, TLIS_CHECKED, TLIS_CHECKED_NULL );
		SetItemParentStatus( pItem, TLIS_CHECKEDMASK );
	}
	else
	{
		SetItemChildStatus( pItem, TLIS_CHECKED_NULL, TLIS_CHECKED );
		SetItemParentStatus( pItem, TLIS_CHECKEDMASK );
	}
	Invalidate();
}

int CTreeListCtrl::GetLock( HTREELISTITEM pItem )
{
	// retrieve an item locked box state
	return m_pItemMap.GetItemLock(pItem);
}

void CTreeListCtrl::SetLock( HTREELISTITEM pItem, BOOL bLock )
{
	// set an item locked box state
	ASSERT( pItem != NULL );
	m_pItemMap.SetItemLock(pItem, bLock);

	if( m_pItemMap.GetItemLock(pItem))
	{
		SetItemChildStatus( pItem, TLIS_LOCKED, TLIS_LOCKED_NULL );
		SetItemParentStatus( pItem, TLIS_LOCKEDMASK );
	}
	else
	{
		SetItemChildStatus( pItem, TLIS_LOCKED_NULL, TLIS_LOCKED );
		SetItemParentStatus( pItem, TLIS_LOCKEDMASK );
	}
	Invalidate();
}

BOOL CTreeListCtrl::GetItemRect( HTREELISTITEM pItem, LPRECT lpRect )
{
	// retrieve rect of an item
	int nFirstRow	=	GetScrollPos( SB_VERT );
	int	nShowRows	=	m_rcTreeList.Height() / GetItemHeight() + 1;

	CRect rcItem;
	rcItem.SetRect( 0, 0, GetWidth(), GetItemHeight() );
	rcItem.OffsetRect( m_rcTreeList.left, m_rcTreeList.top );
	rcItem.OffsetRect( -GetScrollPos( SB_HORZ ), 0 );

	for( int i = nFirstRow; i<= nFirstRow + nShowRows && i < GetVisibleCount(); i++ )
	{
		HTREELISTITEM pShowItem = GetFirstShowItem( i );
		if( pShowItem == pItem )
		{
			lpRect->left	= rcItem.left;
			lpRect->right	= rcItem.right;
			lpRect->top		= rcItem.top;
			lpRect->bottom	= rcItem.bottom;

//			lpRect->top		+= 1;
//			lpRect->bottom	-= 1;

			return TRUE;
		}

		rcItem.OffsetRect( 0, GetItemHeight() );
	}
	return FALSE;
}

BOOL CTreeListCtrl::GetItemRect( HTREELISTITEM pItem, int iSubItem, LPRECT lpRect, BOOL bTextOnly )
{
	// retrieve rect of a column of an item
	CRect rcItem;
	
	if( GetItemRect( pItem, rcItem ) )
	{
		int nColPos = 0;
		CTreeListColumnInfo* pColumnInfo;

		for( int iShow = 0; iShow < m_arShows.GetSize(); iShow++, nColPos += pColumnInfo->GetWidth() )
		{
			int iCol;

			iCol = m_arShows[iShow];
			pColumnInfo = (CTreeListColumnInfo*)m_arColumns[iCol];

			if( iCol == iSubItem )
			{
				CRect rcColumn;
				rcColumn.SetRect( 0, rcItem.top, pColumnInfo->GetWidth(), rcItem.bottom );
				rcColumn.OffsetRect( nColPos, 0 );
				rcColumn.OffsetRect( rcItem.left, 0 );

				lpRect->left	= rcColumn.left;
				lpRect->right	= rcColumn.right;
				lpRect->top		= rcColumn.top;
				lpRect->bottom	= rcColumn.bottom;

				if( iCol == 0 && bTextOnly )
				{
					if ( m_dwStyle&TLC_TREELIST )
						return GetItemRectTree( pItem, lpRect );
					else
						return GetItemRectMain( pItem, lpRect );

				}

//				lpRect->left	+= 1;
//				lpRect->right	-= 1;

				return TRUE;
			}
		}
	}
	
	return FALSE;
}	

BOOL CTreeListCtrl::GetItemRectTree( HTREELISTITEM pItem, LPRECT lpRect )
{
	int nPerfix = 0;

	// skip perfix and tree line
	nPerfix += TLL_BORDER;
	nPerfix += (m_pItemMap.GetItemLevel(pItem)-1) * TLL_WIDTH;

	// skip tree button
	if( m_dwStyle&(TLC_TREELINE|TLC_BUTTON) )
		nPerfix += TLL_WIDTH;

	// skip tree check box
	if( m_dwStyle&TLC_CHECKBOX )
		if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWCHECKBOX )
			nPerfix += TLL_WIDTH;

	// skip tree lock box
	if( m_dwStyle&TLC_LOCKBOX )
		if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWLOCKBOX )
			nPerfix += TLL_WIDTH/2;

	// skip tree image
	if( m_dwStyle&TLC_IMAGE )
		if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWTREEIMAGE )
			nPerfix += TLL_WIDTH;

	// skip gaps between text and image/loc
	if( m_dwStyle&TLC_LOCKBOX || m_dwStyle&TLC_IMAGE )
		nPerfix += 2;

	if( lpRect->left + nPerfix + 2 <= lpRect->right )
	{
		lpRect->left	+= nPerfix;
//		lpRect->left	+= 1;
//		lpRect->right	-= 1;
		return TRUE;
	}
	
	lpRect->left	+= nPerfix;
	return FALSE;
}

BOOL CTreeListCtrl::GetItemRectMain( HTREELISTITEM pItem, LPRECT lpRect )
{
	int nPerfix = 0;

	// skip perfix
	nPerfix += TLL_BORDER;

	// skip tree button
	if( m_dwStyle&(TLC_TREELINE|TLC_BUTTON) )
		nPerfix += TLL_WIDTH;

	// skip tree check box
	if( m_dwStyle&TLC_CHECKBOX )
		if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWCHECKBOX )
			nPerfix += TLL_WIDTH;

	// skip tree lock box
	if( m_dwStyle&TLC_LOCKBOX )
		if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWLOCKBOX )
			nPerfix += TLL_WIDTH/2;

	// skip tree image
	if( m_dwStyle&TLC_IMAGE )
		if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWTREEIMAGE )
			nPerfix += TLL_WIDTH;

	// skip gaps between text and image/loc
	if( m_dwStyle&TLC_LOCKBOX || m_dwStyle&TLC_IMAGE )
		nPerfix += 2;

	if( lpRect->left + nPerfix + 2 <= lpRect->right )
	{
		lpRect->left	+= nPerfix;
//		lpRect->left	+= 1;
//		lpRect->right	-= 1;
		return TRUE;
	}
	
	lpRect->left += nPerfix;
	return FALSE;
}

BOOL CTreeListCtrl::EnsureVisible( HTREELISTITEM pItem, int iSubItem )
{
	// ensure an item is visible that specified by pItem
	int nItem		=	GetTreeListItem( pItem );
	int nFirstRow	=	GetScrollPos( SB_VERT );
	int nShowRows	=	m_rcTreeList.Height() / GetItemHeight();

	if( nShowRows > 0 )
		nShowRows = nShowRows - 1;

	if( nItem < nFirstRow )
	{
		SetScrollPos( SB_VERT, nItem );
	}
	else
	{
		if( nItem > nFirstRow + nShowRows )
			SetScrollPos( SB_VERT, nItem - nShowRows );
	}

	CRect rcClient;
	GetClientRect( rcClient );
	int	nCurrent	=	GetScrollPos( SB_HORZ );
	int nLeft		=	0;
	int nRight		=	0;
	for( int iShow = 0; iShow < m_arShows.GetSize(); iShow ++ )
	{
		CTreeListColumnInfo* pColumnInfo;
		pColumnInfo = (CTreeListColumnInfo*)m_arColumns[m_arShows[iShow]];

		if( m_arShows[iShow] == iSubItem )
		{
			nRight = pColumnInfo->GetWidth();
			break;
		}
		nLeft += pColumnInfo->GetWidth();
	}
	nRight = nLeft + nRight;

	if( iSubItem == 0 )
	{
		int iLevel		= m_pItemMap.GetItemLevel(pItem) - 1;
		CTreeListColumnInfo* pColumnInfo;
		pColumnInfo = (CTreeListColumnInfo*)m_arColumns[0];
		nLeft += TLL_WIDTH * iLevel;
	}

	if( nLeft <= nCurrent )
	{
		SetScrollPos( SB_HORZ, nLeft );
	}
	else
	{
		if( nCurrent + rcClient.Width() < nRight )
		{
			if( nRight-nLeft <= rcClient.Width() )
			{
				SetScrollPos( SB_HORZ, nRight - rcClient.Width() );
			}
			else
			{
				SetScrollPos( SB_HORZ, nLeft );
			}
		}
	}

	Invalidate();
	return TRUE;
}

int	CTreeListCtrl::GetItemCount()
{
	// retrieve count of all items.
	return m_pItemMap.GetChildCount(m_pRootItem)- 1;
}

int CTreeListCtrl::GetNextItem( int nItem, int nFlags )
{
	// retrieve next item specified by nItem
	HTREELISTITEM pItem = NULL;
	pItem = GetTreeListItem( nItem );

	if( pItem == NULL )
		return -1;

	if( nFlags&TLNI_ABOVE )
		return GetTreeListItem( GetPrevSiblingItem( pItem ) );

	if( nFlags&TLNI_BELOW )
		return GetTreeListItem( GetNextSiblingItem( pItem ) );

	return -1;
}

POSITION CTreeListCtrl::GetFirstSelectedItemPosition()
{
	// retrieve position of selected item
	if( m_arSelects.GetSize() == 0 )
		return NULL;

	POSITION pos = (POSITION)m_pRootItem;
	return pos;
}

HTREELISTITEM CTreeListCtrl::GetNextSelectedItem( POSITION& pos )
{
	// retrieve next selected item specified by pItem
	HTREELISTITEM pItem;
	pItem = GetNextSelectedItem( (HTREELISTITEM)pos );
	pos = (POSITION)pItem;
	return pItem;
}

int CTreeListCtrl::GetStringWidth( LPCTSTR lpsz )
{
	// retrieve width of string in pixel
	CPaintDC dc(this);

	CFont* pOldFont = dc.SelectObject( GetFont() );
	CSize sz = dc.GetTextExtent( lpsz, _tcslen(lpsz) );
	dc.SelectObject( pOldFont );

	return sz.cx + 6;
}

DWORD CTreeListCtrl::SetColumnFormat( int nCol, DWORD dwAdd, DWORD dwRemove )
{
	// set column fromat
	ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

	DWORD dwFormat = pColumnInfo->SetFormat( dwAdd, dwRemove );

	Invalidate();

	return dwFormat;
}

DWORD CTreeListCtrl::GetColumnFormat( int nCol, DWORD dwMask )
{
	// retrieve column format
	ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

	return pColumnInfo->GetFormat();
}

DWORD CTreeListCtrl::SetColumnModify( int nCol, DWORD dwModify )
{
	// retrieve column modify
	ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

	return pColumnInfo->SetModify( dwModify );
}

DWORD CTreeListCtrl::GetColumnModify( int nCol )
{
	// retrieve column modify
	ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

	return pColumnInfo->GetModify();
}

int CTreeListCtrl::SetColumnWidth( int nCol, int nWidth, int nMin, int nMax )
{
	// set column width
	if( nWidth == TLSCW_AUTOSIZE || nWidth == TLSCW_AUTOSIZE_USEHEADER )
	{
		// AutoSetWidth();		// all show items
		if( nWidth == TLSCW_AUTOSIZE )
		{
			nWidth = nMin;
		}
		else
		{
			nWidth = GetStringWidth( GetColumnText( nCol ) ) + 4;
		}

		HTREELISTITEM pShowItem = GetFirstShowItem( 0 );
		while( pShowItem != NULL )
		{
			ASSERT( pShowItem != NULL );

			int x = 0;
			if( nCol == 0 )
			{
				if( m_dwStyle&TLC_TREELIST )
					x += (m_pItemMap.GetItemLevel(pShowItem)-1) * TLL_WIDTH;

				if( m_dwStyle&(TLC_TREELINE|TLC_BUTTON) )
					x += TLL_WIDTH;

				if( m_dwStyle&TLC_CHECKBOX )
					if(m_pItemMap.GetItemState(pShowItem)&TLIS_SHOWCHECKBOX )
						x += TLL_WIDTH;

				if( m_dwStyle&TLC_LOCKBOX )
					if( m_pItemMap.GetItemState(pShowItem)&TLIS_SHOWLOCKBOX )
						x += TLL_WIDTH/2;

				if( m_dwStyle&TLC_IMAGE )
					if( m_pItemMap.GetItemState(pShowItem)&TLIS_SHOWTREEIMAGE )
						x += TLL_WIDTH;

				if( m_dwStyle&TLC_LOCKBOX || m_dwStyle&TLC_IMAGE )
					x += 2;
			}

			x += GetStringWidth( m_pItemMap.GetItemText(pShowItem, nCol ) );
			x += 2;

			if( nWidth < x )
				nWidth = x;

			pShowItem = GetNextShowItem( pShowItem );
		}
		return SetColumnWidth( nCol, nWidth+20, nMin, nMax );
	}
	else if( nWidth == TLSCW_AUTOSIZEV || nWidth == TLSCW_AUTOSIZEV_USEHEADER )
	{
		// AutoSetWidth();		// all visible items
		if( nWidth == TLSCW_AUTOSIZEV )
		{
			nWidth = nMin;
		}
		else
		{
			nWidth = GetStringWidth( GetColumnText( nCol ) ) + 4;
		}

		HTREELISTITEM pVisibleItem = GetFirstVisibleItem();
		while( pVisibleItem != NULL )
		{
			ASSERT( pVisibleItem != NULL );
			int x = 0;
			if( nCol == 0 )
			{
				if( m_dwStyle&TLC_TREELIST )
					x += (m_pItemMap.GetItemLevel(pVisibleItem)-1) * TLL_WIDTH;

				if( m_dwStyle&(TLC_TREELINE|TLC_BUTTON) )
					x += TLL_WIDTH;

				if( m_dwStyle&TLC_CHECKBOX )
					if( m_pItemMap.GetItemState(pVisibleItem)&TLIS_SHOWCHECKBOX )
						x += TLL_WIDTH;

				if( m_dwStyle&TLC_LOCKBOX )
					if( m_pItemMap.GetItemState(pVisibleItem)&TLIS_SHOWLOCKBOX )
						x += TLL_WIDTH/2;

				if( m_dwStyle&TLC_IMAGE )
					if( m_pItemMap.GetItemState(pVisibleItem)&TLIS_SHOWTREEIMAGE )
						x += TLL_WIDTH;

				if( m_dwStyle&TLC_LOCKBOX || m_dwStyle&TLC_IMAGE )
					x += 2;
			}

			x += GetStringWidth(m_pItemMap.GetItemText(pVisibleItem, nCol ) );
			x += 2;

			if( nWidth < x )
				nWidth = x;

			pVisibleItem = GetNextShowItem( pVisibleItem );
		}
		return SetColumnWidth( nCol, nWidth, nMin, nMax );
	}
	else
	{
		// SetWidth()
		ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

		CTreeListColumnInfo* pColumnInfo;
		pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

		int iRet = pColumnInfo->SetWidth( nWidth, nMin, nMax );
		Layout();
		Invalidate();
		return iRet;
	}
}

int CTreeListCtrl::GetColumnWidth( int nCol )
{
	// retrieve column width
	ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

	return pColumnInfo->GetWidth();
}

int CTreeListCtrl::SetColumnImage( int nCol, int iImage )
{
	// set index of column image
	ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

	int iRet = pColumnInfo->SetImage( iImage );

	Invalidate();

	return iRet;
}

int CTreeListCtrl::GetColumnImage( int nCol )
{
	// retrieve index of column image
	ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

	return pColumnInfo->GetImage();
}

BOOL CTreeListCtrl::SetColumnText( int nCol, LPCTSTR lpszText )
{
	// set text of column
	ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

	BOOL bRet = pColumnInfo->SetText( lpszText );

	Invalidate();

	return bRet;
}

LPCTSTR CTreeListCtrl::GetColumnText( int nCol )
{
	// retrieve text of column
	ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

	return pColumnInfo->GetText();
}

BOOL CTreeListCtrl::SetColumnDefaultText( int nCol, LPCTSTR lpszText )
{
	// set default text of column
	ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

	return pColumnInfo->SetDefaultText( lpszText );
}

LPCTSTR CTreeListCtrl::GetColumnDefaultText( int nCol )
{
	// retrieve default text of column
	ASSERT( 0 <= nCol && nCol < m_arColumns.GetSize() );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[nCol];

	return pColumnInfo->GetDefaultText();
}

DWORD CTreeListCtrl::GetItemState( int nItem, DWORD nStateMask )
{
	// retrieve item state ( list )
	ASSERT( 0 <= nItem && nItem < GetCount() );
	return GetItemState( GetTreeListItem(nItem), nStateMask );
}

void CTreeListCtrl::SetItemState( int nItem, DWORD dwAddState, DWORD dwRemoveState )
{
	// set item state ( list )
	ASSERT( 0 <= nItem && nItem < GetCount() );
	SetItemState( GetTreeListItem(nItem), dwAddState, dwRemoveState );
}

void CTreeListCtrl::GetItemImage( int nItem, int& nImage, int& nExpandImage, int& nSelectedImage, int& nExpandSelectedImage )
{
	// retrieve index of item image ( list )
	ASSERT( 0 <= nItem && nItem < GetCount() );
	GetItemImage( GetTreeListItem(nItem), nImage, nExpandImage, nSelectedImage, nExpandSelectedImage );
}

void CTreeListCtrl::SetItemImage( int nItem, int nImage, int nExpandImage, int nSelectedImage, int nExpandSelectedImage )
{
	// set index of item image ( list )
	ASSERT( 0 <= nItem && nItem < GetCount() );
	SetItemImage( GetTreeListItem(nItem), nImage, nExpandImage, nSelectedImage, nExpandSelectedImage );
}

LPCTSTR CTreeListCtrl::GetItemTextEx( int nItem, int nSubItem )
{
	ASSERT( 0 <= nItem && nItem < GetCount() );
	return GetItemText( GetTreeListItemEx(nItem), nSubItem );
}

LPCTSTR CTreeListCtrl::GetItemText( int nItem, int nSubItem )
{
	// retrieve text of sub item ( list )
	ASSERT( 0 <= nItem && nItem < GetCount() );
	return GetItemText( GetTreeListItem(nItem), nSubItem );
}

BOOL CTreeListCtrl::SetItemText( int nItem, int nSubItem, LPCTSTR lpszText )
{
	// set text of sub item ( list )
	ASSERT( 0 <= nItem && nItem < GetCount() );
	return SetItemText( GetTreeListItem(nItem), lpszText, nSubItem );
}

DWORD CTreeListCtrl::GetItemData( int nItem )
{
	// retrieve data of item ( list )
	ASSERT( 0 <= nItem && nItem < GetCount() );
	return GetItemData( GetTreeListItem(nItem) );
}

void CTreeListCtrl::SetItemData( int nItem, DWORD dwData )
{
	// set data of item ( list )
	ASSERT( 0 <= nItem && nItem < GetCount() );
	SetItemData( GetTreeListItem(nItem), dwData );
}

BOOL CTreeListCtrl::GetViewRect( LPRECT lpRect )
{
	// retrieve list rect ( list )
	lpRect->left	= m_rcTreeList.left;
	lpRect->right	= m_rcTreeList.right;
	lpRect->top		= m_rcTreeList.top;
	lpRect->bottom	= m_rcTreeList.bottom;

	return m_rcTreeList.IsRectEmpty();
}

int CTreeListCtrl::GetTopIndex()
{
	// retrieve index of first visible item ( list )
	return GetScrollPos( SB_VERT );
}

int CTreeListCtrl::GetCountPerPage()
{
	// retrieve rows of visible item of one page ( list )
	return m_rcTreeList.Height() / GetItemHeight() + 1;
}

BOOL CTreeListCtrl::GetOrigin( LPPOINT lpPoint )
{
	// retrieve origin point of a list ( list )
	lpPoint->x = m_rcTreeList.left;
	lpPoint->y = m_rcTreeList.top;
	return TRUE;
}

UINT CTreeListCtrl::GetSelectedCount()
{
	// retrieve count of selected item ( list )
	return m_arSelects.GetCount() == 1 ? m_arSelects.GetAt(0)!=NULL : m_arSelects.GetSize();
}

BOOL CTreeListCtrl::SetColumnOrderArray( int iCount, LPINT piArray )
{
	// set show order of columns
	m_arShows.RemoveAll();

	for( int iShow=0; iShow<iCount; iShow++ )
		m_arShows.Add( piArray[iShow] );

	Invalidate();

	return TRUE;
}

BOOL CTreeListCtrl::GetColumnOrderArray( LPINT piArray, int iCount )
{
	// retrieve column show order
	if( iCount <= 0 )
		iCount = m_arShows.GetSize();

	for( int iShow=0; iShow<m_arShows.GetSize(); iShow++ )
		piArray[iShow] = m_arShows[iShow];

	Invalidate();

	return TRUE;
}

CTreeListHeaderCtrl* CTreeListCtrl::GetHeaderCtrl()
{
	// retrieve header control
	return &m_wndHeader;
}

int CTreeListCtrl::GetSelectionMark()
{
	// retrieve selection 
	POSITION pos = GetFirstSelectedItemPosition();
	return GetTreeListItem( GetNextSelectedItem( pos ) );
}

int CTreeListCtrl::SetSelectionMark( int iIndex )
{
	// set selection mark of an item
	int nItem = GetSelectionMark();
	SelectItem( GetTreeListItem( iIndex ), 0, FALSE );

	Invalidate();

	return nItem;
}

void CTreeListCtrl::SetFont()
{
	// set font with font of parent window
	CWnd* pWnd = GetParent();
	if( pWnd == NULL )
		return;

	CFont* pFont = pWnd->GetFont();
	if( pFont == NULL )
		return;

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(&lf);

	m_pFontBold.DeleteObject();
	lf.lfWeight = FW_BOLD;
	m_pFontBold.CreateFontIndirect(&lf);

	Invalidate();
}

CFont* CTreeListCtrl::GetFont()
{
	// retrieve font
	return &m_Font;
}

DWORD CTreeListCtrl::SetStyle( DWORD dwStyle )
{
	// set style of treelist control
	DWORD dwoldStyle = m_dwStyle;
	m_dwStyle = dwStyle;

	if( !(m_dwStyle&TLC_TREELIST ) )
	{
		m_dwStyle &= ~TLC_TREELINE;
		m_dwStyle &= ~TLC_ROOTLINE;
		m_dwStyle &= ~TLC_BUTTON;	
	}

	if( !(m_dwStyle&TLC_HGRID) )
	{
		m_dwStyle &= ~TLC_TGRID;
		m_dwStyle &= ~TLC_HGRID_EXT;
		m_dwStyle &= ~TLC_HGRID_FULL;
	}

	if( !(m_dwStyle&TLC_VGRID) )
	{
		m_dwStyle &= ~TLC_VGRID_EXT;
	}

	if( m_dwStyle&TLC_HGRID && m_dwStyle&TLC_VGRID )
	{
		if( m_dwStyle&TLC_HGRID_EXT && !(m_dwStyle&TLC_VGRID_EXT) )
			m_dwStyle &= ~( TLC_HGRID_EXT | TLC_VGRID_EXT );
		if( !(m_dwStyle&TLC_HGRID_EXT) && m_dwStyle&TLC_VGRID_EXT )
			m_dwStyle &= ~( TLC_HGRID_EXT | TLC_VGRID_EXT );
	}

	if( GetStyle()&TLC_HEADER )
		m_wndHeader.SetStyle( m_wndHeader.GetStyle()|TLHS_SHOWHEAD );
	else
		m_wndHeader.SetStyle( m_wndHeader.GetStyle()&~TLHS_SHOWHEAD );

	if( GetStyle()&TLC_HOTTRACK )
		m_wndHeader.SetStyle( m_wndHeader.GetStyle()|TLHS_HOTTRACK );
	else
		m_wndHeader.SetStyle( m_wndHeader.GetStyle()&~TLHS_HOTTRACK );

	Invalidate();

	return dwoldStyle;
}

DWORD CTreeListCtrl::GetStyle()
{
	// retrieve treelist control style
	return m_dwStyle;
}

void CTreeListCtrl::SetState( DWORD dwAddState, DWORD dwRemoveState )
{
	// set retrieve state of control
	m_dwState |= dwAddState;
	m_dwState &=~dwRemoveState;
	return; 
}

DWORD CTreeListCtrl::GetState( DWORD nStateMask )
{
	// retrieve state of control
	return m_dwState&nStateMask;
}

int CTreeListCtrl::GetSelectColumn()
{
	// retrieve select column
	return m_iSelCol;
}

int CTreeListCtrl::GetColumnCount()
{
	// retrieve column count
	return GetColumns();
}

int CTreeListCtrl::GetVisibleCount( HTREELISTITEM pParent )
{
	// retrieve visible rows
	if( pParent == m_pRootItem )
		return m_pItemMap.GetVisibleChildren(pParent) - 1;
	else
		return m_pItemMap.GetVisibleChildren(pParent);
}

void CTreeListCtrl::StatChildAdd( HTREELISTITEM pItem, int nChild, int nVisibleChild )
{
	// stat. number of child & visible child on any level when add some new child
	ASSERT( pItem != NULL );
	HTREELISTITEM pParent = GetParentItem(pItem);
	if( pParent == NULL )
		return;
	
	m_pItemMap.SetChildCount(pParent, m_pItemMap.GetChildCount(pParent)+nChild);

	if( m_pItemMap.GetItemState(pParent)&TLIS_EXPANDED )
	{
		m_pItemMap.SetVisibleChildren(pParent, m_pItemMap.GetVisibleChildren(pParent)+nVisibleChild);
		StatChildAdd( pParent, nChild, nVisibleChild );
	}
	else
	{
		StatChildAdd( pParent, nChild, 0 );
	}
}

void CTreeListCtrl::StatChildDel( HTREELISTITEM pItem, int nChild, int nVisibleChild )
{
	// stat. number of child & visible child on any level when add some new child
	ASSERT( pItem != NULL );
	
	HTREELISTITEM pParent = GetParentItem(pItem);
	if( pParent == NULL )
		return;
	
	m_pItemMap.SetChildCount(pParent, m_pItemMap.GetChildCount(pParent)-nChild);
	
	if( m_pItemMap.GetItemState(pParent)&TLIS_EXPANDED )
	{
		m_pItemMap.SetVisibleChildren(pParent, m_pItemMap.GetVisibleChildren(pParent)-nVisibleChild);
		StatChildDel( pParent, nChild, nVisibleChild );
	}
	else
	{
		StatChildDel( pParent, nChild, 0 );
	}
}

void CTreeListCtrl::StatExpand( HTREELISTITEM pItem )
{
	// stat. number of child & visible child on any level when expand an item
	ASSERT( pItem != NULL );
	if( GetChildItem(pItem) == NULL )
		return;

	int nVisibleChild = 0;
	
	HTREELISTITEM pChild = GetChildItem(pItem);
	while( pChild != NULL )
	{
		nVisibleChild += m_pItemMap.GetVisibleChildren(pChild);
		pChild = GetNextSiblingItem(pChild);
	}
	m_pItemMap.SetVisibleChildren(pItem, m_pItemMap.GetVisibleChildren(pItem)+nVisibleChild);
	
	HTREELISTITEM pParent = GetParentItem(pItem);
	while( pParent != NULL )
	{
		m_pItemMap.SetVisibleChildren(pParent, m_pItemMap.GetVisibleChildren(pParent)+nVisibleChild);
		pParent = GetParentItem(pParent);
	}
}

void CTreeListCtrl::StatCollapse( HTREELISTITEM pItem )
{
	// stat. number of child & visible child on any level when collapse an item
	ASSERT( pItem != NULL );
	if( GetChildItem(pItem) == NULL )
		return;

	int nVisibleChild = 0;

	HTREELISTITEM pChild = GetChildItem(pItem);
	while( pChild != NULL )
	{
		nVisibleChild += m_pItemMap.GetVisibleChildren(pChild);
		pChild = GetNextSiblingItem(pChild);
	}
	m_pItemMap.SetVisibleChildren(pItem, m_pItemMap.GetVisibleChildren(pItem)-nVisibleChild);

	HTREELISTITEM pParent = GetParentItem(pItem);
	while( pParent != NULL )
	{
		m_pItemMap.SetVisibleChildren(pParent, m_pItemMap.GetVisibleChildren(pParent)-nVisibleChild);
		pParent = GetParentItem(pParent);
	}
}

void CTreeListCtrl::UpdateSelectionForRect(LPCRECT pRect, UINT nFlags)
{
	CWaitCursor wait;
	CRect rect;

	SelectItem(NULL);
	HTREELISTITEM hItem = GetRootItem();
	while (hItem) {

		GetItemRect(hItem, &rect);
		if (rect.IntersectRect(rect, pRect)) 
			SelectItem(hItem, 1, false);
		hItem = GetNextVisibleItem(hItem);
	}
	UpdateWindow();
}

void CTreeListCtrl::DoBanding(BOOL bLeft, UINT nFlags, CPoint point)
{
	if (::GetFocus() != m_hWnd)
		::SetFocus(m_hWnd);

	::SetCapture(m_hWnd);

	CClientDC dc(this);
	CRect rectCli;
	GetClientRect(&rectCli);

	MSG msg;
	BOOL bDone = FALSE;
	CPoint pt;
	CSize sizeDrag(::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG));
	BOOL bDrag = FALSE;
	CSize sizeEdge(1, 1);

	UINT nTimer = SetTimer(1, 75, NULL);	//for scroll
	CPoint ptScr(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	CRect rect(0, 0, 0, 0);
	UINT h = 0;
	HTREELISTITEM hItem = GetRootItem();
	if (hItem) {
		GetItemRect(hItem, &rect);
		ptScr.y *= (h = rect.Height());		//this assumes equal height items
	}
	point += ptScr;

	while (!bDone && ::GetMessage(&msg, NULL, 0, 0)) {

		if (::GetCapture() != m_hWnd)
			break;

		switch (msg.message) {
			case WM_TIMER:
				pt = msg.pt;
				ScreenToClient(&pt);
				if (rectCli.PtInRect(pt)) {
					::DispatchMessage(&msg);
					break;
				}
				msg.lParam = MAKELPARAM(pt.x, pt.y);
				//fall through to mousemove

			case WM_MOUSEMOVE:
				{
					// CWaitCursor wait;

					pt.x = GET_X_LPARAM(msg.lParam);
					pt.y = GET_Y_LPARAM(msg.lParam);
					if (!bDrag) {
						if ((abs(pt.x - point.x) <= sizeDrag.cx)
								&& ((abs(pt.y - point.y) <= sizeDrag.cy)) )
							break;
						bDrag = TRUE;
						if (!(nFlags & (MK_CONTROL|MK_SHIFT)))
							SelectItem(NULL);
						UpdateWindow();
						rect.SetRect(point, point);
						dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);
					}

					dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);	//delete

					if (pt.y < rectCli.top)
						::SendMessage(m_hWnd, WM_VSCROLL, SB_LINEUP, 0);
					else if (pt.y >= rectCli.bottom)
						::SendMessage(m_hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
					if (pt.x < rectCli.left)
						::SendMessage(m_hWnd, WM_HSCROLL, SB_LINELEFT, 0);
					else if (pt.x >= rectCli.right)
						::SendMessage(m_hWnd, WM_HSCROLL, SB_LINERIGHT, 0);

					ptScr = point;
					ptScr.x -= GetScrollPos(SB_HORZ);
					ptScr.y -= GetScrollPos(SB_VERT) * h;
					rect.SetRect(ptScr, pt);
					rect.NormalizeRect();
					UpdateSelectionForRect(rect, nFlags);
					dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);	//draw
				}
				break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				bDone = TRUE;
				break;

			case WM_KEYDOWN:
				if (LOWORD(msg.wParam) == VK_ESCAPE) {
					SelectItem(NULL);
					bDone = TRUE;
					break;
				}
				//dispatch
			default:
				::DispatchMessage(&msg);
				break;
		}
	}
	KillTimer(nTimer);
	::ReleaseCapture();

	if (bDrag)
		dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);
	else
		if (!(nFlags & (MK_CONTROL|MK_SHIFT)))
			SelectItem(NULL);

	//when banding, make sure we receive WM_CONTEXTMENU
	// every time - which is what the listview ctrl does
	::DispatchMessage(&msg);
}

void CTreeListCtrl::SetItemCheck( HTREELISTITEM pItem, BOOL bAutoCheck )
{
	// auto set check status ( include parent and child )
	LRESULT lResult;

	lResult = Notify( TLN_ITEMCHECK, pItem, 0 );
	if( lResult == -1 )
		return;

	if( bAutoCheck )
		SetCheck(pItem, !GetCheck(pItem));

	if( m_dwStyle&TLC_NOAUTOCHECK )
		return;

	if( GetCheck(pItem))
	{
		SetItemChildStatus( pItem, TLIS_CHECKED, TLIS_CHECKED_NULL );
		SetItemParentStatus( pItem, TLIS_CHECKEDMASK );
	}
	else
	{
		SetItemChildStatus( pItem, TLIS_CHECKED_NULL, TLIS_CHECKED );
		SetItemParentStatus( pItem, TLIS_CHECKEDMASK );
	}

	Invalidate();
}

void CTreeListCtrl::SetItemLock( HTREELISTITEM pItem, BOOL bAutoLock )
{
	// auto set lock status ( include parent and child )
	LRESULT lResult;

	lResult = Notify( TLN_ITEMLOCK, pItem, 0 );
	if( lResult == -1 )
		return;

	if( bAutoLock )
		SetLock(pItem, !GetLock(pItem));

	if( m_dwStyle&TLC_NOAUTOLOCK )
		return;

	if( GetLock(pItem))
	{
		SetItemChildStatus( pItem, TLIS_LOCKED, TLIS_LOCKED_NULL );
		SetItemParentStatus( pItem, TLIS_LOCKEDMASK );
	}
	else
	{
		SetItemChildStatus( pItem, TLIS_LOCKED_NULL, TLIS_LOCKED );
		SetItemParentStatus( pItem, TLIS_LOCKEDMASK );
	}

	Invalidate();
}

void CTreeListCtrl::SetItemParentStatus( HTREELISTITEM pItem, DWORD dwMask )
{
	// set parent items status;
	HTREELISTITEM pParent;
	pParent = GetParentItem(pItem);

	DWORD dwShow;

	if( pParent == NULL )
		return;

	ASSERT( dwMask == TLIS_CHECKEDMASK || dwMask == TLIS_LOCKEDMASK );
	if( dwMask == TLIS_CHECKEDMASK )
		dwShow = TLIS_SHOWCHECKBOX;
	else
		dwShow = TLIS_SHOWLOCKBOX;
		

	DWORD dwAdd = 0;
	DWORD dwRemove = 0;
	HTREELISTITEM pNext = GetChildItem(GetParentItem(pItem));
	if( m_pItemMap.GetItemState(pNext)&dwShow )
		dwAdd |= m_pItemMap.GetItemState(pNext);
	while( pNext != NULL )
	{
		if( m_pItemMap.GetItemState(pNext)&dwShow )
			dwAdd |= m_pItemMap.GetItemState(pNext);
		pNext = GetNextSiblingItem(pNext);
	}
	dwAdd		 =  dwAdd;
	dwRemove	 = ~dwAdd;
	dwAdd		&=  dwMask;
	dwRemove	&=  dwMask;

	m_pItemMap.SetItemState(pParent, dwAdd, dwRemove );
	SetItemParentStatus( pParent, dwMask );
}


void CTreeListCtrl::SetItemChildStatus( HTREELISTITEM pItem, DWORD dwAdd, DWORD dwRemove )
{
	// set child items status
	HTREELISTITEM pChild;
	pChild = GetChildItem(pItem);

	while( pChild != NULL )
	{
		if( GetChildItem(pChild) != NULL )
			SetItemChildStatus( pChild, dwAdd, dwRemove );

		m_pItemMap.SetItemState(pChild, dwAdd, dwRemove );
		pChild = GetNextSiblingItem( pChild);
	}
}

HTREELISTITEM CTreeListCtrl::GetFirstShowItem( int nShowItem )
{
	// retrieve first visible item after n visible items
	if( nShowItem < 0 )
		return NULL;

	if( nShowItem >= m_pItemMap.GetVisibleChildren(m_pRootItem))
		return NULL;
	
	HTREELISTITEM pItem = GetChildItem(m_pRootItem);
	while( nShowItem != 0 )
	{
		ASSERT( nShowItem > 0 );

		if( nShowItem < m_pItemMap.GetVisibleChildren(pItem))
		{
			nShowItem --;
			pItem = GetChildItem(pItem);
		}
		else
		{
			nShowItem -= m_pItemMap.GetVisibleChildren(pItem);
			pItem = GetNextSiblingItem(pItem);
		}
	}

	return pItem;
}

HTREELISTITEM CTreeListCtrl::GetNextShowItem( HTREELISTITEM pItem, BOOL bChild )
{
	// retrieve next visible item
	if(!pItem)
		return NULL;
	ASSERT( pItem != NULL );

	if( m_pItemMap.GetItemState(pItem)&TLIS_EXPANDED )
		if( bChild && GetChildItem(pItem) != NULL )
			return GetChildItem(pItem);

	if( GetNextSiblingItem(pItem) != NULL )
		return GetNextSiblingItem(pItem);

	pItem = GetParentItem(pItem);
	if( pItem != m_pRootItem )
		return GetNextShowItem( pItem, FALSE );

	return NULL;
}

BOOL CTreeListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// wm_erasebkgnd
	return TRUE;
}

void CTreeListCtrl::OnPaint() 
{
	// wm_paint
	CPaintDC dc(this);
	if( !dc.m_hDC) {
		Default();
		return;
	}

	if ( !GetRootItem())
    {
		CRect rcClient;
		GetClientRect(&rcClient);

        COLORREF clrText = ::GetSysColor(COLOR_HIGHLIGHT);
        COLORREF clrTextBk = ::GetSysColor(COLOR_WINDOW);

        CDC* pDC = &dc;
        // Save dc state
        int nSavedDC = pDC->SaveDC();

        CRect rc=rcClient;
		pDC->FillRect(rc, &CBrush(clrTextBk));
		CRect rcH;
		m_wndHeader.GetClientRect(rcH);
        rc.top += rcH.bottom+5;

        pDC->SetTextColor(clrText);
        pDC->SetBkColor(clrTextBk);
        pDC->SelectStockObject(ANSI_VAR_FONT);
		pDC->DrawText(m_szVacia,rc, DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP);
        pDC->RestoreDC(nSavedDC);
        ReleaseDC(pDC);
		return;
	}

	CPalette* pPalette = NULL;

	if( dc.GetDeviceCaps(RASTERCAPS)&RC_PALETTE )
	{
		pPalette = dc.SelectPalette( &m_Palette, FALSE );
		dc.RealizePalette();
	}

	dc.ExcludeClipRect( m_rcHeader );
	CScrollBar *scroll=GetScrollBarCtrl(SB_VERT);
	if(scroll && scroll->IsWindowVisible()) {

		CRect rect;
		scroll->GetWindowRect(rect);
		ScreenToClient(rect);
		dc.ExcludeClipRect(rect);
	}

	scroll=GetScrollBarCtrl(SB_HORZ);
	if(scroll && scroll->IsWindowVisible()) {

		CRect rect;
		scroll->GetWindowRect(rect);
		ScreenToClient(rect);
		dc.ExcludeClipRect(rect);
	}

	if( GetState( TLS_OFFSCREENBUFFER ) )
	{
		CMemDC TreeListDC(&dc);
		DrawCtrl( &TreeListDC );
	}
	else
	{
		DrawCtrl( &dc );
	}

	if( pPalette != NULL && dc.GetDeviceCaps(RASTERCAPS)&RC_PALETTE )
	{
		dc.SelectPalette( pPalette, FALSE );
	}
}

void CTreeListCtrl::DrawCtrl( CDC* pDC )
{
	// draw the control
	if(!pDC || !pDC->m_hDC)
		return;
	CRect rcClip;
	if( pDC->GetClipBox( &rcClip ) == ERROR )
		return;

	/*CScrollBar *scroll=GetScrollBarCtrl(SB_VERT);
	if(scroll) {
		CRect rect;
		scroll->GetWindowRect(rect);
		rcClip.right-=rect.Width();
		if(rcClip.right<0)
			rcClip.right=0;
	}*/

	DrawBkgnd( pDC, rcClip );
	DrawItems( pDC, rcClip );
}

void CTreeListCtrl::DrawBkgnd( CDC* pDC, CRect rcClip )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw background
	pDC->FillSolidRect( rcClip, m_crBkColor );
}

void CTreeListCtrl::DrawBkgndBmp( CDC* pDC, CRect rcClip, CBitmap* pBkgnd )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw bitmap background
	CRect rcClient;
	BITMAP bmBkgnd;
	
	GetClientRect( rcClient );
	pBkgnd->GetBitmap( &bmBkgnd );
	
	CDC dc;
	dc.CreateCompatibleDC( pDC );
	CBitmap* pBmp = dc.SelectObject( pBkgnd );

	pDC->IntersectClipRect( rcClip );

	int nPerfix = - ( GetScrollPos( SB_HORZ ) % bmBkgnd.bmWidth );
	for( int y = 0; y < rcClient.Height(); y += bmBkgnd.bmHeight )
		for( int x = nPerfix; x < rcClient.Width(); x += bmBkgnd.bmWidth )
			pDC->BitBlt( x, y, bmBkgnd.bmWidth, bmBkgnd.bmHeight, &dc, 0, 0, SRCCOPY);

	::ExtSelectClipRgn( pDC->m_hDC, DEFAULT_HRGN, RGN_COPY );

	dc.SelectObject( pBmp );
	dc.DeleteDC();

}

void CTreeListCtrl::DrawItems( CDC* pDC, CRect rcClip )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw items
	HTREELISTITEM pShowItem;
	
	pShowItem = GetFirstVisibleItem();
	if( pShowItem == NULL )
		return;

	CPen Pen( PS_SOLID, 1, m_crGrid );
	CPen* pOldPen	= pDC->SelectObject( &Pen );
	CFont* pOldFont	= pDC->SelectObject( &m_Font );
	int nOldMode = pDC->SetBkMode( TRANSPARENT );

	CRect rcClient;
	GetClientRect( rcClient );

	CRect rcItem;
	rcItem.SetRect( 0, 0, GetWidth(), GetItemHeight());
	rcItem.OffsetRect( m_rcTreeList.left, m_rcTreeList.top );
	rcItem.OffsetRect( -GetScrollPos( SB_HORZ ), 0 );

	int nFirstRow;
	int nShowRows;

	nFirstRow	=	GetScrollPos( SB_VERT );
	nShowRows	=	m_rcTreeList.Height() / GetItemHeight() + 1;

	pShowItem	=	GetFirstShowItem( nFirstRow );

	for( int iRow=nFirstRow; iRow<= nFirstRow + nShowRows && iRow < GetVisibleCount(); iRow++, pShowItem = GetNextShowItem( pShowItem ) )
	{
		if(!pShowItem)
			continue;

		ASSERT( pShowItem != NULL );

		DrawItem( pDC, rcItem, pShowItem ); 
		
		if( m_dwStyle&TLC_HGRID_FULL && rcItem.right<rcClient.right )
		{
			pDC->MoveTo( rcItem.right, rcItem.bottom - 1);
			pDC->LineTo( rcClient.right, rcItem.bottom - 1);
		}
		rcItem.OffsetRect( 0, m_nItemHeight );
	}

	if( GetVisibleCount() > 0 )
	{
		if( m_dwStyle&TLC_HGRID && !(m_dwStyle&TLC_HGRID_EXT) && !(m_dwStyle&TLC_VGRID_EXT) )
		{
			pDC->MoveTo( rcItem.left, rcItem.top - 1);
			pDC->LineTo( rcItem.right-1, rcItem.top - 1);
		}
	}

	for( int iGrid=GetVisibleCount(); iGrid<= nFirstRow + nShowRows; iGrid++ )
	{
		DrawGrid( pDC, rcItem );

		if( m_dwStyle&TLC_HGRID_EXT && m_dwStyle&TLC_HGRID_FULL && rcItem.right<rcClient.right )
		{
			pDC->MoveTo( rcItem.right, rcItem.bottom - 1 );
			pDC->LineTo( rcClient.right, rcItem.bottom - 1 );
		}

		rcItem.OffsetRect( 0, m_nItemHeight );
	}

	pDC->SetBkMode( nOldMode );
	pDC->SelectObject( pOldFont );
	pDC->SelectObject( pOldPen );
}

void CTreeListCtrl::DrawItem( CDC* pDC, CRect rcItem, HTREELISTITEM pItem )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw item background
	DrawItemBkgnd( pDC, rcItem, pItem );

	// draw all columns
	CTreeListColumnInfo* pColumnInfo;
	int nColPos = 0;
	for( int iShow = 0; iShow < m_arShows.GetSize(); iShow++, nColPos += pColumnInfo->m_nWidth )
	{
		int iCol;
		
		// do NOT draw zero column;
		iCol = m_arShows[iShow];
		pColumnInfo = (CTreeListColumnInfo*)m_arColumns[iCol];
		if( pColumnInfo->m_nWidth == 0 )
			continue;

		CRect rcColumn;
		rcColumn.SetRect( 0, rcItem.top, pColumnInfo->m_nWidth, rcItem.bottom );
		rcColumn.OffsetRect( nColPos, 0 );
		rcColumn.OffsetRect( -GetScrollPos( SB_HORZ ), 0 );

		if( iCol == 0 )
		{
			if ( m_dwStyle&TLC_TREELIST )
				DrawItemTree( pDC, rcColumn, pItem, iCol );
			else
				DrawItemMain( pDC, rcColumn, pItem, iCol );
		}
		else
		{
			DrawItemList( pDC, rcColumn, pItem, iCol );
		}

	}

	CRect rcFocus;
	if( pItem == m_pFocusItem )
	{
		CPen pen;
		pen.CreatePen( PS_SOLID, 1, (COLORREF)0x000000 );
		CPen* ppen = pDC->SelectObject( &pen );

		CRect rcFocus;
		rcFocus.SetRect( 0, rcItem.top, GetWidth(), rcItem.bottom );
		rcFocus.OffsetRect( -GetScrollPos( SB_HORZ ), 0 );

		if( GetStyle()&TLC_HGRID )
			rcFocus.DeflateRect( 0, 0, 0, 1 );

		if( GetStyle()&TLC_VGRID )
			rcFocus.DeflateRect( 0, 0, 1, 0 );

		COLORREF croldBkgnd;
		croldBkgnd = pDC->SetBkColor( TRANSPARENT );
		pDC->SetTextColor( GetItemColor(pItem) ? GetItemColor(pItem) : m_crText );
		// este es el borde

		pDC->SelectObject(::GetStockObject (NULL_BRUSH));
		pen.DeleteObject();

		pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));

		pDC->SelectObject(&pen);

		pDC->Rectangle(rcFocus);
		pDC->SetBkColor( croldBkgnd );
		pDC->SelectObject( ppen );
	}

	return;
}

void CTreeListCtrl::DrawGrid( CDC* pDC, CRect rcItem )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw all grids
	CTreeListColumnInfo* pColumnInfo;
	int nColPos = 0;
	for( int iShow = 0; iShow < m_arShows.GetSize(); iShow++, nColPos += pColumnInfo->m_nWidth )
	{
		int iCol;
		
		iCol = m_arShows[iShow];
		pColumnInfo = (CTreeListColumnInfo*)m_arColumns[iCol];
		if( pColumnInfo->m_nWidth == 0 )
			continue;

		CRect rcColumn;
		rcColumn.SetRect( 0, rcItem.top, pColumnInfo->m_nWidth, rcItem.bottom );
		rcColumn.OffsetRect( nColPos, 0 );
		rcColumn.OffsetRect( -GetScrollPos( SB_HORZ ), 0 );

		if( iCol == 0 )
		{
			// draw vertical grid line of tree
			if( m_dwStyle&TLC_HGRID_EXT && m_dwStyle&TLC_TGRID )
			{
				pDC->MoveTo( rcColumn.left, rcColumn.bottom - 1);
				pDC->LineTo( rcColumn.right-1, rcColumn.bottom - 1);
			}

			if( m_dwStyle&TLC_VGRID_EXT )
			{
				pDC->MoveTo( rcColumn.right-1, rcColumn.bottom - 1);
				pDC->LineTo( rcColumn.right-1, rcColumn.top - 1 );
			}
		}
		else
		{
			// draw vertical grid line and horizonal grid lin
			if( m_dwStyle&TLC_HGRID_EXT )
			{
				pDC->MoveTo( rcColumn.left, rcColumn.bottom - 1);
				pDC->LineTo( rcColumn.right, rcColumn.bottom - 1);
			}

			if( m_dwStyle&TLC_VGRID_EXT )
			{
				pDC->MoveTo( rcColumn.right-1, rcColumn.bottom - 1);
				pDC->LineTo( rcColumn.right-1, rcColumn.top - 1 );
			}
		}
	}
}

void CTreeListCtrl::DrawItemBkgnd( CDC* pDC, CRect rcItem, HTREELISTITEM pItem )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw selected background
	CRect		rcBkgnd;
	if( m_pItemMap.GetItemSelected(pItem) && GetStyle()&TLC_SHOWSELFULLROWS )
	{
		rcBkgnd.SetRect( 0, rcItem.top, GetWidth(), rcItem.bottom );
		rcBkgnd.OffsetRect( -GetScrollPos( SB_HORZ ), 0 );

		if( GetStyle()&TLC_HGRID )
			rcBkgnd.DeflateRect( 0, 0, 0, 1 );

		if( GetStyle()&TLC_VGRID )
			rcBkgnd.DeflateRect( 0, 0, 1, 0 );
		if( m_dwStyle&TLC_SHOWSELALWAYS && GetFocus()==this)
		{
			DrawItemExclude( pDC, rcItem, pItem );
			pDC->FillSolidRect( rcBkgnd, m_crBkSelectedNoFocus);
			::ExtSelectClipRgn( pDC->m_hDC, DEFAULT_HRGN, RGN_COPY );
		}
	}
}

void CTreeListCtrl::DrawItemExclude( CDC* pDC, CRect rcItem, HTREELISTITEM pItem )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// exclude rects of selected item
	CTreeListColumnInfo* pColumnInfo;
	int nColPos = 0;

	if( !m_pItemMap.GetItemSelected(pItem) || !(GetStyle()&TLC_SHOWSELFULLROWS) )
		return;

	for( int iShow = 0; iShow < m_arShows.GetSize(); iShow++, nColPos += pColumnInfo->m_nWidth )
	{
		int iCol;
		
		iCol = m_arShows[iShow];
		pColumnInfo = (CTreeListColumnInfo*)m_arColumns[iCol];

		CRect rcColumn;
		rcColumn.SetRect( 0, rcItem.top, pColumnInfo->m_nWidth, rcItem.bottom );
		rcColumn.OffsetRect( nColPos, 0 );
		rcColumn.OffsetRect( -GetScrollPos( SB_HORZ ), 0 );

		if( iCol == 0 )
		{
			CRect rcExclude;
			int nPerfix = TLL_BORDER;
			if ( m_dwStyle&TLC_TREELIST )
			{
				nPerfix += TLL_WIDTH * ( m_pItemMap.GetItemLevel(pItem) - 1 );

				if( m_dwStyle&(TLC_TREELINE|TLC_BUTTON) )
					nPerfix += TLL_WIDTH;
			}

			if( m_dwStyle&TLC_CHECKBOX )
			{
				if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWCHECKBOX )
				{
					CRect rcCheckBox;
					rcCheckBox.SetRect( 0, 0, TLL_WIDTH, TLL_WIDTH );
					rcCheckBox.OffsetRect( rcColumn.left + nPerfix, rcColumn.top + ( rcColumn.Height() - TLL_HEIGHT)/2 );
					rcExclude = rcColumn & rcCheckBox;

					if( !rcExclude.IsRectEmpty() )
						pDC->ExcludeClipRect( rcExclude );

					nPerfix += TLL_HEIGHT;
				}
			}

			if( m_dwStyle&TLC_LOCKBOX )
			{
				if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWLOCKBOX )
				{
					CRect rcLockBox;
					rcLockBox.SetRect( 0, 0, TLL_WIDTH / 2, TLL_WIDTH );
					rcLockBox.OffsetRect( rcColumn.left + nPerfix, rcColumn.top + ( rcColumn.Height() - TLL_HEIGHT)/2 );
					rcExclude = rcColumn & rcLockBox;

					if( !rcExclude.IsRectEmpty() )
						pDC->ExcludeClipRect( rcExclude );

					nPerfix += TLL_HEIGHT / 2;
				}
			}
		
			if( m_dwStyle&TLC_IMAGE )
			{
				if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWTREEIMAGE )
				{
					CRect rcImage;
					rcImage.SetRect( 0, 0, TLL_WIDTH, TLL_WIDTH );
					rcImage.OffsetRect( rcColumn.left + nPerfix, rcColumn.top + ( rcColumn.Height() - TLL_HEIGHT)/2 );
					rcExclude = rcColumn & rcImage;

					if( !rcExclude.IsRectEmpty() )
						pDC->ExcludeClipRect( rcExclude );

					nPerfix += TLL_HEIGHT;
				}
			}

			break;
		}
	}

}

void CTreeListCtrl::DrawItemTree( CDC* pDC, CRect rcColumn, HTREELISTITEM pItem, int iCol )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw tree column of treelist control
	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[iCol];

	CRect rcGraph;
	rcGraph = rcColumn;
	rcGraph.DeflateRect( TLL_BORDER, 0, TLL_BORDER+1, 0 );
	if( rcGraph.Width() <= 0 )
		return;
	CRect rcTree;
	rcTree = rcGraph;
	rcTree.right = rcTree.left + TLL_WIDTH;

	int nPerfix = TLL_BORDER;
	// draw prefix vertical line
	for( int iLevel=1; iLevel< m_pItemMap.GetItemLevel(pItem); iLevel++, nPerfix += TLL_WIDTH )
	{
		HTREELISTITEM pCheckItem = pItem;
		while( m_pItemMap.GetItemLevel(pCheckItem) != iLevel )
		{
			pCheckItem = GetParentItem(pCheckItem);
		}

		rcTree.left = rcColumn.left + nPerfix;
		rcTree.right = rcTree.left + TLL_WIDTH;

		if( GetNextSiblingItem(pCheckItem) != NULL )
		{
			if( iLevel != 1 || m_dwStyle&TLC_ROOTLINE )
			{
				DrawItemTreeLine( pDC, rcGraph, rcTree, TLL_TOP | TLL_BOTTOM );				// ©§
			}
		}
	}

	// draw cross line
	if( m_dwStyle&(TLC_TREELINE|TLC_BUTTON) )
	{
		rcTree.left = rcColumn.left + nPerfix;
		rcTree.right = rcTree.left + TLL_WIDTH;
		if( m_pItemMap.GetItemLevel(pItem) == 1 )
		{
			// child item of root item
			if( m_dwStyle&TLC_ROOTLINE )
			{
				if( GetPrevSiblingItem(pItem) == NULL && GetNextSiblingItem(pItem) == NULL )
					DrawItemTreeLine( pDC, rcGraph, rcTree, TLL_RIGHT );						// ©¥ ( right part )
				else if( GetPrevSiblingItem(pItem) == NULL )
					DrawItemTreeLine( pDC, rcGraph, rcTree, TLL_RIGHT | TLL_BOTTOM );			// ©³
				else if( GetNextSiblingItem(pItem) == NULL )
					DrawItemTreeLine( pDC, rcGraph, rcTree, TLL_RIGHT | TLL_TOP );				// ©»
				else
					DrawItemTreeLine( pDC, rcGraph, rcTree, TLL_RIGHT | TLL_TOP | TLL_BOTTOM );	// ©Ç
			}
			else
			{
				DrawItemTreeLine( pDC, rcGraph, rcTree, TLL_RIGHT );							// ©¥ ( right part )
			}
		}
		else
		{
			// child item of other items ( not root item )
			if( GetNextSiblingItem(pItem) == NULL )
				DrawItemTreeLine( pDC, rcGraph, rcTree, TLL_RIGHT | TLL_TOP );					// ©»
			else
				DrawItemTreeLine( pDC, rcGraph, rcTree, TLL_RIGHT | TLL_TOP | TLL_BOTTOM );		// ©Ç
		}
		
		// draw button of child item of root item
		if( m_dwStyle&TLC_BUTTON && GetChildItem(pItem))
		{
			if( m_pItemMap.GetItemState(pItem)&TLIS_EXPANDEDONCE )
			{
				if( GetChildItem(pItem) != NULL )
				{
					if( m_pItemMap.GetItemState(pItem)&TLIS_EXPANDED )
						DrawItemTreeButton( pDC, rcGraph, rcTree, TLB_MINUS );
					else
						DrawItemTreeButton( pDC, rcGraph, rcTree, TLB_PLUS );
				}				
			}
			else
			{
				DrawItemTreeButton( pDC, rcGraph, rcTree, TLB_UNKNOW );
			}
		}
		nPerfix += TLL_WIDTH;
	}

	// draw check box
	if( m_dwStyle&TLC_CHECKBOX )
	{
		if(m_pItemMap.GetItemState(pItem)&TLIS_SHOWCHECKBOX )
		{
			CRect rcCheckBox;
			rcCheckBox = rcGraph;
			rcCheckBox.left = rcColumn.left + nPerfix;
			rcCheckBox.right = rcCheckBox.left + TLL_WIDTH;

			POINT pt;
			pt.x = rcCheckBox.left;
			pt.y = rcCheckBox.top + (rcCheckBox.Height() - TLL_HEIGHT)/2;

			pDC->IntersectClipRect( rcGraph );
			pDC->IntersectClipRect( rcCheckBox );
			
			UINT nStyle = ILD_TRANSPARENT;
			m_imgCheck.Draw( pDC, m_pItemMap.GetItemCheck(pItem), pt, nStyle );
			::ExtSelectClipRgn( pDC->m_hDC, DEFAULT_HRGN, RGN_COPY );

			nPerfix += TLL_WIDTH;
		}
	}

	// draw image box
	if( m_dwStyle&TLC_IMAGE )
	{
		if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWTREEIMAGE )
		{
			CRect rcImage;
			rcImage = rcGraph;
			rcImage.left = rcColumn.left + nPerfix;
			rcImage.right = rcImage.left + TLL_WIDTH;

			POINT pt;
			pt.x = rcImage.left;
			pt.y = rcImage.top + (rcImage.Height() - TLL_HEIGHT)/2;

			pDC->IntersectClipRect( rcGraph );
			pDC->IntersectClipRect( rcImage );

			UINT nStyle = ILD_TRANSPARENT;
			
			if( m_pImageList) {
				if( m_pItemMap.GetItemState(pItem)&TLIS_EXPANDED && GetChildItem(pItem) != NULL )
				{
					if( m_arSelects.GetSize() > 0 && pItem == (HTREELISTITEM)m_arSelects[0] )
						m_pImageList->Draw( pDC, m_pItemMap.GetItemExpandedSelectedImage(pItem), pt, nStyle );
					else
						m_pImageList->Draw( pDC, m_pItemMap.GetItemSelectedImage(pItem), pt, nStyle );
				}
				else
				{
					if( m_arSelects.GetSize() > 0 && pItem == (HTREELISTITEM)m_arSelects[0] )
						m_pImageList->Draw( pDC, m_pItemMap.GetItemExpandedImage(pItem), pt, nStyle );
					else
						m_pImageList->Draw( pDC, m_pItemMap.GetItemImage(pItem), pt, nStyle );
				}
			}

			::ExtSelectClipRgn( pDC->m_hDC, DEFAULT_HRGN, RGN_COPY );

			nPerfix += TLL_WIDTH;
		}
	}

	// add gaps between text and image 
	if( m_dwStyle&TLC_LOCKBOX || m_dwStyle&TLC_IMAGE )
		nPerfix += 2;

	// draw item text
	CRect rcBkgnd;
	rcBkgnd = rcColumn;
	rcBkgnd.left = rcColumn.left + nPerfix;

	DrawItemTreeText( pDC, rcBkgnd, pItem, iCol );

	// draw vertical grid line of tree
	if( m_dwStyle&TLC_TGRID )
	{
		pDC->MoveTo( rcColumn.left, rcColumn.bottom - 1);
		pDC->LineTo( rcColumn.right-1, rcColumn.bottom - 1);
	}

	if( m_dwStyle&TLC_VGRID )
	{
		pDC->MoveTo( rcColumn.right-1, rcColumn.bottom - 1);
		pDC->LineTo( rcColumn.right-1, rcColumn.top - 1 );
	}

	return;
}

void CTreeListCtrl::DrawItemMain( CDC* pDC, CRect rcColumn, HTREELISTITEM pItem, int iCol )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw main column of list
	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[iCol];

	CRect rcGraph;
	rcGraph = rcColumn;
	rcGraph.DeflateRect( TLL_BORDER, 0, TLL_BORDER+1, 0 );
	if( rcGraph.Width() <= 0 )
		return;
	
	CRect rcTree;
	rcTree = rcGraph;
	rcTree.right = rcTree.left + TLL_WIDTH;

	int nPerfix = TLL_BORDER;

	// draw check box
	if( m_dwStyle&TLC_CHECKBOX )
	{
		if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWCHECKBOX )
		{
			CRect rcCheckBox;
			rcCheckBox = rcGraph;
			rcCheckBox.left = rcColumn.left + nPerfix;
			rcCheckBox.right = rcCheckBox.left + TLL_WIDTH;

			POINT pt;
			pt.x = rcCheckBox.left;
			pt.y = rcCheckBox.top + (rcCheckBox.Height() - TLL_HEIGHT)/2;

			pDC->IntersectClipRect( rcGraph );
			pDC->IntersectClipRect( rcCheckBox );
			UINT nStyle = ILD_TRANSPARENT;
			m_imgCheck.Draw( pDC, m_pItemMap.GetItemCheck(pItem), pt, nStyle );
			::ExtSelectClipRgn( pDC->m_hDC, DEFAULT_HRGN, RGN_COPY );

			nPerfix += TLL_WIDTH;
		}
	}

	// draw image box
	if( m_dwStyle&TLC_IMAGE )
	{
		if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWTREEIMAGE )
		{
			CRect rcImage;
			rcImage = rcGraph;
			rcImage.left = rcColumn.left + nPerfix;
			rcImage.right = rcImage.left + TLL_WIDTH;

			POINT pt;
			pt.x = rcImage.left;
			pt.y = rcImage.top + (rcImage.Height() - TLL_HEIGHT)/2;

			pDC->IntersectClipRect( rcGraph );
			pDC->IntersectClipRect( rcImage );

			UINT nStyle = ILD_TRANSPARENT;
			if(  m_pImageList) {
				if( m_arSelects.GetSize() > 0 && pItem == (HTREELISTITEM)m_arSelects[0] )
					m_pImageList->Draw( pDC, m_pItemMap.GetItemExpandedImage(pItem), pt, nStyle );
				else
					m_pImageList->Draw( pDC, m_pItemMap.GetItemImage(pItem), pt, nStyle );
			}

			::ExtSelectClipRgn( pDC->m_hDC, DEFAULT_HRGN, RGN_COPY );

			nPerfix += TLL_WIDTH;
		}
	}

	// add gaps between text and image 
	if( m_dwStyle&TLC_LOCKBOX || m_dwStyle&TLC_IMAGE )
		nPerfix += 2;

	// draw item text
	CRect rcBkgnd;
	rcBkgnd = rcColumn;
	rcBkgnd.left = rcColumn.left + nPerfix;

	DrawItemTreeText( pDC, rcBkgnd, pItem, iCol );

	// draw vertical grid line of tree
	if( m_dwStyle&TLC_TGRID )
	{
		pDC->MoveTo( rcColumn.left, rcColumn.bottom - 1);
		pDC->LineTo( rcColumn.right-1, rcColumn.bottom - 1);
	}

	if( m_dwStyle&TLC_VGRID )
	{
		pDC->MoveTo( rcColumn.right-1, rcColumn.bottom - 1);
		pDC->LineTo( rcColumn.right-1, rcColumn.top - 1 );
	}
}

void CTreeListCtrl::DrawItemList( CDC* pDC, CRect rcColumn, HTREELISTITEM pItem, int iCol )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw list
	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[iCol];

	CRect rcBkgnd;
	rcBkgnd = rcColumn;
	rcBkgnd.left = rcColumn.left;

	DrawItemListText( pDC, rcBkgnd, pItem, iCol );

	// draw vertical grid line and horizonal grid lin
	if( m_dwStyle&TLC_HGRID )
	{
		pDC->MoveTo( rcColumn.left, rcColumn.bottom - 1);
		pDC->LineTo( rcColumn.right, rcColumn.bottom - 1);
	}

	if( m_dwStyle&TLC_VGRID )
	{
		pDC->MoveTo( rcColumn.right-1, rcColumn.bottom - 1);
		pDC->LineTo( rcColumn.right-1, rcColumn.top - 1 );
	}
}

void CTreeListCtrl::DrawItemTreeLine( CDC* pDC, CRect rcColumn, CRect rcTree, DWORD dwFormat )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw line of tree
	if( rcColumn.Width() == 0 || rcColumn.Height() == 0 )
		return;

	if( !(m_dwStyle&TLC_TREELINE) )
		return;

	ASSERT( rcTree.Width()	% 2 == 0 );
	ASSERT( rcTree.Height() % 2 == 0 );

	// clip column
	pDC->IntersectClipRect( rcColumn );
	pDC->IntersectClipRect( rcTree );

	CPoint ptCenter;
	ptCenter.x = rcTree.left + rcTree.Width() / 2 - 1;
	ptCenter.y = rcTree.top  + rcTree.Height() / 2 - 1;

	// draw center-left
	if( dwFormat&TLL_LEFT )
	{
		for( int ix = ptCenter.x; ix >= rcTree.left; ix -= 2 )
			pDC->SetPixelV( ix, ptCenter.y, m_crTreeLine );
	}

	// draw center-right
	if( dwFormat&TLL_RIGHT )
	{
		for( int ix = ptCenter.x; ix <= rcTree.right; ix += 2 )
			pDC->SetPixelV( ix, ptCenter.y, m_crTreeLine );
	}

	// draw center-top
	if( dwFormat&TLL_TOP )
	{
		for( int iy = ptCenter.y; iy >= rcTree.top; iy -= 2 )
			pDC->SetPixelV( ptCenter.x, iy, m_crTreeLine );
	}

	// draw center-bottom
	if( dwFormat&TLL_BOTTOM )
	{
		for( int iy = ptCenter.y; iy <= rcTree.bottom; iy += 2 )
			pDC->SetPixelV( ptCenter.x, iy, m_crTreeLine );
	}

	::ExtSelectClipRgn( pDC->m_hDC, DEFAULT_HRGN, RGN_COPY );
}

void CTreeListCtrl::DrawItemTreeButton( CDC* pDC, CRect rcColumn, CRect rcTree, DWORD dwFormat )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw tree button [+][-][?]
	if( rcColumn.Width() == 0 || rcColumn.Height() == 0 )
		return;

	ASSERT( rcTree.Width()  % 2 == 0 );
	ASSERT( rcTree.Height() % 2 == 0 );

	// clip column
	pDC->IntersectClipRect( rcColumn );
	pDC->IntersectClipRect( rcTree );

	// button center
	CPoint ptCenter;
	ptCenter.x = rcTree.left + rcTree.Width() / 2 - 1;
	ptCenter.y = rcTree.top	+ rcTree.Height() / 2 - 1;

	CRect rcButton;
	rcButton.SetRect( -TLL_BUTTON, -TLL_BUTTON, TLL_BUTTON+1, TLL_BUTTON+1 );
	rcButton.OffsetRect( ptCenter );

	CBrush brButton;
	brButton.CreateSolidBrush( m_crTreeLine );

	if ( !(CWnd::GetStyle()&WS_DISABLED) )
		pDC->FillSolidRect( rcButton, 0x00FFFFFF );
	else
		pDC->FillSolidRect( rcButton, 0x00D0D0D0 );
	pDC->FrameRect( rcButton, &brButton );

	rcButton.DeflateRect( 2, 2, 2, 2 );
	// draw unknow button [+] in grey
	if( dwFormat&TLB_UNKNOW )
	{
		CPen Pen, *pOldPen;
		Pen.CreatePen( PS_SOLID, 1, m_crTreeLine );
		pOldPen = pDC->SelectObject( &Pen );
		pDC->MoveTo( rcButton.left, ptCenter.y );
		pDC->LineTo( rcButton.right, ptCenter.y );
		pDC->MoveTo( ptCenter.x, rcButton.top );
		pDC->LineTo( ptCenter.x, rcButton.bottom );
		pDC->SelectObject( pOldPen );
	}

	// draw plus button [+]
	if( dwFormat&TLB_PLUS )
	{
		CPen Pen, *pOldPen;
		Pen.CreatePen( PS_SOLID, 1, (COLORREF)0x000000 );
		pOldPen = pDC->SelectObject( &Pen );
		pDC->MoveTo( rcButton.left, ptCenter.y );
		pDC->LineTo( rcButton.right, ptCenter.y );
		pDC->MoveTo( ptCenter.x, rcButton.top );
		pDC->LineTo( ptCenter.x, rcButton.bottom );
		pDC->SelectObject( pOldPen );
	}

	// draw minus button [-]
	if( dwFormat&TLB_MINUS )
	{
		CPen Pen, *pOldPen;
		Pen.CreatePen( PS_SOLID, 1, (COLORREF)0x000000 );
		pOldPen = pDC->SelectObject( &Pen );
		pDC->MoveTo( rcButton.left, ptCenter.y );
		pDC->LineTo( rcButton.right, ptCenter.y );
		pDC->SelectObject( pOldPen );
	}

	::ExtSelectClipRgn( pDC->m_hDC, DEFAULT_HRGN, RGN_COPY );
}

void CTreeListCtrl::DrawItemTreeText( CDC* pDC, CRect rcBkgnd, HTREELISTITEM pItem, int iCol )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw text on tree
	CRect rcText;
	rcText = rcBkgnd;
	rcText.DeflateRect( 2, 0, 3, 0 );
	rcBkgnd.bottom = rcBkgnd.bottom - 1;
	DrawItemText( pDC, rcBkgnd, rcText, pItem, iCol );
}

void CTreeListCtrl::DrawItemListText( CDC* pDC, CRect rcBkgnd, HTREELISTITEM pItem, int iCol )
{
	if(!pDC || !pDC->m_hDC)
		return;
	// draw text on list
	CRect rcText;
	rcText = rcBkgnd;
	rcText.DeflateRect( 2, 0, 3, 0 );
	rcBkgnd.bottom = rcBkgnd.bottom - 1;
	DrawItemText( pDC, rcBkgnd, rcText, pItem, iCol );
}

void CTreeListCtrl::DrawItemText( CDC* pDC, CRect rcBkgnd, CRect rcText, HTREELISTITEM pItem, int iCol )
{
	if(!pDC || !pDC->m_hDC)
		return;
	COLORREF crBkgnd;
	COLORREF crText = GetItemColor(pItem) ? GetItemColor(pItem) :m_crText;

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[iCol];

	// draw selected background
	if( rcBkgnd.Width() <= 0 || rcBkgnd.Height() <= 0 )
		return;

	if( m_pItemMap.GetItemSelected(pItem))
	{
		if( m_dwStyle&TLC_SHOWSELALWAYS )
		{
			crBkgnd = m_crBkSelectedNoFocus;
			if( !(GetStyle()&TLC_SHOWSELFULLROWS) )
				pDC->FillSolidRect( rcBkgnd, crBkgnd );
		}
	}

	// draw text
	if( rcText.Width() <= 0 || rcText.Height() <= 0 )
		return;

	CFont *oldfont=NULL;
	if( m_pItemMap.GetItemBold(pItem)) 
		oldfont= pDC->SelectObject(&m_pFontBold);

	COLORREF crOldText = pDC->SetTextColor( crText );
	UINT dwFormat = pColumnInfo->GetTextFormat();
	pDC->DrawText( GetItemText(pItem, iCol), rcText, dwFormat );
	pDC->SetTextColor( crOldText );
	if( oldfont)
		pDC->SelectObject(oldfont);
}

void CTreeListCtrl::OnEnable(BOOL bEnable) 
{
	// enable or disable control
	CWnd::OnEnable(bEnable);

	m_wndHeader.EnableWindow( bEnable );
	Invalidate();
}

void CTreeListCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// calculate size of not client area
	if( !(CWnd::GetExStyle()&WS_EX_CLIENTEDGE) )
	{
		LPRECT pRECT = &lpncsp->rgrc[0];
		::InflateRect( pRECT, -1, -1 );
	}
	Default();
}

void CTreeListCtrl::OnNcPaint() 
{
	// draw 3d frame rect
	Default();

	if( !(CWnd::GetExStyle()&WS_EX_CLIENTEDGE) )
	{
		CWindowDC dc(this);
		if(!dc.m_hDC)
			return;
		CRect rcBorder;
		GetWindowRect( &rcBorder );
		ScreenToClient( &rcBorder );
		rcBorder.OffsetRect( -rcBorder.left, -rcBorder.top );
		dc.Draw3dRect( &rcBorder, GetSysColor( COLOR_3DDKSHADOW ), GetSysColor( COLOR_3DHILIGHT ) );
	}
}

void CTreeListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	// resize tree list control
	CWnd::OnSize(nType, cx, cy);	
	Layout();
}

BOOL CTreeListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	SCROLLINFO si;
	GetScrollInfo( SB_VERT, &si, SIF_ALL );

	si.nPos-=zDelta/m_nItemHeight;
	if(si.nPos==si.nMin)
		si.nPos=si.nMin;
	if(si.nPos==si.nMax)
		si.nPos=si.nMax;
	if( si.nPos == si.nMin )
	{
		if(  si.nMax-si.nMin+1 <= m_rcClient.Height()/m_nItemHeight )
		{
			Layout();
		}
	}
	SetScrollPos( SB_VERT, si.nPos );
	Invalidate();
	return TRUE;
}


BOOL CTreeListCtrl::InsertColumn( LPCTSTR lpszColumnHeading, DWORD dwFormat, int nWidth, int iImage, int nMin, int nMax)
{
	// insert a column
	if( GetChildItem(m_pRootItem)) {
		
		HTREELISTITEM subitem, item= GetChildItem(m_pRootItem);
        int cols= m_arColumns.GetCount()+1;
		while(item) {
			m_pItemMap.ResizeItem(item,cols);
            subitem= GetChildItem(item);
			while( subitem) {
				m_pItemMap.ResizeItem(subitem, cols);
				subitem= GetNextSiblingItem(subitem);
			}
			item= GetNextSiblingItem(item);
		}
	}
	// ASSERT( m_pRootItem->m_pChild == NULL );

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = new CTreeListColumnInfo;

	pColumnInfo->m_strCaption	= lpszColumnHeading;
	pColumnInfo->m_dwFormat		= dwFormat;
	pColumnInfo->m_nWidth		= nWidth;
	pColumnInfo->m_iImage		= iImage;
	pColumnInfo->m_nMin			= nMin;
	pColumnInfo->m_nMax			= nMax;

	int nCol = m_arColumns.Add( (void*)pColumnInfo );
	
	if( (pColumnInfo->m_dwFormat&TLF_HIDDEN) == 0 )
		m_arShows.Add( nCol );

	if( pColumnInfo->m_dwFormat&TLF_CAPTION_SORT )
		if( pColumnInfo->m_dwFormat&TLF_SORT_MASK )
			m_arSorts.Add( nCol );

	Layout();
	Invalidate();

	return TRUE;
}

BOOL CTreeListCtrl::DeleteColumn( int iCol )
{
	// delete a column
	ASSERT( GetChildItem(m_pRootItem) == NULL );

	if( iCol < 0  || m_arColumns.GetSize() <= iCol )
		return FALSE;

	CTreeListColumnInfo* pColumnInfo;
	pColumnInfo = (CTreeListColumnInfo*)m_arColumns[iCol];
	m_arColumns.RemoveAt( iCol );

	for( int iShow=0; iShow<m_arShows.GetSize(); iShow++ )
	{
		if( m_arShows[iShow] == iCol )
		{
			m_arShows.RemoveAt( iCol );
		}

		if( iShow<m_arShows.GetSize() )
		{
			if( m_arShows[iShow] > iCol )
				m_arShows[iShow] -= 1;
		}
	}

	for( int iSort=0; iSort<m_arSorts.GetSize(); iSort++ )
	{
		if( m_arSorts[iSort] == iCol )
			m_arSorts.RemoveAt( iCol );

		if( iSort<m_arSorts.GetSize() )
			if( m_arSorts[iSort] > iCol )
				m_arSorts[iSort] -= 1;
	}

	Layout();
	Invalidate();

	return TRUE;
}

int CTreeListCtrl::InsertItem( int nItem, LPCTSTR lpszItem )
{
	// insert an new item ( list )
	ASSERT( !(m_dwStyle&TLC_TREELIST) );
	HTREELISTITEM pItem;

	if( GetChildItem(m_pRootItem) == NULL )
	{
		InsertItemNew( lpszItem, m_pRootItem );
		return 0;
	}

	if( nItem == 0 )
	{
		InsertItemFirst( lpszItem, m_pRootItem );
		return 0;
	}

	if( nItem >= GetCount() )
	{
		pItem = InsertItem( lpszItem, TLI_ROOT, TLI_LAST );
		return GetTreeListItem( pItem );
	}
	pItem = GetTreeListItem( nItem );
	pItem = InsertItem( lpszItem, TLI_ROOT, pItem );
	return GetTreeListItem( pItem );
}

BOOL CTreeListCtrl::DeleteItem( int nItem )
{
	// delete an item ( list )
	ASSERT( !(m_dwStyle&TLC_TREELIST) );
	HTREELISTITEM pItem;

	pItem = GetTreeListItem( nItem );

	if( pItem == NULL )
		return FALSE;

	return DeleteItem( pItem );
}

HTREELISTITEM CTreeListCtrl::InsertItem( LPCTSTR lpszItem, HTREELISTITEM pParent, HTREELISTITEM pInsertAfter )
{
	CWaitCursor wait;
	HTREELISTITEM retorno=NULL;
	// insert an new item
	ASSERT( m_dwStyle&TLC_TREELIST || pParent == TLI_ROOT || pParent == m_pRootItem ) ;
	// add first level item
	if( pParent == TLI_ROOT )
	{
		retorno=InsertItem( lpszItem, m_pRootItem, pInsertAfter );
		return retorno;
	}

	// no child item
	if( GetChildItem(pParent) == NULL )
	{
		retorno= InsertItemNew( lpszItem, pParent );
		return retorno;
	}

	// add befor first child item
	if( pInsertAfter == TLI_FIRST )
	{
		retorno= InsertItemFirst( lpszItem, pParent );
		return retorno;
	}

	// add befor or after a child item by sorting
	if( pInsertAfter == TLI_SORT )
	{
		HTREELISTITEM pPrevItem = GetSortChildItem( pParent );
		if( pPrevItem == NULL ) {
			retorno= InsertItemFirst( lpszItem, pParent );
			return retorno;
		} else {
			retorno= InsertItemNext( lpszItem, pParent, pPrevItem );
			return retorno;
		}
	}

	// add after a child item
	if( pInsertAfter == TLI_LAST )
	{
		HTREELISTITEM pPrevItem = GetLastChildItem( pParent );
		retorno= InsertItemNext( lpszItem, pParent, pPrevItem );
		return retorno;
	}
	
	// add after some child item
	{
		HTREELISTITEM pPrevItem = GetValidChildItem( pParent, pInsertAfter );
		if( pPrevItem == NULL ) {
			
			retorno=InsertItemFirst( lpszItem, pParent );
			return retorno;
		} else {
			retorno=InsertItemNext( lpszItem, pParent, pPrevItem );
			return retorno;
		}
	}
	return NULL;
}

BOOL CTreeListCtrl::DeleteItem( HTREELISTITEM pItem )
{
	// delete an item
	if( pItem == TLI_ROOT )
	{
		HTREELISTITEM pChild = GetChildItem(m_pRootItem);
		HTREELISTITEM pDelete;
		while( pChild != NULL )
		{
			pDelete = pChild;
			pChild = GetNextSiblingItem(pChild);
			DeleteItemX( pDelete );
		}
	}
	else
	{
		DeleteItemX( pItem );
	}

	if( IsWindow( GetSafeHwnd() ) )
	{
		Layout();
		Invalidate();
	}
	return TRUE;
}

void CTreeListCtrl::UnSelectItem(HTREELISTITEM item) {

	m_pItemMap.SetItemSelected(item,false);
	for( int i=0; i<m_arSelects.GetCount(); i++) {
		if( (HTREELISTITEM)m_arSelects.GetAt(i)==item) {
			m_arSelects.RemoveAt(i);
			if( item==m_pFocusItem)
				m_pFocusItem= (HTREELISTITEM) m_arSelects[0];
			break;
		}
	}
}

void CTreeListCtrl::SelectRange(HTREELISTITEM hFirst,HTREELISTITEM hLast, BOOL bOnly /*= TRUE*/)
{
	//locate (and select) either first or last
	// (so order is arbitrary)
	HTREELISTITEM hItem = GetRootItem();
	while (hItem) {
		if ((hItem == hFirst) || (hItem == hLast)) {
			if (hFirst != hLast) {
				if (!m_pItemMap.GetItemSelected(hItem))
					SelectItem(hItem, 0, false);
				hItem = GetNextVisibleItem(hItem);
			}
			break;
		}

		if (bOnly && m_pItemMap.GetItemSelected(hItem)) {
			UnSelectItem(hItem);
		}
		hItem = GetNextVisibleItem(hItem);
	}
	//select rest of range
	while (hItem) {
		if (!m_pItemMap.GetItemSelected(hItem))
			SelectItem(hItem, 0, false);
		if ((hItem == hFirst) || (hItem == hLast)) {
			hItem = GetNextVisibleItem(hItem);
			break;
		}
		hItem = GetNextVisibleItem(hItem);
	}
	if (!bOnly)
		return;
	while (hItem) {
		if (m_pItemMap.GetItemSelected(hItem)) {
			UnSelectItem(hItem);
		}
		hItem = GetNextVisibleItem(hItem);
	}
	SelectItem(hLast, 0, false);
}


CUIntArray *CTreeListCtrl::GetKeys() {

	return &m_pKeys;
}

void CTreeListCtrl::SetKey(int columna, bool primary) {

	m_pKeys.Add(columna);
	if( primary) {
		m_nOrderBy= columna;
		CTreeListColumnInfo* pColumna;
		for( int i=0; i<m_arColumns.GetCount(); i++) {
			pColumna= (CTreeListColumnInfo*) m_arColumns[i];
			pColumna->SetImage(0);
			pColumna->SetFormat( i==m_nOrderBy ? TLF_CAPTION_IMAGE | TLF_IMAGEALIGN_LEFT : 0, i==columna ? 0 : TLF_CAPTION_IMAGE);
		}
	}
}

BOOL CTreeListCtrl::DeleteAllItems()
{
	// delete all items of treelist control
	BOOL bRedraw = IsWindowVisible();
	
	if( bRedraw )
		SetRedraw( FALSE );

	if(!m_bCustomSort) {
		m_nSortedColumn= -1;
		m_wndHeader.SetSortArrow(-1, false);
		m_wndHeader.m_bShowOver=m_bCanSort;
		m_bAscendente= true;
	}

	CTreeListColumnInfo* pColumna;
	for( int i=0; i<m_arColumns.GetCount(); i++) {
		pColumna= (CTreeListColumnInfo*) m_arColumns[i];
		pColumna->SetFormat( 0, TLF_CAPTION_IMAGE);
	}

	if(m_pThread) {
		SetError(NULL, "Listo");
		m_pThread->Kill(true);
	}
	m_nOrderBy= -1;
	m_pKeys.RemoveAll();

	BOOL bDelte = DeleteItem( TLI_ROOT );
	if( bRedraw )
		SetRedraw( TRUE );

	Invalidate();

	return bDelte;
}

BOOL CTreeListCtrl::Expand( HTREELISTITEM pItem, int nCode )
{
	// expand an item
	return Expand( pItem, nCode, m_iSelCol );
}

BOOL CTreeListCtrl::Expand( HTREELISTITEM pItem, int nCode, int iSubItem )
{
	// expand an item or collapse an item
	ASSERT( m_dwStyle&TLC_TREELIST );
	LRESULT lResult;

	if( pItem == NULL )
		return FALSE;

	if( m_pItemMap.GetItemState(pItem)&TLIS_EXPANDEDONCE && GetChildItem(pItem)== NULL )
		return TRUE;

	if( nCode == TLE_TOGGLE )
	{
		if( m_pItemMap.GetItemState(pItem)&TLIS_EXPANDED )
			nCode = TLE_COLLAPSE;
		else
			nCode = TLE_EXPAND;
	}

	switch( nCode )
	{
	case TLE_EXPAND:
		if(m_pItemMap.GetItemState(pItem)&TLIS_EXPANDED )
			return TRUE;
		break;

	case TLE_COLLAPSE:
	case TLE_COLLAPSERESET:
		if( !(m_pItemMap.GetItemState(pItem)&TLIS_EXPANDED ) )
			return TRUE;
		break;

	default:
		return FALSE;
		break;
	}

	lResult = Notify( TLN_ITEMEXPANDING, pItem, iSubItem );
	if( lResult == -1 )
		return FALSE;

	BOOL bRedraw = IsWindowVisible();
	
	if( bRedraw )
		SetRedraw( FALSE );

	BOOL bExpand = FALSE;

	switch( nCode )
	{
	case TLE_EXPAND:
		m_pItemMap.ExpandItem(pItem);
		StatExpand( pItem );
		break;

	case TLE_COLLAPSE:
		m_pItemMap.CollapseItem(pItem);
		StatCollapse( pItem );
		break;

	case TLE_COLLAPSERESET:
		m_pItemMap.CollapseItem(pItem);
		while( GetChildItem(pItem) != NULL )
		{
			DeleteItem( GetChildItem(pItem));
		}
		StatCollapse( pItem );
		break;

	default:
		ASSERT( FALSE );
		break;
	}

	//SelectItem( pItem, iSubItem );
	if( bRedraw )
		SetRedraw( TRUE );

	Layout();
	Invalidate();

	Notify( TLN_ITEMEXPANDED, pItem, iSubItem );

	return bExpand;
}

BOOL CTreeListCtrl::Select( HTREELISTITEM pItem, int nCode )
{
	// select an item
	SelectItem( pItem, 0, FALSE );
	return TRUE;
}

HTREELISTITEM CTreeListCtrl::InsertItemNew( LPCTSTR lpszItem, HTREELISTITEM pParent )
{
	// insert a new item
	ASSERT( pParent != NULL );
	HTREELISTITEM pNewItem= m_pItemMap.AddItem(GetColumns());
	m_pItemMap.SetItemText(pNewItem, lpszItem, 0);
	m_pItemMap.SetParentItem(pNewItem, pParent);
	m_pItemMap.SetItemLevel(pNewItem, m_pItemMap.GetItemLevel(pParent) + 1);

	if( m_pItemMap.GetItemCheck(pParent) == 1 )
		m_pItemMap.SetItemCheck(pNewItem, TRUE );

	if( m_pItemMap.GetItemLock(pParent) == 1 )
		m_pItemMap.SetItemLock(pNewItem, TRUE );

	m_pItemMap.SetChildItem(pParent, pNewItem);
	m_pItemMap.GetItemState(pParent, TLIS_EXPANDEDONCE );

	return InsertItemX( pParent, pNewItem );
}

HTREELISTITEM CTreeListCtrl::InsertItemFirst( LPCTSTR lpszItem, HTREELISTITEM pParent )
{
	// insert a child item before first child item
	ASSERT( pParent != NULL );

	HTREELISTITEM pNewItem= m_pItemMap.AddItem(GetColumns());
	m_pItemMap.SetItemText(pNewItem, lpszItem, 0);

	HTREELISTITEM pFirstItem;
	pFirstItem	= GetChildItem(pParent);

	m_pItemMap.SetParentItem(pNewItem, GetParentItem(pFirstItem));
	m_pItemMap.SetNextSiblingItem(pNewItem, pFirstItem);
	m_pItemMap.SetItemLevel(pNewItem, m_pItemMap.GetItemLevel(pFirstItem));

	if( m_pItemMap.GetItemCheck(pParent) == 1 )
		m_pItemMap.SetItemCheck(pNewItem, TRUE );

	if( m_pItemMap.GetItemLock(pParent) == 1 )
		m_pItemMap.SetItemLock(pNewItem, TRUE );
	
	m_pItemMap.SetPrevSiblingItem(pFirstItem, pNewItem);
	m_pItemMap.SetChildItem(pParent, pNewItem);

	return InsertItemX( pParent, pNewItem );
}

HTREELISTITEM CTreeListCtrl::InsertItemNext( LPCTSTR lpszItem, HTREELISTITEM pParent, HTREELISTITEM pInsertAfter )
{
	// insert a child item after some item
	ASSERT( pParent != NULL && pInsertAfter != NULL );

	HTREELISTITEM pNewItem= m_pItemMap.AddItem(GetColumns());
	m_pItemMap.SetItemText(pNewItem, lpszItem,0);

	m_pItemMap.SetParentItem(pNewItem, GetParentItem(pInsertAfter));
	m_pItemMap.SetNextSiblingItem(pNewItem, GetNextSiblingItem(pInsertAfter));
	m_pItemMap.SetPrevSiblingItem(pNewItem, pInsertAfter);
	m_pItemMap.SetItemLevel(pNewItem, m_pItemMap.GetItemLevel(pInsertAfter));

	if( m_pItemMap.GetItemCheck(pParent) == 1 )
		m_pItemMap.SetItemCheck(pNewItem, TRUE );

	if( m_pItemMap.GetItemLock(pParent) == 1 )
		m_pItemMap.SetItemLock(pNewItem, TRUE );

	m_pItemMap.SetNextSiblingItem(pInsertAfter, pNewItem);

	if( m_pItemMap.GetNextSiblingItem(pNewItem) != NULL )
		m_pItemMap.SetPrevSiblingItem(m_pItemMap.GetNextSiblingItem(pNewItem),pNewItem);

	return InsertItemX( pParent, pNewItem );
}

HTREELISTITEM CTreeListCtrl::InsertItemX( HTREELISTITEM pParent, HTREELISTITEM pNewItem )
{
	// insert an item
	ASSERT( pParent != NULL );
	ASSERT( pNewItem != NULL );

	StatChildAdd( pNewItem, m_pItemMap.GetChildCount(pNewItem), m_pItemMap.GetVisibleChildren(pNewItem));
	m_pItemMap.SetItemImage(pNewItem, 0, 1, 2, 3 );

	Layout();
	Invalidate();

	return pNewItem;
}

BOOL CTreeListCtrl::DeleteItemX( HTREELISTITEM pItem )
{
	// delete an item
	if( GetPrevSiblingItem(pItem) == NULL && GetNextSiblingItem(pItem) == NULL )
	{
		// no brother
		m_pItemMap.SetChildItem(GetParentItem(pItem), NULL);
	}
	else if( GetPrevSiblingItem(pItem) == NULL )
	{
		// first item of parent
		m_pItemMap.SetChildItem(GetParentItem(pItem), GetNextSiblingItem(pItem));
		m_pItemMap.SetPrevSiblingItem(GetNextSiblingItem(pItem), NULL);
	}
	else if( GetNextSiblingItem(pItem) == NULL )
	{
		// last item of parent
		m_pItemMap.SetNextSiblingItem(GetPrevSiblingItem(pItem), NULL);
	}
	else
	{
		// middle item of parent

		m_pItemMap.SetNextSiblingItem(GetPrevSiblingItem(pItem), GetNextSiblingItem(pItem));
		m_pItemMap.SetPrevSiblingItem(GetNextSiblingItem(pItem), GetPrevSiblingItem(pItem));
	}

	StatChildDel( pItem, m_pItemMap.GetChildCount(pItem), m_pItemMap.GetVisibleChildren(pItem));

	DeleteItemFast( pItem );

	return TRUE;
}

BOOL CTreeListCtrl::DeleteItemFast( HTREELISTITEM pItem )
{
	// fast delete child items of pItem
	HTREELISTITEM pChild;
	HTREELISTITEM pDelete;

	pChild = GetChildItem(pItem);
	while( pChild != NULL )
	{
		pDelete = pChild;
		pChild = GetNextSiblingItem(pChild);
		DeleteItemFast( pDelete );
	}

	for( int iSel=0; iSel<m_arSelects.GetSize(); iSel++ )
	{
		if( pItem == (HTREELISTITEM)m_arSelects[iSel] )
		{
			m_arSelects.RemoveAt( iSel );
			break;
		}
	}

	if( pItem == m_pFocusItem )
	{
		m_pFocusItem = NULL;
		m_hSelect= NULL;
	}

	Notify( TLN_SELCHANGING, pItem, 0 );

	m_pItemMap.DeleteItem(pItem);
	
	return TRUE;
}

void CTreeListCtrl::ActiveItem( HTREELISTITEM pItem, int iSubItem )
{
	// active a selected item
	ASSERT( pItem != NULL && m_pItemMap.GetItemSelected(pItem) );

	if( !(m_dwStyle&TLC_MULTIPLESELECT) )
	{
		// single selected
		ASSERT( pItem == (HTREELISTITEM)m_arSelects[0] );
		ASSERT( pItem == m_pFocusItem );
		m_iSelCol = iSubItem;
	}
	else
	{
		// multi selected
		BOOL bFound = FALSE;
		int iItem;
		for( iItem=0; iItem<m_arSelects.GetSize(); iItem++ )
		{
			if( pItem == (HTREELISTITEM)m_arSelects[iItem] )
			{
				bFound = TRUE;
				break;
			}
		}

		ASSERT( bFound );
		
		m_arSelects.RemoveAt( iItem );
		m_arSelects.InsertAt( 0, (void*)pItem );
		m_iSelCol = iSubItem;
		m_pFocusItem = pItem;
	}
	
	EnsureVisible( pItem, iSubItem );
}

void CTreeListCtrl::SelectItem( HTREELISTITEM pItem, int iSubItem, BOOL bClearLastSelected )
{
	LRESULT lResult;

	if( m_arSelects.GetSize() > 0 )
	{
		if( bClearLastSelected )
		{
			int iItem;
			for( iItem=0; iItem<m_arSelects.GetSize(); iItem++ )
			{
				HTREELISTITEM pOldItem;
				if(pOldItem = (HTREELISTITEM)m_arSelects[iItem])
					m_pItemMap.SetItemSelected(pOldItem, FALSE );
			}

			m_pFocusItem = pItem;
			m_hSelect= pItem;
			m_arSelects.RemoveAll();
			if( pItem)
				m_pItemMap.SetItemSelected(pItem);
			m_iSelCol = iSubItem;
			m_arSelects.Add( (void*)pItem );
		}
		HTREELISTITEM pSelItem = (HTREELISTITEM)m_arSelects[0];
		if( pItem == pSelItem && iSubItem == m_iSelCol )
		{
			/*if( pItem)
				EnsureVisible( pItem, iSubItem );*/
			Invalidate();
			return;
		}
	}

	lResult = Notify( TLN_SELCHANGING, pItem, iSubItem );
	if( lResult == -1 )
		return;

	// select a new item
	if( pItem == NULL )
	{
		int iItem;
		for( iItem=0; iItem<m_arSelects.GetSize(); iItem++ )
		{
			HTREELISTITEM pOldItem;
			pOldItem = (HTREELISTITEM)m_arSelects[iItem];
			m_pItemMap.SetItemSelected(pOldItem, FALSE );
		}

		m_arSelects.RemoveAll();

		Notify( TLN_SELCHANGED, pItem, iSubItem );
		Invalidate();
		return;
	}

	// select a new item
	if( !(m_dwStyle&TLC_MULTIPLESELECT) )
	{
		// single select
		ASSERT( m_arSelects.GetSize() <= 1 );

		if( m_arSelects.GetSize() == 1 )
		{
			HTREELISTITEM pOldItem = (HTREELISTITEM)m_arSelects[0];
			m_pItemMap.SetItemSelected(pOldItem, FALSE );
			m_arSelects.RemoveAll();
		}

		m_pFocusItem = pItem;
		m_pItemMap.SetItemSelected(pItem);
		m_iSelCol = iSubItem;
		m_arSelects.Add( (void*) pItem );
		m_hSelect= pItem;
	}
	else
	{
		// multi select
		if( bClearLastSelected )
		{
			int iItem;
			for( iItem=0; iItem<m_arSelects.GetSize(); iItem++ )
			{
				HTREELISTITEM pOldItem;
				pOldItem = (HTREELISTITEM)m_arSelects[iItem];
				m_pItemMap.SetItemSelected(pOldItem, FALSE );
			}

			m_pFocusItem = pItem;
			m_hSelect= pItem;
			m_arSelects.RemoveAll();
			m_pItemMap.SetItemSelected(pItem);
			m_iSelCol = iSubItem;
			m_arSelects.Add( (void*)pItem );
		}
		else
		{
			BOOL bFound = FALSE;
			int iItem;
			for( iItem=0; iItem<m_arSelects.GetSize(); iItem++ )
			{
				if( pItem == (HTREELISTITEM)m_arSelects[iItem] )
				{
					bFound = TRUE;
					break;
				}
			}

			if( bFound )
			{
				m_pItemMap.SetItemSelected(pItem, FALSE );
				m_arSelects.RemoveAt( iItem );
				m_iSelCol = iSubItem;
				if(m_arSelects.GetCount()==0)
					m_hSelect= pItem;
				m_pFocusItem = pItem;
			}
			else
			{
				m_pItemMap.SetItemSelected(pItem);
				m_arSelects.InsertAt( 0, (void*)pItem );
				m_iSelCol = iSubItem;
				if(m_arSelects.GetCount()==1)
					m_hSelect= pItem;
				m_pFocusItem = pItem;
			}
		}
	}

	Notify( TLN_SELCHANGED, pItem, iSubItem );
	// EnsureVisible( pItem, iSubItem );
	Invalidate();
	return;
}

HTREELISTITEM CTreeListCtrl::GetLastChildItem( HTREELISTITEM pParent )
{
	// retrieve last child item
	if( pParent == TLI_ROOT )
		return GetLastChildItem( m_pRootItem );

	if( GetChildItem(pParent) == NULL )
		return NULL;

	HTREELISTITEM pChildItem;
	pChildItem = GetChildItem(pParent);

	while( GetNextSiblingItem(pChildItem) != NULL )
	{
		pChildItem = GetNextSiblingItem(pChildItem);
	}

	return pChildItem;
}

HTREELISTITEM CTreeListCtrl::GetSortChildItem( HTREELISTITEM pParent )
{
	// retrieve sort child item
	if( pParent == TLI_ROOT )
		return GetSortChildItem( m_pRootItem );

	return GetLastChildItem( pParent );
}

HTREELISTITEM CTreeListCtrl::GetValidChildItem( HTREELISTITEM pParent, HTREELISTITEM pChild )
{
	// is it a valid child item
	ASSERT( pChild != NULL );

	if( pParent == TLI_ROOT )
	{
		return GetValidChildItem( m_pRootItem, pChild );
	}

	if( GetChildItem(pParent) == NULL )
	{
		return NULL;
	}

	if( pChild == NULL )
	{
		return NULL;
	}

	BOOL bValid = FALSE;
	HTREELISTITEM pChildItem;
	pChildItem = GetChildItem(pParent);

	while( pChildItem != NULL )
	{
		if( pChildItem == pChild )
		{
			bValid = TRUE;
			break;
		}
		pChildItem = GetNextSiblingItem(pChildItem);
	}
	
	if( !bValid )
	{
		return NULL;
	}

	return pChild;
}

void CTreeListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
}

HTREELISTITEM CTreeListCtrl::AddItem(CString nombre, int columna, CString real) {

	HTREELISTITEM item= GetRootItem();
	while( item) {
		if( !m_pItemMap.GetItemDataSort(item) && m_pItemMap.GetItemText(item, columna)==nombre) {// found
			return item;
		}
		item= GetNextSiblingItem(item);
	}
	item = InsertItem(real);
	m_pItemMap.SetItemColor(item, ::GetSysColor(COLOR_HIGHLIGHT));
	m_pItemMap.SetItemBold(item);
	m_pItemMap.SetItemText(item, nombre, columna);
	return item;
}

HTREELISTITEM CTreeListCtrl::AddItemEx(CString nombre, int columna, CString real) {

	HTREELISTITEM item= GetRootItem();
	while( item) {
		if( m_pItemMap.GetItemText(item, columna)==nombre) {// found
			item= InsertItem(real, item);
			SetItemText(item, nombre, columna);
			return item;
		}
		item= GetNextSiblingItem(item);
	}
	item= InsertItem(real);
	m_pItemMap.SetItemColor(item, ::GetSysColor(COLOR_HIGHLIGHT));
	m_pItemMap.SetItemBold(item);
	m_pItemMap.SetItemText(item, nombre, columna);
	return InsertItem(nombre, item);
}

void CTreeListCtrl::PreSort(int col) {

	HTREELISTITEM item= GetRootItem();
	// marcar los antiguos
	while( item) {
		m_pItemMap.SetItemDataSort(item, 1);
		item= GetNextSiblingItem(item);
	}

	item= GetRootItem();
	HTREELISTITEM subitem= NULL;
	HTREELISTITEM nuevo= NULL;
	HTREELISTITEM subparent= NULL;

	while( item) {
		if( m_pItemMap.GetItemDataSort(item)) {
			subitem= GetChildItem(item);
			while( subitem) {

				subparent= AddItem(m_pItemMap.GetItemText(subitem, col), col, m_pItemMap.GetItemText(subitem, 0));
				nuevo = InsertItem(m_pItemMap.GetItemText(subitem, 0), subparent);
				for( int i=0; i<GetColumnCount(); i++)
					m_pItemMap.SetItemText(nuevo, m_pItemMap.GetItemText(subitem, i), i);
				subitem= GetNextSiblingItem(subitem);
			}
		}
		item= GetPrevSiblingItem(item);
	}
	// borrar los marcados
	item= GetRootItem();
	while( item) {
		subitem= m_pItemMap.GetItemDataSort(item) ? item : NULL;
		item= GetNextSiblingItem(item);
		if( subitem) {
			DeleteItem(subitem);
		}
	}
	ShowStat();
}

void CTreeListCtrl::SortSubItems(int columna, HTREELISTITEM parent, int tipo, bool clean) {

	HTREELISTITEM item= GetChildItem(parent);

	int count=0;

	while( item) {
		count++;
		item= GetNextSiblingItem(item);
	}
	DWORD *items= new DWORD[count];

	m_pItemMap.SortSubItems(columna, parent, tipo, clean,m_bAscendente, items, count);

	for( int i=0; i<count-1; i++) 
		MoveAfterItem((HTREELISTITEM )items[i+1], (HTREELISTITEM ) items[i]);

	// rerdenarlos realmente
	delete []items;
}

void CTreeListCtrl::SetSortColumn(int col, bool ascendente/*=false*/) {

	m_nSortedColumn= col;
	m_bAscendente=ascendente;
	m_wndHeader.SetSortArrow(col, ascendente);
}

int  CTreeListCtrl::GetSortedColumn() {

	return m_nSortedColumn;
}

bool CTreeListCtrl::GetAscendentSort() {

	return m_bAscendente;
}

void CTreeListCtrl::Sort(int columna) {

	// notificamos el click en la columna
	if(!m_bCanSort)
		return;

	m_bAscendente= (columna == m_nSortedColumn) ? !m_bAscendente : TRUE;
	if(m_bCustomSort) {
		m_nSortedColumn = columna;
		m_wndHeader.SetSortArrow(m_nSortedColumn, m_bAscendente);
		Notify(LVN_COLUMNCLICK, NULL, columna);
		return;
	}
	Notify(LVN_COLUMNCLICK, NULL, columna);

	CWaitCursor wait;
	bool thread=false;

	if( m_pThread) {

		m_pThread->Kill(true);
		thread= true;
		SetError(NULL, "Listo");
	}
	SetRedraw(false);
	
	if(columna!=m_nSortedColumn && AfxGetApp()->GetProfileInt("arbol", "agrupar", 0)) {

		PreSort(columna);
		m_nOrderBy= columna;
	}

	CTreeListColumnInfo* pColumna;
	for( int i=0; i<m_arColumns.GetCount(); i++) {
		pColumna= (CTreeListColumnInfo*) m_arColumns[i];
		pColumna->SetImage(0);
		if( i==m_nOrderBy)
			pColumna->SetFormat(TLF_CAPTION_IMAGE | TLF_IMAGEALIGN_LEFT, 0);
		else pColumna->SetFormat( 0, TLF_CAPTION_IMAGE);
	}

	m_nSortedColumn = columna;

	m_wndHeader.SetSortArrow(m_nSortedColumn, m_bAscendente);

	int tipo=1;
	if( m_nColumType.GetCount()>columna) 
		tipo= m_nColumType.GetAt(columna);
	HTREELISTITEM item= m_pRootItem;
	SortSubItems(columna, m_pRootItem, tipo, (AfxGetApp()->GetProfileInt("arbol", "stat", 0)==1 && columna==0));
	item= GetChildItem(item);
	while( item) {
		SortSubItems(columna, item, tipo);
		item= GetNextSiblingItem(item);
	}

	GetParent()->SendMessage(WM_SORTTREE, -1, m_nSortedColumn);

	if(thread || (AfxGetApp()->GetProfileInt("arbol", "stat", 0)==1 && columna==0)) 
		ShowStat();

	SetRedraw(true);
	Invalidate();
}

void CTreeListCtrl::ExpandAll() {

	CWaitCursor wait;
	HTREELISTITEM item= GetRootItem();
	while( item) {
		Expand(item, TLE_EXPAND);
		item= GetNextSiblingItem(item);
	}
}

void CTreeListCtrl::CollapseAll() {

	CWaitCursor wait;
	HTREELISTITEM item= GetRootItem();
	while( item) {
		Expand(item, TLE_COLLAPSE);
		item= GetNextSiblingItem(item);
	}
}

void CTreeListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// left button was down
	SetFocus();

	CWnd::OnLButtonDown( nFlags, point );

	int nFlag, iSubItem;
	CRect rcText;

	HTREELISTITEM pItem = HitTest( point, &nFlag, &iSubItem, &rcText );
	if( pItem != NULL )
	{
		if( m_dwStyle&TLC_TREELIST )
		{
			if( nFlag&TLHT_ONITEMBUTTON )
			{
				if( m_pItemMap.GetItemState(pItem) & TLIS_EXPANDED )
				{
					Expand( pItem, TLE_COLLAPSE, iSubItem );
				}
				else
				{
					Expand( pItem, TLE_EXPAND, iSubItem );
				}
			}
		}

		if( nFlag&TLHT_ONITEMCHECKBOX )
		{
			SetItemCheck( pItem );
		}

		if( nFlag&TLHT_ONITEMLOCKBOX )
		{
			SetItemLock( pItem );
		}

		if( nFlag&TLHT_ONITEM && !(nFlag&TLHT_ONITEMBUTTON) && !(nFlag&TLHT_ONITEMCHECKBOX) && !(nFlag&TLHT_ONITEMLOCKBOX))
		{
			if( nFlags&MK_SHIFT) {
				SelectRange(m_hSelect,pItem, !(nFlags&MK_CONTROL));
				SelectItem(pItem, iSubItem, false);
				SelectItem(pItem, iSubItem, false);
			} else if( nFlags&MK_CONTROL)
			{
				SelectItem( pItem, iSubItem, FALSE );
			} else
			{
				SelectItem( pItem, iSubItem );
			}
		}
		Layout();
		Invalidate();
	}
	else
	{
		DoBanding(true, nFlags, point);
		// SelectItem( NULL );
		Layout();
		Invalidate();
	}
}

void CTreeListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
}

void CTreeListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// left button double click
	CWnd::OnLButtonDblClk( nFlags, point );

	int nFlag, iSubItem;

	HTREELISTITEM pItem = HitTest( point, &nFlag, &iSubItem );
	if( pItem != NULL )
	{
		if( m_dwStyle&TLC_TREELIST )
		{
			if ( nFlag&TLHT_ONITEMIMAGE || nFlag&TLHT_ONITEMTEXT )
			{
				Expand( pItem, TLE_TOGGLE, iSubItem );
			}
		}
		Layout();
		Invalidate();
		Notify(NM_DBLCLK, pItem, 0);
	}

}

void CTreeListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// right button was down
	SetFocus();

	CWnd::OnRButtonDown(nFlags, point);

	int nFlag, iSubItem;
	CRect rcText;

	HTREELISTITEM pItem = HitTest( point, &nFlag, &iSubItem, &rcText );
	if( pItem != NULL )
	{
		if( !m_pItemMap.GetItemSelected(pItem)) {
			if( nFlag&TLHT_ONITEM && !(nFlag&TLHT_ONITEMBUTTON) )
			{
				if( !(nFlags&MK_CONTROL) )
					SelectItem( pItem, iSubItem);
				else
					SelectItem( pItem, iSubItem, false ); 
			}
		}
		Layout();
		Invalidate();
		Notify(NM_RCLICK, pItem, 0);
	}
	else
	{
		SelectItem( NULL );
		Layout();
		Invalidate();
		Notify(NM_RCLICK, NULL, 0);
	}
}

void CTreeListCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// right button was up
	CWnd::OnRButtonUp(nFlags, point);
}

HTREELISTITEM CTreeListCtrl::HitTest( CPoint pt, int* pFlag, int* pSubItem, CRect* prcText )
{
	// hittest of all items
	int nFlag, nSubItem;
	CRect rcText;

	if( pFlag == NULL )
		pFlag = &nFlag;

	if( pSubItem == NULL )
		pSubItem = &nSubItem;

	if( prcText == NULL )
		prcText = &rcText;

	*pFlag		= 0;
	*pSubItem	= 0;
	rcText.SetRect( 0, 0, 0, 0 );

	int nFirstRow	=	GetScrollPos( SB_VERT );
	int	nShowRows	=	m_rcTreeList.Height() / GetItemHeight() + 1;

	CRect rcItem;
	rcItem.SetRect( 0, 0, GetWidth(), GetItemHeight() );
	rcItem.OffsetRect( m_rcTreeList.left, m_rcTreeList.top );
	rcItem.OffsetRect( -GetScrollPos( SB_HORZ ), 0 );

	for( int i = nFirstRow; i <= nFirstRow + nShowRows && i < GetVisibleCount(); i++ )
	{
		if( rcItem.PtInRect( pt ) )
		{
			HTREELISTITEM pItem = GetFirstShowItem( i );
			ASSERT( pItem != NULL );

			return HitTest( pt, pFlag, pSubItem, prcText, rcItem, pItem );
		}
		rcItem.OffsetRect( 0, m_nItemHeight );
	}
	return NULL;
}

HTREELISTITEM CTreeListCtrl::HitTest( CPoint pt, int* pFlag, int* pSubItem, CRect* prcText, CRect rcItem, HTREELISTITEM pItem )
{
	// hittest of one item
	ASSERT( pFlag != NULL && pSubItem != NULL );

	int nColPos = 0;

	CTreeListColumnInfo* pColumnInfo;

	for( int iShow = 0; iShow < m_arShows.GetSize(); iShow++, nColPos += pColumnInfo->m_nWidth )
	{
		int iCol;

		// do NOT test zero column;
		iCol = m_arShows[iShow];
		pColumnInfo = (CTreeListColumnInfo*)m_arColumns[iCol];
		if( pColumnInfo->m_nWidth == 0 )
			continue;

		CRect rcColumn;
		rcColumn.SetRect( 0, rcItem.top, pColumnInfo->m_nWidth, rcItem.bottom );
		rcColumn.OffsetRect( nColPos, 0 );
		rcColumn.OffsetRect( rcItem.left, 0 );

		if( rcColumn.PtInRect( pt ) )
		{
			*pSubItem = iCol;
			if( iCol == 0 )
			{
				if ( m_dwStyle&TLC_TREELIST )
					return HitTestTree( pt, pFlag, prcText, rcColumn, pItem );
				else
					return HitTestMain( pt, pFlag, prcText, rcColumn, pItem );
			}
			else
			{
				*prcText = rcColumn;
				return HitTestList( pt, pFlag, prcText, rcColumn, pItem );
			}
		}
	}

	return NULL;
}

HTREELISTITEM CTreeListCtrl::HitTestTree( CPoint pt, int* pFlag, CRect* prcText, CRect rcColumn, HTREELISTITEM pItem )
{
	// hittest of tree column of treelist control
	ASSERT( pFlag != NULL );
	ASSERT( pItem != NULL );

	int nPerfix = 0;
	CRect rcGraph;
	
	// skip perfix and tree line
	nPerfix += TLL_BORDER;
	nPerfix += (m_pItemMap.GetItemLevel(pItem)-1) * TLL_WIDTH;

	// check tree button
	if( m_dwStyle&(TLC_TREELINE|TLC_BUTTON) )
	{
		if( !(m_pItemMap.GetItemState(pItem)&TLIS_EXPANDEDONCE) || GetChildItem(pItem) != NULL )
		{
			rcGraph.SetRect( 0, rcColumn.top, TLL_WIDTH, rcColumn.bottom );
			rcGraph.OffsetRect( nPerfix, 0 );
			rcGraph.OffsetRect( rcColumn.left, 0 );

			CPoint ptCenter;
			ptCenter.x = rcGraph.left + rcGraph.Width() / 2 - 1;
			ptCenter.y = rcGraph.top + rcGraph.Height() / 2 - 1;

			CRect rcButton;
			rcButton.SetRect( -TLL_BUTTON, -TLL_BUTTON, TLL_BUTTON+1, TLL_BUTTON+1 );
			rcButton.OffsetRect( ptCenter );

			if( rcButton.PtInRect( pt ) )
				*pFlag |= TLHT_ONITEMBUTTON;
		}
		nPerfix += TLL_WIDTH;
	}

	// check tree check box;
	if( m_dwStyle&TLC_CHECKBOX )
	{
		if(m_pItemMap.GetItemState(pItem)&TLIS_SHOWCHECKBOX )
		{
			rcGraph.SetRect( 0, rcColumn.top, TLL_WIDTH, rcColumn.bottom );
			rcGraph.OffsetRect( nPerfix, 0 );
			rcGraph.OffsetRect( rcColumn.left, 0 );

			if( rcGraph.PtInRect( pt ) )
				*pFlag |= TLHT_ONITEMCHECKBOX;

			nPerfix += TLL_WIDTH;
		}
	}

	// check tree lock box;
	if( m_dwStyle&TLC_LOCKBOX )
	{
		if(m_pItemMap.GetItemState(pItem)&TLIS_SHOWLOCKBOX )
		{
			rcGraph.SetRect( 0, rcColumn.top, TLL_WIDTH/2, rcColumn.bottom );
			rcGraph.OffsetRect( nPerfix, 0 );
			rcGraph.OffsetRect( rcColumn.left, 0 );

			if( rcGraph.PtInRect( pt ) )
				*pFlag |= TLHT_ONITEMLOCKBOX;

			nPerfix += TLL_WIDTH/2;
		}
	}

	// check tree image
	if( m_dwStyle&TLC_IMAGE )
	{
		if( m_pItemMap.GetItemState(pItem)&TLIS_SHOWTREEIMAGE )
		{
			rcGraph.SetRect( 0, rcColumn.top, TLL_WIDTH, rcColumn.bottom );
			rcGraph.OffsetRect( nPerfix, 0 );
			rcGraph.OffsetRect( rcColumn.left, 0 );
	
			if( rcGraph.PtInRect( pt ) )
				*pFlag |= TLHT_ONITEMIMAGE;

			nPerfix += TLL_WIDTH;
		}
	}

	// add gaps between text and image/lock 
	if( m_dwStyle&TLC_LOCKBOX || m_dwStyle&TLC_IMAGE )
		nPerfix += 2;

	// check tree text
	CRect rcText;
	rcText = rcColumn;
	rcText.left = rcText.left + nPerfix;

	if( rcText.Width() > 0 )
	{
		if( rcText.PtInRect( pt ) )
		{
			*pFlag |= TLHT_ONITEMTEXT;
			*prcText = rcText;
		}
	}

	if( *pFlag == 0 )
		*pFlag |= TLHT_ONITEMSPACE;

	return pItem;
}

HTREELISTITEM CTreeListCtrl::HitTestMain( CPoint pt, int* pFlag, CRect* prcText, CRect rcColumn, HTREELISTITEM pItem )
{
	// hittest of main column of list control
	ASSERT( pFlag != NULL );
	ASSERT( pItem != NULL );

	int nPerfix = 0;
	CRect rcGraph;
	
	// skip perfix and tree line
	nPerfix += TLL_BORDER;

	// check tree check box;
	if( m_dwStyle&TLC_CHECKBOX )
	{
		if(m_pItemMap.GetItemState(pItem)&TLIS_SHOWCHECKBOX )
		{
			rcGraph.SetRect( 0, rcColumn.top, TLL_WIDTH, rcColumn.bottom );
			rcGraph.OffsetRect( nPerfix, 0 );
			rcGraph.OffsetRect( rcColumn.left, 0 );

			if( rcGraph.PtInRect( pt ) )
				*pFlag |= TLHT_ONITEMCHECKBOX;

			nPerfix += TLL_WIDTH;
		}
	}

	// check tree lock box;
	if( m_dwStyle&TLC_LOCKBOX )
	{
		if(m_pItemMap.GetItemState(pItem)&TLIS_SHOWLOCKBOX )
		{
			rcGraph.SetRect( 0, rcColumn.top, TLL_WIDTH/2, rcColumn.bottom );
			rcGraph.OffsetRect( nPerfix, 0 );
			rcGraph.OffsetRect( rcColumn.left, 0 );

			if( rcGraph.PtInRect( pt ) )
				*pFlag |= TLHT_ONITEMLOCKBOX;

			nPerfix += TLL_WIDTH/2;
		}
	}

	// check tree image
	if( m_dwStyle&TLC_IMAGE )
	{
		if(m_pItemMap.GetItemState(pItem)&TLIS_SHOWTREEIMAGE )
		{
			rcGraph.SetRect( 0, rcColumn.top, TLL_WIDTH, rcColumn.bottom );
			rcGraph.OffsetRect( nPerfix, 0 );
			rcGraph.OffsetRect( rcColumn.left, 0 );
	
			if( rcGraph.PtInRect( pt ) )
				*pFlag |= TLHT_ONITEMIMAGE;

			nPerfix += TLL_WIDTH;
		}
	}

	// add gaps between text and image/lock 
	if( m_dwStyle&TLC_LOCKBOX || m_dwStyle&TLC_IMAGE )
		nPerfix += 2;

	// check tree text
	CRect rcText;
	rcText = rcColumn;
	rcText.left = rcText.left + nPerfix;

	if( rcText.Width() > 0 )
	{
		if( rcText.PtInRect( pt ) )
		{
			*pFlag |= TLHT_ONITEMTEXT;
			*prcText = rcText;
		}
	}

	if( *pFlag == 0 )
		*pFlag |= TLHT_ONITEMSPACE;

	return pItem;
}

HTREELISTITEM CTreeListCtrl::HitTestList( CPoint pt, int* pFlag, CRect* prcText, CRect rcColumn, HTREELISTITEM pItem )
{
	// hittest of other columns of treelist control
	ASSERT( pFlag != NULL );
	ASSERT( pItem != NULL );

	*pFlag = TLHT_ONITEMTEXT;
	*prcText = rcColumn;

	return pItem;
}

void CTreeListCtrl::OnSetFocus( CWnd* pOldWnd ) 
{
	// set focus
	CWnd::OnSetFocus( pOldWnd );
	Invalidate();
	return;
}

void CTreeListCtrl::OnKillFocus( CWnd* pNewWnd ) 
{
	// lost focus
	CWnd::OnKillFocus( pNewWnd );
	Invalidate();
	return;
}

BOOL CTreeListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// pre translate some messages
	switch( pMsg->message )
	{
	case WM_LBUTTONDOWN:
		// cancel midified controls
		break;

	case WM_LBUTTONDBLCLK:
		// filter dbclk message on button & chanbox & lockbox of treelist control
		{
			HTREELISTITEM pItem = NULL;
			POINTS points;
			CPoint pt;
			int nFlag;
			int iSubItem;

			points = MAKEPOINTS( pMsg->lParam );
			pt.x = points.x;
			pt.y = points.y;

			pItem = HitTest( pt, &nFlag, &iSubItem );
			if( pItem != NULL && nFlag&TLHT_ONITEMBUTTON )
			{
				pMsg->message = WM_LBUTTONDOWN;
				return PreTranslateMessage( pMsg );
			}
			if( pItem != NULL && nFlag&TLHT_ONITEMCHECKBOX )
			{
				pMsg->message = WM_LBUTTONDOWN;
				return PreTranslateMessage( pMsg );
			}

			if( pItem != NULL && nFlag&TLHT_ONITEMLOCKBOX )
			{
				pMsg->message = WM_LBUTTONDOWN;
				return PreTranslateMessage( pMsg );
			}
		}
		break;

	case WM_RBUTTONDOWN:
		// cancel modified control
		break;

	case WM_RBUTTONDBLCLK:
		// filter dbclk message on button & chanbox & lockbox of treelist control
		if( pMsg->hwnd == GetSafeHwnd() )
		{
			pMsg->message = WM_RBUTTONDOWN;
			return PreTranslateMessage( pMsg );
		}
		break;

	case WM_NCLBUTTONDOWN:
		// cancel modified controls
		break;

	case WM_NCRBUTTONDOWN:
		// cancel modified controls
		break;

	case WM_KEYDOWN:
		if( PreTranslateKeyDownMessage( pMsg ) )
			return TRUE;
		break;

	case WM_CHAR:
		OnChar(pMsg->wParam,0, pMsg->lParam);
		break;
	case WM_SYSCHAR:
	case WM_DEADCHAR:
	case WM_SYSDEADCHAR:

	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:

	case WM_COMMAND:
	case WM_SYSCOMMAND:
	default:
		break;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CTreeListCtrl::PreTranslateKeyDownMessage( MSG* pMsg )
{
	// pre translate key down messages
	ASSERT( pMsg->message == WM_KEYDOWN );
	HTREELISTITEM pItem;
	int iShow;
	BOOL bFound;

	if( m_arShows.GetSize() == 0 )
		return TRUE;

	switch( pMsg->wParam )
	{
	case VK_ADD:
		{
			if( GetStyle()&TLC_TREELIST )
			{
				Expand( m_pFocusItem, TLE_EXPAND );
				return TRUE;
			}
		}
		break;

	case VK_SUBTRACT:
		{
			if( GetStyle()&TLC_TREELIST )
			{
				Expand( m_pFocusItem, TLE_COLLAPSE );
				return TRUE;
			}
		}
		break;

	case VK_SPACE:
		{
			if( GetStyle()&TLC_TREELIST )
			{
				Expand( m_pFocusItem, TLE_TOGGLE );
				return TRUE;
			}
		}
		break;

	case VK_LEFT:
		if( m_pFocusItem && GetChildItem(m_pFocusItem) && m_pItemMap.GetItemState(m_pFocusItem)&TLIS_EXPANDED ) {
			Expand( m_pFocusItem, TLE_COLLAPSE );
		} else if(m_pFocusItem && GetParentItem(m_pFocusItem) && GetParentItem(m_pFocusItem)!= m_pRootItem) {
			EnsureVisible(GetParentItem(m_pFocusItem), m_iSelCol);
			SelectItem( GetParentItem(m_pFocusItem));
		}
		else PostMessage(WM_KEYDOWN, VK_UP);
		return TRUE;
		break;

	case VK_RIGHT:
		if( m_pFocusItem && (!GetChildItem(m_pFocusItem) || m_pItemMap.GetItemState(m_pFocusItem)&TLIS_EXPANDED))
			PostMessage(WM_KEYDOWN, VK_DOWN);
		else Expand( m_pFocusItem, TLE_EXPAND );
		return TRUE;
		break;

	case VK_UP:
		{
			if( m_pFocusItem != NULL )
				pItem = GetPrevVisibleItem( m_pFocusItem );
			else
				pItem = GetNextVisibleItem( m_pRootItem );

			for( iShow=0, bFound=FALSE; iShow<m_arShows.GetSize(); iShow++ )
			{
				if( m_arShows[iShow] == m_iSelCol )
				{
					bFound = TRUE;
					break;
				}
			}

			if( !bFound )
			{
				m_iSelCol = 0;
			}

			if( pItem != NULL ) {
				if( GetKeyState(VK_SHIFT)>>8 == -1) {
					HTREELISTITEM item= m_hSelect;
                    SelectItem(pItem, m_iSelCol);
					m_hSelect= item;
					SelectRange(m_hSelect, pItem, !(GetKeyState(VK_CONTROL)>>8==-1));
					SelectItem(pItem, m_iSelCol , false);
				} else {
					SelectItem( pItem, m_iSelCol );
				}
				EnsureVisible( pItem, m_iSelCol );
			}
			return TRUE;
		}
		break;

	case VK_DOWN:
		{
			if( m_pFocusItem != NULL )
				pItem = GetNextVisibleItem( m_pFocusItem );
			else
				pItem = GetNextVisibleItem( m_pRootItem );

			for( iShow=0, bFound=FALSE; iShow<m_arShows.GetSize(); iShow++ )
			{
				if( m_arShows[iShow] == m_iSelCol )
				{
					bFound = TRUE;
					break;
				}
			}

			if( !bFound )
			{
				m_iSelCol = 0;
			}

			if( pItem != NULL ) {
				if( GetKeyState(VK_SHIFT)>>8 == -1) {
					HTREELISTITEM item= m_hSelect;
                    SelectItem(pItem, m_iSelCol);
					m_hSelect= item;
					SelectRange(m_hSelect, pItem, !(GetKeyState(VK_CONTROL)>>8==-1));
					SelectItem(pItem, m_iSelCol , false);
					EnsureVisible(pItem, m_iSelCol);
				} else {
					SelectItem( pItem, m_iSelCol );
					EnsureVisible(pItem, m_iSelCol);
				}
			}
			return TRUE;
		}
		break;

	case VK_PRIOR:	// VK_PAGE_UP
		{
			int nFirstItem;
			int nLastItem;
			int nCount;
			HTREELISTITEM pFirstItem = NULL;

			nCount		= m_rcTreeList.Height() / GetItemHeight();
			if( m_rcTreeList.Height() >= GetItemHeight() )
			{
				nCount -= 1;
			}

			nFirstItem	= GetScrollPos( SB_VERT );
			nLastItem	= nFirstItem + nCount;

			pFirstItem = GetTreeListItem( nFirstItem );

			for( iShow=0, bFound=FALSE; iShow<m_arShows.GetSize(); iShow++ )
			{
				if( m_arShows[iShow] == m_iSelCol )
				{
					bFound = TRUE;
					break;
				}
			}

			if( !bFound )
			{
				m_iSelCol = 0;
			}

			if( m_pFocusItem != NULL )
			{
				int nFocusItem = GetTreeListItem( m_pFocusItem );
				if( nFirstItem > nFocusItem )
				{
					EnsureVisible( m_pFocusItem, m_iSelCol );
					return TRUE;
				}
			}

			if( pFirstItem != NULL )
			{
				if( pFirstItem != m_pFocusItem )
				{
					if( GetKeyState(VK_SHIFT)>>8 == -1) {
						HTREELISTITEM item= m_hSelect;
						SelectItem(pFirstItem, m_iSelCol);
						m_hSelect= item;
						SelectRange(m_hSelect, pFirstItem, !(GetKeyState(VK_CONTROL)>>8==-1));
						SelectItem(pFirstItem, m_iSelCol , false);
					} else SelectItem( pFirstItem, m_iSelCol );
				}
				else
				{
					nFirstItem = nFirstItem - nCount - 1;
					if( nFirstItem < 0 )
						nFirstItem = 0;

					nLastItem = nLastItem - nCount - 1;
					if( nLastItem < 0 )
						nLastItem = 0;

					pFirstItem = GetTreeListItem( nFirstItem );
					SetScrollPos( SB_VERT, nFirstItem );
					SelectItem( pFirstItem, m_iSelCol );
				}
			}

			return TRUE;

		}
	case VK_NEXT:	// VK_PAGE_DOWN
		{
			int nFirstItem;
			int nLastItem;
			int nCount;
			HTREELISTITEM pLastItem = NULL;
			
			nCount		= m_rcTreeList.Height() / GetItemHeight();
			if( m_rcTreeList.Height() >= GetItemHeight() )
			{
				nCount -= 1;
			}

			nFirstItem	= GetScrollPos( SB_VERT );
			nLastItem	= nFirstItem + nCount;

			if( nLastItem >= GetVisibleCount() )
			{
				nLastItem  = GetVisibleCount() - 1;
			}

			pLastItem = GetTreeListItem( nLastItem );

			for( iShow=0, bFound=FALSE; iShow<m_arShows.GetSize(); iShow++ )
			{
				if( m_arShows[iShow] == m_iSelCol )
				{
					bFound = TRUE;
					break;
				}
			}

			if( !bFound )
			{
				m_iSelCol = 0;
			}

			if( m_pFocusItem != NULL )
			{
				int nFocusItem = GetTreeListItem( m_pFocusItem );
				if( nLastItem < nFocusItem )
				{
					EnsureVisible( m_pFocusItem, m_iSelCol );
					return TRUE;
				}
			}
			
			if( pLastItem != NULL )
			{
				if( pLastItem != m_pFocusItem )
				{
					if( GetKeyState(VK_SHIFT)>>8 == -1) {
						HTREELISTITEM item= m_hSelect;
						SelectItem(pLastItem, m_iSelCol);
						m_hSelect= item;
						SelectRange(m_hSelect, pLastItem, !(GetKeyState(VK_CONTROL)>>8==-1));
						SelectItem(pLastItem, m_iSelCol , false);
					} else SelectItem( pLastItem, m_iSelCol );
				}
				else
				{
					if( nFirstItem < GetVisibleCount() - nCount - 1 )
					{
						if( nFirstItem + nCount + 1 < GetVisibleCount() - nCount - 1 )
						{
							nFirstItem = nFirstItem + nCount + 1;
						}
					}
					
					nLastItem	= nLastItem + nCount + 1;
					if( nLastItem >= GetVisibleCount() )
					{
						nLastItem = GetVisibleCount() - 1;
					}

					pLastItem = GetTreeListItem( nLastItem );
					SetScrollPos( SB_VERT, nFirstItem );
					SelectItem( pLastItem, m_iSelCol );
				}
			}

			return TRUE;
		}
		break;

	case VK_HOME:
		{
			pItem = GetNextVisibleItem( m_pRootItem );

			for( iShow=0, bFound=FALSE; iShow<m_arShows.GetSize(); iShow++ )
			{
				if( m_arShows[iShow] == m_iSelCol )
				{
					bFound = TRUE;
					break;
				}
			}

			if( !bFound )
			{
				m_iSelCol = 0;
			}

			if( pItem != NULL ) {
				SelectItem( pItem, m_iSelCol );
				EnsureVisible(pItem, m_iSelCol);
			}

			return TRUE;
		}
		break;

	case VK_END:
		{
			HTREELISTITEM pNextItem;
			pItem = GetNextVisibleItem( m_pRootItem );
			pNextItem = pItem;
			while( pNextItem != NULL )
			{
				pItem = pNextItem;
				pNextItem = GetNextVisibleItem( pNextItem );
			}

			for( iShow=0, bFound=FALSE; iShow<m_arShows.GetSize(); iShow++ )
			{
				if( m_arShows[iShow] == m_iSelCol )
				{
					bFound = TRUE;
					break;
				}
			}

			if( !bFound )
			{
				m_iSelCol = 0;
			}

			if( pItem != NULL ) {
				SelectItem( pItem, m_iSelCol );
				EnsureVisible(pItem, m_iSelCol);
			}

			return TRUE;
		}

		break;

	case VK_RETURN:
		break;

	default:
		break;

	}

	return FALSE;
}

HTREELISTITEM CTreeListCtrl::GetTreeListItemEx( int nItem )  {

	// retrieve pointer of item by index of item
	if( GetCount() == 0 )
		return NULL;

	if( nItem < 0 || GetCount() <= nItem )
		return NULL;

	HTREELISTITEM pItem = GetChildItem(m_pRootItem);
	while( nItem != 0 )
	{
		ASSERT( nItem > 0 );

		if( nItem < m_pItemMap.GetChildCount(pItem))
		{
			nItem --;
			pItem = GetChildItem(pItem);
		}
		else
		{
			nItem -= m_pItemMap.GetChildCount(pItem);
			pItem = GetNextSiblingItem(pItem);
		}
	}
	return pItem;
}

HTREELISTITEM CTreeListCtrl::GetTreeListItem( int nItem )
{
	// retrieve pointer of item by index of item
	if( GetCount() == 0 ) {
		return NULL;
	}

	if( nItem < 0 || GetCount() <= nItem ) {
		return NULL;
	}
	HTREELISTITEM pItem = GetChildItem(m_pRootItem);
	while( nItem != 0 )
	{
		ASSERT( nItem > 0 );

		if( nItem < m_pItemMap.GetVisibleChildren(pItem))
		{
			nItem --;
			pItem = GetChildItem(pItem);
		}
		else
		{
			nItem -= m_pItemMap.GetVisibleChildren(pItem);
			pItem = GetNextSiblingItem(pItem);
		}
	}
	return pItem;
}

int CTreeListCtrl::GetTreeListItemEx( HTREELISTITEM pItem ) {

	if( GetCount() == 0 )
		return -1;

	if( pItem == NULL )
		return -1;

	if( pItem == m_pRootItem )
		return -1;

	int iItem = 1;
	while( GetPrevSiblingItem(pItem) != NULL )
	{
		pItem = GetPrevSiblingItem(pItem);
		iItem = iItem + m_pItemMap.GetChildCount(pItem);
	}

	pItem = GetParentItem(pItem);
	iItem = iItem + GetTreeListItem( pItem );
	return iItem;
}

int CTreeListCtrl::GetTreeListItem( HTREELISTITEM pItem )
{
	// retrieve index of item by pointer of item
	if( GetCount() == 0 )
		return -1;

	if( pItem == NULL )
		return -1;

	if( pItem == m_pRootItem )
		return -1;

	int iItem = 1;
	while( GetPrevSiblingItem(pItem) != NULL )
	{
		pItem = GetPrevSiblingItem(pItem);
		iItem = iItem + m_pItemMap.GetVisibleChildren(pItem);
	}

	pItem = GetParentItem(pItem);
	iItem = iItem + GetTreeListItem( pItem );
	return iItem;
}

void CTreeListCtrl::ExchangeItem( HTREELISTITEM pItemA, HTREELISTITEM pItemB )
{
	// exchange itemA and itemB ( the same parent )
	ASSERT( pItemA != NULL && pItemB != NULL );

	if( pItemA == pItemB )
		return;

	if( GetParentItem(pItemA) != GetParentItem(pItemB) )
		return;

	HTREELISTITEM pParentA;
	HTREELISTITEM pParentB;

	pParentA = GetParentItem(pItemA);
	pParentB = GetParentItem(pItemB);

	if( GetChildItem(pParentA) == pItemA )
	{
		m_pItemMap.SetChildItem(pParentA, pItemB);
	}
	else if( GetChildItem(pParentB) == pItemB )
	{
		m_pItemMap.SetChildItem(pParentB, pItemA);
	}

	if(GetNextSiblingItem(pItemA) == pItemB )
	{
		m_pItemMap.SetNextSiblingItem(pItemA, GetNextSiblingItem(pItemB));
		m_pItemMap.SetPrevSiblingItem(pItemB, GetPrevSiblingItem(pItemA));

		m_pItemMap.SetNextSiblingItem(GetPrevSiblingItem(pItemA), pItemB);
		m_pItemMap.SetPrevSiblingItem(GetNextSiblingItem(pItemB), pItemA);
		
		m_pItemMap.SetPrevSiblingItem(pItemA, pItemB);
		m_pItemMap.SetNextSiblingItem(pItemB, pItemA);
	}
	else if( GetPrevSiblingItem(pItemA) == pItemB )
	{
		m_pItemMap.SetNextSiblingItem(pItemB, GetNextSiblingItem(pItemA));
		m_pItemMap.SetPrevSiblingItem(pItemA, GetPrevSiblingItem(pItemB));

		m_pItemMap.SetNextSiblingItem(GetPrevSiblingItem(pItemB), pItemA);
		m_pItemMap.SetPrevSiblingItem(GetNextSiblingItem(pItemA), pItemB);

		m_pItemMap.SetPrevSiblingItem(pItemB, pItemA);
		m_pItemMap.SetNextSiblingItem(pItemA, pItemB);
	}
	else
	{
		HTREELISTITEM pPrevA;
		HTREELISTITEM pPrevB;

		pPrevA	= GetPrevSiblingItem(pItemA);
		pPrevB	= GetPrevSiblingItem(pItemB);

		m_pItemMap.SetPrevSiblingItem(pItemA, pPrevB);
		m_pItemMap.SetPrevSiblingItem(pItemB, pPrevA);

		if( pPrevA != NULL )
			m_pItemMap.SetNextSiblingItem(pPrevA,pItemB);

		if( pPrevB != NULL )
			m_pItemMap.SetNextSiblingItem(pPrevB, pItemA);

		HTREELISTITEM pNextA;
		HTREELISTITEM pNextB;

		pNextA	= GetNextSiblingItem(pItemA);
		pNextB	= GetNextSiblingItem(pItemB);

		m_pItemMap.SetNextSiblingItem(pItemA, pNextB);
		m_pItemMap.SetNextSiblingItem(pItemB, pNextA);

		if( pNextA != NULL )
			m_pItemMap.SetPrevSiblingItem(pNextA, pItemB);

		if( pNextB != NULL )
			m_pItemMap.SetPrevSiblingItem(pNextB, pItemA);
	}
	Invalidate();
	return;
}

void CTreeListCtrl::MoveAfterItem( HTREELISTITEM pItem, HTREELISTITEM pAfter )
{
	// move pItemA after pAfter ( the same parent )
	ASSERT( pItem != NULL );

	HTREELISTITEM pParent;
	HTREELISTITEM pFirst;
	HTREELISTITEM pPrev;
	HTREELISTITEM pNext;
		 
	pParent = GetParentItem(pItem);
	pFirst = GetChildItem(pParent);
	pPrev = GetPrevSiblingItem(pItem);
	pNext = GetNextSiblingItem(pItem);

	if( pAfter == NULL )
	{
		if( GetChildItem(pParent) == pItem ) {
			return;
		}
	}
	else
	{
		if( pItem == pAfter ) {
			return;
		}
	}

	if( GetChildItem(pParent) == pItem )
		m_pItemMap.SetChildItem(pParent, GetNextSiblingItem(pItem));

	if( pPrev != NULL )
		m_pItemMap.SetNextSiblingItem(pPrev, GetNextSiblingItem(pItem));

	if( pNext != NULL )
		m_pItemMap.SetPrevSiblingItem(pNext, GetPrevSiblingItem(pItem));

	if( pAfter == NULL )
	{
		m_pItemMap.SetChildItem(pParent, pItem);

		m_pItemMap.SetPrevSiblingItem(pItem, GetPrevSiblingItem(pFirst));

		m_pItemMap.SetNextSiblingItem(pItem, pFirst);
		m_pItemMap.SetPrevSiblingItem(pFirst, pItem);
	}
	else
	{
		ASSERT( pParent == GetParentItem(pAfter));
		
		pPrev = pAfter;
		pNext = GetNextSiblingItem(pAfter);

		m_pItemMap.SetPrevSiblingItem(pItem, pPrev);
		m_pItemMap.SetNextSiblingItem(pPrev, pItem);

		m_pItemMap.SetNextSiblingItem(pItem, pNext);
		if( pNext != NULL )
			m_pItemMap.SetPrevSiblingItem(pNext, pItem);
	}
	Invalidate();
	return;
}

BOOL CTreeListCtrl::IsInSelectsTree( HTREELISTITEM pItem )
{
	ASSERT( pItem != NULL );
	ASSERT( m_arSelects.GetSize() > 0 );
	
	HTREELISTITEM pSelItem;

	pSelItem	= (HTREELISTITEM)m_arSelects[0];

	while( m_pItemMap.GetItemLevel(pItem) > m_pItemMap.GetItemLevel(pSelItem) )
	{
		pItem = GetParentItem(pItem);
	}
	
	BOOL bFound = FALSE;
	for( int iSel = 0; iSel < m_arSelects.GetSize(); iSel++ )
	{
		pSelItem = (HTREELISTITEM)m_arSelects[iSel];
		if( pItem == pSelItem )
		{
			bFound = TRUE;
			break;
		}
	}

	return bFound;
}

void CTreeListCtrl::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent==100) {

		m_szCadena.Empty();
		KillTimer(100);
	}
	CWnd::OnTimer(nIDEvent);
}

void CTreeListCtrl::OnCancelMode() 
{
	// cancel internal event
	CWnd::OnCancelMode();
}

void CTreeListCtrl::SetCanShowMenu(bool show/*=true*/) {

	m_bCanShowMenu= show;
}

void CTreeListCtrl::SetCanShowStat(bool show) {

	m_bCanStat= show;
}

void CTreeListCtrl::ShowStat() {

	CWaitCursor wait;
	if(m_pThread) {
		m_pThread->Kill();
	}
	if(m_bCanStat) {
		m_pThread= new CThreadTreeStat(m_pThread);
		m_pThread->m_pTreeNuevo= this;
		m_pThread->Run();
		SetError(NULL, "Resumiendo");
	}
}

void CTreeListCtrl::SetMessageLista( CString message) {

	m_szVacia= message;
	if(!GetRootItem())
		Invalidate();
}

void CTreeListCtrl::ResetTipos() {

	m_nColumType.RemoveAll();
}

void CTreeListCtrl::SetType(int columna, int tipo) {

	while(m_nColumType.GetCount()<=columna)
		m_nColumType.Add(1);
	m_nColumType.SetAt(columna, tipo+1);
}

int	CTreeListCtrl::GetColumnType(int columna) {
    
	int tipo=1;
	if( columna<m_nColumType.GetCount())
		tipo= m_nColumType.GetAt(columna);
	return tipo-1;
}

void CTreeListCtrl::AutosizeColumns(bool header) {
    
	CWaitCursor wait;
	for( int i=0; i<GetColumnCount(); i++)
		SetColumnWidth( i, header ? TLSCW_AUTOSIZE_USEHEADER : TLSCW_AUTOSIZE);
}

void CTreeListCtrl::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	// back ground colors of normal status
	m_crBkColor				= ::GetSysColor(COLOR_WINDOW);

	// back ground colors of selected item
	m_crBkSelectedNoFocus	=  HLS_TRANSFORM( ::GetSysColor(COLOR_HIGHLIGHT),  +80, -66);

	// normal text color
	m_crText				= ::GetSysColor(COLOR_WINDOWTEXT);

	// grid color
	m_crGrid				= ::GetSysColor(COLOR_3DLIGHT);
	m_crTreeLine			= ::GetSysColor(COLOR_3DDKSHADOW);
}

LRESULT CTreeListCtrl::OnCopy(WPARAM wParam, LPARAM lParam) {
	
	CopyAll();
	return 0;
}

LRESULT CTreeListCtrl::OnUndo(WPARAM wParam, LPARAM lParam) {

	CollapseAll();
	return 0;
}

LRESULT CTreeListCtrl::OnCut(WPARAM wParam, LPARAM lParam) {
	
	ExpandAll();
	return 0;
}

CNewMenu *CTreeListCtrl::GetMenuContextual(CNewMenu	*menu) {
	
	ASSERT(menu);
	if( GetSelectedItem()) {
		menu->AppendMenu(MF_STRING, ID_EDIT_COPY, GetSelectedCount()>1 ? "Copiar Filas" : "Copiar Fila");
		menu->AppendMenu(MF_SEPARATOR);
	}
	menu->AppendMenu(MF_STRING, ID_EDIT_CUT, "Expandir Todo");
	menu->AppendMenu(MF_STRING, ID_EDIT_UNDO, "Contraer Todo");
	return menu;
}

CNewMenu *CTreeListCtrl::GetMenuContextual() {

	if(m_hMenuContextual)
		delete m_hMenuContextual;

	m_hMenuContextual= new CNewMenu();

	m_hMenuContextual->CreatePopupMenu();
	return GetMenuContextual(m_hMenuContextual);
}

bool CTreeListCtrl::CopyToClipBoard(CString &valor) {

	HGLOBAL hglbCopy;
	if (!OpenClipboard()) 
        return FALSE;
	EmptyClipboard(); 
	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (valor.GetLength() + 1) * sizeof(TCHAR));
    if (hglbCopy == NULL) 
    {
        CloseClipboard(); 
        return FALSE; 
    }
	LPTSTR  lptstrCopy;
	lptstrCopy = (LPTSTR) GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, valor, valor.GetLength() * sizeof(TCHAR));
	lptstrCopy[valor.GetLength()] = (TCHAR) 0;
    GlobalUnlock(hglbCopy);
 
    SetClipboardData(CF_TEXT, hglbCopy);
	CloseClipboard();
	return true;
}

void CTreeListCtrl::CopyAll() {

	CString all;
	HTREELISTITEM item= m_pRootItem;
	while( item=GetNextSelectedItem(item)) {
		for( int i=0; i<GetColumnCount(); i++) {
			all+= m_pItemMap.GetItemText(item, i);
			all+="\t";
		}
		all+="\r\n";
	}
	if( !all.IsEmpty())
		CopyToClipBoard(all);
}

HTREELISTITEM CTreeListCtrl::GetNextItemEx(HTREELISTITEM hItem)
{
	HTREELISTITEM hti;
	if(!hItem)
		return NULL;
	if(GetChildItem(hItem) && m_pItemMap.GetItemState(hItem)&TLIS_EXPANDED)
		return GetChildItem(hItem);
	else
		while((hti= GetNextSiblingItem(hItem))== NULL)
			if((hItem=GetParentItem(hItem))== NULL)
				return NULL;
	return hti;
}

HTREELISTITEM CTreeListCtrl::FindChildItem(const CString &cadena, HTREELISTITEM pParent,int index/*=0*/, HTREELISTITEM pFrom/*=NULL*/) {

	HTREELISTITEM item= pParent ? GetChildItem(pParent) : GetRootItem();
	if(pFrom)
		item= pFrom;
	while(item) {
		if(cadena.Compare(m_pItemMap.GetItemText(item, index))==0) {
			return item;
		}
		item= GetNextSiblingItem(item);
	}
	return NULL;
}

HTREELISTITEM CTreeListCtrl::InsertSorted(const CString &texto, HTREELISTITEM pParent, bool duplicate/*=false*/) {

	CString temp;
	int cmp=0;

	HTREELISTITEM item= pParent ? GetChildItem(pParent) : GetRootItem();
	while(item) {

		temp= m_pItemMap.GetItemText(item, 0);
		cmp=temp.Compare(texto);
		if(cmp==0 && !duplicate) {
			return item;
		}
		if(cmp>0) {

			item= InsertItem(texto, pParent, GetPrevSiblingItem(item) ? GetPrevSiblingItem(item) : TLI_FIRST);
			return item;
		}
		item= GetNextSiblingItem(item);
	}
	return InsertItem(texto, pParent);
}

HTREELISTITEM CTreeListCtrl::FindItem(CString cadena, HTREELISTITEM hItem)
{
	HTREELISTITEM htiCurrent = hItem ? hItem : GetRootItem();
	CString texto;
	while(htiCurrent)
	{
		texto= m_pItemMap.GetItemText(htiCurrent, 0);
		texto.MakeLower();
		if(texto.Find(cadena)==0) {// found

			return htiCurrent;
		}
		htiCurrent = GetNextItemEx(htiCurrent);
	}
	return NULL;
}

void CTreeListCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KillTimer(100);
	if( isprint(nChar)) {

		CWaitCursor wait;

		m_szCadena+=(char)nChar;
		m_szCadena.MakeLower();

		HTREELISTITEM item= GetSelectedItem();
		item= item ? GetNextVisibleItem(item) : GetRootItem();
		item= FindItem(m_szCadena, item);
		if(item) {
			SetTimer(100, 500, NULL);
			SelectItem(item);
			ActiveItem(item);
			return;
		} else m_szCadena.Empty();
	}
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}
